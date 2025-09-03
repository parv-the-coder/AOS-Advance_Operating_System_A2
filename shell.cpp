#include<bits/stdc++.h>
#include<unistd.h>
#include<pwd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include"header.h"
using namespace std;

// Function declarations
void execom(char* cmd);
void execomredir(vector<char*> args, string& home_dir);
vector<char*> exepipcom(char* line);
void execom(vector<char*> args, int input_fd, int output_fd);
int exepip(vector<vector<char*>> commands);
char* trim(char* str);
vector<char*> token(char* str, const char* delimiter);
char* command_generator(const char* text, int state);


// Global variables
char input[4096];
char cur_dir[PATH_MAX];
string  home_dir;
char cwd[PATH_MAX];
char pre_dir[PATH_MAX];
char name[256];
struct passwd *password = getpwuid(getuid());

string currentDir;
string previousDir;


// Handle Ctrl-C (SIGINT)
void handle_sigint(int sig) {
    if (fg_pid > 0) {
        kill(fg_pid, SIGINT); // forward SIGINT to child
    }
    cout << "\n";   // move cursor to new line
    fflush(stdout); // ensure it prints immediately
}

// Handle Ctrl-Z (SIGTSTP)
void handle_sigtstp(int sig) {
    if (fg_pid > 0) {
        kill(fg_pid, SIGTSTP);   // stop foreground process
        fflush(stdout);
    }
}







int main()
{
    rl_attempted_completion_function = my_completion;

    getcwd(cwd, sizeof(cwd));
    home_dir = cwd;
    getcwd(cur_dir,PATH_MAX);
    getcwd(pre_dir,PATH_MAX);
    gethostname(name,sizeof(name));
    getcwd(cur_dir,sizeof(cur_dir));
    currentDir = cur_dir;
    previousDir = cur_dir;

    signal(SIGINT, handle_sigint);    // forward Ctrl-C to foreground process
    signal(SIGTSTP, handle_sigtstp);  // forward Ctrl-Z to foreground process


    // Load history from file so that UP/DOWN keys work
    // Load previous history from file
    load_history_file();

    string histFile = home_dir + "/history.txt";
    using_history();
    read_history(histFile.c_str()); // load previous history

    while (true) {
        // Build prompt string
        string promptDir = currentDir;
        if (promptDir.find(home_dir) == 0) {
            promptDir.replace(0, home_dir.length(), "~");
        }

        const string RED    = "\033[1;31m";
        const string GREEN  = "\033[1;32m";
        const string BLUE   = "\033[1;34m";
        const string YELLOW = "\033[1;33m";
        const string RESET  = "\033[0m";

        string prompt = GREEN + string(password->pw_name) + RESET + "@" +
                        BLUE + string(name) + RESET + ":" +
                        YELLOW + promptDir + RESET + "> ";

        // Read input
        char* inputCstr = readline(prompt.c_str());
        if (!inputCstr) {  // Ctrl+D
            cout << "\nExiting...\n";
            break;
        }
        

        strncpy(input, inputCstr, sizeof(input));
        input[sizeof(input)-1] = '\0';
if (strlen(input) > 0) {
            add_history(input);             // for arrow keys
            append_history(1, histFile.c_str());    // save to file
        }

        free(inputCstr);

      

        char* inputCopy = strdup(input);
        char* command = strtok(inputCopy, ";");

        while (command != nullptr) {
            command = trim(command);
            if (strlen(command) > 0) {
                string c = command;
                size_t found= c.find("|");
                if(found!=string::npos){
                    vector<char*>argc=execute_piped_commands(command);
                    vector<vector<char*>>commands;
                    for(size_t i=0; i<argc.size();i++){
                        vector<char*>t=token(argc[i]," ");
                        commands.push_back(t);
                    }
                    execute_pipeline(commands);
                }
                else{
                executeCommand(command);
                }
            }

            command = strtok(nullptr, ";");
        }
        free(inputCopy);
    }
}

