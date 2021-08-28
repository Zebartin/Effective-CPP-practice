# Item 47: Use traits classes for information about types.

这一节从STL提供的函数模板`advance`开始，它能将某个迭代器移动特定的距离：

```cpp
template<typename IterT, typename DistT>
void advance(IterT& iter, DistT d);
```

## STL迭代器类别

### 输入迭代器（input iterator）

只能向前移动，每次只能移动一步，只能进行读取，并且只能读取一次。

类似指向输入文件的读指针。

只适用于one-pass algorithm。

代表：C++库的`istream_iterator`。

### 输出迭代器（output iterator）

与输入迭代器类似，但用于输出，写入而非读取。

类似指向输出文件的写指针。

同样只适用于one-pass algorithm。

代表：`ostream_iterator`

### 正向迭代器（forward iterator）

能做到输入输出迭代器能做到的所有事，并且能够读取或写入多次。

适用于multi-pass algorithm。

代表：单向链表中的迭代器，TR1的hashed container中的迭代器（可能）

### 双向迭代器（bidirectional iterator）

与正向迭代器类似，但能反向移动。

代表：`list`、`set`、`multiset`、`map`、`multimap`中的迭代器

### 随机访问迭代器（random access iterator）

在双向迭代器基础上增加了“迭代器算术”的能力，类似于指针算术。

代表：`vector`、`deque`、`string`中的迭代器

### tag struct

对应5种迭代器，C++提供了它们对应的“tag struct”：

```cpp
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag: public input_iterator_tag {};
struct bidirectional_iterator_tag: public forward_iterator_tag {};
struct random_access_iterator_tag: public bidirectional_iterator_tag {};
```

它们之间的继承关系是代表着is-a的public继承。

## Traits Classes

我们希望`advance`能做到这样的事：

```cpp
template <typename IterT, typename DistT>
void advance(IterT &iter, DistT d)
{
    if (iter is a random access iterator)
    {
        iter += d;
    }
    else
    {
        if (d >= 0)
        {
            while (d--)
                ++iter;
        }
        else
        {
            while (d++)
                --iter;
        }
    }
}
```

这正是traits想实现的：提供在编译阶段可见的类型信息。

Traits不是C++中的关键字或者预定义好的结构，它是一种技巧，是一种C++编程人员遵循的惯例。它要求在用于用户定义类型的同时，能适用于内置类型。比如说，如果在一个普通指针上调用`advance`，`advance`需要像处理随机访问迭代器一样处理它。

这种需求使得traits的实现没有那么直截了当。标准的实现是：

```cpp
template <typename IterT>
struct iterator_traits
{
    typedef typename IterT::iterator_category iterator_category;
    ...
};
template <typename T>
struct iterator_traits<T *>
{
    typedef random_access_iterator_tag iterator_category;
    ...
};
```

虽然叫traits classes，但实际上是结构体。

为了兼容内置类型，traits进行了模板偏特化（partial template specialization）

与此同时，各种迭代器内部也需要做相应的声明，比如`list`：

```cpp
template <...>
class list
{
public:
    class iterator
    {
    public:
        typedef bidirectional_iterator_tag iterator_category;
        ...
    };
    ...
};
```

由此可以看到一个traits class的实现方式：

- 确定你想要使其可见的类型信息，比如迭代器的类型；
- 为这个类型信息起一个名字，比如`iterator_category`；
- 提供一个模板及其特化，其中包含有相应的信息，比如`iterator_traits`。

这样，`advance`中可以这样确定迭代器类型：

```cpp
typeid(typename std::iterator_traits<IterT>::iterator_category) == typeid(std::random_access_iterator_tag)
```

## 在编译时确定类型

上面看起来已经够好了，但其实还不够。首先，它会出现编译问题（Item 48）。更关键的是，明明编译阶段就已经确定了`iterator_traits<IterT>::iterator_category`，却还需要在运行时通过if语句进行判断。这不仅浪费时间，还会增大可执行文件的体积。

重载（overloading）事实上提供了在编译时进行条件判断的方式。于是乎：

```cpp
template <typename IterT, typename DistT>
void doAdvance(IterT &iter, DistT d,
               std::random_access_iterator_tag)
{
    iter += d;
}
template <typename IterT, typename DistT>
void doAdvance(IterT &iter, DistT d,
               std::bidirectional_iterator_tag)
{
    if (d >= 0)
    {
        while (d--)
            ++iter;
    }
    else
    {
        while (d++)
            --iter;
    }
}
template <typename IterT, typename DistT>
void doAdvance(IterT &iter, DistT d,
               std::input_iterator_tag)
{
    if (d < 0)
    {
        throw std::out_of_range("Negative distance");
    }
    while (d--)
        ++iter;
}

template <typename IterT, typename DistT>
void advance(IterT &iter, DistT d)
{
    doAdvance(iter, d,
              typename std::iterator_traits<IterT>::iterator_category());
}
```

使用traits class的方式：

- 定义一系列重载的“worker”函数或函数模板，它们的traits参数和相应的实现各有不同；
- 定义一个“master”函数或函数模板，调用worker函数，并从traits class中获取类型信息传递给worker。

## 其他traits classes

traits在标准库中广泛使用。`iterator_traits`除了提供了`iterator_category`以外，还提供了：

- `value_type`：迭代器内部元素的类型；
- `char_trait`：字符类型的相关信息；
- `numeric_limit`：数值类型的相关信息，比如取值范围。

TR1引入了更多的traits classes，超过50个，比如：

- `is_fundamental<T>`：`T`是否是内置类型；
- `is_array<T>`：`T`是否是数组类型；
- `is_base_of<T1, T2>`：`T1`是否与`T2`相同或者是`T2`的基类。

## 总结

- Traits classes提供在编译阶段可见的类型信息，可以通过模板和模板特化来实现；
- 借助重载，traits classes能在编译阶段对类型做类似if...else的检测。
