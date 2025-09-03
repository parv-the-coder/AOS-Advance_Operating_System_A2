#include <bits/stdc++.h>
using namespace std;

void echo(char input[4096]) 
{
    string s(input);

    // remove leading/trailing spaces
    s.erase(0, s.find_first_not_of(" \t"));
    s.erase(s.find_last_not_of(" \t") + 1);

    // remove the "echo " prefix
    if (s.rfind("echo", 0) == 0) 
    {
        s = s.substr(4);
        s.erase(0, s.find_first_not_of(" \t"));
    }

    // if quoted → remove quotes
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"') 
    {
        s = s.substr(1, s.size() - 2);
    }

    cout << s << "\n";
}
