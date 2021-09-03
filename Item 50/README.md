# Item 50: Understand when it makes sense to replace `new` and `delete`.

## 为什么要自定义`new`或`delete`？

### 检测使用错误

典型的错误比如：

- `delete`失败，导致内存泄漏；
- 在同一个对象上多次`delete`。

如果`new`和`delete`能够维护已分配的地址列表，就可以检测到以上错误。

类似地，有一些错误可能会导致数据overrun（出上界）或者underrun（出下界）。`new`可以分配一个稍大的空间块，在开头和结尾写入特定的字节（signature），在`delete`时检查signature是否完整，并记录相关log。

### 改善性能

编译器自带的`new`和`delete`是为通用用途而设计的，性能上势必比不上专门针对特定项目的内存分配情况而设计的自定义`new`和`delete`。

比如说单线程的程序不必使用默认的线程安全的内存分配方式，自定义非线程安全的内存分配可能能够提升性能。

### 收集使用数据

在真正自定义`new`和`delete`之前，需要收集一下软件的动态内存使用情况。分配的块大小的分布情况怎样？它们的生存周期又怎样？它们倾向于怎样的分配和释放方式，LIFO、FIFO还是其他？内存使用情况会随着事件而改变吗？动态分配内存的峰值有多大？

可以通过自定义`new`和`delete`来收集这些信息。

### 减少默认的时空开销

通用的内存分配方式一般会消耗更多的时间和空间，专门为小对象设计的分配器（比如Boost的Pool库）能够减少这些开销。

### 保证最优对齐

有的默认分配方式可能不会保证对齐。

### 把相关的对象放在一起

这样做可能能够减少缺页错误发生的频率。

### 做一些非常规的事

比如在共享内存中进行分配和释放，并只用一个C API来控制。

再比如在`delete`的时候把释放的内存全填0以保护数据。

## 自定义并不简单

一个自定义`new`的例子：

```cpp
static const int signature = 0xDEADBEEF;
typedef unsigned char Byte;
void *operator new(std::size_t size) throw(std::bad_alloc)
{
    using namespace std;
    size_t realSize = size + 2 * sizeof(int);
    void *pMem = malloc(realSize);
    if (!pMem)
        throw bad_alloc();
    *(static_cast<int *>(pMem)) = signature;
    *(reinterpret_cast<int *>(static_cast<Byte *>(pMem) + realSize - sizeof(int))) = signature;
    return static_cast<Byte *>(pMem) + sizeof(int);
}
```

这个函数的大多数问题都与`new`应遵循的规则有关，比如没有反复调用new-handler。但且不提这些，这个函数还有一个微妙的问题：对齐（alignment）。

很多计算机体系结构都要求特定的数据在内存的位置要符合特定的对齐要求，比如`double`的内存地址应该是8字节的倍数（eight-byte aligned）。不遵循这样的要求会导致运行时的硬件错误，或者性能下降。

C++要求`new`返回的指针应满足对齐的要求，`malloc`也是，然而上述的自定义`new`由于加了signature，并不一定满足要求。

像对齐这样的细节能够区分专业和业余的内存管理开发者。一般来说，最好不要尝试自定义内存管理，除非有必要。事实上，有很多编译器提供了内存管理函数中的debug和logging功能的开关，多看看编译器的文档可能能满足你对自定义内存管理的需求。在很多平台，有一些商业产品能够替换内存管理函数，你只需要relink（还有购买）就可以使用它们。

另一个选择是开源的内存管理程序/库，比如Boost的Pool库。哪怕有自定义`new`和`delete`的需求，多看看这些库的实现，也能避免很多容易忽视的细节。

## 总结

自定义`new`和`delete`的动机有很多，比如改善性能，辅助修复堆使用错误，以及收集堆使用信息。
