#include "header.h"

// to execute built-ins, redirection, background/foreground execution
void executeCommand(char* cmd)
{
    try 
    {
        // split comd into arg
        vector<char*> args = token(cmd, " ");

        // no args
        if (args.empty())
        {
            return;
        }

        // check for redir
        bool redirectFlag = false; // flag for <, >, or >>
        for (char* a : args)
        {
            if (!a)
            {
                break;
            }

            if (strcmp(a, "<") == 0 || strcmp(a, ">") == 0 || strcmp(a, ">>") == 0)
            {
                redirectFlag = true;
                break;
            }
        }

        // if redir is there call redir() 
        if (redirectFlag)
        {
            redir(args, home_dir);
            return;
        }

        // cd comd
        if (strcmp(args[0], "cd") == 0)
        {
            cd(args);
            return;
        }

        // pwd comd
        if (strcmp(args[0], "pwd") == 0)
        {
            pwd(args.size() - 1); // pass no of arg
            return;
        }

        // history_update comd
        if (strcmp(args[0], "history_update") == 0)
        {
            his_save(string(input)); // save inp to history
            return;
        }

        // history cmd
        if (strcmp(args[0], "history") == 0)
        {
            int n = 10; // default last 10 comd

            if (args[1])
            {
                try
                {
                    n = stoi(args[1]);
                }
                catch (...)
                {
                    cerr << "Invalid argument for history\n";
                    return;
                }
            }

            his_print(n); // print last n comd
            return;
        }

        // pinfo cmd
        if (strcmp(args[0], "pinfo") == 0)
        {
            pid_t pid;

            if (args[1])
            {
                pid = stoi(args[1]); // use PID
            }
            else
            {
                pid = getpid(); // curr process PID
            }

            pinfo(pid);
            return;
        }

        // echo comd
        if (strcmp(args[0], "echo") == 0)
        {
            bool rFlag = false; // flag for redir inside echo

            // check if echo uses redir symbols
            for (int i = 1; args[i] != nullptr; i++)
            {
                if (strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0)
                {
                    rFlag = true;
                    break;
                }
            }

            if (rFlag)
            {
                redir(args, home_dir); // handle redir
            }
            else
            {
                // join all arg in single string
                string s;
                for (int i = 1; args[i] != nullptr; i++)
                {
                    s += args[i];
                    if (args[i + 1]) s += " ";
                }

                char* buf = strdup(s.c_str());
                echo(buf);
                free(buf);
            }

            return;
        }

        // search comd
        if (strcmp(args[0], "search") == 0)
        {
            if (args.size() > 3)
            {
                cout << "invalid arguments\n";
                return;
            }

            if (args[1] && search(args[1]))
            {
                cout << "True\n";
            }
            else
            {
                cout << "False\n";
            }

            return;
        }

        // ls cmd
        if (strcmp(args[0], "ls") == 0)
        {
            bool showAll = false;   // -a flag
            bool showLong = false;  // -l flag
            vector<string> paths;   // directories/files to list

            for (size_t i = 1; args[i] != nullptr; i++)
            {
                if (strcmp(args[i], "-a") == 0)
                {
                    showAll = true;
                }
                else if (strcmp(args[i], "-l") == 0)
                {
                    showLong = true;
                }
                else if (strcmp(args[i], "-al") == 0 || strcmp(args[i], "-la") == 0)
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

        bool bgFlag = false;   // background flag
        size_t argCount = 0;

        // count arg
        for (; args[argCount] != nullptr; argCount++);

        // check if last arg is &
        if (argCount > 0 && strcmp(args[argCount - 1], "&") == 0)
        {
            bgFlag = true;
            args[argCount - 1] = nullptr; // remove & from args
        }

        // for execvp
        vector<char*> cargs;
        for (char* a : args)
        {
            if (a)
            {
                cargs.push_back(a);
            }
        }

        cargs.push_back(nullptr); // null-terminate

        // decide background or foreground
        if (bgFlag)
        {
            background(cargs);
        }
        else
        {
            foreground(cargs);
        }
    }
    
    catch (const std::exception& e)
    {
        cerr << "Unknown error occurred: " << e.what() << "\n";
    }
    catch (...)
    {
        cerr << "Unknown error occurred\n";
    }
}
