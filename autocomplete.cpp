#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

vector<string> builtin_commands = {"cd","pwd","ls","history","echo","pinfo","search","exit"};

char* command_generator(const char* text, int state) {
    static size_t list_index;
    static vector<string> matches;

    if (state == 0) {
        matches.clear();
        list_index = 0;
        string prefix(text);

        // Built-in commands
        for (const string &cmd : builtin_commands) {
            if (cmd.find(prefix) == 0) matches.push_back(cmd);
        }

        // Files/directories in current directory
        DIR* dir = opendir(".");
        if (dir) {
            struct dirent* entry;
            while ((entry = readdir(dir)) != nullptr) {
                string fname = entry->d_name;
                if (fname.find(prefix) == 0) matches.push_back(fname);
            }
            closedir(dir);
        }
    }

    if (list_index < matches.size())
        return strdup(matches[list_index++].c_str());
    else
        return nullptr;
}

// char** my_completion(const char* text, int start, int end) {
//     rl_attempted_completion_over = 1; // disable default filename completion
//     return rl_completion_matches(text, command_generator);
// }

char** my_completion(const char* text, int start, int end) {
    // If completing the first word -> run our custom generator
    if (start == 0) {
        rl_attempted_completion_over = 1; 
        return rl_completion_matches(text, command_generator);
    }

    // For arguments (not the first word), let readline default handle filenames
    rl_attempted_completion_over = 0; 
    return nullptr;
}
