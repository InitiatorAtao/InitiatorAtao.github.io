## DSA Report of LAB 1

### 2

#### 2A

 $10n^2\times 10^{-8}=\frac{1}{2}$ ,解得 $n\approx 2236$ .

#### 2B

 $20n\log_{2}{n}\times 10^{-8}=\frac{1}{2}$ ,解得 $n \approx 145746$ 

### 3

#### 3A

我使用观察法所找到的 Bug:

1. 数组容量为 2000,在输入规模达到 2000 时会产生越界访问

2. sum 没有在每次处理询问时初始化

3. sum 使用 int 类型,在输入规模和值过大时会产生上溢

4. solution_2 在处理 rowsum 时第二维下标需要减 1

其中,使用 AIGC 工具"智谱清言"进行测试,其指出了前两个 Bug,在经过提示后勉强找出了第三个问题,但解释较为模糊,对于第四个问题则在接受提示后仍坚称其无误,且在说明 int 上限为 2147483647 并自己计算出总体和最大为 4e10 后仍表示 "这个值远小于 int 类型的最大值，所以在这个特定的问题中不会发生整数溢出。"

其调试能力尚可,但有一些较为细节的地方难以观察到,并且在进一步提供修改方向时会产生模糊和自相矛盾的解答,不建议使用该类工具对代码进行深度分析

#### 3B

使用图形界面单步执行程序时可以使用 VSCode "调试" 侧边栏进行操作,在点击代码段行号左侧添加断点后可以用 F5 或 "开始调试" 按钮开始调试,代码会在第一个可用断电处停止,可以使用 "单步跳过(F10)", "单步进入(F11)", "单步退出(Shift+F11)" 等按钮或快捷键单步执行代码.为了调试在编译时应当添加 `-g` 选项让编译器生成调试信息.

#### 3C

 `srand()` 函数配置 `rand()` 的随机种子, `time(0)` 函数返回当前时间,使用时间作为随机种子使得程序每次运行得到不一样的随机数,从而生成不一样的数据用于对拍.

#### 3D

对拍程序先调用 g++ 编译数据生成和检测程序以及两个对拍程序,然后循环使用数据生成程序将生成的随机数据写入 rand.in ,使用数据检测程序检测数据是否合法,不合法则报错退出,否则调用两个对拍程序以刚生成的数据为输入,将各自的输出重定向到各自的文件中,然后调用 diff 命令比较两个输出文件,如果不一致则报错退出.

#### 3E

在 WSL Debian 中调试,运行后输出:

    Starting program: /mnt/d/work/temp/temp 
    Aborted

表示程序运行后被终止,终止原因应该是因为无穷递归导致的栈溢出,使用 `handle SIGSEGV stop print` 让 gdb 输出更详细的信息后得到:

    Starting program: /mnt/d/work/temp/temp 

    Program received signal SIGSEGV, Segmentation fault.
    recursive () at temp.cpp:2
    2           recursive();

表明 gdb 接收到了系统发送的 Segmentation fault 信息,在调用递归函数时产生了段错误,应当是无穷递归栈溢出导致.

### 4

#### 4A

使用二维前缀和预处理,即 `sum[i][j]` 表示从整个矩阵左上角到 `(i,j)` 的矩形元素和,初始化递推关系如:

 `sum[i][j] = sum[i][j-1] + sum[i-1][j] - sum[i-1][j-1] + matrix[i][j];`

于是对于一个询问,直接计算得到所求的和即:

 `sum[x+a-1][y+b-1]-sum[x-1][y+b-1]-sum[x+a-1][y-1]+sum[x-1][y-1]`

时间复杂度为常数.

#### 4B

当 n=1,m=1,q=1 时,在我的机器上 solution 1~3 的运行用时分别为 0.015 S, 0.016 S, 0.018 S, solution 3 并没有明显优势.

当 n=2000,m=2000,q=100000 时,在我的机器上 solution 3 的运行用时为 0.468 S, solution 2 的运行用时为 2.142 S, solution 1 的运行时间过长 (超过 5 min) 不再精确测试, solution 3 展现出明显优势.

#### 4C

当测试数据的数量规模增大,即 $n,m,q$ 增大时 solution 3 的优势增大,当 $n\times m\times q$ 达到 $10^{8}$ 以上时 solution 1 的运行用时剧增,当 $n\times q$ 达到 $10^{8}$ 以上时 solution 2 的运行用时剧增.同时扩张每次询问的范围也会使 1~2 的运行用时增加, solution 3 的运行用时近似不变,优势进一步扩大.