# Item 14: Think carefully about copying behavior in resource-managing classes.
Item 13中介绍了RAII思想，以及适用于基于堆（heap-based）的资源的两个RAII类，`auto_ptr`和`tr1::shared_ptr`。但不是所有资源都是基于堆的，对于这些资源，我们需要自定义自己的资源管理类。

以对互斥锁的管理为例：

```cpp
class Lock
{
public:
    explicit Lock(Mutex *pm) : mutexPtr(pm)
    {
        lock(mutexPtr);
    }
    ~Lock() { unlock(mutexPtr); }

private:
    Mutex *mutexPtr;
};
```

这样用户就可以像使用RAII一样使用`Lock`，即在函数内部开头声明一个`Lock`对象，退出函数体时就会自动`unlock`。

## 自定义拷贝行为

这种时候，`Lock`的拷贝函数应该有怎样的行为呢？

对于RAII的拷贝行为，通常有以下几种选择。

### 禁止拷贝

很多情况下，拷贝RAII对象都是不可理喻的。这时可以用[Item 6](../Item%2006)的技巧，通过继承`Uncopyable`类来禁用拷贝。

### 引用计数

类似`tr1::shared_ptr`的拷贝行为，可以直接通过把它作为RAII类的一个成员变量，来实现拷贝函数。

但是它的默认析构函数会直接`delete`指向的对象，而这可能并不是我们想要的。`tr1::shared_ptr`提供了自定义“deleter”的接口，在上述例子中可以将其设置为`unlock`：

```cpp
class Lock
{
public:
    explicit Lock(Mutex *pm) : mutexPtr(pm, unlock)
    {
        lock(mutexPtr.get());
    }

private:
    std::tr1::shared_ptr<Mutex> mutexPtr;
};
```

[勘误](https://www.aristeia.com/BookErrata/ec++3e-errata.html#p68LockCtorProb)：以上代码不是exception-safe的，如果`mutexPtr`构造时抛出异常，deleter会被自动调用，导致`unlock`在未`lock`的情况下被调用。同样地，如果`lock`中抛出异常，也会导致这个问题。因此可以改成：

```cpp
explicit Lock(Mutex *pm)
{
    lock(pm);
    mutexPtr.reset(pm, unlock);
}
```

注意，以上`Lock`的定义中不再需要显式的析构函数，直接使用默认析构函数即可，因为默认析构函数会自动调用`mutexPtr`的析构函数。关于此可以加上相关注释，提高可读性。

### 拷贝资源

有时候资源管理类不需要保证唯一性，只需要能够自动释放资源。这时候需要对相应资源做**深拷贝**（deep copy）。

标准库的`string`的一些实现涉及到指向堆内存的指针，这样的指针在`string`拷贝的时候同样被拷贝。

### 转移所有权

类似`auto_ptr`的拷贝行为

## 总结

- 拷贝RAII对象涉及到对相应资源的拷贝，所以资源的拷贝方式决定了RAII对象的拷贝方式；
- 一般来说，RAII类禁止拷贝，或进行引用计数，不过也可能有其他类型的拷贝行为。
