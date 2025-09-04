#include "header.h"
#include <stack>

// check for dot dot-dot
bool dot(const char* s)
{
    return (s[0] == '.' && (s[1] == '\0' || (s[1] == '.' && s[2] == '\0')));
}

// search comd
bool search(const string& target)
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == nullptr)
    {
        perror("cannot get cwd");
        return false;
    }

    string base(cwd);

    // stack for dfs traversal
    stack<string> dirs;
    dirs.push(base);

    while (!dirs.empty())
    {
        string current = dirs.top();
        dirs.pop();

        DIR* dir = opendir(current.c_str());
        if (!dir)
        {
            continue;
        }

        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr)
        {
            
            if (dot(entry->d_name))
            {
                continue;
            } 

            string path = current + "/" + entry->d_name;

            // check if it matches the target
            if (entry->d_type != DT_DIR && entry->d_name == target)
            {
                closedir(dir);
                return true;
            }

            // if a directory push it to stack
            if (entry->d_type == DT_DIR)
            {
                dirs.push(path);
            }
        }

        closedir(dir);
    }

    return false;
}
