# Item 38: Model “has-a” or “is-implemented-in-terms-of” through composition.

当一个类的对象包含另一个类的对象时，这两个类的关系可以被称作组合（composition），也可以称作layering、containment、aggregation以及embedding。

组合有两种含义：

- 如果是在应用领域（application domain）里，它意味着has-a，比如`Person`类包含`Address`类；
- 如果是在实现领域（implementation domain）里，它意味着implemented-in-terms-of，比如[set.cpp](./set.cpp)的`Set`类包含标准库的`list`类。这里的`Set`类为了用时间换空间，没有复用标准库的`set`类。

## 总结

- 组合（composition）与public继承的意思完全不同；
- 从应用角度看，组合意味着has-a。从实现角度看，它意味着is-implemented-in-terms-of。
