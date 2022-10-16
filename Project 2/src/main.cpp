#include "../include/calculate.hpp"
#include "../include/compNoX.hpp"
#include "../include/compX.hpp"
#include "../include/specialComp.hpp"
#include <iostream>
using namespace std;

int main()
{
    while (true)
    {
        string in;
        cin >> in;
        if (ifXform(in))
        {
            variable var[100]{};
            int count = 0;
            while (ifXform(in))
            {
                var[count] = identify(in);
                count++;
                cin >> in;
            }
            string out = pushIn(in, var);
            cout << computeXform(out) << endl;
        }
        else if (ifSpecial(in))
        {
            string numIn = "";
            for (int i = 0; i < in.length(); i++)
            {
                if (in[i] == '(')
                {
                    numIn = in.substr(i + 1, in.length() - 2 - i);
                    break;
                }
            }
            if (getFunc(in) == "max" || getFunc(in) == "min" || getFunc(in) == "pow")
            {
                string a = "", b = "";
                for (int i = 0; i < numIn.length(); i++)
                {
                    if (numIn[i] == ',')
                    {
                        a = numIn.substr(0, i);
                        b = numIn.substr(i + 1);
                        break;
                    }
                }
                if (getFunc(in) == "max")
                {
                    cout << Mymax(a, b) << endl;
                }

                else if (getFunc(in) == "min")
                {
                    cout << Mymin(a, b) << endl;
                }
                else
                {
                    string d=b;
                    int positive=1;
                    if(b[0]=='-'){
                        d=b.substr(1);
                        positive=-1;
                    }
                    int c = 0;
                    for (int i = 0; i < d.length(); i++)
                    {
                        c += pow(10, d.length() - 1 - i)*(d[d.length()-1-i]-'0');
                    }
                    c*=positive;
                    cout << Mypow(a, c) << endl;
                }
            }
            else if (getFunc(in) == "abs")
            {
                cout << Myabs(numIn) << endl;
            }
            else if (getFunc(in) == "sqrt")
            {
                cout << Mysqrt(numIn) << endl;
            }
            else
            {
                cout << "The function is not supported!" << endl;
            }
        }
        else
        {
            cout << compute(in) << endl;
        }
    }
    return 0;
}