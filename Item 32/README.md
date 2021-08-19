# Item 32: Make sure public inheritance models “is-a.”

C++面向对象编程最重要的一条规则是，public继承意味着"is-a"。

## 反直觉

企鹅是鸟，鸟能飞，所以企鹅能飞？反映到面向对象编程中：

```cpp
class Bird
{
public:
    virtual void fly();
    ...
};
class Penguin : public Bird
{
    ...
};
```

这显然是不对的，因为并不是所有鸟都能飞，而是“通常来说鸟能飞”。

对此有两种解决途径：

### 细化继承结构

有能飞的鸟，也有不能飞的鸟，因此细化结构如下：

```cpp
class Bird
{
    ...
};
class FlyingBird : public Bird
{
public:
    virtual void fly();
    ...
};
class Penguin : public Bird
{
    ...
};
```

在编译时就拒绝了企鹅的飞翔。

### 报错

```cpp
void error(const std::string &msg); // defined elsewhere
class Penguin : public Bird
{
public:
    virtual void fly() { error("Attempt to make a penguin fly!"); }
    ...
};
```

直到运行时才会拒绝企鹅的飞翔，会导致不必要的编译。

## 矩形和正方形

正方形类应该public地继承矩形类吗？显然，正方形是矩形，但反过来不是，所以符合public继承的准则，这对吗？

考虑以下处理矩形对象的函数：

```cpp
void makeBigger(Rectangle &r)
{
    int oldHeight = r.height();
    r.setWidth(r.width() + 10);
    assert(r.height() == oldHeight);
}
```

这个函数显然不适用于正方形。public继承意味着基类的所有东西都同样适用于派生类。在这个例子中，使用public继承来描述两者关系是不正确的。

## 总结

public继承意味着“is-a”，所有适用于基类的同样也适用于派生类，因为派生类对象**就是（is a）**基类对象。
