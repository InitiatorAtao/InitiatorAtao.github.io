### 期中复习

MDP: Markov Decision Process,马尔可夫决策过程,包含如下内容:
    \
    1.状态集 $s\in S$ .
    2.(可选)可观测状态集(observations) $o\in O$ .
    3.决策集 $a\in A$ .
    4.状态转移函数 $T(s,a,s')=P(s'|s,a)$ ,状态 $s$ 下执行操作 $a$ 转移到 $s'$ 的概率.
    5.奖励函数 $R(s,a,s')$ ,可简化为 $R(s,a)$ .
    6.初始状态分布
    7.终止状态
    \
    相当于非确定搜索问题,以马尔可夫命名是因为状态转移与先前状态无关,仅由当前状态及决策决定.
    POMDP: Partially Observable MDP,部分可观测的马尔可夫决策过程
<br>

 $\pi:S\rightarrow A$ : policy ,策略, $\pi^{*}$ 是最佳策略(最大化预期 utility,效用,或者时间序奖励序列(和),通常带衰减系数 $\gamma$ ). $\pi(a|s)$ 表示 $s$ 下决策 $a$ 的可能性.
 $V:S\rightarrow \mathbb{R}$ : value(utility) ,价值,从 $s$ 开始按最佳策略的效用期望.
 $Q:S\times A\rightarrow\mathbb{R}$ 从 $s$ 开始执行决策 $a$ 后按最佳策略的效用期望.
<br>

Bellman Equation: 贝尔曼方程:$$\begin{align*}
V^{*}(s)=&\max_{a}Q^{*}(s,a)\\
Q^{*}(s,a)=&\sum_{s'}^{}T(s,a,s')[R(s,a,s')+\gamma V^{*}(s')]\\
\end{align*}$$求解方式: Value Iteration: 数值迭代:
    1. $V_{k}(s)$ : 如果游戏在 $k$ 步内结束, $s$ 的价值
    2. $V_{0}(s)=0$ : 当 $s$ 为终止状态
    3. $V_{k+1}(s)\leftarrow \max_{a}\sum_{s'}^{}T(s,a,s')[R(s,a,s')+\gamma V_{k}(s')]$ 如果 $s'$ 是终止状态,则不累加 $\gamma V_{k}(s)$ 
    4. 重复直到收敛
策略收敛快于价值,复杂度 $O(S^{2}A)$ 
求解方式: Policy Iteration: 策略迭代:
    1. $V^{\pi}(s)$ :从 $s$ 开始依照 $\pi$ 决策的奖励衰减和.
    2. Policy evaluation: $V^{\pi}_{k+1}(s)=\sum_{s'}^{}T(s,\pi(s),s')[R(s,\pi(s),s')+\gamma V^{\pi}_{k}(s')]$ ,固定策略计算到收敛
    3. Policy improvement: $\pi_{i+1}(s)=\arg \max_{a}(\sum_{s'}^{}T(s,a,s')[R(s,a,s')+\gamma V^{\pi_{i}}(s')])$ 
最终仍收敛到最佳策略,有时会快(很多)
在没有 $\max$ 的情况下 $V$ 变为一个线性系统 $V=P(r+\gamma V)$ ,小情况下可以线性求解 $V=(1-\gamma P)^{-1}Pr$ 
<br>

Monte-Carlo Policy Evaluation (Direct Evaluation): 蒙特卡洛策略评估(直接评估)
    1. 目标:由 $\pi$ 下的决策经验 $s_{1},a_{1},r_{1},\cdots,s_{k}-\pi$ 学习价值 $v_{\pi}$ 
    2. $G_{t}=r_{t+1}+\gamma r_{t+2}+\cdots+\gamma^{T-1}r^{T}$ :回报,奖励序列的衰减和
    3. $v_{\pi}(s)=\mathbb{E}_{\pi}[G_{t}|S_{t}=s]$ 使用策略 $\pi$ 下的回报期望.
    4. 从 $s$ 开始依照 $\pi$ 行动直到终止,重复多次,计算 $G_{t}$ 均值作为期望.
计算技巧: Incremental mean,累加均值,在 $n$ 元素均值为 $x$ 的集合内加入值为 $y$ 元素后均值为 $x+\frac{y-x}{n+1}$ .也可以使用学习率 $V(S_{t})\leftarrow V(S_{t})+\alpha (G_{t}-V(S_{t}))$ .
最终会收敛到正确值
简单,纯经验驱动,无需引导(bootstrapping),但需要行动到终止,需要大量采样,并未利用贝尔曼方程(马尔可夫性质)
<br>

Temporal Difference Learning,时间差学习
    1. 在线从运行策略 $\pi$ 的经验中学习 $v_{\pi}$ 
    2. 每次行动更新当前状态的价值
    3. TD(0): $V(S_{t})\leftarrow V(S_{t})+\alpha(R_{t+1}+\gamma V(S_{t+1})-V(S_{t}))$ 
    4. TD target: $R_{t+1}+\gamma V(S_{t+1})$ 
    5. TD error: $\delta _{t}=R_{t+1}+\gamma V(S_{t+1})-V(S_{t})$ 
    6. $\alpha$ : learning rate
    7. n-step TD: $G_{t}^{(n)}=R_{t+1}+\gamma R_{t+2}+\cdots+\gamma^{n}V(S_{t+n}),V(S_{t})\leftarrow V(S_{t})+\alpha(G_{t}^{(n)}-V(S_{t}))$ 
    8. TD( $\lambda$ ): $G_{t}^{\lambda}=(1-\lambda)\sum_{n=1}^{+\infty}\lambda^{n-1}G_{t}^{(n)},V(S_{t})\leftarrow V(S_{t})+\alpha(G_{t}^{\lambda}-V(S_{t}))$ ,系数 $(1-\lambda)$ 来源于 $\sum_{n=1}^{+\infty}\lambda^{n-1}=\frac{1}{1-\lambda}$ ,相当于对 $G_{t}^{(n)}$ 作加权平均
    9. Eligibility Traces,资格迹, $E_{0}(s)=0,E_{t}(s)=\gamma\lambda E_{t-1}(s)+1[S_{t}==s]$ ,确定过去经验对当前决策的影响程度,当前出现状态 $s$ 会累加 $s$ 的重要值,按时间衰减.
    10. Backward view TD( $\lambda$ ): $V(s)\leftarrow V(s)+\alpha\delta_{t}E_{t}(s)$ ,展开就是之前各状态到当前状态的 $G_{t}^{\lambda}-V(S_{t})$ 影响累加,使得依赖于向后决策的 TD( $\lambda$ ) 变为依赖历史决策.(有细微差异,但不影响,参见 True Online TD)
按步学习,可以从不完整的序列中学习,更加高效,但对 $v_{\pi}(s)$ 的估计有 $\pi$ 的偏置,灵活性不如 MC (因为更新只依赖一步),初始值敏感
<br>

 $\epsilon$ -Greedy Exploration, $\epsilon$ -贪婪探索:$$\begin{align*}
 \pi(a|s)=\left\{\begin{matrix}
 \frac{\epsilon}{m}+1-\epsilon  &   \operatorname{if}   a^{*}=\argmax_{a\in A}  Q(s,a)\\
 \frac{\epsilon}{m} &   \operatorname{otherwise}
 \end{matrix}\right.
 \end{align*}$$
 Greedy in the Limit with Infinite Exploration(GLIE),在迭代次数足够多时,每个 $(s,a)$ 对被尝试无穷多次, $\pi(a|s)$ 全收敛至 $\argmax_{a'\in A}Q(s,a')$ .
<br>

Boltzman Exploration,玻尔兹曼探索: $\operatorname{Pr}(a|s)=\frac{\exp(Q(s,a)/T)}{\sum_{a'\in A}^{}\exp(Q(s,a')/T)}$ , $T$ 是温度,类似模拟退火从高温逐渐降低,探索程度逐渐减小, $T\rightarrow0$ 时收敛为完全的 Exploitation.
<br>

Q-learning: $Q(s,a)=R(s)+\gamma \sum_{s'}^{}T(s,a,s')V(s')=R(s)+\gamma\sum_{s'}^{}T(s,a,s')\max_{a'}Q(s',a')$ 
    1. 可以像 TD 一样执行单步更新 $Q(s,a)\leftarrow Q(s,a)+\alpha(R(s)+\gamma \max_{a'}Q(s',a')-Q(s,a))$ .
    2. 一般执行过程:为 $Q$ 赋初值(如全 0),按照探索/利用策略采取行动, TD 更新,再次采取行动
    3. 当所有 $(s,a)$ 对被无限次访问,策略收敛至最佳 $Q$ 
    4. State-Action-Reward-State-Action (SARSA),即上述 Q 值的 TD 更新过程,是基于策略的(即 Q 值更新依赖于当前的决策)
<br>

Gradient Descent,梯度下降法:
    1. 目标:优化参数向量 $\vec{w}$ ,最小化拟合价值的均方误差(mean square loss,MSE) $J(\vec{w})=\mathbb{E}_{\pi}[(v_{\pi}(S)-\hat{v}(S,\vec{w}))^{2}]$ 
    2. 梯度下降: $\Delta \vec{w}=-\frac{1}{2}\alpha \nabla_{\vec{w}}J(\vec{w})=\alpha\mathbb{E}_{\pi}[(v_{\pi}(S)-\hat{v}(S,\vec{w}))\nabla_{\vec{w}}\hat{v}(S,\vec{w})]$ 
    3. 使用采样移除期望计算(Stochastic gradient descent,随机梯度下降)
    4. $v_{\pi}(S)$ 的初始值,由选用的 RL 策略计算,如 MC 的 $G_{t}$ , TD(0) 的 $R_{t+1}+\gamma \hat{v}(S_{t+1},\vec{w})$ , TD( $\lambda$ ) 的 $G_{t}^{\lambda}$ 
    5. 对于 Q 函数同样处理, $\hat{q}(S,A,\vec{w})\approx q_{\pi}(S,A),J(\vec{w})=\mathbb{E}_{\pi}[(q_{\pi}(S,A)-\hat{q}(S,A,\vec{w}))^{2}],\Delta \vec{w}=-\frac{1}{2}\nabla _{\vec{w}}J(\vec{w})=\alpha(q_{\pi}(S,A)-\hat{q}(S,A,\vec{w}))\nabla_{\vec{w}}\hat{q}(S,A,\vec{w})$ 
    6. 在现代 DRL 中 $\vec{w}$ 通常指一个神经网络,特征向量可能是高维感官信息,又称表征(representations)
    7. 在 Q-learning 中只需要将 TD 更新过程改为计算损失和反向传播过程即可.
<br>

Linear Value Function Approximation,线性价值函数拟合:
    1. 通过特征向量的线性组合表示价值函数, $\hat{v}(S,\vec{w})=\vec{x}(S)^{T}\vec{w}=\sum_{j=1}^{n}\vec{x}_{j}(S)\vec{w_{j}}$ 
    2. $J(\vec{w})=\mathbb{E}_{\pi}[(v_{\pi}(S)-\vec{x}(S)^{T}\vec{w})^{2}]$ 
    3. $\Delta \vec{w}=\alpha (v_{\pi}(S)-\hat{v}(S,\vec{w})\vec{x}(S))$ 
<br>

Reinvent DQN,(工程化)重塑 DQN:
    1. 堆叠(stack)连续几帧图像,调整图像比例(rescale)
    2. 使用 $|a|$ 头的神经网络,而不是将 $a$ 作为特征传入 (直觉上,神经网络难以理解这样的数据)
    3. Q 值大波动,可能由于数据分布使得神经网络过拟合(数据利用效率低),解决方法是经验重放,存储转移 $(s_{t},a_{t},r_{t},s_{t+1})$ ,每次在存储中随机批次取样更新网络.
    4. Q 值的小变化使得策略大波动,可能由于数据分布在极端摇摆,解决方法是使用目标 Q 网络 (Target Q networks) 计算转移后的 $Q$ 函数: $\mathrm{L}_{i}(\vec{w}_{i})=\mathbb{E}_{s,a,r,s'-D_{i}}[(r+\gamma\max_{a'}Q(s',a',\vec{w}_{i}^{-})-Q(s,a,\vec{w}_{i}))]$ ,其中 $\vec{w}^{-}$ 表示定时更新的目标 Q 网络
    5. 梯度不稳定,可能由于奖励过大,为避免 Q 值过大或梯度条件良好,DQN 将奖励限制(clips)在 $[-1,1]$ 
<br>

Overestimation in Q Learning,Q 值高估:
    1. $E(\max(X_{1},X_{2},\cdots))\ge \max(E(X_{1}),E(X_{2}),\cdots)$ ,在计算损失时总是先取最大再更新(拟合)均值,使得 Q 值比真实值偏高
    2. Double DQN,双重 DQN,处理 Q 值高估,使用当前的 Q 网络选取决策,使用目标 Q 网络评估决策, $I=(r+\gamma Q(s',\argmax_{a'}Q(s',a',\vec{w}),\vec{w}^{-})-Q(s,a,\vec{w}))^{2}$ 
    3. Prioritized Experience Replay(PER),优先经验重放,处理随机取样中的无效信息,按 DQN error $|r+\gamma\max_{a'}Q(s',a',\vec{w}^{-})-Q(s,a,\vec{w})|$ 优先队列存储经验重放,按照概率分布 $P(i)=\frac{p_{i}^{\alpha}}{\sum_{k}^{}p_{k}^{\alpha}}$ 抽样.
    4. Dueling Networks,对抗网络,处理 Q 值高估,将 $Q(s,a)$ 拆分成 $Q(s,a)=V(s)+A(s,a)$ ,在神经网络中拆分两个(组)输出头,分别输出 $V(s)$ 和 $A(s,a)$ 
    5. n-step return, n 步奖励,类似 n-step TD 的序列衰减奖励, $R_{t}^{(n)}=\sum_{k=0}^{n-1}\gamma_{t}^{(k)}R_{t+k+1},I=(R_{t}^{(n)}+\gamma_{t}^{(n)}\max_{a'}Q(S_{t+n},a',\vec{w})-Q(s,a,\vec{w}))^{2}$ 
<br>

Policy-based Reinforcement Learning,基于策略的 RL:
    1. $\pi_{\theta}(s,a)=\mathbb{P}[a|s,\theta],\theta$ 是参数,直接输出策略分布
    2. Policy Objective Functions,策略目标函数,用于评价策略参数的优劣,如使用每步的平均奖励进行评价 $\theta$ : $J_{\overline{R}}(\theta)=\sum_{s}^{}d^{\pi_{\theta}}(s)\sum_{a}^{}\pi_{\theta}(s,a)R_{s}^{a}$ ,其中 $d^{\pi_{\theta}}(s)$ 是遵循策略 $\pi_{\theta}$ 时系统处于状态 $s$ 的概率,可求解 $d^{\pi_{\theta}}(s)=\sum_{s'}^{}d^{\pi_{\theta}}(s')P^{\pi_{\theta}}(s|s')$ ,其中 $P^{\pi_{\theta}}(s|s')$ 是当前策略参数下 $s'$ 到 $s$ 的概率. $d^{\pi_{\theta}}(s)$ 可用幂迭代法(power iteration)或者贝尔曼方程的线性形式求解,或者直接使用 $V^{\pi}(s)$ 或者 $R(s)$ .
    3. Policy Gradient,策略梯度,对于任意策略目标函数 $J(\theta)$ ,使用梯度上升计算局部最大值, $\Delta \theta=\alpha \nabla_{\theta}J(\theta),\alpha$ 是步长/学习率
    4. 解决 $\nabla_{\theta}\mathbb{E}_{\tau}[R(\tau)]$ ( $\tau$ 是轨迹(trajectory), $(s,a,r,s')$ 序列从初状态到终止,可看作一局完整的游戏)的可微分性问题,可以使用可微的模拟器,或者使用如下技巧:$$\begin{align*}
    \nabla_{\theta}\log z=&\frac{1}{z}\nabla_{\theta}z\\
    \mathbb{E}_{x\sim p(x)}[f(x)]=&\int_{x}^{}p(x)f(x)\mathrm{d}x\\
    \frac{a}{b}=&\frac{a\cdot p(x)}{b\cdot p(x)}\\
    \nabla_{\theta}\mathbb{E}_{a\sim p(a|s,\theta)}[r(a)]=&\nabla_{\theta}\sum_{a}^{}p(a|s,\theta)r(a)\\
    =&\sum_{a}^{}r(a)\nabla_{\theta}p(a|s,\theta)\\
    =&\sum_{a}^{}r(a)p(a|s,\theta)\frac{\nabla_{\theta}p(a|s,\theta)}{p(a|s,\theta)}\\
    =&\sum_{a}^{}r(a)p(a|s,\theta)\nabla_{\theta}\log p(a|s,\theta)\\
    =&\mathbb{E}_{a\sim p(a|s,\theta)}[r(a)\nabla_{\theta}\log p(a|s,\theta)]\\
    \end{align*}$$如此便可将对 $r(a)$ 的微分问题转化为对 $\log p(a|s,\theta)$ 的微分问题,要最大化奖励收益,还需要计算在给定策略( $\theta$ )下智能体经过一个特定轨迹 $\tau$ 的概率 $p(\tau|\theta)$ :$$\begin{align*}
    p(\tau|\theta)=\mu(s_{0})\cdot\prod_{t=0}^{T-1}[\pi(a_{t}|s_{t},\theta)\cdot p(s_{t+1},r_{t}|s_{t},a_{t})]\\
    \end{align*}$$其中 $\mu (s_{0})$ 是初始状态的分布下 $\tau$ 对应的 $s_{0}$ 出现的概率, $\pi(a_{t}|s_{t},\theta)$ 是策略选择 $\tau$ 中 $(s_{t},a_{t})$ 决策的概率, $p(s_{t+1},r_{t}|s_{t},a_{t})$ 是在 $\tau$ 中转移组 $(s_{t},a_{t},r_{t},s_{t+1})$ 对应的转移函数出现概率.于是便可对 $\mathbb{E}_{\tau}[R(\tau)]$ 求梯度得:$$\begin{align*}
    \nabla_{\theta}\mathbb{E}_{\tau}[R(\tau)]=&\mathbb{E}_{\tau}[R(\tau)\nabla_{\theta}\log p(\tau |\theta)]\\
    \end{align*}$$其中:$$\begin{align*}
    \log p(\tau|\theta)=\log \mu(s_{0})+\sum_{t=0}^{T-1}\log [\pi(a_{t}|s_{t},\theta)\cdot p(s_{t+1},r_{t}|s_{t},a_{t})]\\
    \end{align*}$$由于我们对 $\theta$ 求梯度,与其无关的 $\log \mu(s_{0})$ 和 $\log p(s_{t+1},r_{t}|s_{t},a_{t})$ 项都可以消去,留下 $\sum_{t=0}^{T-1}\log \pi(a_{t}|s_{t},\theta)$ ,于是:$$\begin{align*}
    \nabla_{\theta}\mathbb{E}_{\tau}[R(\tau)]=\mathbb{E}_{\tau}[R(\tau)\sum_{t=0}^{T-1}\nabla_{\theta}\log \pi(a_{t}|s_{t},\theta)]
    \end{align*}$$
    5. Reinforce--MC-based method,强化学习,主要过程为 (1) 从当前策略 $\pi_{\theta}(a_{t}|s_{t})$ 取样 $\{\tau^{i}\}$ ,也就是多次运行策略记录游戏过程(轨迹) (2) $\nabla_{\theta}J(\theta)\approx \sum_{i}^{}(\sum_{t}^{}\nabla_{\theta}\log \pi_{\theta}(a_{t}^{i}|s_{t}^{i}))(\sum_{t}^{}r(s_{t}^{i},a_{t}^{i}))$ (3) $\theta\leftarrow \theta+\alpha\nabla_{\theta}J(\theta)$ 
    6. 为了解决 $R(\tau)$ 的偏置问题(可能所有的奖励都非正,导致学习到的都是"禁止做某事",但实际上还需要"应该做某事"),需要特性 $\mathbb{E}_{\pi_{\theta}}[\nabla_{\theta}\log \pi_{\theta}(s,a)B(s)]=0$ (因为决策分布总量 1 是确定的,所以一处概率提升必然有等量的概率下降),因此可以在 $\nabla_{\theta}\mathbb{E}_{\tau}[R(\tau)]$ 上加基线 (baseline, $B$ ) 使得 $\nabla_{\theta}\mathbb{E}_{\tau}[R(\tau)]=\mathbb{E}_{\tau}[(R(\tau)-B)\sum_{t=0}^{T-1}\nabla_{\theta}\log \pi(a_{t}|s_{t},\theta)]$ ,从而使得 $(R(\tau)-B)$ 有正有负,给予两方面的学习信息.
<br>

Parameterize the policy,参数化策略 $\theta$ :
    1. 神经网络,当然
    2. Softmax,即对特征 $\phi(s,a)$ 使用 $\theta$ 进行线性组合 $\phi(s,a)^{T}\theta$ 后加指数转换为正值, $\pi_{\theta}(s,a)\propto e^{\phi(s,a)^{T}\theta}$ (亦即总和归一化后作为策略分布),分数函数(score function,也就是上述求导的核心部分) $\nabla_{\theta}\log \pi_{\theta}(s,a)=\phi(s,a)-\mathbb{E}_{\pi_{\theta}}[\phi(s,\cdot)]$ ,右边是总和归一化分式 $\frac{e^{\phi(s,a)^{T}\theta}}{\sum_{i}^{}e^{\phi(s,i)^{T}\theta}}$ 的对数形式计算梯度后的结果,其中的期望是梯度作用在 $\log$ 上由链式法则得到 $\frac{\sum_{i}^{}e^{\phi(s,i)^{T}\theta}\phi(s,i)}{\sum_{i}^{}e^{\phi(s,i)^{T}\theta}}$ 按定义出现的,注意分子又使用了一次链式法则, $\phi(s,i)^{T}\theta$ 中的 $\theta$ 被梯度算符消去,故这可看作 $\phi(s,a)$ 对 $\pi_{\theta}(s,a)$ 的加权平均,此即 $\mathbb{E}_{\pi_{\theta}}[\phi(s,\cdot)]$ 的定义.
    3. Gaussian,高斯分布,均值是特征的线性组合 $\mu (s)=\phi(s)^{T}\theta$ ,方差可以是固定常数 $\sigma^{2}$ 或作为参数,策略即高斯分布 $a\sim \mathrm{N}(\mu(s),\sigma^{2})$ ,分数函数 $\nabla_{\theta}\log \phi_{\theta}(s,a)=\frac{(a-\mu(s))\phi(s)}{\sigma^{2}}$ (硬导咯,要出来咯~)
<br>

Reduce varience in Policy gradient,降低策略梯度中的高方差:
    1. 使用基线(baseline),但基线主要解决的是更新过于剧烈(aggressiveness)的问题
    2. 控制奖励的回传,在得到奖励之后的决策对该奖励没有贡献,自然不应得到该奖励产生的梯度:$$\begin{align*}
    \nabla_{\theta}J(\theta)\approx&\frac{1}{N}\sum_{i=1}^{N}(\sum_{t=1}^{T}\nabla_{\theta}\log \pi_{\theta}(a_{i,t}|s_{i,t}))(\sum_{t=1}^{T}r(s_{i,t},a_{i,t}))\\
    =&\frac{1}{N}\sum_{i=1}^{N}\sum_{t=1}^{T}[\nabla_{\theta}\log\pi_{\theta}(a_{i,t}|s_{i,t})\sum_{t'=1}^{T}r(s_{i,t},a_{i,t})]\\
    =&\frac{1}{N}\sum_{i=1}^{N}\sum_{t=1}^{T}[\nabla_{\theta}\log\pi_{\theta}(a_{i,t}|s_{i,t})\hat{Q}(s_{i,t},a_{i,t})]\\
    \end{align*}$$
<br>

On-policy troubles,基于策略的问题:
    1. 策略梯度中的 $\nabla_{\theta}\mathbb{E}_{\tau}[R(\tau)]$ 中的 $\tau$ 必须是由当前智能体运行的轨迹(其他智能体的决策与本智能体的策略参数无关)
    2. Importance Sampling,重要性采样,使用不同于当前分布的采样求当前分布下函数期望, $E[f(x)]=\int_{}^{}f(x)p(x)\mathrm{d}x=\int_{}^{}f(x)\frac{p(x)}{q(x)}q(x)\mathrm{d}x\approx \frac{1}{n}\sum_{i}^{}f(x_{i})\frac{p(x_{i})}{q(x_{i})}$ ,其中求和的是依照 $q$ 分布取样得到的值集
    3. Off-policy PG,离(本)策略的策略梯度,使用来自其他策略的轨迹分布 $\overline{p}(\tau),J(\theta)=E_{\tau\sim\overline{p}(\tau)}[\frac{p_{\theta}(\tau)}{\overline{p}(\tau)}r(\tau)]$ ,其中 $\frac{p_{\theta}(\tau)}{\overline{p}(\tau)}=\frac{p(s_{0})\prod_{t=0}^{T-1}\pi_{\theta}(a_{t}|s_{t})p(s_{t+1},r_{t}|s_{t},a_{t})}{p(s_{0})\prod_{t=0}^{T-1}\overline{\pi}(a_{t}|s_{t})p(s_{t+1},r_{t}|s_{t},a_{t})}=\frac{\prod_{t=0}^{T-1}\pi_{\theta}(a_{t}|s_{t})}{\prod_{t=0}^{T-1}\overline{\pi}(a_{t}|s_{t})}$ ,由此依靠来自其他策略的轨迹计算当前策略的梯度即可( $\overline{\pi}(a_{t}|s_{t})$ 可能需要通过统计轨道采样过程来计算)
<br>

Actor-Critic,演员-评论家学习:
    1. 可以使用类似基线的方法进行优化: $B(s_{i,t})=V(s_{i,t}),V^{\pi}(s_{t})=E_{a_{t}\sim \pi_{\theta}(a_{t}|s_{t})}[Q^{\pi}(s_{t},a_{t})],A^{\pi}(s_{t},a_{t})=Q^{\pi}(s_{t},a_{t})-V^{\pi}(s_{t})$ ,即将 Q 的期望(均值)作为当前状态的价值,将减去价值的 Q 作为操作的回报.可以使用 MC,TD 等方式计算价值
    2. 具体过程: (1) 从当前策略(演员, $\pi_{\theta}(a|s)$ )中取样并运行得到操作 $\{s_{i},a_{i}\}$ (2) 将 $\hat{V}_{\phi}^{\pi}(s)$ 设为状态 $s$ 下得到的奖励总和(如果使用 TD 学习价值,使用记录重放,目标 $r+\gamma\hat{V}_{\phi}^{\pi}(s')$ 和一定的学习率更新 $\hat{V}_{\phi}^{\pi}$ ) (3) 评价操作的回报 $\hat{A}^{\pi}(s_{i},a_{i})=r(s_{i},a_{i})+\hat{V}^{\pi}_{\phi}(s_{i'})-\hat{V}_{\phi}^{\pi}(s_{i})$ (4) $\nabla_{\theta}J(\theta)\approx\sum_{i}^{}\nabla_{\theta}\log \pi_{\theta}(a_{i}|s_{i})\hat{A}^{\pi}(s_{i},a_{i})$ (5) $\theta\leftarrow\theta+\alpha\nabla_{\theta}J(\theta)$ 
    3. Design Choices,一些设计选择,使用多输出头神经网络同时拟合价值和决策(可能导致互相影响?演员和评论家的拟合应分离),使用多采样同时进行训练(Asynchronous Advanced Actor Critic(A3C),异步优势演员评论家),或者使用自己的历史记录 (TD学习目标 $r_{i}+\gamma \hat{V}_{\phi}^{\pi}(s_{i}')$ 和产生的 $\hat{A}^{\pi}$ 都是离策略的,处理策略 $\pi$ 时从 $a_{i}\pi\sim \pi_{\theta}(a|s_{i})$ 取样)(TD 学习中的状态分布不一致,但是不要紧?)
    4. 缺陷,大步长带来的坏决策,决策-取样-决策恶性循环,小步长带来的学习缓慢,随机策略的非必要性,分布式训练等
<br>

Continuous action,连续决策:
    1. 通常的策略选择 $\argmax$ 难以解决连续的决策空间
    2. 直接将决策按一定的分布离散化会损失信息，可以使用交叉熵(Cross-entropy method(CEM),使用参数维护一个连续分布,通常是高斯分布,通过选择按该分布采样最佳的前 $k$ 个参数子集更新分布)或者协方差矩阵适应演化策略(Covariance Matrix Adaptation Evolution Strategy,CMA-ES,一种基于种群的进化策略(Evolution Strategy, ES)的优化算法)
    3. 也可以将 Q 值输出变为 Q 函数输出,输出一个状态估价 $V_{\phi}(s)$ 和一个理想决策 $\mu_{\phi}(s)$ 以及拟合矩阵 $P_{\phi}(s)$ , $Q_{\phi}(s,a)=-\frac{1}{2}(a-\mu_{\phi}(s))^{T}P_{\phi}(s)(a-\mu_{\phi}(s))+V_{\phi}(s)$ 即二阶 Taylor 展式形式,以此获得一个连续的分布对应决策空间(可能表现力不足?)
    4. 直接用神经网络拟合对 Q 取 $\argmax$ 的过程,即训练 $\mu_{\theta}(s)=\argmax_{a}(s,a),\frac{\mathrm{d}Q_{\phi}}{\mathrm{d}\theta}=\frac{\mathrm{d}a }{\mathrm{d}\theta}\frac{\mathrm{d}Q_{\phi}}{\mathrm{d}a}$ ,使用目标 $r_{j}+\gamma Q_{\phi'}(s_{j}',\mu_{\theta}(s_{j}'))$ 进行更新
<br>

Deep Deterministic Policy Gradient (DDPG),深度确定性策略梯度学习:
    1. 进行一些决策,并记录 $B:\{(s_{i},a_{i},s'_{i},r_{i})\}$ 
    2. 从 $B$ 中随机取样
    3. 计算目标 $y_{j}=r_{j}+\gamma Q_{\phi'}(s_{j}',\mu_{\theta'}(s_{j}'))$ ,使用目标网络 $Q_{\phi'}$ 和 $\mu_{\theta'}$ 
    4. $\phi\leftarrow -\alpha \sum_{j}^{}\frac{\mathrm{d}Q_{\phi}}{\mathrm{d}\phi}(s_{j},a_{j})(Q_{\phi}(s_{j},a_{j})-y_{i})$ 
    5. $\theta\leftarrow\theta+\beta\sum_{j}^{}\frac{\mathrm{d}\mu}{\mathrm{d}\theta}(s_{j})\frac{\mathrm{d}Q_{\phi}}{\mathrm{d}a}(s_{j},\mu(s_{j}))$ 
    6. 定时更新目标网络 $\phi',\theta'$ 
    7. 一些问题,如 Q 值高估问题仍存在,评价可能不稳定(评价函数不平滑,出现尖峰或悬崖)
<br>

Twin Delayed DDPG (TD3),双延迟 DDPG:
    1. 使用两个独立网络生成 Q 值取最小,应对 Q 值高估问题
    2. 延迟策略(Q?)网络更新,降低更新频率,应对评估不稳定问题
    3. Target Policy Smoothing,目标策略平滑化,向策略添加噪声(涂抹)以平滑输出值,模拟在最佳决策附近的操作,应对评价函数不平滑问题
    4. 进一步的有 Soft Actor-critic(SAC) 算法,使用温度控制探索/利用平衡
<br>

Proximal Policy Optimization (PPO),近端策略优化,简化的 trust-region policy optimization (TRPO),信任区域策略优化:
    1. KL Divergence,KL 分歧,评价两个概率分布间的差异的算法, $D_{KL}(P||Q)=\sum_{x\in \mathrm{X}}^{}P(x)\log(\frac{P(x)}{Q(x)})$ 
    2. PPO 仅仅使用很短时间之内的历史记录,常被看作基于策略的(或者"偏策略的")
    3. Adaptive KL Penalty,自适应 KL 惩罚,调整 $\theta$ 以最大化 $\hat{\mathbb{E}}_{t}[\frac{\pi_{\theta}(a_{t}|s_{t})}{\pi_{\theta_{old}}(a_{t}|s_{t})}\hat{A}_{t}]-\beta \hat{\mathbb{E}}_{t}[\operatorname{KL}[\pi_{\theta_{old}}(\cdot |s_{t}),\pi_{\theta}(\cdot |s_{t})]]$ ,参数 $\beta$ 难以确定,可以计算 $d$ 即公式中 $\operatorname{KL}$ 分歧的期望,以与目标相差 1.5 倍为界将 $\beta$ 乘 2 或除 2.
    4. PPO with Clipped Objective,裁剪目标 $r_{t}(\theta)=\frac{\pi_{\theta} (a_{t}|s_{t})}{\pi_{\theta_{old}}(a_{t}|s_{t})}$ 到 $(1-\epsilon,1+\epsilon)$ 之内,计算其与操作价值 $\hat{A}_{t}$ 的积,与未裁剪的积取最小值,作为裁剪后的损失梯度取样计算期望回传
    5. PPO in Practice,实际应用中的 PPO,在损失中加入价值函数的平方误差损失(使用 MC 或 TD 计算价值后对输出的价值做平方损失)以提高价值估值的准确性,和熵正则化项 $S[\pi_{\theta}](s_{t})$ 来鼓励探索
    6. 探索经验值:使用裁剪目标 Clipping, $\epsilon=0.2$ 
    7. 面对实际工程,选择 PPO
    8. 从策略梯度开始,策略梯度实际上是策略迭代的"软化"版本(即分步多次更新 $J(\theta')-J(\theta)=E_{\tau\sim p_{\theta'}(\tau)}[\sum_{t}^{}\gamma^{t}A^{\pi_{\theta}}(s_{t},a_{t})]$ ),又因为要更新的值等于 $\sum_{t}^{}E_{s_{t}\sim p_{\theta'}(s_{t})}[E_{a_{t}\sim \pi_{\theta'}}[\gamma^{t}A^{\pi_{\theta}}(s_{t},a_{t})]]$ ,通过替换期望分布,又等于 $\sum_{t}^{}E_{s_{t}\sim p_{\theta'}(s_{t})}[E_{a_{t}\sim \pi_{\theta}(a_{t},s_{t})}[\frac{\pi_{\theta'}(a_{t}|s_{t})}{\pi_{\theta}(a_{t}|s_{t})}\gamma^{t}A^{\pi_{\theta}(s_{t},a_{t})}]]$ (因此可以通过重要性采样进行学习).再通过控制 $\pi_{\theta}$ 和 $\pi_{\theta'}$ 之间的距离,使得 $p_{\theta}(s_{t})$ 接近于 $p_{\theta'}(s_{t})$ ,从而忽略最外层分布的差异(应当为 $s_{t}\sim p_{\theta'}(s_{t})$ ,但 $\theta'$ 还未计算出来,因此用 $s_{t}\sim p_{\theta}(s_{t})$ 代替),总之经过一堆推导 $\operatorname{KL}$ 惩罚可以使得策略分布差异足够小,但不能保证参数 $\theta$ 的差异足够小,在 $\operatorname{KL}$ 限制下可以进行线性化(?没看懂)
<br>

#### 附加概念

Model-based Planning,基于模型的计划
    1. Open loop control/planning,开环控制/计划,执行决策时不再观测新信息.计划就是猜测然后检查,称随机射击(random shooting),其优化就是交叉熵方法,利用选择的优势决策更新猜测分布(高斯等)
    2. Closed loop control,闭环控制,执行决策的同时使用信息更新
    3. 离散规划方法: Monte-Carlo Tree Search,蒙特卡洛树搜索
    4. 可以利用导数进行轨迹优化
    5. Linear Quadratic Regulator,线性二次调节器?
<br>

Model-based RL with learned models,基于学习模型的强化学习

Model-based RL with Images,基于图片模型的强化学习

Imitation Learning,模仿学习

Special settings in Learning from Demonstration,从示范中学习的特殊设置

#### 参考文献

价值迭代神经网络,Value Iteration Networks, NeurIPS 2016 Best Paper, A. Tamar, Y.  Wu, G. Thomas, S. Levine, P.  Abbeel

利用隐式微分扩大和稳定可微分规划,Scaling up and Stabilizing Differentiable Planning with Implicit Differentiation, L. Zhao, H. Xu, L. Wong, ICLR’23

连续决策中的价值迭代,Value Iteration in Continuous Actions, States and Time, M. Lutter, S. Mannor, J. Peters, D. Fox, A. Garg, ICML’21

真在线 TD,True Online TD, ICML’14, H. Seijen, R. Sutton, they use a different Eligibility Trace

利用广义优势估计进行高维连续控制,High-Dimensional Continuous Control Using Generalized Advantage Estimation, J. Schulman, P.  Moritz, S. Levine, M. Jordan, P.  Abbeel, ICLR’16

近端策略优化算法,Proximal Policy Optimization Algorithms, J. Schulman et al, never published.. But it is the most influential DRL paper!

通过自我监督预测进行好奇心驱动的探索,Curiosity-driven Exploration by Self-supervised Prediction, Pathak et al., ICML’17

通过随机网络蒸馏进行探索,Exploration by Random Network Distillation, Burda et al, ICLR’19

NovelD：简单而有效的探索标准,NovelD: A Simple yet Effective Exploration Criterion, Zhang et al., NeurIPS’21

论随机迭代动态编程算法的收敛性,Tommi Jaakkola, Michael I. Jordan, and Satinder P. Singh. On the convergence of stochastic iterative dynamic programming algorithms. Neural Computation, 6(6):1185–1201, 1994.

R3M：用于机器人操纵的通用视觉表示法,R3M: A Universal Visual Representation for Robot Manipulation, Nair et al., CoRL’22

预训练图像编码器，用于通用视觉强化学习的预训练图像编码器,Pre-Trained Image Encoder for Generalizable Visual Reinforcement Learning, Yuan et al., NeurIPS’22

克服神经网络值函数逼近的频谱偏差,Overcoming the Spectral Bias of Neural Value Approximation, Yang et al., ICLR’2