#include "header.h"

// cd comand
void cd(vector<char *> args)
{

    // excluding null pointer count arg
    int argc = 0;
    while (args[argc] != nullptr)
    {
        argc++;
    }

    // arguments check
    if (argc > 2)
    {
        cout << "Invalid Arguments for cd\n";
        return;
    }

    // target directory
    string destdir;

    // no arg or '~' goes home dir
    if (argc == 1 || (argc == 2 && strcmp(args[1], "~") == 0))
    {
        destdir = home_dir;
    }

    // '-' prev dir
    else if (argc == 2 && strcmp(args[1], "-") == 0)
    {
        destdir = prev_dir;
    }

    // treat as path
    else if (argc == 2)
    {
        destdir = args[1];
    }

    // change directory
    if (chdir(destdir.c_str()) == -1)
    {
        perror("Error changing directory");
        return;
    }

    //  '-' new dir
    if (argc == 2 && strcmp(args[1], "-") == 0)
    {
        cout << destdir << "\n";
    }

    // update curr and prev
    if (!(argc == 2 && strcmp(args[1], "-") == 0))
    {
        prev_dir = currdir;
    }
    else
    {
        swap(prev_dir, currdir);
    }

    // Always update cur_dir and currdir
    getcwd(cur_dir, PATH_MAX);
    currdir = cur_dir;
}
