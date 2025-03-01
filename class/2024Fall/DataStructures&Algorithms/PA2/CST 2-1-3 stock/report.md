## 算法思路和实现要点
由于股票的发行时间互相不包含,后来的可购入数更多的股票将覆盖前面的股票,被覆盖的股票将不再被购入,可以记录其位置后丢弃.从前向后扫描股票序列,使用单调序列(算法)维护一个发行时间单调上升,购入数单调下降的序列,每次新股发行队尾出队直到队列空或队尾股票购入数比当前大,同时维护队中每只股票到队中前一只股票中间有多少被删除但尚未退市的股票.每次旧股退市,检测队首股票之前是否还有被删除的股票,如果有则减少其数目,否则从队首出队当前股票.每次有股票事件发生,在答案上累加队首股票的购入数乘上间隔天数.
## 渐进时间复杂度的分析，包含过程
只需要扫描一遍股票序列,每只股票进队出队各一次,维护删除股票数在出队的过程中每次 $O(1)$ 同步实现,总时间复杂度即线性扫描的时间复杂度 $O(n)$.
## 渐进空间复杂度的分析，包含过程
在读入股票的同时在线处理,只需要存储队列中的股票,由于进队出队数目至多为 $n$, 只需要一个 $O(n)$ 的数组线性存储,维护删除股票数也需要一个 $O(n)$ 的数组与队列同步更新,总时间复杂度为 $O(n)$.
## 遇到的困难和解决方法
无
## 估计完成本题所用时间 
0.5 小时
## 关于本题的更多感想
