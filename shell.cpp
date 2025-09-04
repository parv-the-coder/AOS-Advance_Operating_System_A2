
// #include"header.h"
// // using namespace std;

#include "header.h"

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


// // Handle Ctrl-C (SIGINT)
// void handle_sigint(int sig) {
//     if (fg_pid > 0) {
//         kill(fg_pid, SIGINT); // forward SIGINT to child
//     }
//     cout << "\n";   // move cursor to new line
//     fflush(stdout); // ensure it prints immediately
// }

// // Handle Ctrl-Z (SIGTSTP)
// void handle_sigtstp(int sig) {
//     if (fg_pid > 0) {
//         kill(fg_pid, SIGTSTP);   // stop foreground process
//         fflush(stdout);
//     }
// }







// int main()
// {
//     rl_attempted_completion_function = my_completion;

//     getcwd(cwd, sizeof(cwd));
//     home_dir = cwd;
//     getcwd(cur_dir,PATH_MAX);
//     getcwd(pre_dir,PATH_MAX);
//     gethostname(name,sizeof(name));
//     getcwd(cur_dir,sizeof(cur_dir));
//     currentDir = cur_dir;
//     previousDir = cur_dir;

//     signal(SIGINT, handle_sigint);    // forward Ctrl-C to foreground process
//     signal(SIGTSTP, handle_sigtstp);  // forward Ctrl-Z to foreground process


//     // Load history from file so that UP/DOWN keys work
//     // Load previous history from file
//     his_load();

//     string histFile = home_dir + "/history.txt";
//     using_history();
//     read_history(histFile.c_str()); // load previous history

//     while (true) {
//         // Build prompt string
//         string promptDir = currentDir;
//         if (promptDir.find(home_dir) == 0) {
//             promptDir.replace(0, home_dir.length(), "~");
//         }

//         const string RED    = "\033[1;31m";
//         const string GREEN  = "\033[1;32m";
//         const string BLUE   = "\033[1;34m";
//         const string YELLOW = "\033[1;33m";
//         const string RESET  = "\033[0m";

//         string prompt = GREEN + string(password->pw_name) + RESET + "@" +
//                         BLUE + string(name) + RESET + ":" +
//                         YELLOW + promptDir + RESET + "> ";

//         // Read input
//         char* inputCstr = readline(prompt.c_str());
//         if (!inputCstr) {  // Ctrl+D
//             cout << "\nExiting...\n";
//             break;
//         }
        

//         strncpy(input, inputCstr, sizeof(input));
//         input[sizeof(input)-1] = '\0';
// if (strlen(input) > 0) {
//             add_history(input);             // for arrow keys
//             append_history(1, histFile.c_str());    // save to file
//         }

//         free(inputCstr);

      

//         char* inputCopy = strdup(input);
//         char* command = strtok(inputCopy, ";");

//         while (command != nullptr) {
//             command = trim(command);
//             if (strlen(command) > 0) {
//                 string c = command;
//                 size_t found= c.find("|");
//                 if(found!=string::npos){
//                     vector<char*>argc=splitpipe(command);
//                     vector<vector<char*>>commands;
//                     for(size_t i=0; i<argc.size();i++){
//                         vector<char*>t=token(argc[i]," ");
//                         commands.push_back(t);
//                     }
//                     pipeline(commands,commands.size());
//                 }
//                 else{
//                 executeCommand(command);
//                 }
//             }

//             command = strtok(nullptr, ";");
//         }
//         free(inputCopy);
//     }
// }


// signal handler for ctrl-c
// sends SIGINT to foreground process
void sigint_handler(int sig)
{
    if(fg_pid > 0)
    {
        kill(fg_pid, SIGINT); // forward SIGINT to the foreground process
    }
    cout << "\n";   // move cursor to new line
    fflush(stdout); // ensure it prints immediately
}

// signal handler for ctrl-z
// sends SIGTSTP to foreground process
void sigtstp_handler(int sig)
{
    if(fg_pid > 0)
    {
        kill(fg_pid, SIGTSTP); // stop the foreground process
        fflush(stdout);        // flush stdout
    }
}

int main()
{
    // set readline autocomplete function
    rl_attempted_completion_function = my_completion;

    // initialize directories
    getcwd(cwd, sizeof(cwd));  // current working directory
    home_dir = cwd;             // save home directory
    getcwd(cur_dir, PATH_MAX);  // current directory
    getcwd(pre_dir, PATH_MAX);  // previous directory
    gethostname(name, sizeof(name)); // host name
    getcwd(cur_dir, sizeof(cur_dir));
    currentDir = cur_dir;       // current directory string
    previousDir = cur_dir;      // previous directory string

    // register signal handlers
    signal(SIGINT, sigint_handler);    // handle ctrl-c
    signal(SIGTSTP, sigtstp_handler);  // handle ctrl-z

    // load history from history file so arrow keys work
    his_load();
    string histfile = home_dir + "/history.txt";
    using_history();
    read_history(histfile.c_str());

    // main shell loop
    while(true)
    {
        // build the prompt string
        string pd = currentDir;  // copy current directory
        if(pd.find(home_dir) == 0)  // replace home path with '~'
        {
            pd.replace(0, home_dir.length(), "~");
        }

        // colors for prompt
        const string green  = "\033[1;32m"; // username
        const string blue   = "\033[1;34m"; // hostname
        const string yellow = "\033[1;33m"; // directory
        const string reset  = "\033[0m";    // reset color

        // final prompt string
        string prompt = green + string(password->pw_name) + reset + "@" +
                        blue + string(name) + reset + ":" +
                        yellow + pd + reset + "> ";

        // read user input
        char* line = readline(prompt.c_str());
        if(!line) // if Ctrl-D pressed
        {
            cout << "\nExiting...\n";
            break;
        }

        // copy input safely into global buffer
        strncpy(input, line, sizeof(input)-1);
        input[sizeof(input)-1] = '\0';

        // save to history if not empty
        if(strlen(input) > 0)
        {
            add_history(input);               // add to readline history
            append_history(1, histfile.c_str()); // save to file
        }
        free(line); // free readline buffer

        // split commands separated by ';'
        string s_input = input;
        vector<string> cmds; // store individual commands
        size_t pos = 0;

        // manual split to avoid strtok (different flow)
        while((pos = s_input.find(";")) != string::npos)
        {
            string t = s_input.substr(0, pos); // extract command
            cmds.push_back(t);                 // add to command list
            s_input.erase(0, pos + 1);         // remove processed part
        }
        if(!s_input.empty()) cmds.push_back(s_input); // add remaining

        // iterate over each command
        for(auto &cmdline : cmds)
        {
            // trim whitespace from command
            char* cptr = strdup(cmdline.c_str());
            cptr = trim(cptr);
            if(strlen(cptr) == 0) { free(cptr); continue; } // skip empty

            string temp = cptr;

            // check if command contains pipe '|'
            if(temp.find("|") != string::npos)
            {
                // split piped commands
                vector<char*> parts = splitpipe(cptr);
                vector<vector<char*>> piped;

                // tokenize each part by space
                for(size_t i = 0; i < parts.size(); i++)
                {
                    piped.push_back(token(parts[i], " "));
                }

                // execute piped commands
                pipeline(piped, piped.size());
            }
            else
            {
                // single command execution
                executeCommand(cptr);
            }

            free(cptr); // free temporary buffer
        }
    }

    return 0;
}
