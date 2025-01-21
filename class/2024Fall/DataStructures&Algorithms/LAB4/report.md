## CST LAB 4-1 BBST

### 数据结构实现过程及复杂度分析

#### AVL

使用类模板 `template<typename K,typename V>class AVL` ,接口函数 `V& insert(K key,V value)`,`V remove(K key)`,`K lower_bound(K key)` 对应插入,删除和 lower_bound. 实现上,插入时根据 BST 性质进行查询,有重复元素则返回,否则在查询的 `_hot` 节点上挂载新节点,然后从 `_hot` 到根查询失衡,如有则进行 3+4 重构.删除同理查询,删节点,查询失衡并 3+4 重构.lower_bound 即进行一次查询,对所有访问到的且值小于等于查询值的节点取最大值返回.

复杂度分析:插入和删除只会产生一条长度不超过树高的查询和更新链,同时 3+4 重构保证树高为 $O(\log n)$ 级别,故单次操作复杂度维持在 $O(\log n)$.lower_bound 操作只会产生长度不超过树高的查询链,由于树高得到保证,单次操作复杂度也是 $O(\log n)$.由此总时间复杂度为 $O(n\log n)$.

#### Splay

使用类模板 `template<typename K,typename V>class Splay`,接口函数 `V& operator[](K key)`,`bool erase(K key)`,`K lower_bound(K key)` 插入,删除和 lower_bound.实现上,插入时 BST 查询,重复则翻转到根返回,否则挂载新节点并将其翻转到根.删除进行查询并翻转到根后,将根节点的后继翻转到根再做一次查询,此时可以合并根节点的左右子树并删除根.lower_bound 则可以直接进行一次查询,根据结果返回根节点或其前驱即可.

复杂度分析:插入和删除操作的复杂度由势能分析可知其为均摊 $O(\log n)$ 的.lower_bound 操作相当于一次查询和一次查询根节点的前驱,同理可以进行势能分析得到均摊 $O(\log n)$ 的复杂度,由此总时间复杂度为 $O(n\log n)$.

### 设计测例思路

使用不同的 $n$ 大小,不同的操作生成方式 (随机生成,生成大量的插入操作后的少量查询操作等),加入随机方法,重复生成多组测试数据.

### 测例生成方式

见附件中的 `data_test.py`, 使用 python 中的 `sortedcontainers` 第三方库,`random` 库进行数据生成和随机化,实现了随机操作和大量插入操作后的少量查询操作两种数据生成器,同时生成标准答案并进行测试和时间记录.

### 性能描述及原因分析

运行 `data_test.py` 会自动生成如下 `.csv` 统计文件:

|$n$|数据生成器|AVL 平均运行时|Splay 平均运行时|
|-|-|-|-|
|100|gen_random|0.031676745|0.032927775
|100|gen_one_query_after_all_inserted|0.033626604|0.033809233
|1000|gen_random|0.034003758|0.034061646
|1000|gen_one_query_after_all_inserted|0.034829211|0.03489778
|10000|gen_random|0.041074276|0.042864394
|10000|gen_one_query_after_all_inserted|0.043054128|0.045546961
|100000|gen_random|0.113497043|0.138315344
|100000|gen_one_query_after_all_inserted|0.143975449|0.178490973
|1000000|gen_random|0.918196511|1.209429693
|1000000|gen_one_query_after_all_inserted|1.915073752|2.393290854

可见两种实现的用时随数据规模增长较缓慢,大致符合线性对数的增长趋势,且 `gen_one_query_after_all_inserted` 数据生成器造成的耗时要长于 `gen_random` 数据生成器,这应当是因为大量插入操作造成的树规模增长导致.Splay 的用时长于 AVL,推测是翻转操作带来了一定时间常数.以上的定性时间比较在数据规模增大时愈发明显.
