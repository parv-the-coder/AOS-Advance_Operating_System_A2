#include "header.h"

// Function to handle 'cd' command
void cd(vector<char *> args)
{

    // Count the number of arguments passed (excluding nullptr)
    int argc = 0;
    while (args[argc] != nullptr)
    {
        argc++;
    }

    // Too many arguments check
    if (argc > 2)
    {
        cout << "Invalid Arguments for cd\n";
        return;
    }

    // Default target directory
    string destdir;

    // Case 1: No argument or '~' → go to home directory
    if (argc == 1 || (argc == 2 && strcmp(args[1], "~") == 0))
    {
        destdir = home_dir;
    }

    // Case 2: Argument is '-' → swap with previous directory
    else if (argc == 2 && strcmp(args[1], "-") == 0)
    {
        destdir = previousDir;
    }

    // Case 3: Any other argument → treat as path
    else if (argc == 2)
    {
        destdir = args[1];
    }

    // Attempt to change directory
    if (chdir(destdir.c_str()) == -1)
    {
        perror("Error changing directory");
        return;
    }

    // If argument was '-' → print new directory (bash behavior)
    if (argc == 2 && strcmp(args[1], "-") == 0)
    {
        cout << destdir << "\n";
    }

    // Update current and previous directories
    if (!(argc == 2 && strcmp(args[1], "-") == 0))
    {
        previousDir = currentDir;
    }
    else
    {
        swap(previousDir, currentDir);
    }

    // Always update cur_dir and currentDir
    getcwd(cur_dir, PATH_MAX);
    currentDir = cur_dir;
}
