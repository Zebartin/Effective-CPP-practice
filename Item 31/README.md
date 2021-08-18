# Item 31: Minimize compilation dependencies between files.

C++没有严格地把接口和实现分离开来，类的定义不仅包含了接口，同时也包含了很多实现细节，比如：

```cpp
class Person
{
public:
    Person(const std::string &name, const Date &birthday, const Address &addr);
    std::string name() const;
    std::string birthDate() const;
    std::string address() const;
    ...
private:
    std::string theName;    // implementation detail
    Date theBirthDate;      // implementation detail
    Address theAddress;     // implementation detail
};
```

这种实现下，`Person`类需要有`string`、`Date`以及`Address`的定义（definition）才能够编译，而这些定义通过`#include`相应的头文件来引入：

```cpp
#include <string>
#include "date.h"
#include "address.h"
```

不幸的是，这就导致了`Person`所在的文件对这些头文件产生了编译依赖。如果这些头文件中任何一个发生改动，或者他们所依赖的头文件发生改动，`Person`所在的文件都必须重新编译，进而所有使用到`Person`的文件也需要重新编译。

## 尝试：前向声明

尝试把实现细节分离出去：

```cpp
namespace std
{
    class string;
}
class Date;
class Address;

class Person
{
public:
    Person(const std::string &name, const Date &birthday, const Address &addr);
    std::string name() const;
    std::string birthDate() const;
    std::string address() const;
    ...
};
```

如果这是可行的，那么`Person`的用户只有在类接口发生变化的时候才需要对它重新编译。但这种方式有两个问题：

- `string`不是一个类，而是一个`basic_string<char>`的`typedef`。所以它正确的前向声明方式要复杂得多，至于应怎么声明并不重要，因为手动声明标准库的一部分是不可取的，直接用`#include`就好了；
- 更重要的问题是，编译器需要知晓对象大小以进行内存分配。实现细节被分离后，编译器也就无从知晓`Person`类实际上有多大。

## Handle Classes

上述第二个问题在Smalltalk和Java就不会出现，因为它们在定义一个对象时，编译器只会把一个指针大小的空间分配给它，用指针来操作对象。

由此可以得到启发，将实现细节隐藏在一个指针里：

```cpp
#include <string>
#include <memory>
class PersonImpl;
class Date;
class Address;

class Person
{
public:
    Person(const std::string &name, const Date &birthday, const Address &addr);
    std::string name() const;
    std::string birthDate() const;
    std::string address() const;
    ...
private:
    std::tr1::shared_ptr<PersonImpl> pImpl;
};
```

这种设计被称作*pimpl idiom*（pointer to implementation），`Person`这样的类被称作Handle classes。一种使用方式是，把Handle class里的函数都复制到相应的实现类中，让后者做真正的工作，而前者则调用后者提供的函数，比如：

```cpp
#include "Person.h"
#include "PersonImpl.h"

Person::Person(const std::string &name, const Date &birthday, const Address &addr) : pImpl(new PersonImpl(name, birthday, addr)) {}
std::string Person::name() const
{
    return pImpl->name();
}
```

## Interface Classes

Interface classes作为抽象基类，为派生类声明了接口。它通常没有数据成员，没有构造函数，有一个虚析构函数，以及一系列表示接口的**纯虚函数**。

通常来说，为了给用户提供创建新对象的途径，Interface class会提供一个静态成员函数，在其中按需求创建派生类对象，返回对应的指针。这样的函数被称作工厂函数或虚构造函数（virtual constructors）

为一个Interface class提供实现的方式有：

- 继承并实现接口；
- 涉及到多重继承（Item 40）

## 分离接口与实现

分离接口与实现的关键在于，把对**实现**的依赖替换为对**声明**的依赖。只要有可能，就让头文件做到自给自足（self-sufficient）；如果不行，就依赖于声明而非定义。

### 避免直接使用对象

如果只有类的声明，定义这个类的指针和引用是可以的，而直接定义这个类的对象则还要求有类的定义。

### 尽可能依赖于类声明而非类定义

声明使用某个类的函数时，不需要用到类定义，即使是这个函数在参数或返回值中按值传递这个类的对象的时候也是这样：

```cpp
class Date;
Date today();
void clearAppointments(Date d);
```

当然，按值传递是不好的（Item 20），但如果你不得不用，还是需要注意不要造成不必要的编译依赖。

这种情况可能令人惊讶，但也并非难以理解。如果有谁调用了这些函数，他就需要`#include`包含有相应类定义的头文件，于是`Date`的定义势必在函数的调用之前可见。

问题的关键在于，不是所有人都会使用到这些函数。如果你有一个包含了很多函数声明的库，每个用户不太可能会调用所有函数。通过把提供类定义的职责从带有函数声明的头文件转移到用户文件上，关于不需要用到的类定义的编译依赖就被消除了。

### 为声明和定义分别设置头文件

头文件应成对出现，一个用作声明，另一个用作实现。用户在做出声明时，可以`#include`包含有声明的头文件，而不是手动进行前向声明，比如`class Date;`

关于用来替代前向声明的头文件，值得一提的是C++标准库的`<iosfwd>`头文件。它包含了iostream的各种组件声明，而它们的实现分布在`<sstream>`、`<streambuf>`等其他头文件中。

`<iosfwd>`还说明了另一个事实：上述建议也适用于模板。

C++还提供了`export`关键字来允许模板声明和定义的分离。

## Handle classes和Interface classes的劣势

两者都存在时空上的性能开销。

两者都不能使用内联函数，因为内联函数必须在头文件中定义好函数体，与两者矛盾。

在开发（development）的过程中，使用Handle classes和Interface classes有利于减少实现变化对用户的影响；在生产（production）使用时，如果具体类（concrete classes）能足够多地表现出速度和/或大小的优势，则把它们换掉。

## 总结

- 尽可能避免编译依赖的通用思路是依赖于声明而非定义。基于此有两种做法：句柄类（handle classes）和接口类（interface classes）；
- 库头文件理应是完整的，并且只包含有声明，无论其中是否包含了模板。
