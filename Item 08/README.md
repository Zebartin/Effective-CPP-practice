# Item 8: Prevent exceptions from leaving destructors.

## 不要在析构函数中抛出异常

C++不禁止在析构函数中抛出异常，但我们最好还是不要这么做。

比如说，`Widget`类的析构函数可能会抛出异常，那么当某个`vector<Widget> `对象销毁时，会同时产生两个活跃异常，C++处理不了这种情况，因而产生undefined behavior。在其他一些情况，则可能会造成程序提前终止。

## 不得不抛出异常的情况

有时候会遇到不得不在析构函数中处理异常的情况。比如用于数据库连接的类：

```cpp
class DBConnection {
public:
    ...
    static DBConnection create();
    void close();		// throw an exception if closing fails
};
```

为了保证用户能在关闭数据库连接的时候调用`close`，一个合理的做法是用另一个类管理其资源：

```cpp
class DBConn {
public:
    ...
    DBConn(const DBConnection &dbc): db(dbc) {}
	~DBConn()
    {
        db.close();
    }
private:
    DBConnection db;
};
```

这样一来，`DBConn`的析构函数中就可能会抛出异常。

### 主动提前处理异常

这时有两种处理方式。

- 直接终止程序

    ```cpp
    DBConn::~DBConn()
    {
        try { db.close(); }
        catch (...) {
            // make log entry that the call to close failed;
            std::abort();
        }
    }
    ```

- 无视异常

    ```cpp
    DBConn::~DBConn()
    {
        try { db.close(); }
        catch (...) {
            // make log entry that the call to close failed;       
        }
    }
    ```
    
    通常来说，无视异常（swallowing exceptions）不是什么好主意，因为它隐藏了异常。但是，在异常发生的情况下，保持程序持续运行有时候比终止程序更可取。

### 更好的设计策略

修改`DBConn`的接口设计，使其既可以自动通过析构函数关闭`DBConnection`，又可以由用户手动关闭。

```cpp
class DBConn
{
public:
    ...
    void close()
    {
        db.close();
        closed = true;
    }
    ~DBConn()
    {
        if (!closed)
        {
            try
            {
                db.close();
            }
            catch (...)
            {
                // make log entry that call to close failed;
                ...
            }
        }
    }

private:
    DBConnection db;
    bool closed;
};
```

## 总结

- 永远不要在析构函数中抛出异常。如果析构函数中调用的某个函数抛出了异常，析构函数应该主动处理它，要么无视它，要么终止程序。
- 如果类的用户需要能够对某个操作过程中抛出的异常做出响应，类应该提供一个进行这个操作的普通函数（非析构函数）。

<a href="../Item%2007"><-</a>

<a href="../Item%2009">-></a>
