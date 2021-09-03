# Item 2: Prefer `const`s, `enum`s, and `inline`s to `#define`s.

这么表述也许更合理：prefer the compiler to the preprocessor

## 常量

```cpp
#define ASPECT_RATIO 1.653
```

坏处：编译时宏名不会进入符号表（symbol table），导致debug受阻

解决方式：

```cpp
const double AspectRatio = 1.653;
```

好处：

- 在符号表中
- smaller code：宏会进行盲目的替换，产生多个复制（copies），而`const`变量不会产生多于一个的复制

### 特殊情况1：常量指针

需要两次`const`：

```cpp
const char * const authorName = "Scott Meyers";
```

在这个例子中，使用`string`更好：

```cpp
const std::string authorName("Scott Meyers");
```

### 特殊情况2：类内部的常量

需要配合`static`

```cpp
class GamePlayer {
private:
    static const int NumTurns = 5;
    int scores[NumTurns];
    ...
};
```

上面只是declaration，如果需要用到常量的地址，或者编译器错误地要求有definition，可以在类声明外的implementation file中另外作一个definition：

```cpp
const int GamePlayer::NumTurns;
```

与上相比，`#define`无法用于仅限于某个类的情况，也就是说没有private版本的`#define`

以上语法在以下情况不适用：

- 有一些旧编译器可能会不允许初始化静态类变量
- 类内初始化只适用于integral类型（例如int、bool、char）的常量

此时可以用另一种方式：

```cpp
class CostEstimate {
private:
    static const double FudgeFactor; // declaration of static class constant;
                                     // goes in header file
    ...
};
// definition of static class constant;
// goes in impl. file
const double CostEstimate::FudgeFactor = 1.35;
```

⚠️C++11之后，可以使用`constexpr`来指定变量或函数的值可以在常量表达式中出现：

```cpp
class AnotherCostEstimate {
private:
    static constexpr double FudgeFactor = 1.35;
};
```

[https://zh.cppreference.com/w/cpp/language/constexpr](https://zh.cppreference.com/w/cpp/language/constexpr)

## the enum hack

需要在编译期间用到类静态变量的值，而又因为某些原因无法在类内初始化时，可以用the enum hack来取巧：

```cpp
class GamePlayer {
private:
    enum { NumTurns = 5 };
    int scores[NumTurns];
    ...
};
```

- 在某些方面比`const`更像`#define`，比如
    - 取`enum`的地址是非法的
    - `enum`不会导致不必要的内存分配
- the enum hack是模板元编程的一项基本技术

## 使用inline而不是宏定义

使用`#define`来定义一个“函数”有以下弊端：

- 需要到处打括号，否则在表达式中使用这个宏可能会发生奇怪的事情
- 表达式可能会被执行多次

   ```cpp
  #define CALL_WITH_MAX(a, b) f((a) > (b) ? (a) : (b))

  int a = 5, b = 0;
  CALL_WITH_MAX(++a, b);    // a加了2次
  CALL_WITH_MAX(++a, b+10); // a加了1次
  ```

改用模板+内联函数：

```cpp
template<typename T>
inline void callWithMax(const T& a, const T& b)
{
    f(a > b ? a : b);
}
```

另外，一个类私有的宏无法实现，而内联函数可以

## 总结

- 对于简单对象，使用`const`或`enum`而不是`#define`
- 对于类似函数的宏，使用内联函数而不是`#define`
<a href="../Item%2001"><-</a>

<a href="../Item%2003">-></a>
