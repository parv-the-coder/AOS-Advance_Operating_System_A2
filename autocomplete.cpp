#include <dirent.h>            // For directory operations (opendir, readdir, closedir)
#include <readline/readline.h> // For interactive command-line input
#include <readline/history.h>  // For command history support
#include "header.h"

// ==================== BUILT-IN COMMANDS ====================
// We define a list of commands our shell knows intrinsically.
// This allows autocompletion to suggest shell-specific commands.
vector<string> builtin_commands = {"cd", "pwd", "ls", "history", "echo", "pinfo", "search"};

// ==================== COMMAND GENERATOR ====================
// This function is called repeatedly by Readline for autocompletion.
// Each time it is called, it must return the 'state'-th match for the
// given input prefix 'text'. If there are no more matches, it returns nullptr.
// The naive approach: we regenerate the match list on every call.
char *command_generator(const char *text, int state)
{

    // 1. Create a local vector for matches (every call recomputes it)
    vector<string> matches;

    // 2. Convert input C-string to C++ string for easier operations
    string prefix(text);

    // ---------------- BUILT-IN MATCHING ----------------
    // We iterate through all built-in commands
    for (const string &cmd : builtin_commands)
    {
        // Check if the command starts with the user-typed prefix
        if (cmd.find(prefix) == 0)
        { // starts with prefix
            matches.push_back(cmd);
        }
    }

    // ---------------- DIRECTORY MATCHING ----------------
    // Open the current directory (".") for reading
    DIR *dir = opendir(".");
    if (dir)
    {
        struct dirent *entry;

        // Iterate over all files and directories
        while ((entry = readdir(dir)) != nullptr)
        {
            string fname = entry->d_name; // name of file/directory
            // If the filename starts with the prefix, add to matches
            if (fname.find(prefix) == 0)
            {
                matches.push_back(fname);
            }
        }
        closedir(dir); // Always close directory after use
    }

    // ---------------- RETURN THE MATCH BASED ON STATE ----------------
    // 'state' is an index starting from 0.
    // For example, first call: state=0 → return first match
    // Second call: state=1 → return second match, etc.
    if (state < matches.size())
    {
        return strdup(matches[state].c_str()); // Return a copy as C-string
    }

    // If state >= matches.size(), we have exhausted matches
    return nullptr;
}

// ==================== READLINE COMPLETION FUNCTION ====================
// This function hooks into Readline's autocompletion system.
// It is called whenever the user presses the TAB key.
char **my_completion(const char *text, int start, int end)
{

    // If the cursor is at the start of the input, we're completing a command
    if (start == 0)
    {
        rl_attempted_completion_over = 1; // disable default filename completion
        return rl_completion_matches(text, command_generator);
        // Readline calls command_generator repeatedly until nullptr is returned
    }

    // Otherwise, let Readline handle filename completion for arguments
    rl_attempted_completion_over = 0; // default completion
    return nullptr;
}
