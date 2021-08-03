#include <iostream>

class GamePlayer
{
private:
    static const int NumTurns = 5;
    int scores[NumTurns];
};

class EnumGamePlayer
{
private:
    enum
    {
        NumTurns = 5
    };
    int scores[NumTurns];
};

class CostEstimate
{
private:
    static const double FudgeFactor;
};
const double CostEstimate::FudgeFactor = 1.35;

class AnotherCostEstimate
{
private:
    static constexpr double FudgeFactor = 1.35;
};

int main()
{
    return 0;
}