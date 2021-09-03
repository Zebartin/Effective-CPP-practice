# Item 55: Familiarize yourself with Boost.

## Boost简介

想找一个人类高质量、开源、独立于平台和编译器的库吗？有兴趣加入一个满是雄心勃勃、天赋异禀的C++开发者、专注于最前沿的库设计和实现的社区吗？想要看看未来的C++会是怎样的吗？答案都在[Boost](http://boost.org)！

Boost作为众多C++组织和网站中的一个，有其独有的两个优势：

- 与C++标准委员会有着独一无二的紧密关系，影响力很大。它由委员会成员创立，且两者成员始终保持着很大的重合；
- Boost有着一套接收库的独特流程，它基于公开的同行评审，包含“讨论、改善、重新提交、重复直至满意”的循环流程。

## Boost的内容

Boost中的库差别很大：

- 有些库只需要几行代码就可以上手，比如**Conversion**，提供了更安全或更便利的类型转换操作。比如：
  - `numeric_cast`，在转换数值类型时发生上溢或下溢或其他类似问题时抛出异常；
  - `lexical_cast`，把支持`<<`操作的类型转为string；
- 有些库提供一整套扩展功能，比如**Boost Graph Library**（处理图结构）、**Boost MPL Library**（“metaprogramming library”）。

Boost包含了很多个库，它们基于多个不同的议题：

- 字符串和文本处理：

  - 类型安全的类似`printf`的格式化；
  - 正则表达式；
  - tokenizing；
  - parsing；

- 容器：

  - 固定大小的数组；
  - variable-sized bitset；
  - 多维数组；

- 函数对象和higher-order programming，比如Lambda库，它的一个例子如下：

  ```cpp
  using namespace boost::lambda;
  std::vector<int> v;
  ...
  std::for_each(v.begin(), v.end(), 
                std::cout << _1 * 2 + 10 << "\n");
  ```

- 通用编程（generic programming），包括traits classes的一些扩展；

- 模板元编程，包括一个用于compile-time assertion的库，以及Boost MPL库。MPL中支持STL-like data structures of compile-time entities like *type*s，比如：

  ```cpp
  // create a list-like compile-time container of three types (float, 
  // double, and long double) and call the container “floats”
  typedef boost::mpl::list<float, double, long double> floats; 
  // create a new compile-time list of types consisting of the types in
  // “floats” plus “int” inserted at the front; call the new container “types”
  typedef boost::mpl::push_front<floats, int>::type types;
  ```

- 数学和数值计算，比如有理数的库、八元数和四元数的库、最大公约数和最小公倍数的计算、随机数等等；

- 正确性和测试，包括使模板的隐式接口具体化（见[Item 41](../Item%2041)），以及帮助test-first programming的库；

- 数据结构，包括类型安全的union（可以存放任意类型），以及tuple；

- 跨语言支持，包括一个可以无缝结合C++和Python的库；

- 内存，包括提供了高性能固定大小的allocator的Pool库，多种智能指针；

- 其他，比如CRC检查，日期和时间的操作，以及跨文件系统支持等等。

Boost也有一些没有涉足的领域，比如GUI开发库。

## 总结

- Boost是一个开发免费开源、经同行评审的C++库的社区和网站，在C++标准制定中扮演着重要角色；
- Boost不仅提供了很多TR1组件的实现，还提供了很多其他各种各样的库。

<a href="../Item%2054"><-</a>
