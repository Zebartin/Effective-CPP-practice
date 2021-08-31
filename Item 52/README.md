# Item 52: Write placement `delete` if you write placement `new`.

在`new`一个新对象时：

```cpp
Widget *pw = new Widget;
```

有两个函数会被调用：

- 首先是`new`分配内存；
- 然后是`Widget`的构造函数。

如果第一个成功了，第二个抛出异常，第一步中的内存分配应该被取消，否则会发生内存泄漏。用户无法释放内存，这应该由C++运行时系统负责。此时运行时系统会查找与`new`对应版本的`delete`。

对于带有普通签名的`new`和`delete`，运行时系统能够顺利识别：

```cpp
void *operator new(std::size_t) throw(std::bad_alloc);
void operator delete(void *rawMemory) throw();
void operator delete(void *rawMemory,
                     std::size_t size) throw();
```

但如果不是普通的函数签名，运行时系统将会需要从多个版本的`delete`中查找。

## placement `new`

C++标准库的`<new>`中提供了原版的placement `new`，`vector`在创建对象时，为了利用空余的容量会用到它：

```cpp
void *operator new(std::size_t, void *pMemory) throw();
```

placement `new`从广义上说，还可以指称其他带有额外参数的`new`。

运行时系统会认为与额外参数数量和类型一致的`new`和`delete`是相互对应的，因此在开头那种情况下，运行时系统会寻找对应额外参数的placement `delete`。所以在使用placement `new`时，要同时写好对应版本的placement `delete`：

```cpp
class Widget
{
public:
    ...
    static void *operator new(std::size_t size, std::ostream &logStream) throw(std::bad_alloc);
    static void operator delete(void *pMemory) throw();
    static void operator delete(void *pMemory, std::ostream &logStream) throw();
    ...
};
...
Widget *pw = new (std::cerr) Widget;
```

然而placement `delete`实际上只会在开头所述那种情况下被调用，在普通的`delete pw`时，调用的还是普通的`delete`。所以同时也需要提供普通的`delete`。

## 避免非刻意的隐藏

正如Item 33中所说，成员函数的命名可能会被不经意间隐藏，`new`和`delete`尤其同理。在自定义`new`之后，普通的`new`就被隐藏了；类似地，派生类的`new`也会被全局或基类的自定义`new`隐藏。

Item 33讨论了一些避免隐藏的方式。一种简单通用的方式是创建一个包含了所有普通`new`和`delete`的基类，如下：

```cpp
class StandardNewDeleteForms
{
public:
    // normal new/delete
    static void *operator new(std::size_t size) throw(std::bad_alloc) { return ::operator new(size); }
    static void operator delete(void *pMemory) throw()
    {
        ::operator delete(pMemory);
    }
    // placement new/delete
    static void *operator new(std::size_t size, void *ptr) throw() { return ::operator new(size, ptr); }
    static void operator delete(void *pMemory, void *ptr) throw() { return ::operator delete(pMemory, ptr); }
    // nothrow new/delete
    static void *operator new(std::size_t size, const std::nothrow_t &nt) throw() { return ::operator new(size, nt); }
    static void operator delete(void *pMemory, const std::nothrow_t &) throw()
    {
        ::operator delete(pMemory);
    }
};
```

然后让自定义了`new`和`delete`的类去继承它，并用`using`声明来保证其命名可见：

```cpp
class Widget : public StandardNewDeleteForms
{
public:
    using StandardNewDeleteForms::operator new;
    using StandardNewDeleteForms::operator delete;
    ...
};
```

## 总结

- 如果你写了一个placement版本的`new`，务必写一个对应的placement版本的`delete`。否则，你的程序可能会时不时发生微妙的内存泄漏；
- 如果你写了placement版本的`new`和`delete`，确保不要不经意间隐藏了这些函数的普通版本。
