// #include<bits/stdc++.h>
// #include<unistd.h>
// #include<pwd.h>
// #include <sys/wait.h>
// using namespace std;

// void pwd(int arg)
// {
//     if(arg>1)
//     {
//         perror("Invalid Arguments for pwd");
//         return;
//     }
    
//     char prewordir[PATH_MAX];
//     getcwd(prewordir,PATH_MAX);

//     if(prewordir!=nullptr)
//     {
//         cout<<prewordir<<endl;
//     }
    
//     else
//     {
//         perror("Error in PWD: ");
//         return;
//     }
// }

#include <bits/stdc++.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/wait.h>
using namespace std;

void pwd(int arg)
{
    if(arg>1)
    {
        cerr << "Invalid Arguments for pwd" << "\n";
        return;
    }
    
    char prewordir[PATH_MAX];
    if (getcwd(prewordir, PATH_MAX) != nullptr) {
        cout << prewordir << endl;
    } else {
        perror("Error in PWD");
    }
}
