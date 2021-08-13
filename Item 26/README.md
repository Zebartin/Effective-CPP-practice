# Item 26: Postpone variable definitions as long as possible.

## 尽可能延后变量定义

比如：

```cpp
std::string encryptPassword(const std::string& password) {
    using namespace std;
    string encrypted;
    if (password.length() < MinimumPasswordLength) {
        throw logic_error("Password is too short");
    }
    ...
    return encrypted;
}
```

应将`encrypted`的定义挪到if语句之后

## 尽可能直接初始化

尽可能以某个值初始化对象，而不是先按默认构造函数生成对象再进行赋值。

除了能提高代码效率以外，还便于记录变量的用途和含义。

## 循环中的变量定义

以下有两种在循环中定义变量的方式：

```cpp
// Approach A: define outside loop
Widget w;
for (int i = 0; i < n; ++i) {
    w = some value dependent on i;
    ...
}
// Approach B: define inside loop
for (int i = 0; i < n; ++i) {
    Widget w(some value dependent on i);
    ...
}
```

两种方式的开销：

- Approach A：1次构造函数+1次析构函数+n次赋值
- Approach B：n次构造函数+n次析构函数

Approach A把变量名`w`暴露在更大的作用域里，可读性和可维护性更差。

除非明确有以下情况，否则用Approach B更好：

- 赋值比构造+析构开销更小；
- 这部分代码对性能很敏感（performance-sensitive）。

## 总结

尽可能延后变量定义。它提高了程序清晰度和执行效率。
