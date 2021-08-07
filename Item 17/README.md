# Item 17: Store `new`ed objects in smart pointers in standalone statements.

## 问题来源

设想两个函数：

```cpp
int priority();
void processWidget(std::tr1::shared_ptr<Widget> pw, int priority);
```

由于`tr1::shared_ptr`的构造函数是`explicit`的，需显式构造：

```cpp
// processWidget(new Widget, priority());	// won't compile
processWidget(std::tr1::shared_ptr<Widget>(new Widget), priority());
```

在编译器生成代码时，可能会安排成这样的顺序：

1. 构造`Widget`对象；
2. 调用`priority`；
3. 构造`tr1::shared_ptr`对象。

如果在第2步时抛出了异常，`Widget`资源将不会接受管理，也不会被正确释放，进而导致资源泄露。

## 解决方式

在单独的语句里，创建`Widget`并保存到智能指针（smart pointer）中，然后再将智能指针传递给函数：

```cpp
std::tr1::shared_ptr<Widget> pw(new Widget);
processWidget(pw, priority());
```

这样处理限制了编译器的重排。

## 总结

在单独的语句里，把刚`new`出来的对象放到智能指针中。否则有异常抛出时可能会导致微妙的资源泄露。
