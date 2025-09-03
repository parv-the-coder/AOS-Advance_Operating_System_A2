#include<bits/stdc++.h>
#include<unistd.h>
#include<pwd.h>
#include <fcntl.h>
#include <sys/wait.h>
using namespace std;

vector<char*>execute_piped_commands(char *line){
  vector<char*>commands;
  char *command;

  command = strtok(line, "|");
  while (command) {
        while (isspace((unsigned char)*command)) command++;

        char* end = command + strlen(command) - 1;
        while (end > command && isspace((unsigned char)*end)) end--;
        *(end + 1) = '\0';
        if (*command != '\0') {
            commands.push_back(command);
        }

        command = strtok(NULL, "|");
    }
  return commands;
}

void execute_command(vector<char*>args, int input_fd, int output_fd) {
  pid_t pid;
  pid_t wpid;
  int status;

  if ((pid = fork()) == 0) {
    if (input_fd != STDIN_FILENO) {
      dup2(input_fd, STDIN_FILENO);
      close(input_fd);
    }

    if (output_fd != STDOUT_FILENO) {
      dup2(output_fd, STDOUT_FILENO);
      close(output_fd);
    }

    if (execvp(args[0], args.data()) == -1) {
      perror("execvp");
      exit(EXIT_FAILURE);
    }
  } else if (pid < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
}


int execute_pipeline(vector<vector<char*>> commands) {
    int num_commands = commands.size();
    if (num_commands == 0) return 1;

    int input_fd = STDIN_FILENO;
    vector<pid_t> child_pids;

    for (int i = 0; i < num_commands; ++i) {
        int pipefd[2] = {-1, -1};
        if (i < num_commands - 1) {
            if (pipe(pipefd) == -1) {
                perror("pipe failed");
                exit(EXIT_FAILURE);
            }
        }

        int output_fd = (i == num_commands - 1) ? STDOUT_FILENO : pipefd[1];

// Check for output redirection in the last command
if (i == num_commands - 1) {
    for (size_t j = 0; commands[i][j] != nullptr; ++j) {
        if (strcmp(commands[i][j], ">") == 0) {
            output_fd = open(commands[i][j+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            commands[i].erase(commands[i].begin() + j, commands[i].begin() + j + 2);
            break;
        } else if (strcmp(commands[i][j], ">>") == 0) {
            output_fd = open(commands[i][j+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            commands[i].erase(commands[i].begin() + j, commands[i].begin() + j + 2);
            break;
        }
    }
}

// Check for input redirection in the first command
if (i == 0) {
    for (size_t j = 0; commands[i][j] != nullptr; ++j) {
        if (strcmp(commands[i][j], "<") == 0) {
            input_fd = open(commands[i][j+1], O_RDONLY);
            commands[i].erase(commands[i].begin() + j, commands[i].begin() + j + 2);
            break;
        }
    }
}

        pid_t pid = fork();
        if (pid == 0) {
            // CHILD
            if (input_fd != STDIN_FILENO) {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            if (output_fd != STDOUT_FILENO) {
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            }
            if (execvp(commands[i][0], commands[i].data()) == -1) {
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
        } else if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else {
            // PARENT
            child_pids.push_back(pid);
            if (input_fd != STDIN_FILENO) close(input_fd);
            if (output_fd != STDOUT_FILENO) close(output_fd);
            input_fd = (pipefd[0] != -1) ? pipefd[0] : STDIN_FILENO;
        }
    }

    // Wait for all children
    for (auto pid : child_pids) {
        int status;
        waitpid(pid, &status, 0);
    }

    return 1;
}
