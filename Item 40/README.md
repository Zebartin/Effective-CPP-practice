# Item 40: Use multiple inheritance judiciously.

大多数情况下，如果可以使用单一继承，就别用多重继承（multiple inheritance，MI）；如果想不出多重继承以外的实现方式，那最好再用心想一想。而有些时候多重继承则是最为清晰、最可维护、最合理的实现方式。总之，审慎而明智地使用它。

## MI存在的问题

### 同名的继承

```cpp
class BorrowableItem
{
public:
    void checkOut();
};
class ElectronicGadget
{
private:
    bool checkOut() const;
};
class MP3Player : public BorrowableItem, public ElectronicGadget
{ ... };
MP3Player mp;
mp.checkOut();	// ambiguous
```

在这个例子中，两个`checkOut`中其中一个是`private`的，但还是会出现重名问题。这是因为C++会先解析函数的重载，再检查函数是否可以访问。

这种情况下应这样指定要调用的基类函数：

```cpp
mp.BorrowableItem::checkOut();
```

### 虚继承与虚基类

```cpp
class File { ... };
class InputFile: public File { ... };
class OutputFile: public File { ... };
class IOFile: public InputFile, public OutputFile
{ ... };
```

这种情况被称作deadly MI diamond，`IOFile`类与`File`类之间有多条继承路径，这会要求你自行决定基类是否在每条路径中都重复一遍。

C++默认是会重复的，如果不想重复，则需要将带有重复数据的类（比如`File`类）作为虚基类：

```cpp
class File { ... };
class InputFile: virtual public File { ... };
class OutputFile: virtual public File { ... };
class IOFile: public InputFile, public OutputFile
{ ... };
```

C++标准库中也包含有类似的MI结构：`basic_ios`、`basic_istream`、`basic_ostream`以及`basic_iostream`。

从正确性的角度来看，虚继承似乎总是好的；但它会导致不小的开销。它会导致类大小变大，访问速度变慢，使得虚基类的初始化和赋值变得复杂。对虚基类的初始化是由the most derived class负责的。

关于虚继承的使用建议：

- 不要使用，除非真的需要；
- 如果需要使用，尽可能避免在虚基类中放数据，进而避免虚基类初始化和赋值的特殊处理。这一点与Java和.NET禁止在接口定义中声明数据变量很相似。

## MI的应用场景

某个类：

- 一方面需要实现Interface class提供的接口；
- 另一方面需要利用某个类来帮助实现。

此时将public继承和private继承一起使用是很合适的。

## 总结

- 多重继承比单一继承更为复杂。它可能导致新的模棱两可的问题，以及虚继承的需要；
- 虚继承会导致空间、时间上的开销，以及初始化和赋值上的复杂性。当虚基类不包含数据时，虚继承是最为可行的；
- 多重继承可以有相当合理的应用场景。其中一种是将对Interface class的public继承和帮助实现的private继承结合起来。
