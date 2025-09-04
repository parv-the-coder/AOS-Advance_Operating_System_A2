#include "header.h"

// ---------------------------------------------------------------------
// helper function: check if a string is "." or ".."
// ---------------------------------------------------------------------
bool dot(const char* s)
{
    return (s[0] == '.' && (s[1] == '\0' || (s[1] == '.' && s[2] == '\0')));
}

// ---------------------------------------------------------------------
// helper function: search all entries in a directory (no recursion yet)
// ---------------------------------------------------------------------
bool dircheck(const string& path, const string& target)
{
    DIR* dir = opendir(path.c_str());          // try opening directory
    if (dir == nullptr)                         // if cannot open, skip
    {
        return false;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr)   // read each entry
    {
        if (dot(entry->d_name))    // skip "." and ".."
        {
            continue;
        }

        string name(entry->d_name);             // current entry name
        if (name == target)                      // check if matches target
        {
            closedir(dir);
            return true;
        }
    }

    closedir(dir);                              // close directory
    return false;                               // not found here
}

// ---------------------------------------------------------------------
// main search function: current dir + all subdirectories recursively
// ---------------------------------------------------------------------
bool search(const string& target)
{
    char cwd[PATH_MAX];                         // buffer to store current dir
    if (getcwd(cwd, sizeof(cwd)) == nullptr)   // get current working directory
    {
        perror("cannot get cwd");
        return false;
    }

    string base(cwd);                           // convert to string

    // -----------------------------------------------------------------
    // first, check current directory itself
    // -----------------------------------------------------------------
    if (dircheck(base, target))
    {
        return true;
    }

    // -----------------------------------------------------------------
    // now, loop through all entries to find subdirectories
    // -----------------------------------------------------------------
    DIR* dir = opendir(base.c_str());
    if (dir == nullptr)
    {
        return false;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        if (dot(entry->d_name))      // skip "." and ".."
        {
            continue;
        }

        if (entry->d_type == DT_DIR)              // only directories
        {
            string newpath = base + "/" + entry->d_name;

            // check target in this subdirectory
            if (dircheck(newpath, target))
            {
                closedir(dir);
                return true;
            }
        }
    }

    closedir(dir);
    return false;                                 // not found
}

// ---------------------------------------------------------------------
// example usage (uncomment for testing)
// ---------------------------------------------------------------------
// int main(int argc, char* argv[])
// {
//     if (argc != 2)
//     {
//         printf("usage: %s <filename>\n", argv[0]);
//         return 1;
//     }
//
//     string target = argv[1];
//     if (search(target))
//     {
//         printf("true\n");
//     }
//     else
//     {
//         printf("false\n");
//     }
//     return 0;
// }
