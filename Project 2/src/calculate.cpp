#include "../include/calculate.hpp"

string Add(string a, string b)
{
    int num1 = a.length();
    int num2 = b.length();
    int Max = max(num1, num2);
    int n1[10000]{};
    int n2[10000]{};
    for (int i = 0; i < num1; i++)
    {
        n1[i] = a[num1 - i - 1] - '0';
    }
    for (int i = 0; i < num2; i++)
    {
        n2[i] = b[num2 - i - 1] - '0';
    }
    long long length = max(num1, num2) + 1;
    int c[10000]{};
    int jin = 0;
    for (int i = 0; i < length - 1; i++)
    {
        c[i] = (n1[i] + n2[i] + jin) % 10;
        jin = (n1[i] + n2[i] + jin) / 10;
    }
    c[length - 1] = jin;
    ostringstream os;
    for (int i = 1 - jin; i < length; i++) //若最后的进位为零则直接从倒数第二位开始输出
    {
        os << c[length - i - 1];
    }
    return os.str();
}

string AnyAdd(string a, string b)
{
    int dot1 = Dot(a);
    int dot2 = Dot(b);
    string a1 = deleteDot(a);
    string b1 = deleteDot(b);
    if (dot1 >= dot2)
    {
        for (int i = 0; i < dot1 - dot2; i++)
        {
            b1 = b1 + "0";
        }
        string partAns = Add(a1, b1);
        ostringstream os;
        int count = 0;
        for (int i = partAns.length() - 1; i >= 0; i--)
        {
            if (partAns[i] == '0')
            {
                count++;
            }
            else
                break;
        }
        if (count < dot1)
        {
            for (int i = 0; i < partAns.length() - dot1; i++)
            {
                os << partAns[i];
            }
            os << ".";
            for (int i = partAns.length() - dot1; i < partAns.length() - count; i++)
            {
                os << partAns[i];
            }
        }
        else if (count == dot1)
        {
            for (int i = 0; i < partAns.length() - count; i++)
            {
                os << partAns[i];
            }
        }
        else
        {
            for (int i = 0; i < partAns.length() - dot1; i++)
            {
                os << partAns[i];
            }
        }
        return os.str();
    }
    else
        return AnyAdd(b, a);
}

string Minus(string a, string b)
{
    int num1 = a.length();
    int num2 = b.length();
    int Max = max(num1, num2);
    int n1[10000]{};
    int n2[10000]{};
    for (int i = 0; i < num1; i++)
    {
        n1[i] = a[num1 - i - 1] - '0';
    }
    for (int i = 0; i < num2; i++)
    {
        n2[i] = b[num2 - i - 1] - '0';
    }
    int ans[10000]{};
    ostringstream os;
    if (num1 >= num2)
    {
        int jie = 0;
        for (int i = 0; i < num1; i++)
        {
            if (n1[i] + jie >= n2[i])
            {
                ans[i] = n1[i] + jie - n2[i];
                jie = 0;
            }
            else
            {
                ans[i] = n1[i] + 10 + jie - n2[i];
                jie = -1;
            }
        }
        if (jie == -1)
        {
            string part = Minus(b, a);
            os << "-" << part;
        }
        else
        {
            int begin = num1 - 1;
            for (int i = 0; i < num1; i++)
            {
                if (ans[num1 - i - 1] != 0)
                {
                    begin = i;
                    break;
                }
            }
            if (ans[num1 - begin - 1] == 0)
            {
                os << "0";
            }
            else
            {
                for (int i = begin; i < num1; i++)
                {
                    os << ans[num1 - i - 1];
                }
            }
        }
    }
    else
    {
        string answer = Minus(b, a);
        os << "-" << answer;
    }
    return os.str();
}

string AnyMinus(string a, string b)
{
    if (a == b)
    {
        return "0";
    }
    int dot1 = Dot(a);
    int dot2 = Dot(b);
    string a1 = deleteDot(a);
    string b1 = deleteDot(b);
    string a2 = a1;
    string b2 = b1;
    if (a1[0] == '0')
    {
        a2 = a1.substr(1);
    }
    if (b1[0] == '0')
    {
        b2 = b1.substr(1);
    }
    if (dot1 >= dot2)
    {
        for (int i = 0; i < dot1 - dot2; i++)
        {
            b2 += "0";
        }
    }
    else
    {
        for (int i = 0; i < dot2 - dot1; i++)
        {
            a2 += "0";
        }
    }
    int bigger = max(dot1, dot2);
    string partAns = Minus(a2, b2);
    bool positive;
    if (partAns[0] == '-')
    {
        positive = 0;
        partAns = partAns.substr(1);
    }
    else
    {
        positive = 1;
    }
    int count = 0;
    ostringstream os;
    for (int i = partAns.length() - 1; i >= 0; i--)
    {
        if (partAns[i] == '0')
        {
            count++;
        }
        else
            break;
    }
    if (count < bigger)
    {
        if (partAns.length() <= bigger)
        {
            os << "0.";
            for (int i = 0; i < bigger - partAns.length(); i++)
            {
                os << "0";
            }
            for (int i = 0; i < partAns.length() - count; i++)
            {
                os << partAns[i];
            }
        }
        else
        {
            for (int i = 0; i < partAns.length() - bigger; i++)
            {
                os << partAns[i];
            }
            os << ".";
            for (int i = partAns.length() - bigger; i < partAns.length() - count; i++)
            {
                os << partAns[i];
            }
        }
    }
    else if (count == bigger)
    {
        for (int i = 0; i < partAns.length() - count; i++)
        {
            os << partAns[i];
        }
    }
    else
    {
        for (int i = 0; i < partAns.length() - bigger; i++)
        {
            os << partAns[i];
        }
    }
    if (positive)
    {
        return os.str();
    }
    else
    {
        return "-" + os.str();
    }
}

