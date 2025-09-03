#include<bits/stdc++.h>
#include<unistd.h>
#include<pwd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "header.h"

using namespace std;

void executeCommand(char* cmd) 
{
    vector<char*> args = token(cmd, " ");
    
    // check tokens, not raw string
    bool hasRedirect = false;
    for (char* a : args) {
        if (!a) break;
        if (strcmp(a, "<") == 0 || strcmp(a, ">") == 0 || strcmp(a, ">>") == 0) {
            hasRedirect = true;
            break;
        }
    }
    
    if (hasRedirect) {
        execute_command_with_redirection(args, home_dir);
        return;
    }
    
    
    else if (strcmp(args[0], "cd") == 0) {
        cd_func(args);
        return ;
    } 
    else if (strcmp(args[0], "history_update") == 0) {
        save_history(string(input));  // input is the command string
        
        return;
    }
    
    else if (strcmp(args[0], "pwd") == 0) {
        pwd(args.size()-1);
        return ;
    } 
    
    else if (strcmp(args[0], "history") == 0) {
        int n = 10; // default
        
        // Count only non-null arguments after command name
        int argCount = 0;
        for (int i = 1; args[i] != nullptr; i++) argCount++;
        
        if (argCount > 1) {
            cerr << "Too many arguments for history\n";
            return;
        }
        
        if (argCount == 1) {
            try {
                n = stoi(args[1]);
                if (n < 0) {
                    cerr << "Invalid argument for history\n";
                    return;
                }
            } catch (...) {
                cerr << "Invalid argument for history\n";
                return;
            }
        }
        
        print_history(n);
        return;
    }
    
    
    else if (strcmp(args[0], "pinfo") == 0) {
        pid_t pid;
        if (args.size()>3){
            perror("Invalid Arguments with Pinfo");
            return ;
        }
        if (args.size() == 2) 
            pid = getpid();
        else 
            pid = stoi(args[1]);

        printProcessInfo(pid);
        return ;
        }




    else if (strcmp(args[0], "ls") == 0) {
        bool showall = false;
        bool showlong = false;
        vector<string> paths;
        for (size_t  i = 1; i < args.size()-1; i++) {
            if (strcmp(args[i], "-a") == 0) {
            showall = true;
        } else if (strcmp(args[i], "-l") == 0) {
            showlong = true;
        }
        else if ((strcmp(args[i], "-al") == 0) || (strcmp(args[i], "-la") == 0)) {
            showlong = true;
            showall = true;
        } 
         else {
            paths.push_back(args[i]);
        }
        }
        lsCommand(paths, showall, showlong);
        return ;
    } 

    else if (strcmp(args[0], "search") == 0) {
        if(args.size()>3){
            cout<<"Invalid Arguments with search"<<"\n";
            return;
        }
        if (searchInCurrentDirectory(args[1])){
            cout<<"True"<<endl;
        }
        else cout<<"False"<<endl;
        return ;
    }
    else if (strcmp(args[0], "echo") == 0) {
    bool hasRedirect = false;
    for (int i = 1; args[i] != nullptr; i++) {
        if (strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0 || strcmp(args[i], "<") == 0) {
            hasRedirect = true;
            break;
        }
    }

    if (hasRedirect) {
        execute_command_with_redirection(args, home_dir);
    }
    
    return;
    }

    // else if (strcmp(args[0], "echo") == 0) {
    //     echo(input);
    // }
    // else{
    //     vector<string> stringArgs(args.begin(), args.end()-1);
    //     background_foreground_call(stringArgs);
    //     return;
    // }
    else {
    // Convert args (vector<char*>) into execvp-compatible format
    vector<char*> cargs;
    for (char* a : args) {
        if (a) cargs.push_back(a);
    }
    cargs.push_back(nullptr);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }
    else if (pid == 0) {
        // Child process → execute command
        signal(SIGINT, SIG_DFL);   // default Ctrl-C
        signal(SIGTSTP, SIG_DFL);  // default Ctrl-Z
        execvp(cargs[0], cargs.data());
        perror("execvp");  // only runs if execvp fails
        exit(1);
    }
    else {
        // Parent → wait for foreground process
        fg_pid = pid;                   // ← track foreground process
        int status;
        waitpid(pid, &status, WUNTRACED); // allow stopping with Ctrl-Z
        fg_pid = -1;                     // reset after process exits/stops

        if (WIFSTOPPED(status)) {
            cout << "\nProcess " << pid << " stopped\n";
        }
    }
}
    return;
}