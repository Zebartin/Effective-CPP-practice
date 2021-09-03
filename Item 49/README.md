# Item 49: Understand the behavior of the new-handler.

## new-handler

如果无法满足内存分配的请求，`new`会抛出`bad_alloc`异常，但在此之前，它会先调用一个可供用户自定义的异常处理函数new-handler（这是简化的说法，真正的细节见[Item 51](../Item%2051)）。用户可以通过调用声明在`<new>`中的`set_new_handler`来设定这个异常处理函数：

```cpp
namespace std {
    typedef void (*new_handler)();
    new_handler set_new_handler(new_handler p) throw();
}
```

`new_handler`是返回值为`void`的函数指针，`set_new_handler`是一个**基本上**（见[Item 29](../Item%2029)）不会抛出异常的函数。它将new-handler设置为`p`，同时返回之前的new_handler。

在`new`能找到足够的内存进行分配之前，它会反复地调用new-handler。

一个设计得当的new-handler势必会做以下事情之一：

- 腾出更多可用内存。一种实现方式是，在程序开始时就预留一大块内存，等需要时再将其释放；
- 安装一个不同的new-handler。如果当前的new-handler不能找到更多可用内存，可能它知道另一个符合需求的new-handler，并将其设置为当前的new-handler。那么在下次`new`失败时，这个新的new-handler就会被调用。这种做法的一个变种是直接修改自身的行为。
- 去除当前的new-handler，也就是用空指针调用`set_new_handler`。如此一来，`new`失败时将会直接抛出异常；
- 抛出`bad_alloc`或其派生异常。`new`中不会捕获这些异常，会继续往上传播；
- 不返回，通常调用`abort`或`exit`。

## 类专用的new-handler

C++的new-handler是全局的，没有提供类专属的new-handler，需要自行实现。首先用一个RAII类来进行资源管理：

```cpp
class NewHandlerHolder
{
public:
    explicit NewHandlerHolder(std::new_handler nh)
        : handler(nh) {}
    ~NewHandlerHolder() { std::set_new_handler(handler); }

private:
    std::new_handler handler;
    NewHandlerHolder(const NewHandlerHolder &);
    NewHandlerHolder &
    operator=(const NewHandlerHolder &);
};
```

这个RAII类用于保存旧的new-handler，并在`new`完成后将其还原。其中通过[Item 14](../Item%2014)介绍的技巧禁止了拷贝。

然后用一个mixin-style的基类使其能够通用：

```cpp
template <typename T>
class NewHandlerSupport
{
public:
    static std::new_handler set_new_handler(std::new_handler p) throw();
    static void *operator new(std::size_t size) throw(std::bad_alloc);
    ...
private:
    static std::new_handler currentHandler;
};
template <typename T>
std::new_handler NewHandlerSupport<T>::set_new_handler(std::new_handler p) throw()
{
    std::new_handler oldHandler = currentHandler;
    currentHandler = p;
    return oldHandler;
}
template <typename T>
void *NewHandlerSupport<T>::operator new(std::size_t size) throw(std::bad_alloc)
{
    NewHandlerHolder h(std::set_new_handler(currentHandler));
    return ::operator new(size);
}
template <typename T>
std::new_handler NewHandlerSupport<T>::currentHandler = 0;
```

- `NewHandlerSupport<T>::set_new_handler`只是为用户设置`currentHandler`，并不是真正的`std::set_new_handler`；

- `new`中用`NewHandleHolder`进行资源管理，在函数结束的时候还原new-handler；

- `NewHandlerSupport<T>`作为一个基类，能让派生类继承`set_new_handler`和操作符`new`；而作为一个模板，则可以保证每个派生类都能得到不同的静态成员，尤其是`currentHandler`，尽管模板参数`T`没有实际作用。

  这种模板化的基类是一种很有用的技巧，被称作curiously recurring template pattern（CRTP）。

  ```cpp
  class Widget: public NewHandlerSupport<Widget> {
      ...
  };
  ```

## nothrow？

在1993年以前，C++会在分配内存失败时返回null，现在则会抛出`bad_alloc`异常。C++标准委员会为了兼容test-for-null这种代码，提供了`new`的`nothrow`形式（定义在`<new>`）：

```cpp
Widget *pw2 = new (std::nothrow) Widget;
if (pw2 == 0)
    ...
```

nothrow的`new`在分配内存失败时，确实会返回null；但如果成功了，相应对象的构造函数就会被调用，而这里就不存在nothrow的保证了。

## 总结

- `set_new_handler`允许你指定一个函数，使其在内存分配请求失败时被调用；
- nothrow的`new`基本上没什么用，因为它所谓“不抛出异常”的保证只适用于内存分配的时候，后续调用构造函数时仍然可能会抛出异常。

<a href="../Item%2048"><-</a>

<a href="../Item%2050">-></a>
