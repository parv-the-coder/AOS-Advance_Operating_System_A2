#include "header.h"

void executeCommand(char* cmd)
{
    vector<char*> args = token(cmd, " ");
    
    if(args.empty())
    {
        return;
    }

    // check redirection
    bool redirectFlag = false;
    for(char* a : args)
    {
        if(!a)
        {
            break;
        }

        if(strcmp(a, "<") == 0 || strcmp(a, ">") == 0 || strcmp(a, ">>") == 0)
        {
            redirectFlag = true;
            break;
        }
    }

    if(redirectFlag)
    {
        redir(args, home_dir);
        return;
    }

    // built-ins
    if(strcmp(args[0], "cd") == 0)
    {
        cd(args);
        return;
    }

    if(strcmp(args[0], "pwd") == 0)
    {
        pwd(args.size() - 1);
        return;
    }

    if(strcmp(args[0], "history_update") == 0)
    {
        his_save(string(input));
        return;
    }

    if(strcmp(args[0], "history") == 0)
    {
        int n = 10;

        if(args[1])
        {
            try
            {
                n = stoi(args[1]);
            }
            catch(...)
            {
                cerr << "invalid argument for history\n";
                return;
            }
        }

        his_print(n);
        return;
    }

    if(strcmp(args[0], "pinfo") == 0)
    {
        pid_t pid;

        if(args[1])
        {
            pid = stoi(args[1]);
        }
        else
        {
            pid = getpid();
        }

        pinfo(pid);
        return;
    }

    if(strcmp(args[0], "echo") == 0)
    {
        bool rFlag = false;

        for(int i = 1; args[i] != nullptr; i++)
        {
            if(strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0)
            {
                rFlag = true;
                break;
            }
        }

        if(rFlag)
        {
            redir(args, home_dir);
        }
        else
        {
            echo(input);
        }

        return;
    }

    if(strcmp(args[0], "search") == 0)
    {
        if(args.size() > 3)
        {
            cout << "invalid arguments\n";
            return;
        }

        if(args[1] && search(args[1]))
        {
            cout << "True\n";
        }
        else
        {
            cout << "False\n";
        }

        return;
    }

    if(strcmp(args[0], "ls") == 0)
    {
        bool showAll = false;
        bool showLong = false;
        vector<string> paths;

        for(size_t i = 1; args[i] != nullptr; i++)
        {
            if(strcmp(args[i], "-a") == 0)
            {
                showAll = true;
            }
            else if(strcmp(args[i], "-l") == 0)
            {
                showLong = true;
            }
            else if(strcmp(args[i], "-al") == 0 || strcmp(args[i], "-la") == 0)
            {
                showAll = true;
                showLong = true;
            }
            else
            {
                paths.push_back(args[i]);
            }
        }

        ls(paths, showAll, showLong);
        return;
    }

    // external commands
    bool bgFlag = false;
    size_t argCount = 0;

    for(; args[argCount] != nullptr; argCount++);

    if(argCount > 0 && strcmp(args[argCount - 1], "&") == 0)
    {
        bgFlag = true;
        args[argCount - 1] = nullptr;
    }

    vector<char*> cargs;

    for(char* a : args)
    {
        if(a)
        {
            cargs.push_back(a);
        }
    }

    cargs.push_back(nullptr);

    if(bgFlag)
    {
        background(cargs);
    }
    else
    {
        foreground(cargs);
    }
}
