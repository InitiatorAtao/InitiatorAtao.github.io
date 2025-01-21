### PPO function

1. sample 直接使用当前策略进行给定步数的游戏,记录下轨迹,还要记录下选择的策略的对数概率,使用 $TD(\lambda)$ 等计算 advantages.

2. train 从 sample 中取小样本计算