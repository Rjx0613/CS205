#include "../include/specialComp.hpp"
#include "../include/calculate.hpp"

string getFunc(string s)
{
    int count;
    for (int i = 0; i < s.length(); i++)
    {
        count = i;
        if (s[i] == '(')
            break;
    }
    string func = s.substr(0, count);
    return func;
}

string functions[5] = {"max", "min", "sqrt", "pow", "abs"};

bool ifSpecial(string a)
{
    string func = getFunc(a);
    for (int i = 0; i < 5; i++)
    {
        if (func == functions[i])
            return true;
    }
    return false;
}

string Myabs(string a)
{
    string ans = a;
    if (a[0] == '-')
    {
        ans = ans.substr(1);
    }
    return ans;
}

string Mymax(string a, string b)
{
    if (a[0] == '-' && b[0] == '-')
    {
        if (compareTo(Myabs(a), Myabs(b)))
            return b;
        else
            return a;
    }
    else if (a[0] == '-')
    {
        return b;
    }
    else if (b[0] == '-')
    {
        return a;
    }
    else if (compareTo(a, b))
        return a;
    else
        return b;
}

string Mymin(string a, string b)
{
    string bigger = Mymax(a, b);
    if (a == bigger)
        return b;
    else
        return a;
}

string Mysqrt(string a)
{
    string bigger = a;
    string smaller = "0";
    string mid;
    for (int i = 0; i < 35; i++)
    {
        mid = AnyMultiply(AnyAdd(bigger, smaller), "0.5");
        string comp = AnyMultiply(mid, mid);
        if (compareTo(comp, a))
            bigger = mid;
        else
            smaller = mid;
    }
    return mid.substr(0, 10);
}

string Mypow(string a, int b)
{
    string ans = a;
    if (b == 0)
    {
        return "1";
    }
    else if (b >= 1)
    {
        for (int i = 1; i < b; i++)
        {
            ans = AnyMultiply(ans, a);
        }
        return ans;
    }
    else
        for (int i = 1; i < (-b); i++)
        {
            ans = AnyMultiply(ans, a);
        }
    return AnyDivide("1", ans);
}