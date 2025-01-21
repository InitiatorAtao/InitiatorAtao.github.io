### iterator

迭代器提供对集合(容器)元素的操作能力,统一的迭代器接口允许C++程序以统一的方式处理不同的数据结构.迭代器充当容器和通用算法之间的中介,使得算法无需针对特定数据类型进行操作.在本题中,你需要设计一个简化的迭代器及其对应的容器类,实现其基本功能与拓展接口.

#### 一:基本操作 (40pts)

为简化实现,将容器类 `class Container<T>` 设置为简单的可变长线性存储容器,类似于 `std::vector<T>` ,容器可以返回指向其第一个元素的迭代器 `Iterator<T> Container<T>::begin()`,以及指向最后一个元素之后的虚拟元素的迭代器 `Iterator<T> Container<T>::end()` .迭代器类 `class Iterator<T>` 需要支持左右自增运算符,迭代器之间的相等与不等判断,以及返回指向元素引用的取址运算符.

为实现容器的基本功能,你还需要在容器类中实现一个基本的 `void Contatiner<T>::push_back(const T&)` 函数,作用是将一个元素加入容器末尾.

#### 二:汇聚操作 (60pts)

汇聚操作是对迭代器所代表的容器元素整体的操作,调用后会立即得到结果,是迭代器提供的函数式编程功能的一个组成部分,在本部分中,你需要实现以下功能:

`Container<T> Iterator<T>::collect(Iterator<T>)` :将迭代器到给定尾后迭代器之间的元素集合变为一个具体的容器.

`bool Iterator<T>::any(Iterator<T>,std::function<bool(T)>)` :返回迭代器到给定尾后迭代器之间是否有使得传入函数返回值为真的元素.

`bool Iterator<T>::all(Iterator<T>,std::function<bool(T)>)` :返回指定区间的元素是否全部使得传入函数返回值为真.

`R Iterator<T>::fold(Iterator<T>,R,std::function<R(R,T)>)` :将指定区间的元素使用给定初始值和累加函数计算累加值.

为保证模板推断成功,你可能需要将所有类的实现放在 `iterator.h` 的尾部而不是新的 `iterator.cpp` 文件