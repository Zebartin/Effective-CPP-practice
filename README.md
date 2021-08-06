# Effective-CPP-practice
阅读 *Effective C++: 55 Specific Ways to Improve Your Programs and Designs, 3rd edition*，中文译名《Effective C++:改善程序与设计的55个具体做法》（第三版）一书时，做的一些代码验证练习和笔记。

按章节划分：
<details open>
  <summary>Chapter 1: Accustoming Yourself to C++</summary>

  - [Item 1: View C++ as a federation of languages.](Item%201)
  - [Item 2: Prefer `const`s, `enum`s, and `inline`s to `#define`s.](Item%202)
  - [Item 3: Use `const` whenever possible.](Item%203)
  - [Item 4: Make sure that objects are initialized before they’re used.](Item%204)
</details>

<details open>
  <summary>Chapter 2: Constructors, Destructors, and Assignment Operators</summary>

  - [Item 5: Know what functions C++ silently writes and calls.](Item%205)
  - [Item 6: Explicitly disallow the use of compiler-generated functions you do not want.](Item%206)
  - [Item 7: Declare destructors virtual in polymorphic base classes.](Item%207)
  - [Item 8: Prevent exceptions from leaving destructors.](Item%208)
  - [Item 9: Never call virtual functions during construction or destruction.](Item%209)
  - [Item 10: Have assignment operators return a reference to `*this`.](Item%2010)
  - [Item 11: Handle assignment to self in `operator=`.](Item%2011)
  - [Item 12: Copy all parts of an object.](Item%2012)
</details>
