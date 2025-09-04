#include "header.h"

// redir
void redir(vector<char*> cmd, string &home)
{
    int in = 0;     
    int out = 1;    
    int i = 0;      // index for parsing vector

    // loop through cmd to parse all redir
    while (size_t (i) < cmd.size() && cmd[i])
    {
        // check for input redir <
        if (strcmp(cmd[i], "<") == 0)
        {
            string f = cmd[i+1];        // get the filnam 
            in = open(f.c_str(), O_RDONLY);  // open file for reading
            
            // checking error for opeing
            if (in < 0)                 
            {
                perror("input");  
                return;           
            }

            cmd.erase(cmd.begin() + i, cmd.begin() + i + 2); // remove <  filenam
            continue;
        }

        // check for output redir >
        else if (strcmp(cmd[i], ">") == 0)
        {
            string f = cmd[i+1];        // get filename
            out = open(f.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644); // open for write truncate if exists
            
            // error handling
            if (out < 0)                
            {
                perror("output");
                return;
            }
            cmd.erase(cmd.begin() + i, cmd.begin() + i + 2); // remove > filenam
            continue;                   
        }

        // check for output append redir >>
        else if (strcmp(cmd[i], ">>") == 0)
        {
            string f = cmd[i+1];        // get filename
            out = open(f.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644); // open for write append mode
            
            if (out < 0)
            {
                perror("output");
                return;
            }

            cmd.erase(cmd.begin() + i, cmd.begin() + i + 2); // remove >> and filenam
            continue;             
        }

        i++;                      
    }

    if (cmd.empty() || cmd.back() != nullptr)
    {
        cmd.push_back(nullptr);
    }

    // create a new process
    int pid = fork();
    
    // fork failed
    if (pid < 0)                       
    {
        perror("fork");
        return;
    }
    
    // child process
    else if (pid == 0)                 
    {
    
        if (in != 0)
        {
            dup2(in, 0);               // duplicate fd
            close(in);              
        }

        if (out != 1)
        {
            dup2(out, 1);              // duplicate fd 
            close(out);               
        }

        execvp(cmd[0], cmd.data());    // execute the command
        perror("execvp");              // only reached if execvp fails
        exit(1);                        // terminate child process on error
    }
    
    // parent process
    else                                
    {
        waitpid(pid, nullptr, WUNTRACED); // wait for child to complete or stop
    }

    if (in != 0)
    {
        close(in);
    }

    if (out != 1)
    {
        close(out);
    }

}
