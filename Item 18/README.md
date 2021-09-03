# Item 18: Make interfaces easy to use correctly and hard to use incorrectly.

理想的接口设计下，如果尝试的接口使用方式不能做到用户期望的事情，代码从一开始就不能够编译成功；而如果编译成功了，则意味着代码所做的正是用户想要的。

## 恰当的封装

要做到理想的接口设计，就需要考虑到用户可能犯下的各种错误。比如假设你在设计一个表示日期的类：

```cpp
class Date
{
public:
	Date(int month, int day, int year);
	...
};
```

乍一看没什么毛病，但用户仍然可能会犯下奇怪的错误：

- 使用时参数顺序不对；
- 参数超出有效范围，比如13月、40日。

很多用户错误可以通过引进新类型来解决。对于第一个错误，可以用带有`explicit`构造函数的`struct`将`Day`、`Month`、`Year`封装起来，从而在参数中区分顺序。通常来说使用`class`效果更好，但仅针对这个错误的话，`struct`已经够用了。

对于第二个错误，则需要通过进一步封装限制取值范围。比如说可以用`enum`来代表月份，但这样并不type-safe，因为如[Item 2](../Item%2002)的the enum hack所示，`enum`可以被直接当做`int`使用。更安全的解决方式使用一系列有效的静态函数代替：

```cpp
class Month {
public:
    static Month Jan() { return Month(1); }
    ...
    static Month Dec() { return Month(12); }
private:
    explicit Month(int m);
};
```

> 为什么要用函数而非对象？注意回顾[Item 4](../Item%2004)

## 保持一致

与内置类型兼容有利于保持接口的一致性。这个问题在[Item 3](../Item%2003)中也有提及，通过`const`的返回值来避免意外赋值。

STL容器接口的一致性使其简单易用。

## 不要指望用户

如果一个接口指望用户记得做点什么事，就很容易会出事。比如[Item 13](../Item%2013)中的工厂函数：

```cpp
Investment* createInvestment();
```

它希望用户在拿到指针后移交给`tr1::shared_ptr`对其进行管理，但这是不可靠的，直接返回`tr1::shared_ptr`更合理。

这种方式同时也避免了很多其他潜在的使用错误。假设`Investment`资源要求使用特殊的自定义deleter而非`delete`，比如`getRidOfInvestment`，用户可能会使用错误的deleter。这种情况下可以让工厂函数直接返回设置了正确deleter的`tr1::shared_ptr`。

```cpp
std::tr1::shared_ptr<Investment> createInvestment()
{
    std::tr1::shared_ptr<Investment> retVal(static_cast<Investment*>(0), getRidOfInvestment);
    ...				// make retVal point to the correct object
    return retVal;
}
```

- 使用`static_cast`将0转换为指针是因为`tr1::shared_ptr`的构造函数要求一个真实的指针；
- 根据[Item 26](../Item%2026)，如果情况允许，直接用正确的指针初始化`tr1::shared_ptr`更好。

## 跨DLL问题

`tr1::shared_ptr`还有另一个很棒的特性，使得它解决了跨DLL问题。DLL是动态链接库，dynamic linked library。

当一个对象在一个DLL中通过`new`创建，但在另一个DLL中销毁时，跨DLL问题就会发生。

`tr1::shared_ptr`避免了这个问题，因为它的默认deleter会使用创建它的DLL中的`delete`。

## 总结

- 好接口想要正确使用很容易，想要错误使用很困难。你应该在你的所有接口中努力实现这一点；
- 保证接口的一致性，行为上与内置类型兼容，有助于接口的正确使用；
- 避免错误的方法有，创造新类型（封装），限制类型的操作方式，限制对象的取值，以及消除用户需要为资源管理承担的责任；
- `tr1::shared_ptr`支持自定义删除操作。这防止了跨DLL的问题，能够用来实现自动解锁互斥锁。

<a href="../Item%2017"><-</a>

<a href="../Item%2019">-></a>
