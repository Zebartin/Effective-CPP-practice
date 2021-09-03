# Item 11: Handle assignment to self in `operator=`.
虽然把自己赋值给自己看上去很蠢，但由于别名（aliasing）机制的存在，这种情况还是时有发生的。为此，需要在实现`operator=`的时候把这种情况考虑在内。

## 不安全的`operator=`

```cpp
class Bitmap { ... };
class Widget
{
private:
    Bitmap *pb;
public:
	Widget& operator=(const Widget& rhs)
    {
        delete pb;
        pb = new Bitmap(*rhs.pb);
        return *this;
    }
};
```

- 一方面，如果`rhs`是自身，最终新的`pb`将是已被删除的；
- 另一方面，如果在生成新的`Bitmap`发生了异常，最终新的`pb`也会是不存在的。

## identity test

为了解决第一个问题，可以先检查是否与自身相同，相同则直接返回自己。

## 破除exception-unsafe

仅仅是identity test并不能解决第二个问题。

重新组织操作顺序，可以在解决第二个问题的同时，也解决第一个问题。

```cpp
Widget& Widget::operator=(const Widget& rhs)
{
    Bitmap *pOrig = pb;
    pb = new Bitmap(*rhs.pb);
    delete pOrig;
    return *this;
}
```

### 效率问题？

如果程序中有较多的self-assignment，可以考虑把identity test加回去。这样做会增大代码和类的体积，引入新的分支，使得指令预取、缓存、pipelining的效率提升效果减弱。

## 另一种方式：copy-and-swap

```cpp
class Widget {
    ...
    void swap(Widget& rhs);		// exchange *this’s and rhs’s data;
    ...
};
Widget& Widget::operator=(const Widget& rhs)
{
    Widget temp(rhs);
    swap(temp);
    return *this;
}
```

这样同样可以同时避免上述两个问题。

### 简化

利用pass by value机制可以简化成以下形式，牺牲了可读性，但可能能帮助编译器生成更有效的代码。

```cpp
Widget& Widget::operator=(Widget rhs)
{
	swap(rhs);
    return *this;
}
```

## 总结

- 保证`operator=`在自己赋值给自己的情况下也能正常工作。实现方式包括：
  - 比较源对象和目标对象的地址；
  - 小心组织语句顺序；
  - copy-and-swap
- 保证操作多个可能相同的对象的函数工作正常。

<a href="../Item%2010"><-</a>

<a href="../Item%2012">-></a>
