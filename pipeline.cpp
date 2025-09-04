#include"header.h"


// execute a single command with input/output redirection
void runpipe(vector<char*> args, int inputfd, int outputfd) 
{
    pid_t pid = fork();
    
    // child
    if (pid == 0)
    { 
        if (inputfd != STDIN_FILENO) 
        {
            dup2(inputfd, STDIN_FILENO);
            close(inputfd);
        }

        if (outputfd != STDOUT_FILENO) 
        {
            dup2(outputfd, STDOUT_FILENO);
            close(outputfd);
        }

        if (execvp(args[0], args.data()) == -1) 
        {
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }

    }
    else if (pid < 0) 
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
}


// split input line by | into multi com
vector<char*> splitpipe(char *line) 
{
    vector<char*> cmds;
    char *cmd = strtok(line, "|");

    while (cmd != nullptr) 
    {
        // removing spaces leading
        while (*cmd && isspace((unsigned char)*cmd))
        {
            cmd++;
        } 

        // remove  spaces trailing
        char *end = cmd + strlen(cmd) - 1;
        while (end > cmd && isspace((unsigned char)*end))
        {
            end--;
        }

        *(end + 1) = '\0';

        if (*cmd != '\0') 
        {
            cmds.push_back(cmd);
        }

        cmd = strtok(nullptr, "|");
    }

    return cmds;
}


// execute multi piped comm
void pipeline(vector<vector<char*>> &piped, int n)
{
    int in_fd = 0;
    int fd[2];

    for (int i = 0; i < n; i++)
    {
        // create pipe]
        if (i < n - 1) 
        {
            if (pipe(fd) < 0)
            {
                perror("pipe failed");
                return;
            }
        }

        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork failed");
            return;
        }

        // child
        if (pid == 0) 
        {
            
            if (in_fd != 0)
            {
                dup2(in_fd, 0);
                close(in_fd);
            }

            // stdout next pipe if not last command
            if (i < n - 1)
            {
                close(fd[0]);
                dup2(fd[1], 1);
                close(fd[1]);
            }

            // handling redir
            bool redirectFlag = false;
            for (char* a : piped[i])
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

            if (redirectFlag)
            {
                redir(piped[i], home_dir);
                exit(0);
            }

            // execute normal command
            execvp(piped[i][0], piped[i].data());
            perror("execvp failed");
            exit(1);
        }

        // parent
        else
        {
            waitpid(pid, nullptr, 0);

            // close write end in parent
            if (i < n - 1)
            {
                close(fd[1]);
            }

            // next command reads from previous pipe
            if (in_fd != 0)
            {
                close(in_fd);
            }
            if (i < n - 1)
            {
                 in_fd = fd[0];
            }
              
        }
    }
}
