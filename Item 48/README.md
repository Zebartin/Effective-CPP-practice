# Item 48: Be aware of template metaprogramming.

模板元编程（template metaprogramming，TMP）可以写出在编译期间运行的、基于模板的C++代码。

C++并没有专门为TMP设计。TMP是被发现而非被发明的。

## TMP的两大优势

### 运行时性能更好

TMP把运行时的工作量转移到编译阶段带来了以下好处：

- 一些错误能被提前发现；
- 可执行文件更小；
- 运行时耗时更短；
- 内存需求更小。

另一方面，使用TMP的程序会花更长时间在编译上。

比如说，[Item 47](../Item%2047)中的基于traits classes和重载函数的实现就比基于`typeid`的实现性能更好。

### 能轻易实现非TMP难以实现的事情

同样是[Item 47](../Item%2047)，基于`typeid`的实现实际上是存在编译问题的。如果有代码调用`advance`的时候，传入的类型不是随机访问迭代器，那么`iter += d;`这一语句将会编译失败。

而TMP则能轻易避开这一个问题。

## TMP是图灵完备的

TMP已经被证明是图灵完备的，这意味着能够用它来计算任何东西。在TMP中，可以声明变量，可以进行循环，可以写和调用函数，等等，但它们和普通的C++会有很大差别。例如[Item 47](../Item%2047)中的条件判断的汇编级别的TMP。TMP的一些库（比如Boost的MPL）提供高层次的语法，不过还是和普通的C++存在明显差别。

## TMP中的循环

TMP中没有真正的循环，取而代之的是递归，而且不是普通的递归函数，而是模板的递归实例化。

```cpp
template <unsigned n>
struct Factorial
{
    enum { value = n * Factorial<n - 1>::value };
};
template <>
struct Factorial<0>
{
    enum { value = 1 };
};
```

这里使用了the enum hack（见[Item 2](../Item%2002)）。

## TMP的更多用途

### 保证量纲单位的正确性

使用TMP可以在编译阶段保证量纲单位的正确性，无论计算公式有多么复杂。

有趣的是，TMP能够支持分数的量纲指数（fractional dimensional exponent）。这要求分数能在编译阶段被约分，比如<img src="https://render.githubusercontent.com/render/math?math=\text{time}^{\frac{1}{2}}">与<img src="https://render.githubusercontent.com/render/math?math=\text{time}^{\frac{4}{8}}">相同。

### 优化矩阵运算

```cpp
typedef SquareMatrix<double, 10000> BigMatrix;
BigMatrix m1, m2, m3, m4, m5;
...
BigMatrix result = m1 * m2 * m3 * m4 * m5;
```

如上普通的矩阵计算中，会产生4个临时矩阵，每个`*`号产生一个。而且互相独立的乘法会产生四次循环。

使用TMP的高阶技术expression templates，可以消去这些临时矩阵，并合并循环。同时不必修改上述代码中的语法。这样的代码将使用更少的内存，运行也会快很多。

### 生成设计模式的自定义实现

策略（strategy）、观察者（observer）、访问者（visitor）等设计模式可以有很多实现方式。基于TMP的policy-based design技术可以创建代表着独立设计选项（policies）的模板，这些policies可以以任意方式组合，从而得到自定义行为的模式实现。

比如说，这种技术被用来从几个智能指针的policies模板中生成数百个不同的智能指针类型。这种通用技术是generative programming的基础。

## TMP的前景

TMP不适合所有人，它的语法反直觉，并且工具支持很弱。

尽管TMP可能永远不会成为主流，但它很重要，而且对TMP的支持也在变得越来越好。

## 总结

- 模板元编程（template metaprogramming，TMP）能将运行时的工作量转移到编译阶段，从而使得更早的异常检测和更高的运行时性能成为可能；
- TMP能够被用来生成基于策略选择的结合的自定义代码，也可以用于避免生成不适合特定类型的代码。
