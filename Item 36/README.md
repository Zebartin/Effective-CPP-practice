# Item 36: Never redefine an inherited non-virtual function.

[Item 7](../Item%2007)是本Item的关于析构函数的特例。

## 从实际应用的角度看

以下代码中重新定义了父类的非虚函数：

```cpp
class D: public B {
public:
    void mf();
    ...
};

D x;
B *pB = &x;
D *pD = &x;
pB->mf();	// calls B::mf
pD->mf();	// calls D::mf
```

这会导致同一个对象的行为不一致，取决于使用这个对象的指针类型。这是因为非虚函数是statically bound的，而虚函数是dynamically bound的。

## 从理论的角度看

[Item 32](../Item%2032)说明了public继承意味着“is-a”，[Item 34](../Item%2034)说明了非虚函数建立了在不同的子类之间共享的不变量。那么：

- 如果`D`**确实**需要重新定义`mf`，而`B`的所有对象都**确实**需要`B`中的`mf`实现，那么`D` is-a `B`就是一个伪命题，于是两者不应该是public的继承关系；
- 如果`D`**确实**需要public地继承`B`，而`D`**确实**需要与`B`不同的`mf`实现，那么`mf`就不是`B`不同子类共有的不变量，于是`mf`应该是虚函数；
- 如果`D`**确实**is-a `B`，而`mf`**确实**对应着`B`不同子类共有的不变量，那么`D`就没道理重新定义`mf`。

## 总结

不要重新定义从父类继承的非虚函数。
