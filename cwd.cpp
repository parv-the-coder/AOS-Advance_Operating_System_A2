#include "header.h"

void pwd(int arg_count)
{
    // check arg count
    if (arg_count > 1)
    {
        cerr << "Too many arguments for pwd\n";
        return;
    }

    // buffer for storing cur dir path
    char cwd_buff[PATH_MAX];

    // getcwd returns nullptr on error
    if (getcwd(cwd_buff, sizeof(cwd_buff)) != nullptr)
    {
        cout << cwd_buff << endl;
    }
    
    else
    {
        perror("PWD error");
    }

}