# Item 21: Don’t try to return a reference when you must return an object.

如果要返回一个引用，那这个引用就应该指向某个已存在的对象，而没有理由假设用户在函数体外创建了这样的对象，因此只能在函数内部中创建新对象，而这有两种方式：

- 在栈顶创建新对象：然而函数体结束后这样的对象会被销毁；
- 在堆上创建新对象：什么时候`delete`？

不管怎样，且不说是否可行，两者都避不开创建新对象。尝试这样避免：

- 在函数内部声明静态对象：函数返回同一个静态对象的引用不合理吧？
- 单个静态对象 -> 静态对象数组：数组大小如何选择？如何以微小的代价进行数组元素移动或赋值？

这些尝试都是徒劳无用的。正确方式是，该返回一个新对象时，就返回一个新对象。

不用在意这样做可能会带来的构造和析构开销，让编译器去优化处理吧。

## 总结

如果需要用到多个类似的目标对象，永远不要在函数中返回某个局部栈对象的指针或引用，某个分配于堆中的对象的引用，或者某个局部静态对象的指针或引用。

[Item 4](../Item%2004)中提供了一个不适用这个原则的例子，它在某个函数返回了一个局部静态对象的引用。

<a href="../Item%2020"><-</a>

<a href="../Item%2022">-></a>
