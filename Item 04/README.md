# Item 4: Make sure that objects are initialized before they’re used.

在C++中试图读取未初始化的值可能会引发undefined behavior。由于C++中初始化规则比较复杂，对此最佳的应对方式是总是在用之前进行初始化，比如给定初始值，或者从输入流中赋值给变量。

## member initialization list

尽可能使用类成员变量初始化列表（member initialization list），比先默认初始化再赋值更有效率，也更清晰。即使某些成员变量可以被默认初始化，也应该显式在列表中写出来。

有些时候成员变量只能通过初始化列表的方式初始化，比如`const`或者引用。

### 例外情况

类中有多个构造函数和很多个成员变量，初始化列表会很长，可读性低。对于部分可以通过赋值来初始化的成员变量，可以将它们的初始化提取成一个函数，由构造函数调用它。这种方式在初始值需要通过读取文件或数据库来获取的情况下也适用。

### 注意顺序

类初始化时，会先初始化基类（base class），然后**按声明顺序**初始化各个成员变量。

为了避免困惑，初始化列表的顺序最好和声明顺序一致。

## non-local static objects across translation units

### 问题

static object指那些与程序共存亡的object，直到main执行完毕才会消亡。有一些是全局的（non-local），一些是局部的。局部的只有：

- 函数内部的static object

全局的有：

- 全局变量
- 定义在命名空间中的
- 类中的static成员
- 文件范围的static变量

translation unit是指经过预处理器宏展开后得到的实现文件，一个程序由一个或多个翻译单元组成。

问题的关键在于，不同translation unit中的non-local static object初始化的相对顺序是不确定的。这个问题在[cppreference.com](https://en.cppreference.com/w/cpp/language/siof)也有提到。

### 解决方式

将全局静态对象（non-local static object）的初始化放到某个函数中，使之成为一个局部的静态对象。这样一来，只有在第一次调用这个函数的时候，这个对象才会被初始化。

这种手法有点类似设计模式中的一种——[单例模式](https://refactoringguru.cn/design-patterns/singleton)。

## 总结

- 手动初始化内置类型的对象，因为C++不会总是自动初始化它们
- 在构造函数中使用类成员变量初始化列表而非在函数体内逐一赋值。列表中各变量顺序应与声明顺序保持一致
- 把全局静态对象替换成局部静态对象，从而避免不同翻译单元之间的初始化顺序问题
<a href="../Item%2003"><-</a>

<a href="../Item%2005">-></a>
