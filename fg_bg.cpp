// #include "header.h"

// pid_t fg_pid = -1;   // definition (only once)

// using namespace std;
// extern void display();  // shell prompt

// int foreGroundProcess(vector<string>& args) {
//     vector<char*> argv;
//     for (auto &arg : args) argv.push_back(const_cast<char*>(arg.c_str()));
//     argv.push_back(nullptr);

//     pid_t pid = fork();
//     int status;

//     if (pid == 0) { // child
//         signal(SIGINT, SIG_DFL);
//         signal(SIGTSTP, SIG_DFL);
//         if (execvp(argv[0], argv.data()) == -1) {
//             perror("Error executing command");
//             exit(EXIT_FAILURE);
//         }
//     } else if (pid < 0) {
//         perror("Fork failed");
//         return -1;
//     } else { // parent
//     fg_pid = pid;
//     waitpid(pid, &status, WUNTRACED);

//     if (WIFSTOPPED(status)) {
//         cout << "\nProcess " << pid << " stopped and moved to background\n";
//         // Optionally: add pid to a background jobs list
//     }

//     fg_pid = -1;

//     }

//     // After child exits, print prompt
//     // display();
//     return 1;
// }

// int backGroundProcess(vector<string>& args) {
//     vector<char*> argv;
//     for (auto &arg : args) argv.push_back(const_cast<char*>(arg.c_str()));
//     argv.push_back(nullptr);

//     pid_t pid = fork();

//     if (pid == 0) { // CHILD
//         setsid();  // detach
//         if (execvp(argv[0], argv.data()) == -1) {
//             perror("Error executing background command");
//         }
//         exit(EXIT_FAILURE);
//     } 
//     else if (pid > 0) { // PARENT
//         cout << "Background process started with PID: " << pid << endl;
//         // Do NOT call display() here
//     } 
//     else {
//         perror("Fork failed");
//     }

//     return 1;
// }


// int background_foreground_call(vector<string>& args) {
//     if (!args.empty() && args.back() == "&") { 
//         args.pop_back();
//         return backGroundProcess(args);
//     } 
//     else {  
//         return foreGroundProcess(args);
//     }
// }


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
