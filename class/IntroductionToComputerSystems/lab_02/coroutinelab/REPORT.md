# Report of Lab 2 CoroutineLab

姓名:陶文华

学号:2023010782

班级:计33

## Task 1

首先完成 `context.S` 中的协程切换,由于每个 `context` 类的栈空间在结构体内独立分配,故在协程切换时只需要更改栈顶指针 `%rsp` 而不需额外维护栈帧.由此只需进行寄存器的存取:

```assembly
coroutine_switch:
    # TODO: Task 1
    # 保存 callee-saved 寄存器到 %rdi 指向的上下文
    # 保存的上下文中 rip 指向 ret 指令的地址（.coroutine_ret）
    mov %rax, (%rdi)
    mov %rdi, 0x8(%rdi)
    ...
    mov %r15, 0x70(%rdi)
    leaq .coroutine_ret(%rip), %r15
    mov %r15, 0x78(%rdi)
    mov 0x70(%rdi), %r15

    # 从 %rsi 指向的上下文恢复 callee-saved 寄存器
    # 最后 jmpq 到上下文保存的 rip
    mov %rsi, %rdi
    mov (%rdi), %rax
    mov 0x10(%rdi), %rsi
    ...
    mov 0x70(%rdi), %r15
    jmpq *0x78(%rdi)
```

由此, `resume` 进程只需要调用 `coroutine_switch` 保存当前上下文并用 `context` 中的上下文替换即可:

```C++
virtual void resume() {
    coroutine_switch(caller_registers,callee_registers);
}
```

又注意到在 `basic_context` 类中已经完成了对上下文的初始化,即将上下文指向 `coroutine_main` 函数,该函数被调用时运行 `context-> run()` ,在完成后设置 `finish` 标记并将切换回调用者的上下文.因此初始化完成后的 `context` 可以直接调用 `resume` 开始运行,在串行执行中轮询调用即可:

```C++
void serial_execute_all() {
    is_parallel = false;
    g_pool = this;
    while(coroutines.size()) {
        for (int i=0;i<coroutines.size();++i) {
            if (coroutines[i]->finished) {
                delete coroutines[i];
                coroutines.erase(coroutines.begin()+i);
                --i;    continue;
            }
            if (!coroutines[i]->ready) {
                coroutines[i]->ready=coroutines[i]->ready_func();
                if (!coroutines[i]->ready) continue;
            }
            context_id=i;
            current_context=coroutines[i];
            coroutines[i]->resume();
        }
    }
}
```

对于协程的 `yield`,由于协程开始运行时保存的是 `serial_execute_all` 调用的 `resume` 中的上下文,在协程暂停时只需要恢复该上下文继续轮询过程即可:

```C++
void yield() {
    if (!g_pool->is_parallel) {
        // 从 g_pool 中获取当前协程状态
        auto context = g_pool->coroutines[g_pool->context_id];
        // 调用 coroutine_switch 切换到 coroutine_pool 上下文
        coroutine_switch(context->callee_registers, context->caller_registers);
    }
}
```

以下为额外要求:

1. 协程运行时的栈 (`coroutines[1]` 运行中,`coroutines[0]` 暂停,`coroutines[2]` 未启动,上下文栈为 `uint64_t *stack`):

```
system:         ...|serial_execute_all|resume
coroutines[0]:  coroutine_main|run|CALL|f(args)|yield
coroutines[1]:  coroutine_main|run|CALL|f(args) <- %rsp
coroutines[2]:  |
```

切换时,上下文先被 `yield` 返回到调用 `resume` 启动时的状态,栈由 `basic_context` 中的 `uint64_t *stack` 返回到系统栈:

```
system:         ...|serial_execute_all|resume <- %rsp
coroutines[0]:  coroutine_main|run|CALL|f(args)|yield
coroutines[1]:  coroutine_main|run|CALL|f(args)|yield
coroutines[2]:  |
```

随后 `resume` 返回到 `serial_execute_all` 继续轮询过程,开始执行 `coroutines[2]`, `coroutine_main`, `run`, `CALL`, `f(args)` 依次入栈,指定的函数开始运行:

```
system:         ...|serial_execute_all|resume
coroutines[0]:  coroutine_main|run|CALL|f(args)|yield
coroutines[1]:  coroutine_main|run|CALL|f(args)|yield
coroutines[2]:  coroutine_main|run|CALL|f(args) <- %rsp
```

当 `coroutines[2]` 调用 `yield`, 上下文和栈被返回到 `resume` 和系统栈:

```
system:         ...|serial_execute_all|resume <- %rsp
coroutines[0]:  coroutine_main|run|CALL|f(args)|yield
coroutines[1]:  coroutine_main|run|CALL|f(args)|yield
coroutines[2]:  coroutine_main|run|CALL|f(args)|yield
```

继续轮询,找到 `coroutines[0]`, 将上下文恢复到其调用 `yield` 的位置继续运行:

```
system:         ...|serial_execute_all|resume
coroutines[0]:  coroutine_main|run|CALL|f(args) <- %rsp
coroutines[1]:  coroutine_main|run|CALL|f(args)|yield
coroutines[2]:  coroutine_main|run|CALL|f(args)|yield
```

