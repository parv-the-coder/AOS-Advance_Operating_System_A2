#include "header.h"

// fg/bg pid tracking
pid_t fg_pid = -1;  

// handle ctrl-c
void handle_sigint(int sig) {
    if (fg_pid > 0) kill(fg_pid, SIGINT); // forward to child
    cout << "\n";
    fflush(stdout);
}

// handle ctrl-z
void handle_sigtstp(int sig) {
    if (fg_pid > 0) kill(fg_pid, SIGTSTP); // stop child
    fflush(stdout);
}

// run command in foreground
int foreground(vector<char*>& args) {
    pid_t pid = fork();
    int status;

    if (pid == 0) { // child
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        execvp(args[0], args.data());
        perror("error executing command");
        exit(EXIT_FAILURE);
    } 
    else if (pid < 0) {
        perror("fork failed");
        return -1;
    } 
    else { // parent
        fg_pid = pid;
        waitpid(pid, &status, WUNTRACED);
        fg_pid = -1;

        if (WIFSTOPPED(status)) {
            cout << "\nprocess " << pid << " stopped and moved to background\n";
        }
    }

    return 1;
}


// run command in background
int background(vector<char*>& args) {
    pid_t pid = fork();

    if (pid == 0) {
        setsid();
        execvp(args[0], args.data());
        perror("error executing background command");
        exit(EXIT_FAILURE);
    } 
    else if (pid > 0) {
        cout << "background process started with pid: " << pid << endl;
    } 
    else {
        perror("fork failed");
        return -1;
    }

    return 1;
}


// wrapper to detect & and call fg/bg accordingly
int run_command(vector<char*>& args) {
    if (!args.empty() && strcmp(args.back(), "&") == 0) {
        args.pop_back();  // remove &
        return background(args);
    } else {
        return foreground(args);
    }
}
