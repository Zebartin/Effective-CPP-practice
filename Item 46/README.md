# Item 46: Define non-member functions inside templates when type conversions are desired.

将Item 24中的例子改造成模板：

```cpp
template <typename T>
class Rational
{
public:
    Rational(const T &numerator = 0, const T &denominator = 1);
    const T numerator() const;
    const T denominator() const;
    ...
};
template <typename T>
const Rational<T> operator*(const Rational<T> &lhs, const Rational<T> &rhs)
{ ... }
```

与Item 24一样，想要支持：

```cpp
Rational<int> oneHalf(1, 2);
Rational<int> result = oneHalf * 2;
```

但这样子会编译失败。

## 问题根源

编译器无法推断出函数模板中的模板参数`T`应该是什么，因为在模板参数推断这个阶段，编译器不会考虑通过构造函数进行的隐式类型转换。

对于`oneHalf`，编译器能推断出`T`是`int`；但对于第二个参数，`int`类型的`2`，由于不考虑隐式转换，编译器无法确定`T`是什么。

## 将函数放入类中

为了避免推断函数模板的参数，将这个函数放入类中。这样一来，当这个类模板实例化的时候，这个函数也会随之实例化，于是就不用推断模板参数了。而根据Item 24，这个函数理应是非成员函数，因此需要将它声明为友元函数。

这里使用友元，跟访问类的非public部分毫无关系，只是因为使用友元是在类中定义非成员函数的唯一途径。

值得一提的是，在类模板中，可以用类模板名来代替类模板名+模板参数，所以可以如此简写：

```cpp
friend const Rational operator*(const Rational& lhs,
const Rational& rhs);
```

到这一步为止，代码能够成功编译，但其链接会失败。因为这里只提供了函数声明，链接器找不到对应的定义。

## 避免链接问题

直接简单的做法是直接把函数体和声明放在一起。这样一来，这个函数就是隐式内联的。

如果这个函数的实现比较短小，内联是很合适的；但如果不适合内联，则可以让这个函数调用一个非成员helper函数。

作为一个模板，helper函数不支持隐式类型转换，但它不需要，这部分工作由类模板中的那个函数负责，两者各司其职，协同工作。

## 总结

在写类模板时，如果有函数模板需要在所有参数上支持隐式类型转换，将这个函数模板作为友元函数放在类模板中。
