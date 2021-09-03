# Item 13: Use objects to manage resources.

## 手动释放资源是不可靠的

设想由用户控制资源释放：

```cpp
void f()
{
    Investment *pInv = createInvestment();
    ...
    delete pInv;
}
```

可能有几种情况导致资源未被正确释放：

- 函数提前返回；
- 函数报错；
- 如果这部分资源是在循环中获取和释放，循环可能会提前跳出。

尽管小心谨慎的编程能避免这些情况，但随着时间的推移，代码可能会随着维护发生意料之外的变化，比如：

- 在释放资源前加了`return`或`continue`；
- 其中调用的某个函数被改得可能会抛出异常。

## 依靠析构函数自动释放资源

很多资源都是动态地分配在堆上，只在单独一小块或函数里使用，并且应该在这部分使用完毕之后释放。

### `auto_ptr`

标准库的`auto_ptr`就是为这种情况而生的。它的析构函数会自动调用对应指针的`delete`。

```cpp
std::auto_ptr<Investment> pInv(createInvestment());
```

使用对象来管理资源的两个特点：

- 获取资源后，会立即将其移交给资源管理对象，其中“移交”可以是初始化，也可以是赋值。这也是为什么这种技巧也被称作Resource Acquisition Is Initialization（RAII）；
- 资源管理对象利用自己的析构函数来确保资源被正确释放。

由于`auto_ptr`能够自动删除对应的指针，所以如果同一时刻有多个`auto_ptr`指向同一个对象，这个对象会被`delete`多次，引发undefined behavior。为了避免这一点，`auto_ptr`被设计拥有一项古怪的特性，将某个`auto_ptr`拷贝给其他将会把自身设置为`null`，从而尽量保证`auto_ptr`不会指向同一个对象。

由于这种特性，`auto-ptr`在有些情况下并不适用，比如STL容器要求其中的内容有着“正常”的拷贝方式，因此`auto-ptr`不能放在STL容器之中。

### reference-counting smart pointer（RCSP）

除了`auto_ptr`以外的另一种选择是RCSP，它能够追踪指向某个特定资源的对象数量。它与垃圾回收机制类似，但处理不了循环引用的情况。`tr1::shared_ptr`就是一种RCSP，而且它的拷贝行为是“正常”的：

```cpp
std::tr1::shared_ptr<Investment> pInv(createInvestment());
```

### 数组资源的管理

`auto_ptr`和`tr1::shared_ptr`在其析构函数中都只是使用了`delete`而非`delete []`，因此并不适用于动态分配的数组。C++本身也不提供适用于数组的类似行为的库，因为大多数情况下可以用`vector`和`string`代替。如果不得不用到的话，可以考虑Boost库的`boost::scoped_array`和`boost::shared_array`类

### 自定义的资源管理类

有时候上述提到的各种类可能不尽人意，这时候需要根据[Item 14](../Item%2014)&15中的建议，自定义相应的资源管理类，

## 工厂函数的安全性

上述的`createInvestment`函数不应返回普通的`Investment *`指针，这样可能会由于用户的遗漏而导致资源管理问题。按照[Item 18](../Item%2018)的要求修改该函数接口可以修复这个问题。

## 总结

- 为了防止资源泄露，使用RAII对象，在其构造函数中获取资源，并在其析构函数中释放资源；
- 两个常用RAII类是`auto_ptr`和`tr1::shared_ptr`，后者通常更好，因为其拷贝行为符合直觉。拷贝`auto_ptr`会把自身置为`null`。