2. `coroutine_main` 函数的功能已经分析过, `coroutine_entry` 作为调用入口只是简单的调用 `coroutine_main`.协程初始化时并未保存 `caller_registers`, 只是简单设置了 `callee_registers` 使其启动时从 `coroutine_entry` 开始运行并加载当前上下文作为参数, 在调用 `resume` 时保存上下文到 `caller_registers` 并通过 `coroutine_entry` 调用 `coroutine_main`.

3. 如果要考虑浮点和向量寄存器,由于其一般不参与流程控制,只需要简单地将其保存或恢复到内存空间中即可,对于比较大量的读写过程,这些寄存器也可能提供向量化操作方法以加快存取.

## Task 2

`sleep` 函数负责注册 `ready_func` 并将 `ready` 设置为 `false`, 最后从当前协程切出即调用 `yield`.

```C++
void sleep(uint64_t ms) {
    if (g_pool->is_parallel) {
        auto cur = get_time();
        while (
            std::chrono::duration_cast<std::chrono::milliseconds>(get_time() - cur)
                .count() < ms)
          ;
    } else {
        basic_context *coroutine_now=g_pool->coroutines[g_pool->context_id];
        auto time_start=get_time();
        coroutine_now->ready=false;
        coroutine_now->ready_func=[time_start,ms](){
            return std::chrono::duration_cast<std::chrono::milliseconds>(get_time()-time_start).count()>=ms;
        };
        yield();
    }
}
```

`serial_execute_all` 函数之前修改时便已考虑 `ready` 和 `ready_func` 的情况.具体的说,如果当前协程尚未准备好,则调用一次 `ready_func` 更新 `ready`,如果仍未准备好则继续轮询,否则启动当前协程.

以下是额外要求:

1. 以 `README` 中的示例来说,每个协程均在开始时调用 `sleep`,故在第一轮轮询时,每个协程均启动后注册自己的 `ready_func` 并 `yield` 让协程池启动下一个协程:

```
system:         ...|serial_execute_all|resume
coroutines[0]:  coroutine_main|run|CALL|f(args)|sleep|yield <- %rsp
coroutines[1]:  |
...
coroutines[4]:  |
```

在一轮轮询过后:

```
system:         ...|serial_execute_all <- %rsp
coroutines[0]:  coroutine_main|run|CALL|f(args)|sleep|yield
coroutines[1]:  coroutine_main|run|CALL|f(args)|sleep|yield
...
coroutines[4]:  coroutine_main|run|CALL|f(args)|sleep|yield
```

此时 `serial_execute_all` 持续轮询直到最快的协程 `coroutines[0]` 准备好:

```
system:         ...|serial_execute_all|resume
coroutines[0]:  coroutine_main|run|CALL|f(args)|printf <- %rsp
coroutines[1]:  coroutine_main|run|CALL|f(args)|sleep|yield
...
coroutines[4]:  coroutine_main|run|CALL|f(args)|sleep|yield
```

此时输出第一个数 `1`, 协程退出,继续轮询直到第二个协程就绪:

```
system:         ...|serial_execute_all|resume
coroutines[0]:  |
coroutines[1]:  coroutine_main|run|CALL|f(args)|sleep|yield
...
coroutines[4]:  coroutine_main|run|CALL|f(args)|printf <- %rsp
```

如此反复,直到最后一个协程输出,所有协程退出,主程序返回.

2. 对于优化方案:

如果协程要在指定的时间唤醒,可以先行计算出唤醒时间,加入一个堆维护的事件队列,协程池只检测最快触发的事件是否就绪,可以节省轮询和判断事件,提高触发精度.

如果协程要根据指定的事件唤醒,可以使用中断触发 (如 IO 引起的唤醒可以使用 IO 多路复用等待 IO 事件.

## Task 3

为测试协程库带来的具体常数变化,分别在不调用 `yield`,每次都调用 `yield`,根据当前搜索范围选择是否调用 `yield` 的情况下分别运行 20 次默认测试程序 (即不额外添加参数的 `binary_search`),取数据平均值,得到的结果如下:

|计算方式|搜索耗时|访问耗时|
|-|-|-|
|暴力|1804.933|56.40|
|不调用 `yield`|2520.351|78.76|
|每次调用 `yield`|2074.155|64.82|
|选择调用 `yield`|2017.302|62.88|

对数据的比较表明在我使用的设备上,协程库带来了较大的额外时间开销,但协程切换可以较为有效的减小搜索和访问的耗时,如果协程库实现得更加精细,应当能获得超越暴力访问的时间效率.另外一种方法是增大测试使用的数据范围,协程库应当在大数据量情况下获得更佳的性能 (由于设备内存限制,暂未进行 8 GB 数据量的测试)

所有测试的数据及统计分析在压缩包内随附,也可以在[此处](https://cloud.tsinghua.edu.cn/d/f7b2051dce36478bab3f/)下载.

## Honor Code

完成本实验的过程中未与其他同学进行交流,关于协程库的实现方法询问了[智谱清言](https://chatglm.cn/),部分参考了其思路,未使用其生成代码.

## 总结和感想

C/C++ 是与系统底层架构紧密相关的语言,其与汇编语言的相关性为其提供了强大的底层优化能力,随时插入汇编语言的能力有时可以用于实现超越语言架构的某些功能.
