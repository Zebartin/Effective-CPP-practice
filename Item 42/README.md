# Item 42: Understand the two meanings of `typename`.

在声明模板参数时，`class`和`typename`没有差别，可以互相交换。然而，两者并不总是等价的。

## 模板中的两种名称

```cpp
template <typename C>
void print2nd(const C &container)
{
    if (container.size() >= 2)
    {
        C::const_iterator iter(container.begin()); // can not compile
        ++iter;
        int value = *iter;
        std::cout << value;
    }
}
```

在上述代码中，有两种类型的名称：

- `value`的类型是`int`，它与模板参数无关，是一个非待决名（non-dependent name）；
- `C::const_iterator`依赖于模板参数，被称作待决名（dependent name）。特别地，它还是一个嵌套待决类型名（nested dependent type name）。

嵌套待决名会导致解析的问题，编译器难以分辨它是类型名称还是成员变量名称，比如下述代码，可以被理解为两个变量相乘，也可以被理解为声明一个指针：

```cpp
C::const_iterator * x;
```

此时需要在嵌套待决类型名之前加上`typename`关键字。

## 例外情况

在基类列表，或者成员初始化列表中，不能在嵌套待决类型名之前加`typename`：

```cpp
template <typename T>
class Derived : public Base<T>::Nested              // typename not allowed
{
    explicit Derived(int x) : Base<T>::Nested(x)    // typename not allowed
    {
        typename Base<T>::Nested temp;              // typename required
    }
};
```

## 与`typedef`结合使用

```cpp
template <typename IterT>
void workWithIterator(IterT iter)
{
    typedef typename std::iterator_traits<IterT>::value_type value_type;
    value_type temp(*iter);
    ...
}
```



## 总结

- 在声明模板参数时，`class`和`typename`是等价的，可以互相交换；
- 使用`typename`来表明嵌套的待决类型名，除了在基类列表中或者在成员初始化列表中。

<a href="../Item%2041"><-</a>

<a href="../Item%2043">-></a>
