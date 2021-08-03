[TOC]

# Chapter 1: Accustoming Yourself to C++

## Item 1: View C++ as a federation of languages.

将C++看作四种子语言（sublanguage）的组合：

- 纯C，没有模板，没有异常处理，没有重载等等
- 面向对象的C++，类（构造函数和析构函数）、封装、继承、多态、虚函数（动态绑定）等等
- 模板C++，通用（generic），带来了新的编程范式：template metaprogramming（TMP）
- STL

在不同的sublanguage中，应遵循的准则有所不同。

## Item 2: Prefer `const`s, `enum`s, and `inline`s to `#define`s.

这么表述也许更合理：prefer the compiler to the preprocessor

### 常量

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

#### 特殊情况1：常量指针

需要两次`const`：

```cpp
const char * const authorName = "Scott Meyers";
```

在这个例子中，使用`string`更好：

```cpp
const std::string authorName("Scott Meyers");
```

#### 特殊情况2：类内部的常量

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

### the enum hack

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

### 使用inline而不是宏定义

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

## Item 3: Use `const` whenever possible.

### const与指针

- `const`在星号左边，表示指针指向的数据是常量；
- `const`在星号右边，表示指针本身是常量
- 星号两边都有，表示指针本身和数据都是常量

```cpp
char greeting[] = "Hello";
char *p = greeting;                 // non-const pointer, non-const data
const char *p = greeting;           // non-const pointer, const data
char const *p = greeting;           // non-const pointer, const data
char * const p = greeting;          // const pointer, non-const data
const char * const p = greeting;    // const pointer, const data
```

`const`在类型的左边或者右边都一样

### const与STL iterator

STL iterator基于指针，因此表现与`T*`指针类似。

- 将iterator声明为`const`，类似于`T* const`的指针，也就是可变数据、不可变指针
- 要达到`const T*`指针的效果，可以使用`const_iterator`

### 函数声明中的const

#### const式的返回值

对于`if(a * b = c)`这种错误，如果`a*b`的返回值是const的，就可以直接报错，避免意外赋值。这与a和b是built-in类型的情况是表现一致的

尽可能使用`const`修饰可以帮助编译器检查错误

#### const式的成员函数

决定哪些成员函数会被`const`对象调用，哪些会被非`const`对象调用

在`const`声明上有所不同的成员函数是可以重载的：

```cpp
class TextBlock {
public:
...
	const char& operator[](std::size_t position) const
	{ return text[position]; }
	char& operator[](std::size_t position)
	{ return text[position]; }
private:
	std::string text;
};
```

> `const`对象最常出现的时机是作为常量参数传递给函数

经过以上重载，可以使得对于`const TextBlock ctb`，`ctb[0] = 'x'`是非法的，使得`ctb`不可被修改

#### 什么样的成员函数称得上是const的？

一种是C++语法要求的bitwise constness，成员函数内部不允许修改任何非静态成员变量。但由于指针机制，这种表面上的const并不能真正做到不可变；

另一种是作者推荐的logical constness，允许成员函数内部以用户无法察觉的方式修改成员变量，但这样会打破C++语法的要求。此时将需要修改的变量声明为`mutable`，就可以将成员函数声明为`const`

#### 避免const和非const成员函数的重复代码

如果两者有几乎相同的实现，可以让非const成员函数调用const成员函数：

```cpp
class TextBlock {
public:
	...
	const char& operator[](std::size_t position) const
	{
		...
		return text[position];
	}
	char& operator[](std::size_t position)
	{
		return
			const_cast<char&>(
				static_cast<const TextBlock&>(*this)[position]
			);
	}
	...
};
```

- 使用`static_cast`将`*this`变为`const`
- 使用`const_cast`将返回结果变为非`const`

反过来让const成员函数调用非const成员函数是不可取的，因为这打破了const成员函数的承诺。

## Item 4: Make sure that objects are initialized before they’re used.