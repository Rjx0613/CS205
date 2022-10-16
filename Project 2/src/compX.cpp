#include "../include/compX.hpp"
#include "../include/compNoX.hpp"

bool ifXform(string in)
{
    if ((in[0] > '9' || in[0] < '0') && in[1] == '=')
        return true;
    else
        return false;
}

bool ifX(string a)
{
    if (ifOper(a) || (a[0] > '0' && a[0] < '9')||a[0]=='.'||a[0]=='-')
        return false;
    else
        return true;
}

variable identify(string in)
{
    variable var;
    var.name = in[0];
    var.value = in.substr(2);
    return var;
}

string pushIn(string equation, variable unknown[])
{
    string out="";
    for(int i=0;i<equation.length();i++){
        string part=equation.substr(i,1);
        if(!ifX(part)){
            out+=part;
        }else{
            int j=0;
            while(unknown[j].name!=part){
                j++;
            }
            out+=unknown[j].value;
        }
    }
    return out;
}

string computeXform(string out)
{
    return compute(out);
}