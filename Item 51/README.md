# Item 51: Adhere to convention when writing `new` and `delete`.

## 操作符`new`应遵循的准则

操作符`new`应有正确的返回值，应在内存不足的时候调用new-handler，应能够处理0内存大小的请求。

### 非成员函数

一个比较合理的非成员操作符`new`的实现如下：

```cpp
void *operator new(std::size_t size) throw(std::bad_alloc)
{
    using namespace std;
    if (size == 0)
    {
        size = 1;
    }
    while (true)
    {
        attempt to allocate size bytes;
        if (the allocation was successful)
            return (a pointer to the memory);
        new_handler globalHandler = set_new_handler(0);
        set_new_handler(globalHandler);
        if (globalHandler)
            (*globalHandler)();
        else
            throw std::bad_alloc();
    }
}
```

- C++要求即使只有0字节，也要返回合法的指针。这种看似古怪的行为简化了其他地方的要求；
- 这里获取`globalHandler`的做法看起来很蠢，但别无他法。如果涉及到多线程，这里需要小心处理；
- `new`中包含一个无限循环，在其中尝试分配内存和调用new-handler。

### 成员函数

作为成员函数的操作符`new`，很多时候都是为了特定一个类设计的，而不包括其派生类。而派生类会从基类继承`new`，如果派生类没有自定义的`new`，则派生类将会用基类的方式分配内存，这显然是错误的。

为了避开这一点，最好的方式是让默认的`new`来处理“错误”内存大小的请求：

```cpp
void *Base::operator new(std::size_t size) throw(std::bad_alloc)
{
    if (size != sizeof(Base))
        return ::operator new(size);
    ...
}
```

这里实际上处理了`size`为0的情况。按照C++的规定，`sizeof(Base)`必不为0，因此`size`为0时会交由默认`new`处理。

### 数组情况

要记住，你在分配一块内存，你无法对数组里尚未存在的对象做任何事情。事实上，你连数组里有多少个对象都不知道，因为：

- 你不知道每个对象大小是多大，因为它们不一定是基类对象，基类的`new[]`可能会被用于分配派生类对象的内存；
- 请求的内存大小可能会比对象的总大小更大，因为需要有额外空间来保存数组元素的数目信息。

## 操作符`delete`应遵循的准则

需要记住和遵守一点：C++保证`delete`null指针总是安全的。

非成员的操作符`delete`实现如下：

```cpp
void operator delete(void *rawMemory) throw()
{
    if (rawMemory == 0)
        return;
    deallocate the memory pointed to by rawMemory;
}
```

成员函数版本的`delete`类似，但与类专用的`new`对应，也要注意处理“错误大小”的情况：

```cpp
void Base::operator delete(void *rawMemory, std::size_t size) throw()
{
    if (rawMemory == 0)
        return;
    if (size != sizeof(Base))
    {
        ::operator delete(rawMemory);
        return;
    }
    deallocate the memory pointed to by rawMemory;
    return;
}
```

有趣的一点是，如果待删除的对象派生于某个缺少虚析构函数的基类，传递给`delete`的`size_t`值可能是错误的。这算是做好[Item 7](../Item%2007)的另一个理由。

## 总结

- 操作符`new`应有一个无限循环来分配内存，应在无法满足内存分配请求时调用new-handler，应能够0字节大小的请求。类专用的版本应能够处理比预期更大的数据块请求；
- 操作符`delete`在传入的指针为null时不做任何事。类专用的版本应能够处理比预期更大的数据块。
