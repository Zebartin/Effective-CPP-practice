# Item 35: Consider alternatives to virtual functions.

很多时候使用虚函数是自然而然的考虑，但与此同时也不要忘了有很多其他方式可以达到类似的目的。

本节以下面的代码作为例子，看看可以用什么其他方式实现它：

```cpp
class GameCharacter
{
public:
    virtual int healthValue() const;
    ...
};
```

## 基于NVI idiom的模板方法模式

有一派人坚持，虚函数应该几乎总是private的。基于这种观念，`healthValue`应该是非虚的，在其中调用某个private的虚函数来完成真正的工作（此处为了方便把函数定义写在类定义中，与隐式内联无关）：

```cpp
class GameCharacter
{
public:
    int healthValue() const
    {
        ...     // before
        int retVal = doHealthValue();
        ...     // after
        return retVal;
    }
    ...
private :
    virtual int doHealthValue() const
    {
        ...
    }
};
```

这种设计方式被称作non-virtual interface(NVI) idiom，同时它也是[模板方法（template method）设计模式](https://refactoringguru.cn/design-patterns/template-method)的一种实现方式。

NVI idiom的一个优势可以从上述代码中的注释看出来。在真正调用虚函数之前，可以先准备好合适的环境，比如设置互斥锁、输出日志、验证类常量以及检查函数前提是否满足；之后可以解锁互斥锁、验证函数后续条件以及再次验证类常量等等。

NVI idiom涉及到派生类重新定义private的虚函数，而派生类实际上调用不了这些函数。这可能会让人觉得奇怪，但其实并无矛盾。虚函数的调用时机和实现方式是相互独立互不干扰的。

NVI idiom中虚函数不严格要求是private的。比如有的类继承架构下，派生类的实现被要求调用基类的实现，这时候虚函数需要是protected的。有时候虚函数也可能必须是public的，比如虚析构函数，然而这种时候并不适合NVI idiom。

## 基于函数指针的策略模式

NVI idiom有丶意思，但归根结底还是在使用虚函数。另一种替代方式如下，它不使用虚函数：

```cpp
class GameCharacter; // forward declaration
// function for the default health calculation algorithm
int defaultHealthCalc(const GameCharacter &gc);
class GameCharacter
{
public:
    typedef int (*HealthCalcFunc)(const GameCharacter &);
    explicit GameCharacter(HealthCalcFunc hcf = defaultHealthCalc) : healthFunc(hcf) {}
    int healthValue() const { return healthFunc(*this); }
    ...
private:
    HealthCalcFunc healthFunc;
};
```

这种设计基于[策略（strategy）设计模式](https://refactoringguru.cn/design-patterns/strategy)，它提供了一些有趣的灵活性：

- 同一类的不同对象可以有不同的策略；
- 一个对象的策略可以在运行时改变。

然而，这种设计存在一个问题：类以外的非成员非友元函数访问不了类内部的非public成员。解决这个问题的唯一方法是弱化类的封装。

## 基于`tr1::function`的策略模式

把上述代码中的`typedef`做如下替换即可：

```cpp
typedef std::tr1::function<int (const GameCharacter&)> HealthCalcFunc;
```

`int (const GameCharacter&)`被称作target signature。这个对象能够管理任何与这个target signature兼容的callable实体。“兼容”意味着`const GameCharacter&`要么是，要么可以被转换为这个实体的参数；并且这个实体的返回值要么是，要么可以被隐式转换为`int`。

`tr1::function`的灵活不仅体现在参数和返回值的兼容上，还体现在实体种类上。这些实体可以是普通函数、函数对象或者成员函数。成员函数的情况比较特别：

```cpp
class GameLevel
{
public:
    float health(const GameCharacter &) const;
    ...
};

class EvilBadGuy: public GameCharacter
{
public:
    explicit EvilBadGuy(HealthCalcFunc hcf = defaultHealthCalc) : GameCharacter(hcf) 
    { ... }
    ...
};
...
GameLevel currentLevel;
EvilBadGuy ebg(
    std::tr1::bind(&GameLevel::health, currentLevel, _1));
```

这里`tr1::bind`的作用是将`currentLevel`与函数绑定。

## 经典策略模式

相比于上述两种，策略模式的经典实现会更加直观：

![策略模式UML](https://refactoringguru.cn/images/patterns/diagrams/strategy/structure-2x.png)

对应的代码如下：

```cpp
class GameCharacter;
class HealthCalcFunc
{
public:
    ...
    virtual int calc(const GameCharacter &gc) const {...}
    ...
};
HealthCalcFunc defaultHealthCalc;
class GameCharacter
{
public:
    explicit GameCharacter(HealthCalcFunc *phcf = &defaultHealthCalc) : pHealthCalc(phcf) {}
    int healthValue() const { return pHealthCalc->calc(*this); }

private:
    HealthCalcFunc *pHealthCalc;
};
```

## 总结

- 虚函数的替代品包括NVI idiom和各种实现方式的[策略（strategy）设计模式](https://refactoringguru.cn/design-patterns/strategy)。NVI idiom本身是[模板方法（template method）设计模式](https://refactoringguru.cn/design-patterns/template-method)的一个例子；
- 把成员函数的功能移到类之外存在一个劣势，即非成员函数缺乏访问类的非public成员的权限；
- `tr1::function`对象表现得就像笼统的函数指针。这样的对象支持所有与给定函数签名兼容的callable实体。

