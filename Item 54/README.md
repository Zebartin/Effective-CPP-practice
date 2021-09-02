# Item 54: Familiarize yourself with the standard library, including TR1.

> 注：此item比较过时，文中介绍的TR1的很多组件应该都并入`std`命名空间了。

C++标准于1998年确定。2003年发布了一个小的“bug-fix”更新。预期将在2009年发布“Version 2.0”的C++标准。这一时代的C++标准被称作“C++0x”。

## C++98规定的C++标准

- 标准模板库（Standard Template Library，STL），包括各种容器（`vector`、`string`、`map`）、迭代器、算法（`find`、`sort`、`transform`）、函数对象（`less`、`greater`），以及各种容器和函数对象adapter（`stack`、`priority_queue`、`mem_fun`、`not1`）；
- iostream，包括对用户定义缓冲区的支持、internationalized IO（什么东西？），以及预定义的对象`cin`、`cout`、`cerr`以及`clog`；
- 对国际化（internationalization）的支持，包括拥有多个active locale（？）的能力。`wchar_t`（通常16bits/char）和`wsting`（`wchar_t`的`string`）这样的类型方便了Unicode的处理；
- 对数值处理的支持，包括复数的模板（`complex`），以及纯值数组（`valarray`）；
- 异常的继承结构，包括基类`exception`，它的派生类`logic_error`和`runtime_error`，以及其他从它们派生的各种类；
- C89标准库

## TR1规定的新东西

TR1规定了14种新组件，都在`std::tr1`这个命名空间中，包括：

- 智能指针`shared_ptr`和`weak_ptr`：
  - `shared_ptr`行为上类似于内置指针，但带有引用计数（reference counting）的功能。当引用计数降到0时，`shared_ptr`会自行删除自己指向的对象。常用于避免无环结构的资源管理问题，但如果存在环状引用则不适用；
  - `weak_ptr`专门用于处理环状引用。它不参与引用计数，当其指向的对象由于引用计数为0被删除时，它本身会自动被标记为无效；
- `function`：代表callable实体，使用样例可见Item 35；
- `bind`：合并和扩展了STL中`bind1st`和`bind2nd`的功能。Item 35中有一个相关例子。

其他TR1组件可以分成两个部分，第一部分是一些零散的功能：

- 哈希表，用来实现set、multiset、map以及multimap。它们的名字都带有`unordered`前缀，比如`unordered_map`，表示内部元素没有顺序，因为它们的内部实现与原来的`map`等等不一样；
- 正则表达式，包括搜索、替换、匹配等等；
- 多元组（tuple），相当于`pair`模板的多元素版本。Python和Eiffel中也有tuple；
- `array`，“STL化”的数组，支持`begin`、`end`之类的成员函数，它的大小在编译时确定，不使用动态内存；
- `mem_fn`，一种改造（adapt？）成员函数指针的语法统一的方式，合并和扩展了C++98的`mem_fun`和`men_fun_ref`；
- `reference_wrapper`，使得引用表现得像对象；
- 随机数生成，比C的`rand`更高级；
- 数学函数，包括拉格朗日多项式、贝塞尔函数、完整的椭圆积分等等；
- C99的新特性。

第二部分包含对复杂模板编程技术的支持，比如模板元编程：

- type traits，提供编译阶段可见的类型信息，比如是否是内置类型，是否提供虚析构函数等等；
- `result_of`，可推断出函数调用的返回值类型。

## 使用TR1

TR1本身只是一个文档，想使用它的功能需要自行寻找它的实现。Boost提供了TR1的10种组件的近似实现。

在使用基于Boost的TR1时，可以用这样的命名空间技巧：

```cpp
namespace std {
    namespace tr1 = ::boost;
}
```

严格来讲，往`std`里面加入新东西不好，但这么做一般不会出什么岔子。

## 总结

- 标准C++库的主要功能包括STL，iostream，以及locale（区域设置？）。C89标准库也被包含在内；
- TR1增加了对智能指针的支持（比如`tr1::shared_ptr`），通用的函数指针（`tr1::function`），基于哈希的容器，正则表达式，以及另外10种组件；
- TR1本身只是一个规范。为了利用TR1，你需要某种实现，其中一种实现来源是Boost。
