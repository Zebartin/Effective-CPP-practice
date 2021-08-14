# Item 27: Minimize casting.

C++的设计保证了，只要能够编译，类型错误就是不可能的。但是，类型转换却在暗中侵蚀C++的类型系统。

## 类型转换语法

- C-style:

  ```cpp
  (T)expression
  ```

- Function-style:

  ```cpp
  T(expression)
  ```

- C++-style:

  ```cpp
  const_cast<T>(expression)
  dynamic_cast<T>(expression)
  reinterpret_cast<T>(expression)
  static_cast<T>(expression)
  ```

前两种统称为旧式转换，本质上没有什么差别；C++-style则是新式转换，这四种转换有不同的用途：

- `const_cast`用来从`const`对象转换成非`const`对象。在C++-style中只有它能做到这一转换；
- `dynamic_cast`主要用来做“safe downcasting”，也就是在将基类转换为派生类，同时检查继承关系是否成立。
  - 唯一一种无法通过旧式转换实现的类型转换；
  - 运行时开销相当大。
- `reinterpret_cast`专门用来进行底层的类型转换，会导致实现依赖（implement-dependent）、不可移植的结果。比如将指针转换为`int`；
- `static_cast`可以被用于强制隐式转换，比如非`const`转`const`，`int`转`double`等。它也可以被用来做很多其他转换的反向转换，比如`void *`指针转带类型的指针，指向基类的指针转指向派生类的指针，但它不能将`const`转非`const`。

与旧式转换相比，新式转换更好，因为：

- 新式转换易于识别定位，不管是对人还是对编程工具来说；
- 在新式转换中，类型转换的目的更明确。

当然也有例外，比如在创建对象时，函数式地调用构造函数显得更自然。

## 对类型转换的误解

很多人以为类型转换没干什么，只是告诉编译器应该以什么样的类型对待变量而已，但这是错误的。不管哪种类型转换，通常都会产生运行时的代码。

一个明显的例子是，从`int`转`double`，在很多体系下，两者底层的表示形式是不同的，因此这几乎肯定会产生新的代码。

另一个比较惊人的例子是，从派生类指针转换为基类指针，也可能会产生新代码，甚至转换前后的值也可能不同。在这种情况下，会在运行时给派生类指针值加上某个offset来得到正确的基类指针值。

由第二个例子可以知道，在C++中，单独一个对象可能有多个地址。这种情况在多重继承时更是随处可见。所以，不应该对C++底层布局做一厢情愿的假设，并基于这种假设做类型转换。比如，将对象地址转换为`char *`指针然后做指针运算，几乎总是会发生难以预料的事情。

## 避免类型转换

### 派生类调用基类方法

在某个虚成员函数中，如果需要调用父类特有的那部分，应该这么写：

```cpp
Base::virtualFunc();
```

而不是：

```cpp
static_cast<Base>(*this).virtualFunc();
```

这只是在拷贝出来的新对象上做操作而已，影响不到当前对象。

### 避免`dynamic_cast`

上面的例子从某种程度上证明了，如果你发现自己想要做类型转换，那可能意味着你处理事情的方式不太对。`dynamic_cat`的使用更是如此。

首先需要知道一点，`dynamic_cast`的很多实现都相当慢。比如某个实现会对类的名字作字符串比较，而在继承关系比较复杂或比较深的时候，多次字符串比较就会成为性能瓶颈。这种实现方式是有原因的，它们需要支持动态链接（dynamic linking）。所以，在性能敏感的代码中使用`dynamic_cast`要尤为谨慎。

通常来说，想使用`dynamic_cast`是因为想在某个基类指针/引用上做派生类的操作。有两种通用方法可能可以避免使用它：

- 直接使用多个type-safe容器，比如使用`vector<Derived>`而非`vector<Base>`；
- 使用虚函数。

### 隐藏类型转换

尽量减少类型转换，同时把难以避免的部分尽可能隔离起来。

## 总结

- 尽可能避免类型转换，在性能敏感的代码里尤其要避免`dynamic_cast`；
- 如果有必要进行类型转换，把它隐藏到某个函数中。用户可以直接调用这个函数而不是自己进行类型转换；
- 尽量用C++式的类型转换而不是旧式的。前者方便查阅，而且在含义上更为明确。