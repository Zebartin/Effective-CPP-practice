# Item 6: Explicitly disallow the use of compiler-generated functions you do not want.

## 特殊需求

有的类可能天然地具有**唯一性**，也就是说，复制构造函数或拷贝赋值操作符在这样的类里是非法的。

## 简单的解决方式

显式声明这些可能会自动生成的函数，但用`private`来修饰。但只是这样，成员函数和友元函数仍然可以访问它们。

## 更进一步

只声明，而**不定义**。同时，由于没有定义，参数名也可以统统省略，函数声明中只剩下参数类型。这种技巧在C++的输入输出流库中比较常见，比如`ios_base`、`basic_ios`以及`sentry`这几个类。

## 终极目标

以上的技巧能让链接过程出现错误，如何把报错时机从链接提前到编译？定义一个基类来阻止复制：

```cpp
class Uncopyable {
protected:
    Uncopyable() {}
    ~Uncopyable() {}
private:
    Uncopyable(const Uncopyable&);
    Uncopyable& operator=(const Uncopyable&);
};
```

然后继承它即可：

```cpp
class HomeForSale: private Uncopyable {
    ...
};
```

这种技巧有时用起来会比较微妙：

- 从`Uncopyable`的继承不必是`public`的
- `Uncopyable`的析构函数不必是虚函数
- 由于`Uncopyable`中不包含数据，可以对它进行空基类的优化（Item 39），但可能会因此导致多重继承（Item 40），而多重继承可能会使得空基类优化失效

另外，也可以直接使用Boost库中的`noncopyable`类，虽然名字有点古怪。

## 总结

为了禁止编译器自动生成的函数，可以在将其声明为`private`的同时，不给出实现。其中一种可取方式是使用像`Uncopyable`这样的类。
