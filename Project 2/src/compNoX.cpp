#include "../include/compNoX.hpp"
#include "../include/calculate.hpp"

int priority(string a) //给操作符赋值，代表计算的优先等级
{
    if (a == "+" || a == "-")
        return 1;
    if (a == "*" || a == "/")
        return 2;
    return 0;
}

bool ifOper(string a)
{
    if (a == "+" || a == "-" || a == "*" || a == "/" || a == "(" || a == ")")
        return true;
    else
        return false;
}

queue<string> toArray(string a)
{
    queue<string> ans;
    string b = "";
    int count = 0;
    for (int i = 0; i < a.length(); i++)
    {
        if (!ifOper(a.substr(i, 1)))
        {
            b += a[i];
            if (i == a.length() - 1)
            {
                ans.push(b);
            }
        }
        else
        {
            if (b != "")
            {
                ans.push(b);
                count++;
                b = "";
            }
            ans.push(a.substr(i, 1));
            count++;
        }
    }
    return ans;
}

string compute(string a)
{
    queue<string> arr = toArray(a);
    int l = arr.size();
    stack<string> nums;
    stack<string> operators;
    for (int i = 0; i < l; i++)
    {
        string now = arr.front();
        arr.pop();
        if (!ifOper(now))
        {
            nums.push(now);
        }
        else
        {
            if (now == "(")
            {
                string part = "";
                int count = 0;
                while (arr.front() != ")")
                {
                    part += arr.front();
                    arr.pop();
                    count++;
                }
                i += (count + 1);
                arr.pop();
                nums.push(compute(part));
            }
            else if (operators.empty())
            {
                operators.push(now);
            }
            else if (priority(now) > priority(operators.top()))
            {
                operators.push(now);
            }
            else if (priority(now) <= priority(operators.top()))
            {
                string b = nums.top(); //进行单次计算
                nums.pop();
                string c = nums.top();
                nums.pop();
                string oper = operators.top();
                operators.pop();
                if (oper == "+")
                {
                    nums.push(AnyAdd(c, b));
                }
                if (oper == "-")
                {
                    nums.push(AnyMinus(c, b));
                }
                if (oper == "*")
                {
                    nums.push(AnyMultiply(c, b));
                }
                if (oper == "/")
                {
                    nums.push(AnyDivide(c, b));
                }
                operators.push(now);
            }
        }
    }
    while (!operators.empty())
    {
        string b = nums.top();
        nums.pop();
        string c = nums.top();
        nums.pop();
        string oper = operators.top();
        operators.pop();

        if (oper == "+")
        {
            nums.push(AnyAdd(c, b));
        }
        if (oper == "-")
        {
            nums.push(AnyMinus(c, b));
        }
        if (oper == "*")
        {
            nums.push(AnyMultiply(c, b));
        }
        if (oper == "/")
        {
            nums.push(AnyDivide(c, b));
        }
    }
    return nums.top();
}