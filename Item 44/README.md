# Item 44: Factor parameter-independent code out of templates.

使用模板可能会导致代码体积膨胀（code bloat）。源代码很简洁，但生成的对象代码十分臃肿，因为模板生成了许多重复代码。进行共性与变性分析（commonality and variability analysis）可以解决这个问题。

## 非类型模板参数导致的代码重复

```cpp
template <typename T, std::size_t n>
class SquareMatrix
{
public:
    void invert();
};
```

在上述模板生成的`invert`的代码中，除了常数`n`以外没有任何不同。

### 借助不带非类型参数的模板

```cpp
template <typename T>
class SquareMatrixBase
{
protected:
    void invert(std::size_t matrixSize);
};
template <typename T, std::size_t n>
class SquareMatrix : private SquareMatrixBase<T>
{
private:
    using SquareMatrixBase<T>::invert; 
public:
    void invert() { invert(n); }    // implicitly inline
};
```

有几点需要注意：

- 基类的`invert`不能是内联的，否则派生类中的代码又会复制一遍，进而又导致体积膨胀；
- 基类的`invert`的目的只是为派生类减少代码重复，因此是`protected`而非`public`；
- 为了避免付出额外代价，派生类的`invert`是内联的；
- 两者的继承关系是private的。

### 基类如何获取需要操作的数据？

一种方式是将指向相应数据的指针作为函数参数传递给`invert`。

这种方式可行，但在实际情况中，类似于`invert`的函数可能有很多，对所有这样的函数都需要传递相同的指针，这看上去不太好。

更合适的选择是，让基类保存该指针，之前通过函数参数传递的`n`也同样这样处理更为合适：

```cpp
template <typename T>
class SquareMatrixBase
{
protected:
    SquareMatrixBase(std::size_t n, T *pMem) : size(n), pData(pMem) {}
    void setDataPtr(T *ptr)
    {
        pData = ptr;
    }

private:
    std::size_t size;
    T *pData;
};
```

这样一来，派生类的数据即可以存在栈中，也可以存在堆中：

```cpp
template <typename T, std::size_t n>
class SquareMatrix : private SquareMatrixBase<T>
{
public:
    SquareMatrix()
        : SquareMatrixBase<T>(n, data) {}

private:
    T data[n * n];
};
```

```cpp
template <typename T, std::size_t n>
class SquareMatrix : private SquareMatrixBase<T>
{
public:
    SquareMatrix()
        : SquareMatrixBase<T>(n, 0), pData(new T[n * n])
    {
        this->setDataPtr(pData.get());
    }

private:
    boost::scoped_array<T> pData;
};
```

### 减少代码重复的代价

- 硬编码的模板可能能通过常数折叠之类的的优化生成更好的代码。然而，代码重复的减少同时也提高了指令cache的局部性，提高了程序运行的速度。这两者的效果孰强孰弱需要权衡；
- 如果不够小心，减少代码重复可能还会导致对象大小膨胀，比如上述代码中多了指针占用的空间；
- 可能减弱类的封装性；
- 可能导致资源管理问题。

## 类型模板参数导致的代码重复

在很多平台，`long`和`int`的二进制表示是完全一样的，所以`vector<long>`和`vector<int>`这样的模板实例就会导致重复。有些链接器会对其进行自动整合，但有些不会。

在大多数平台，所有类型的指针的二进制表示都是一样的，因而有类似的问题存在。借助使用无类型指针（`void *`）来实现的函数可以减少重复。C++标准库的`vector`、`deque`和`list`等模板的一些实现就是这么做的。

## 总结

- 模板会生成多个类和多个函数，所以不依赖于模板参数的模板代码会导致代码体积膨胀；
- 把模板参数替换为函数参数或类数据成员，可以解决由非类型模板参数导致的代码体积膨胀；
- 让有着相同二进制表示的初始化类型共享相同的实现，可以减少由类型模板参数导致的代码体积膨胀。
