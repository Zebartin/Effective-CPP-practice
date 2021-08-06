#include <iostream>

class Transaction
{
public:
    explicit Transaction(const std::string &logInfo);
    void logTransaction(const std::string &logInfo) const;
};

Transaction::Transaction(const std::string &logInfo)
{
    logTransaction(logInfo);
}
void Transaction::logTransaction(const std::string &logInfo) const
{
    std::cout << logInfo << std::endl;
}

class BuyTransaction : public Transaction
{
public:
    BuyTransaction() : Transaction(createLogString()) {}

private:
    static std::string createLogString();
};
std::string BuyTransaction::createLogString()
{
    return "Buy Transaction created";
}

int main()
{
    BuyTransaction bt;
    return 0;
}
