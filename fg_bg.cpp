#include "header.h"

// for tracking cur foreground process id 
pid_t fg_pid = -1;  

// Ctrl-C if foreground process exists forward SIGINT kill it
void handle_sigint(int sig) 
{
    if (fg_pid > 0) 
    {
        kill(fg_pid, SIGINT);
    }   
    cout << "\n";      
    fflush(stdout);    
}

// Ctrl-Z if foreground process exists move background
void handle_sigtstp(int sig) 
{

    if (fg_pid > 0) 
    {
        kill(fg_pid, SIGTSTP);
    }
    
    fflush(stdout);            
}


// foreground shell waits to complete
int foreground(vector<char*>& args) 
{

    pid_t pid = fork();  // forking creating child
    int status;

     // child process
    if (pid == 0) 
    {
        signal(SIGINT, SIG_DFL);   
        signal(SIGTSTP, SIG_DFL);
        execvp(args[0], args.data()); // execute command
        perror("error executing command"); // if execvp fails
        exit(EXIT_FAILURE);        // exit child with error

    } 

     // fork failed
    else if (pid < 0)
    {
        perror("fork failed");
        return -1;
    } 

    // parent
    else 
    { 
        fg_pid = pid;                  // store foreground process id
        waitpid(pid, &status, WUNTRACED); // wait for child
        fg_pid = -1;                    // reset foreground PID


        // If the child was stopped by Ctrl-Z
        if (WIFSTOPPED(status)) 
        {
            cout << "\nprocess " << pid << " stopped and moved to background\n";
        }
    }

    return 1;
}

// background shell not wait for process over
int background(vector<char*>& args) 
{

    pid_t pid = fork();

    // child
    if (pid == 0) 
    {
        setsid();                  // start new session
        execvp(args[0], args.data()); // execute command
        perror("error executing background command"); // if execvp fails
        exit(EXIT_FAILURE);
    } 

    // parent
    else if (pid > 0) 
    {
        cout << "background process started with pid: " << pid << endl;
    } 

    // fork failed
    else 
    {
        perror("fork failed");
        return -1;
    }

    return 1;
}


// decides to com run in foreground or background if & run in background else foreground
int run_command(vector<char*>& args) 
{
    if (!args.empty() && strcmp(args.back(), "&") == 0) 
    {
        args.pop_back();  // remove the & from arg
        return background(args); // run in background
    }
    else 
    {
        return foreground(args); // run in foreground
    }
}
