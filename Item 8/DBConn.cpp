#include <stdexcept>
#include <iostream>
#include <cstdlib>

class DBConnection
{
public:
    static DBConnection create()
    {
        DBConnection db;
        return db;
    }
    void close()
    {
        throw std::runtime_error("close error");
    }
};

class DBConn
{
public:
    DBConn(const DBConnection &dbc) : db(dbc), closed(false) {}
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
                std::cout << "swallowing error" << std::endl;
            }
        }
    }

private:
    DBConnection db;
    bool closed;
};

int main()
{
    DBConn dbc(DBConnection::create());

    std::srand(time(0));
    int randBit = rand() % 2;
    if (randBit == 1)
    {
        std::cout << "closing manually" << std::endl;
        dbc.close();
    }
    else
    {
        std::cout << "closing through destructor" << std::endl;
    }
}