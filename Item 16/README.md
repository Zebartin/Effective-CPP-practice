# Item 16: Use the same form in corresponding uses of `new` and `delete`.
## `new`和`delete`

`new`做了2件事：

1. 分配内存；
2. 调用一个或多个构造函数。

同样地，`delete`也做了2件事：

1. 调用一个或多个析构函数；
2. 释放内存。

关于`delete`有一个问题，如何知晓指针指向的是一个单独的对象还是对象数组？这个问题很重要，因为两者在内存的布局方式是不同的。通常来说，后者会在内存中指明数组的大小，从而让`delete`知悉需要对多少个对象进行析构。

对`delete`来说，解决这个问题的唯一方法是由用户通过`[]`来指定。

## 保持一致

`new`和`delete`的`[]`使用必须保持一致。

如果一个类包含有指针，这个指针指向动态分配的内存，并且这个类有多个构造函数，那么这些构造函数中应使用相同的`new`形式来初始化这个指针，否则，析构函数中的`delete`形式无法确定。

## `typedef`带来的问题

```cpp
typedef std::string AddressLines[4];

std::string *pal = new AddressLines;
delete [] pal;
```

`typedef`会带来不必要的困惑，因此最好弃用它，改用`vector`或`string`等。

## 总结

如果在`new`中用了`[]`，在对应的`delete`中也必须用`[]`；反之亦然。

<a href="../Item%2015"><-</a>

<a href="../Item%2017">-></a>
