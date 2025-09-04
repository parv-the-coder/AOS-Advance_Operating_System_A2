#include "header.h"

// function to handle input/output redirection for a single command
// cmd     : vector of char* containing command and its arguments
// home    : string containing home directory (not used here but kept for consistency)
void redir(vector<char*> cmd, string &home)
{
    int in = 0;     // file descriptor for input (default: stdin)
    int out = 1;    // file descriptor for output (default: stdout)
    int i = 0;      // iterator index for parsing cmd vector

    // loop through cmd to parse all redirection operators
    while (i < cmd.size() && cmd[i])  // check cmd not empty and cmd[i] is not nullptr
    {
        // check for input redirection "<"
        if (strcmp(cmd[i], "<") == 0)
        {
            string f = cmd[i+1];        // get the filename following '<'
            in = open(f.c_str(), O_RDONLY);  // open file for reading
            if (in < 0)                 // error if file cannot be opened
            {
                perror("input");       // print system error
                return;                // exit the function
            }
            cmd.erase(cmd.begin() + i, cmd.begin() + i + 2); // remove '<' and filename from cmd
            continue;                   // continue parsing from current index
        }
        // check for output redirection ">" (overwrite)
        else if (strcmp(cmd[i], ">") == 0)
        {
            string f = cmd[i+1];        // get filename
            out = open(f.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644); // open for write, truncate if exists
            if (out < 0)                // error handling
            {
                perror("output");
                return;
            }
            cmd.erase(cmd.begin() + i, cmd.begin() + i + 2); // remove '>' and filename
            continue;                   // continue parsing
        }
        // check for output append redirection ">>"
        else if (strcmp(cmd[i], ">>") == 0)
        {
            string f = cmd[i+1];        // get filename
            out = open(f.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644); // open for write, append mode
            if (out < 0)
            {
                perror("output");
                return;
            }
            cmd.erase(cmd.begin() + i, cmd.begin() + i + 2); // remove '>>' and filename
            continue;                   // continue parsing
        }
        i++;                            // move to next argument
    }

    // ensure cmd ends with nullptr as required by execvp
    if (cmd.empty() || cmd.back() != nullptr)
    {
        cmd.push_back(nullptr);
    }

    // create a new process
    int pid = fork();
    if (pid < 0)                       // fork failed
    {
        perror("fork");
        return;
    }
    else if (pid == 0)                 // child process
    {
        // if input is redirected, replace stdin
        if (in != 0)
        {
            dup2(in, 0);               // duplicate file descriptor to stdin
            close(in);                 // close old fd
        }

        // if output is redirected, replace stdout
        if (out != 1)
        {
            dup2(out, 1);              // duplicate fd to stdout
            close(out);                // close old fd
        }

        execvp(cmd[0], cmd.data());    // execute the command
        perror("execvp");              // only reached if execvp fails
        exit(1);                        // terminate child process on error
    }
    else                                // parent process
    {
        waitpid(pid, nullptr, WUNTRACED); // wait for child to finish or stop
    }

    // close input/output fds if they were redirected
    if (in != 0)
    {
        close(in);
    }
    if (out != 1)
    {
        close(out);
    }
}
