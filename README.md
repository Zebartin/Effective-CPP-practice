# Effective-CPP-practice
阅读 *Effective C++: 55 Specific Ways to Improve Your Programs and Designs, 3rd edition*，中文译名《Effective C++:改善程序与设计的55个具体做法》（第三版）一书时，做的一些代码验证练习和笔记。

按章节划分：
<details>
  <summary>Chapter 1: Accustoming Yourself to C++</summary>

  - [Item 1: View C++ as a federation of languages.](Item%2001)
  - [Item 2: Prefer `const`s, `enum`s, and `inline`s to `#define`s.](Item%2002)
  - [Item 3: Use `const` whenever possible.](Item%2003)
  - [Item 4: Make sure that objects are initialized before they’re used.](Item%2004)
</details>

<details>
  <summary>Chapter 2: Constructors, Destructors, and Assignment Operators</summary>

  - [Item 5: Know what functions C++ silently writes and calls.](Item%2005)
  - [Item 6: Explicitly disallow the use of compiler-generated functions you do not want.](Item%2006)
  - [Item 7: Declare destructors virtual in polymorphic base classes.](Item%2007)
  - [Item 8: Prevent exceptions from leaving destructors.](Item%2008)
  - [Item 9: Never call virtual functions during construction or destruction.](Item%2009)
  - [Item 10: Have assignment operators return a reference to `*this`.](Item%2010)
  - [Item 11: Handle assignment to self in `operator=`.](Item%2011)
  - [Item 12: Copy all parts of an object.](Item%2012)
</details>

<details>
  <summary>Chapter 3: Resource Management</summary>

  资源（resource）是指使用完毕后要返还给系统的东西。比如内存、文件描述符、互斥锁、GUI的字体和笔刷、数据库连接以及网络套接字。

  - [Item 13: Use objects to manage resources.](Item%2013)
  - [Item 14: Think carefully about copying behavior in resource-managing classes.](Item%2014)
  - [Item 15: Provide access to raw resources in resource-managing classes.](Item%2015)
  - [Item 16: Use the same form in corresponding uses of `new` and `delete`.](Item%2016)
  - [Item 17: Store `new`ed objects in smart pointers in standalone statements.](Item%2017)
</details>

<details>
  <summary>Chapter 4: Designs and Declarations</summary>

  - [Item 18: Make interfaces easy to use correctly and hard to use incorrectly.](Item%2018)
  - [Item 19: Treat class design as type design.](Item%2019)
  - [Item 20: Prefer pass-by-reference-to-`const` to pass-by-value.](Item%2020)
  - [Item 21: Don’t try to return a reference when you must return an object.](Item%2021)
  - [Item 22: Declare data members `private`.](Item%2022)
  - [Item 23: Prefer non-member non-friend functions to member functions.](Item%2023)
  - [Item 24: Declare non-member functions when type conversions should apply to all parameters.](Item%2024)
  - [Item 25: Consider support for a non-throwing `swap`.](Item%2025)
</details>

<details>
  <summary>Chapter 5: Implementations</summary>

  - [Item 26: Postpone variable definitions as long as possible.](Item%2026)
  - [Item 27: Minimize casting.](Item%2027)
  - [Item 28: Avoid returning “handles” to object internals.](Item%2028)
  - [Item 29: Strive for exception-safe code.](Item%2029)
  - [Item 30: Understand the ins and outs of inlining.](Item%2030)
  - [Item 31: Minimize compilation dependencies between files.](Item%2031)
</details>

<details>
  <summary>Chapter 6: Inheritance and Object-Oriented Design</summary>

  - [Item 32: Make sure public inheritance models “is-a.”](Item%2032)
  - [Item 33: Avoid hiding inherited names.](Item%2033)
  - [Item 34: Differentiate between inheritance of interface and inheritance of implementation.](Item%2034)
  - [Item 35: Consider alternatives to virtual functions.](Item%2035)
  - [Item 36: Never redefine an inherited non-virtual function.](Item%2036)
  - [Item 37: Never redefine a function’s inherited default parameter value.](Item%2037)
  - [Item 38: Model “has-a” or “is-implemented-in-terms-of” through composition.](Item%2038)
  - [Item 39: Use private inheritance judiciously.](Item%2039)
  - [Item 40: Use multiple inheritance judiciously.](Item%2040)
</details>
