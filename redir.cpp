#include<bits/stdc++.h>
#include<unistd.h>
#include<pwd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include"header.h"
using namespace std;

void execute_command_with_redirection(vector<char*> args, string &home_dir) {
    int input_fd = STDIN_FILENO;
    int output_fd = STDOUT_FILENO;

    // Parse for redirections
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == nullptr) break;

        if (strcmp(args[i], "<") == 0) {
            input_fd = open(args[i+1], O_RDONLY);
            if (input_fd < 0) { perror("Input file error"); return; }
            args.erase(args.begin() + i, args.begin() + i + 2);
            i--;
        }
        else if (strcmp(args[i], ">") == 0) {
            output_fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd < 0) { perror("Output file error"); return; }
            args.erase(args.begin() + i, args.begin() + i + 2);
            i--;
        }
        else if (strcmp(args[i], ">>") == 0) {
            output_fd = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (output_fd < 0) { perror("Output file error"); return; }
            args.erase(args.begin() + i, args.begin() + i + 2);
            i--;
        }
    }

    // Make sure args ends with nullptr
    if (args.empty() || args.back() != nullptr)
        args.push_back(nullptr);

    pid_t pid = fork();
    if (pid == 0) { // CHILD
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO) {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        execvp(args[0], args.data());
        perror("execvp");
        exit(EXIT_FAILURE);
    } 
    else if (pid > 0) { // PARENT
        fg_pid = pid;  // set current foreground process
        waitpid(pid, nullptr, WUNTRACED); 
        fg_pid = -1;   // reset after child is done or stopped
} 
 
    else {
        perror("fork");
    }

    if (input_fd != STDIN_FILENO) close(input_fd);
    if (output_fd != STDOUT_FILENO) close(output_fd);
}
