#include "header.h"

// Global variables
char input[4096];
char cur_dir[PATH_MAX];
string  home_dir;
char cwd[PATH_MAX];
char pre_dir[PATH_MAX];
char name[256];
struct passwd *password = getpwuid(getuid());

string currdir;
string prev_dir;

// ctrl C
void sigint_handler(int sig)
{
    if(fg_pid > 0)
    {
        kill(fg_pid, SIGINT); // forward SIGINT to the foreground process
    }

    cout << "\n";
    fflush(stdout);
}

// ctrl Z
void sigtstp_handler(int sig)
{
    if(fg_pid > 0)
    {
        kill(fg_pid, SIGTSTP); // stop the foreground process
        fflush(stdout);        
    }
}


int main()
{

    try 
    {
    // setting readline for autocomplete function
    rl_attempted_completion_function = tab_com;

    // init dir
    getcwd(cwd, sizeof(cwd));  // curr dir
    home_dir = cwd;             // save home dir
    getcwd(cur_dir, PATH_MAX);  // curr dir
    getcwd(pre_dir, PATH_MAX);  // prev dir
    gethostname(name, sizeof(name)); // host name
    getcwd(cur_dir, sizeof(cur_dir));
    currdir = cur_dir;       
    prev_dir = cur_dir;      

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
        // build the prompt
        string pd = currdir;  // copy current directory
        if(pd.find(home_dir) == 0)  // replace home path with '~'
        {
            pd.replace(0, home_dir.length(), "~");
        }

        // colors for prompt
        const string green  = "\033[1;32m"; // username
        const string blue   = "\033[1;34m"; // hostname
        const string yellow = "\033[1;33m"; // directory
        const string reset  = "\033[0m";    // reset color

        // final prompt
        string prompt = green + string(password->pw_name) + reset + "@" +
                        blue + string(name) + reset + ":" +
                        yellow + pd + reset + "> ";

        // read user input
        char* line = readline(prompt.c_str());
        
        // if Ctrl-D pressed
        if(!line) 
        {
            cout << "\nExiting...\n";
            break;
        }

        // copy input
        strncpy(input, line, sizeof(input)-1);
        input[sizeof(input)-1] = '\0';

        // save to history if not empty
        if(strlen(input) > 0)
        {
            add_history(input);               // add to readline history
            append_history(1, histfile.c_str()); // save to file
        }
        free(line);

        // split comd by ;
        string s_input = input;
        vector<string> cmds; // store individual comd
        size_t pos = 0;

        // split
        while((pos = s_input.find(";")) != string::npos)
        {
            string t = s_input.substr(0, pos); // extract comd
            cmds.push_back(t);                 // add comd list
            s_input.erase(0, pos + 1);        
        }

        if(!s_input.empty()) 
        {
            cmds.push_back(s_input); // add others
        }
        
        // go through each comd
        for(auto &cmdline : cmds)
        {
            char* orig = strdup(cmdline.c_str());
            char* cptr = trim(orig);               
            
            if(strlen(cptr) == 0)
            {
                free(cptr);
                continue; 
            }

            string temp = cptr;

            // check if pipe | present
            if(temp.find("|") != string::npos)
            {
                // split piped comd
                vector<char*> parts = splitpipe(cptr);
                vector<vector<char*>> piped;

                // tokenize by space
                for(size_t i = 0; i < parts.size(); i++)
                {
                    piped.push_back(token(parts[i], " "));
                }

                // execute piped comd
                pipeline(piped, piped.size());
            }
            else
            {
                executeCommand(cptr);
            }

            free(orig);
        }
    }
    }
    catch(const std::exception& e)
    {
        cerr << "Unknown error occurred: " << e.what() << "\n";
    }
    catch(...)
    {
        cerr << "Unknown error occurred\n";
    }
    return 0;
}
