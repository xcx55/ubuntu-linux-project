#include "protocol.hpp"

class cal
{
public:
    int Getresult(int a, int b, char op)
    {
        if (op == '+')
            return a + b;
        else if (op == '-')
            return a - b;
        else if (op == '/')
            return a / b;
        else if (op == '*')
            return a * b;
        else
            return 0;
    }
    response calc(const request &x)
    {
        int a = x._data_x;
        int b = x._data_y;
        char op = (char)x._oper;
        if (op == '/' && b == 0)
        {
            return {0,2};
        }
        int result = Getresult(a, b, op);
        return {result, 0};
    }
};