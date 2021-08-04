# Item 3: Use `const` whenever possible.

## const与指针

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

## const与STL iterator

STL iterator基于指针，因此表现与`T*`指针类似。

- 将iterator声明为`const`，类似于`T* const`的指针，也就是可变数据、不可变指针
- 要达到`const T*`指针的效果，可以使用`const_iterator`

## 函数声明中的const

### const式的返回值

对于`if(a * b = c)`这种错误，如果`a*b`的返回值是const的，就可以直接报错，避免意外赋值。这与a和b是built-in类型的情况是表现一致的

尽可能使用`const`修饰可以帮助编译器检查错误

### const式的成员函数

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

### 什么样的成员函数称得上是const的？

一种是C++语法要求的bitwise constness，成员函数内部不允许修改任何非静态成员变量。但由于指针机制，这种表面上的const并不能真正做到不可变；

另一种是作者推荐的logical constness，允许成员函数内部以用户无法察觉的方式修改成员变量，但这样会打破C++语法的要求。此时将需要修改的变量声明为`mutable`，就可以将成员函数声明为`const`

### 避免const和非const成员函数的重复代码

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

## 总结

- 声明为`const`有助于编译器检测使用错误，`const`能在很多地方使用，比如函数参数、返回值、成员函数整体等等
- 编译器强制要求bitwise constness，但作为编程者应该按照logical constness的原则来
- 如果`const`函数和非`const`函数有基本一致的实现，可以让后者调用前者来避免代码重复