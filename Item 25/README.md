# Item 25: Consider support for a non-throwing `swap`.

## 默认的`std::swap`

默认的`std::swap`实现大概是这样的：

```cpp
namespace std {
    template<typename T>
    void swap(T& a, T& b) {
        T temp(a);
        a = b;
        b = temp;
    }
}
```

在其中会调用三次拷贝函数，可能会导致效率比较低。

## 自定义`swap`

有时候默认的`swap`不能满足要求，比如默认实现中的一些拷贝可能是没有必要的。一个关于pimpl idiom（pointer to implementation，[Item 31](../Item%2031)）的例子是这样的：

```cpp
class WidgetImpl {
public: 
    ...
private:
    int a, b, c;
    std::vector<double> v;
    ...
};

class Widget {
public:
    Widget(const Widget& rhs);
    Widget& operator=(const Widget& rhs)
    {
        ...
        *pImpl = *(rhs.pImpl);
        ...
    }
    ...
private:
    WidgetImpl *pImpl;
};
```

此时`Widget`的拷贝代价会很大，但其实没有必要，交换两者指针就可以了。

### 自定义`swap`成员函数

由于交换时需要访问对象的私有指针，所以`swap`应是一个成员函数。

```cpp
class Widget {
public:
    ...
    void swap(Widget& other) {
        using std::swap;
        swap(pImpl, other.pImpl);
    }
    ...
};
```

### 自定义`swap`非成员函数

然后在同一个命名空间下，声明一个非成员函数`swap`，在其中调用成员函数`swap`，使其重载`std::swap`。

### 模板特化

同时，如果这个类是普通类而非类模板，为了避免用户错误地在这个类上使用`std::swap`，在`std`中对`swap`函数模板进行模板特化：

```cpp
namespace std {
    template<>
    void swap<Widget>(Widget& a, Widget& b)
    {
        a.swap(b);
    }
}
```

如果是类模板，由于C++不允许对函数模板的部分特化，所以只能止步于上一步。

## 用户使用`swap`时

期待的行为：

1. 优先调用全局或同一命名空间下满足特定类型的`swap`；
2. 调用`std`中的`swap`，优先模板特化的`swap`。

使用`using std::swap`，使得编译器能找到它：

```cpp
template<typename T>
void doSomething(T& obj1, T& obj2) {
    using std::swap;
    ...
    swap(obj1, obj2);
    ...
}
```

不能直接调用`std::swap(obj1, obj2)`。

## 不要在`swap`成员函数中抛出异常

`swap`成员函数最大的应用就是帮助类和类模板提供异常安全的保证，前提是它不会抛出异常。

当你在自定义`swap`时，通常来说你是在提供一个更为有效的方式来交换数据；同时这个过程不会发生异常。一般来说，`swap`的这两个特点是同时存在的，因为高效的`swap`几乎总是基于内置类型的操作（比如指针），而这些操作是永远不会发生异常的。

## 总结

- 当`std::swap`不适用时，提供一个自定义的`swap`成员函数，并保证这个函数中不会抛出异常；
- 如果你提供了一个`swap`成员函数，也同时提供一个非成员的`swap`函数，在其中调用`swap`成员函数。对于类（不包括类模板），同时也使`std::swap`模板特化；
- 调用`swap`时，先声明`using std::swap`，然后不指明命名空间地调用`swap`；
- 为用户类特殊化某个`std`模板是可以的，但永远不要尝试往`std`里加入全新的东西。
