#include<bits/stdc++.h>
#include<unistd.h>
#include<pwd.h>
#include <sys/wait.h>
using namespace std;
extern char cwd[PATH_MAX];
extern string currentDir;
extern string previousDir;
extern string  home_dir;
extern char cur_dir[PATH_MAX];
extern char pre_dir[PATH_MAX];


void cd_func(vector<char*> args) {
    // int n = args.size() - 1;   // args includes nullptr at end
    int n = 0;
    for (; args[n] != nullptr; ++n);  // count arguments excluding nullptr

    if (n > 2) {
        cout << "Invalid Arguments for cd" << "\n";
        return;
    }

    // Case 1: cd OR cd ~
    if (n == 1 || (n == 2 && strcmp(args[1], "~") == 0)) {
        if (chdir(home_dir.c_str()) == -1) {
            perror("Error");
            return;
        }
        previousDir = currentDir;
        getcwd(cur_dir, PATH_MAX);
        currentDir = cur_dir;
        return;
    }

    // Case 2: cd <path> (not -)
    else if (n == 2 && strcmp(args[1], "-") != 0) {
        if (chdir(args[1]) == -1) {
            perror("No Such Directory or File exists");
            return;
        }
        previousDir = currentDir;
        getcwd(cur_dir, PATH_MAX);
        currentDir = cur_dir;
        return;
    }

else if (n == 2 && strcmp(args[1], "-") == 0) {
    if (chdir(previousDir.c_str()) == -1) {
        perror("No such directory or file exists");
        return;
    }
    string temp = currentDir;
    currentDir = previousDir;
    previousDir = temp;
    getcwd(cur_dir, PATH_MAX);
    cout << currentDir << "\n";   // bash-like behaviour
    return;
}
// fallback
else {
    cout << "Invalid Arguments for cd" << "\n";
}

}
