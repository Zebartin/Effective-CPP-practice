# Item 43: Know how to access names in templatized base classes.

有这样一个类模板及其派生：

```cpp
template <typename Company>
class MsgSender
{
public:
    void sendClear(const MsgInfo &info)
    {
        std::string msg;
        // create msg from info;
        Company c;
        c.sendCleartext(msg);
    }
    void sendSecret(const MsgInfo &info) { ... }
};

template <typename Company>
class LoggingMsgSender : public MsgSender<Company>
{
public:
    void sendClearMsg(const MsgInfo &info)
    {
        // write "before sending" info to the log;
        sendClear(info);    // can not compile
        // write "after sending" info to the log;
    }
    ...
};
```

编译器将找不到上述的`sendClear`函数，因为在模板参数`Company`尚未确定的情况下，无从得知`MsgSender<Company>`是怎样的类。

比如说有一个`CompanyZ`类，`MsgSender<CompanyZ>`是为它特化的一个模板，没有实现`sendClear`函数：

```cpp
template <>
class MsgSender<CompanyZ>
{
public:
    void sendSecret(const MsgInfo &info) { ... }
};
```

这就是为什么上述情况中C++会拒绝编译。一般来说，编译器不会在模板化的基类中查找继承的命名，需要通过某种方式来启用这种行为。

## 三种启用方式

### `this->`前缀

修改为：

```cpp
this->sendClear(info);
```

编译器将假定该函数被继承。

### `using`声明

通过`using`将隐藏的基类命名带到派生类作用域中：

```cpp
public:
    using MsgSender<Company>::sendClear;
    ...
    void sendClearMsg(const MsgInfo& info) { ... }
```

### 显式声明函数所属类

```cpp
MsgSender<Company>::sendClear(info);
```

这一般是最不好的一种做法，因为显式声明会使得虚函数失效。

## 总结

在派生类类模板中，要指向基类类模板中的名称可以有三种方式：

- `this->`前缀；
- 通过`using`使其可见；
- 显式地指明基类。
