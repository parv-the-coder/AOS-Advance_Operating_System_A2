#include"header.h"


// execute a single command with input/output redirection
void runpipe(vector<char*> args, int inputfd, int outputfd) {
    pid_t pid = fork();
    if (pid == 0) { // child process
        if (inputfd != STDIN_FILENO) {
            dup2(inputfd, STDIN_FILENO);
            close(inputfd);
        }
        if (outputfd != STDOUT_FILENO) {
            dup2(outputfd, STDOUT_FILENO);
            close(outputfd);
        }

        if (execvp(args[0], args.data()) == -1) {
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
}


// split input line by '|' into multiple commands
vector<char*> splitpipe(char *line) {
    vector<char*> cmds;
    char *cmd = strtok(line, "|");

    while (cmd != nullptr) {
        // remove leading spaces
        while (*cmd && isspace((unsigned char)*cmd)) cmd++;

        // remove trailing spaces
        char *end = cmd + strlen(cmd) - 1;
        while (end > cmd && isspace((unsigned char)*end)) end--;
        *(end + 1) = '\0';

        if (*cmd != '\0') {
            cmds.push_back(cmd);
        }

        cmd = strtok(nullptr, "|");
    }

    return cmds;
}


// execute multiple piped commands
int pipeline(vector<vector<char*>> cmds, int n) {
    int num = cmds.size();
    if (num == 0) return 1;

    int inputfd = STDIN_FILENO;
    vector<pid_t> children;

    for (int i = 0; i < num; i++) {
        int pipefd[2];
        int outputfd;

        // create pipe if not last command
        if (i < num - 1) {
            if (pipe(pipefd) == -1) {
                perror("pipe failed");
                exit(EXIT_FAILURE);
            }
            outputfd = pipefd[1];
        } else {
            outputfd = STDOUT_FILENO;
        }

        // handle output redirection in last command
        if (i == num - 1) {
            for (size_t j = 0; cmds[i][j] != nullptr; j++) {
                if (strcmp(cmds[i][j], ">") == 0) {
                    outputfd = open(cmds[i][j+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    cmds[i].erase(cmds[i].begin() + j, cmds[i].begin() + j + 2);
                    break;
                }
                if (strcmp(cmds[i][j], ">>") == 0) {
                    outputfd = open(cmds[i][j+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                    cmds[i].erase(cmds[i].begin() + j, cmds[i].begin() + j + 2);
                    break;
                }
            }
        }

        // handle input redirection in first command
        if (i == 0) {
            for (size_t j = 0; cmds[i][j] != nullptr; j++) {
                if (strcmp(cmds[i][j], "<") == 0) {
                    inputfd = open(cmds[i][j+1], O_RDONLY);
                    cmds[i].erase(cmds[i].begin() + j, cmds[i].begin() + j + 2);
                    break;
                }
            }
        }

        // fork and execute
        pid_t pid = fork();
        if (pid == 0) { // child
            if (inputfd != STDIN_FILENO) {
                dup2(inputfd, STDIN_FILENO);
                close(inputfd);
            }
            if (outputfd != STDOUT_FILENO) {
                dup2(outputfd, STDOUT_FILENO);
                close(outputfd);
            }

            if (execvp(cmds[i][0], cmds[i].data()) == -1) {
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
        } else if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else { // parent
            children.push_back(pid);

            if (inputfd != STDIN_FILENO) close(inputfd);
            if (outputfd != STDOUT_FILENO) close(outputfd);

            if (i < num - 1) {
                inputfd = pipefd[0];
            }
        }
    }

    // wait for all children to finish
    for (size_t i = 0; i < children.size(); i++) {
        int status;
        waitpid(children[i], &status, 0);
    }

    return 1;
}
