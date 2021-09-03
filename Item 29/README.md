# Item 29: Strive for exception-safe code.

从以下这段代码开始：

```cpp
class PrettyMenu
{
public:
    ...
    void changeBackground(std::istream &imgSrc);
    ...
private:
    Mutex mutex;
    Image *bgImage;
    int imageChanges;
};

void PrettyMenu::changeBackground(std::istream &imgSrc)
{
    lock(&mutex);
    delete bgImage;
    ++imageChanges;
    bgImage = new Image(imgSrc);
    unlock(&mutex);
}
```

## 异常安全的要求

当有异常抛出时，异常安全的函数：

- 不会泄露资源；
- 不会导致数据结构崩坏。

上述代码中，如果`Image`的构造函数发生异常，一方面会导致互斥锁无法解锁，另一方面会导致`bgImage`指向不明，并且`imageChanges`莫名加1。

修正第一个问题比较简单，可以使用[Item 13](../Item%2013)和[Item 14](../Item%2014)介绍的用RAII进行资源管理的办法进行修复。

## 异常安全的三种保证

### 基本保证

如果异常发生，程序所有状态保持正常自洽，但可能无法确定具体是怎样的状态。

### 强保证

如果异常发生，程序所有状态与调用函数前的状态相同。也就是说，调用异常安全的函数后，程序只可能有两种确定的状态。

### 不抛出异常的保证（nothrow)

所有基于内置类型的操作都是不抛出异常的。

是否抛出异常取决于函数的实现，而非声明。如下函数也可能抛出异常：

```cpp
int doSomething() throw();
```

## 选择要实现的异常安全保证

### 不抛出异常

这是最理想的，但如果不想局限在C++中的C语言部分的话，就很难避免使用可能会抛出异常的函数。比如，任何使用动态分配内存的东西通常都会抛出`bad_alloc`异常。

更多的时候需要在基本保证和强保证之间选择。

### 强保证

把开头的代码改为**几乎是**强保证的实现并不困难：

- 首先，使用智能指针管理`Image *`；
- 其次，调整语句顺序，保证数值不会在异常可能发生之前变动。

```cpp
class PrettyMenu
{
    ...
    std::tr1::shared_ptr<Image> bgImage;
    ...
};
void PrettyMenu::changeBackground(std::istream &imgSrc)
{
    Lock ml(&mutex);
    bgImage.reset(new Image(imgSrc));
    ++imageChanges;
}
```

修改前的`Image`指针的`delete`操作现在交给智能指针自动管理了。

然而，这些修改尚且不能做到强保证。这个函数的参数`imgSrc`仍然可能发生意外：

> If the `Image` constructor throws an exception, it’s possible that the read marker for the input stream has been moved, and such movement would be a change in state visible to the rest of the program.

但先把这个问题放到一边，或许可以将`istream`改成相应的文件名来解决。

#### copy and swap

实现强保证的常见技巧是copy and swap，也就是先拷贝想要做改动的对象，在新的对象上做修改，再把两者交换。这里的交换需要保证不会抛出异常。

但这个技巧不能完全做到强保证，究其本质是因为在对局部数据（比如对象内部）做修改的时候，也可能影响到全局状态，比如数据库修改。

copy-and-`swap`的另一个问题是它的开销比较大。

### 基本保证

出于种种原因不提供强保证时，应提供基本保证。

如果不提供任何保证，整个系统将完全失去异常安全性。作者这里做了一个类比，女性要么怀孕要么没有怀孕，没有部分怀孕的；一个软件系统也是如此，没有部分异常安全的。

## 总结

- 异常安全的函数不会泄露资源，也不会导致数据结构崩坏，即使是有异常抛出的情况下。这样的函数提供基本保证，或强保证，或不抛异常的保证；
- 强保证通常可以通过copy-and-`swap`来实现，但强保证不总是符合实际需求的；
- 通常来说，一个函数所能提供的异常安全保证，不会强于它所调用的函数所提供的最弱保证。

<a href="../Item%2028"><-</a>

<a href="../Item%2030">-></a>