string AnyMultiply(string a, string b)
{
    if (a == "0" || b == "0")
    {
        return "0";
    }
    string a1 = deleteDot(a);
    string b1 = deleteDot(b);
    int dot1 = Dot(a);
    int dot2 = Dot(b);
    int num1 = a1.length();
    int num2 = b1.length();
    int n1[10000]{};
    int n2[10000]{};
    for (int i = 0; i < num1; i++)
    {
        n1[i] = a1[num1 - 1 - i] - '0';
    }
    for (int i = 0; i < num2; i++)
    {
        n2[i] = b1[num2 - 1 - i] - '0';
    }
    int ans[10000]{};
    for (int i = 0; i < num1; i++)
    {
        for (int j = 0; j < num2; j++)
        {
            ans[i + j] += n1[i] * n2[j];
            ans[i + j + 1] += ans[i + j] / 10;
            ans[i + j] %= 10;
        }
    }
    ostringstream os;
    int j = 0;
    for (int i = 0; i < num1 + num2; i++)
    {
        if (ans[num1 + num2 - 1 - i] != 0)
        {
            j = i;
            break;
        }
    }
    for (int i = j; i < num1 + num2; i++)
    {
        os << ans[num1 + num2 - 1 - i];
    }
    string partAns = os.str();
    ostringstream os1;
    if ((dot1 + dot2) == 0)
    {
        return partAns;
    }
    else
    {
        int count = 0;
        for (int i = partAns.length() - 1; i >= 0; i--)
        {
            if (partAns[i] == '0')
            {
                count++;
            }
            else
                break;
        }
        if (partAns.length() == dot1 + dot2)
        {
            os1 << "0";
        }
        if (count < dot1 + dot2)
        {

            for (int i = 0; i < partAns.length() - dot1 - dot2; i++)
            {
                os1 << partAns[i];
            }
            os1 << ".";
            for (int i = partAns.length() - dot1 - dot2; i < partAns.length() - count; i++)
            {
                os1 << partAns[i];
            }
        }
        else if (count == dot1 + dot2)
        {
            for (int i = 0; i < partAns.length() - count; i++)
            {
                os1 << partAns[i];
            }
        }
        else
        {
            for (int i = 0; i < partAns.length() - dot1 - dot2; i++)
            {
                os1 << partAns[i];
            }
        }
        return os1.str();
    }
}

string AnyDivide(string a, string b) //二分逼近
{
    if (a == "0")
    {
        return "0";
    }
    string bigger = "1";
    string smaller = "0";
    bool positive = ((a[0] == '-' && b[0] == '-') || (a[0] != '-' && b[0] != '-'));
    if (a[0] == '-')
    {
        a = a.substr(1);
    }
    if (b[0] == '-')
    {
        b = b.substr(1);
    }
    while (compareTo(a, AnyMultiply(bigger, b))) //找到比a大的最小10的n次方数
    {
        bigger += "0";
    }
    string mid;
    for (int i = 0; i < 1000; i++)
    {
        mid = AnyMultiply(AnyAdd(bigger, smaller), "0.5");
        if (compareTo(AnyMultiply(mid, b), a))
        {
            bigger = mid;
        }
        else
        {
            smaller = mid;
        }
    }
    ostringstream os;
    for (int i = 0; i < 11; i++)
    {
        os << mid[i];
    }
    if (positive)
    {
        return os.str();
    }
    else
    {
        return "-" + os.str();
    }
}

int Dot(string a)
{
    for (int i = 0; i < a.length(); i++)
    {
        if (a[i] == '.')
        {
            return a.length() - i - 1;
        }
    }
    return 0;
}

string deleteDot(string a)
{
    ostringstream os;
    for (int i = 0; i < a.length(); i++)
    {
        if (a[i] != '.')
        {
            os << a[i];
        }
    }
    return os.str();
}

bool compareTo(string a, string b)
{
    string difference = AnyMinus(a, b);
    if (difference[0] == '-')
    {
        return false;
    }
    else
        return true;
}