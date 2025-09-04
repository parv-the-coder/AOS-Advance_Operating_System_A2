#include "header.h"

// list of commands of my shell 
vector<string> lis_com = {"cd", "pwd", "ls", "history", "echo", "pinfo", "search"};


// given input prefix 'text' there are no more matches, it returns nullptr.
char *com_gen(const char *text, int state)
{

    // vector for matches
    vector<string> matches;

    // convert for easier operations
    string prefix(text);
    
    // going through all our com
    for (const string &cmd : lis_com)
    {
        // check command starts with the user prefix
        if (cmd.find(prefix) == 0)
        { 
            matches.push_back(cmd);         // starts with prefix
        }
    }


    // current dir for reading
    DIR *dir = opendir(".");
    if (dir)
    {
        struct dirent *entry;

        // going through all files dir
        while ((entry = readdir(dir)) != nullptr)
        {

            string fname = entry->d_name;   // name of file/dir

            // add to matches if file start with pre
            if (fname.find(prefix) == 0)
            {
                matches.push_back(fname);
            }
        }
        closedir(dir);
    }

    //  nullptr if none left or return match at pos state
    if (size_t(state )< matches.size())
    {
        return strdup(matches[state].c_str());
    }

    // state is more than match size all are done
    return nullptr;
}


// tab key is pressed this is called
char **tab_com(const char *text, int start, int end)
{

    // if start = 0, completing comm
    if (start == 0)
    {
        rl_attempted_completion_over = 1; // disable default filnam
        return rl_completion_matches(text, com_gen);   // repeative calls to com_gen until nullptr
    }
    
    rl_attempted_completion_over = 0; // default completion
    return nullptr;
}
