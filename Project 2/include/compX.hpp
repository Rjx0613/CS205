#include <bits/stdc++.h>
#pragma once
using namespace std;

struct variable{
    string name;
    string value;
};

bool ifXform(string in);

bool ifX(string in);

variable identify(string in);

string pushIn(string equation,variable unknown[]);

string computeXform(string out);