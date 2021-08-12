# Item 23: Prefer non-member non-friend functions to member functions.

设想一个代表着web浏览器的类：

```cpp
class WebBrowser {
public:
    ...
    void clearCache();
    void clearHistory();
    void removeCookies();
    ...
};
```

很多用户会把上面三个函数放在一起使用，因此可以另外定义一个函数来做这件事，关于这个函数的位置有两个选择：

- 作为一个类成员函数`clearEverything`；
- 作为一个非成员函数`clearBrowser`，参数为`WebBrowser`对象。

哪种选择更好呢？

## 从封装的角度看

面向对象的设计原则要求数据应尽可能被封装起来。

粗粒度地看，能够访问类成员数据的函数越多，数据的封装度越差。

因此，对于非`private`的数据成员，能够访问它的函数无限多，因此它根本就没有封装；对于`private`的数据成员，只有成员函数和友元函数能够访问它。因此，尽可能使用非成员非友元函数，能够提高类的封装程度。

⚠️此处的非成员、非友元是针对当前类`WebBrowser`来说的，即使`clearBrowser`在其他类，只要不是`WebBrowser`的成员函数、友元函数，就无碍于它的封装程度。

## 提高灵活性

在C++中，一个更自然的做法是将`clearBrowser`定义成与`WebBroswer`在同一个命名空间里的非成员函数：

```cpp
namespace WebBrowserStuff
{
    class WebBrowser { ... };
    void clearBrowser(WebBrowser &wb);
    ...
}
```

这样做有特别的好处，因为命名空间和类不一样，它能够跨越不同的源文件，加之`clearBrowser`是一个便利函数（convenience function），这一点尤为重要。

`WebBrowser`这样的类可能有一系列的便利函数，使用命名空间可以将它们按功能拆分到不同的文件里，同时避免编译上的依赖：

```cpp
// header “webbrowser.h” — header for class WebBrowser itself
// as well as “core” WebBrowser-related functionality
namespace WebBrowserStuff
{
    class WebBrowser { ... };
    ...
}
// header “webbrowserbookmarks.h”
namespace WebBrowserStuff
{
    ...
}
// header “webbrowsercookies.h”
namespace WebBrowserStuff
{
    ...
}
...
```

这正是C++标准库的组织方式。类不能像这样做到函数的拆分。

这同时也意味着，用户可以轻易地扩展这一系列便利函数。

还有一个类无法提供的特性，命名空间中的类定义对用户的扩展来说是封闭的。

## 总结

尽可能使用非成员非友元函数，而非成员函数。这样做提高了封装程度，打包灵活性以及功能可扩展性。
