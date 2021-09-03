# Item 45: Use member function templates to accept “all compatible types.”

## 通用复制构造函数

智能指针（smart pointer）提供了内置指针做不到的功能，比如`auto_ptr`、`tr1::shared_ptr`以及STL中的迭代器。

内置指针拥有的一个很好的功能是，它支持隐式类型转换：派生类指针能够隐式转换为基类指针，指向非const对象的指针能转换为指向const对象的指针。

我们希望智能指针也能做到类似的事情：

```cpp
template <typename T>
class SmartPtr
{
public:
    explicit SmartPtr(T *realPtr);
    ...
};
SmartPtr<Top> pt1 =
    SmartPtr<Middle>(new Middle);
SmartPtr<Top> pt2 =
    SmartPtr<Bottom>(new Bottom);
SmartPtr<const Top> pct2 = pt1;
```

一个一个为每对继承关系手工定义构造函数是不现实的，应使用构造函数模板：

```cpp
template <typename T>
class SmartPtr
{
public:
    template <typename U>
    SmartPtr(const SmartPtr<U> &other);
    ...
};
```

这种构造函数模板有时也被称为通用复制构造函数（generalized copy constructors）。为了支持隐式转换，上面的代码有意没有将它声明为`explicit`。

然而，单单只是这种声明，并不能把类型转换限制在可兼容的转换之中，比如派生类到基类、非const到const，而非反过来。为此需要在实现上作进一步限制：

```cpp
template <typename T>
class SmartPtr
{
public:
    template <typename U>
    SmartPtr(const SmartPtr<U> &other)
    : heldPtr(other.get()){...}
    T * get() const { return heldPtr; }
    ...
private:
    T *heldPtr;
};
```

这里假定`SmartPtr`跟`auto_ptr`等一样，也提供一个`get`成员函数来获取相应的内置指针，利用这个指针的默认行为，就可以限制类型转换。

## 通用赋值函数

成员函数模板的用处不只是在构造函数上。比如说，`tr1::shared_ptr`不仅在构造函数上支持所有可兼容内置指针、`tr1::shared_ptr`、`auto_ptr`以及`tr1::weak_ptr`，还在赋值上支持除`tr1::weak_ptr`以外的所有指针。这是它的声明节选：

```cpp
template <class T>
class shared_ptr
{
public:
    template <class Y>
    explicit shared_ptr(Y *p);
    template <class Y>
    shared_ptr(shared_ptr<Y> const &r);
    template <class Y>
    explicit shared_ptr(weak_ptr<Y> const &r);
    template <class Y>
    explicit shared_ptr(auto_ptr<Y> &r);
    
    template <class Y>
    shared_ptr &operator=(shared_ptr<Y> const &r);
    template <class Y>
    shared_ptr &operator=(auto_ptr<Y> &r);
};
```

- 所有构造函数都是`explicit`，除了需要支持隐式转换的通用复制构造函数；
- 只有`auto_ptr`是不带`const`的，因为它的复制比较特殊。

## 通用拷贝函数不包括“正常”拷贝函数在内

在上述代码中，如果`T`和`Y`相同，编译器会选择自动生成一个默认的拷贝函数。为了避免这种情况，需要自行声明“正常的”拷贝函数。所以，`tr1::shared_ptr`的定义中也包含了以下两个拷贝函数：

```cpp
shared_ptr(shared_ptr const &r);
shared_ptr &operator=(shared_ptr const &r);
```

## 总结

- 使用成员函数模板，可以生成能够接受所有可兼容类型的函数；
- 即使你为通用复制构造函数和通用赋值函数声明了成员模板，你仍然需要声明“正常的”复制构造函数和拷贝赋值操作符。
