## 算法思路和实现要点
令 `f(i,j)` 表示将 "字符串 A 从位置 $i$ 开始的后缀" 与 "字符串 B 位置 $j$ 开始的后缀" 修改为完全一致所需要的时间单位,若超出阈值,则返回 +INF. 所求答案即 `f(0,0)` (若返回 +INF 则改为 -1).

对于 `f(i,j)` 的计算,边界情况如下:
    
    下标 i 合法范围为 [0,n),下标 j 合法范围为 [0,m), 其余返回 +INF
    f[n][m]=0;

递推公式如下:

$$
\begin{align*}
f_{i,j}=\min(f_{i+1,j}+1,f_{i,j+1}+1,f_{i+1,j+1}+2)\tag{i j 处字符不同}\\
f_{i,j}=\min(f_{i+1,j}+1,f_{i,j+1}+1,f_{i+1,j+1})\tag{i j 处字符相同}
\end{align*}
$$

由于操作次数的限制,每次操作至多产生 1 字符的错位,我们有 $|i-j|<k$ ,由于空间限制,在代码中使用 $i-j+K$ 替代 $j$ 作为数组下标,其中常数 $K$ 等于 $k$ 上界加 10 即 110 以避免越界访问和负数下标,并使用滚动数组以避免空间超限.

## 渐进时间复杂度的分析，包含过程
复杂度上界取决于 `f` 的计算,考虑 `f` 计算过程中访问的状态数 $O(nk)$ ,其中 $n$ 来自于下标 $i$ 的迭代, $k$ 来自于下标 $i-j+K$ 的迭代 (上限为 $2k$ ),每个状态的计算成本为常数,故总时间复杂度即 $O(nk)$ .
## 渐进空间复杂度的分析，包含过程
主要空间占用为动态规划迭代中的占用,与访问状态数相关,不优化的情况下即为状态数 $O(nk)$ ,为了通过空间限制,使用滚动数组仅保留下标 $i,i+1$ 对应的列,可将其优化为 $O(k)$ 即两列长度为 $2K$ 总空间 $4K$ 的固定数组空间.
## 遇到的困难和解决方法
没有 STL (虽然也不用),时间和空间卡得较死,优化过程均如上,状态设计和转移方程搞得不好容易空耗时间,动手前要仔细思考.
## 估计完成本题所用时间 
约 1 小时
## 关于本题的更多感想
这题放第一题是否有些...
没有 STL 的 C++ 还剩下啥,可以直接写 C 了...