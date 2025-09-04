#include "header.h"

// max number of history entries to keep
int MAX_HISTORY = 20;

// to save in history file and in readline in-meomry list
void his_save(const string &cmd) 
{

    string file = home_dir + "/history.txt"; // history file path
    vector<string> lines;                    // store existing lines

    // open the history file
    int fd = open(file.c_str(), O_RDWR | O_CREAT, 0644);
    if (fd < 0)
    {
        perror("Failed to open history file");
        return;
    }

    char buffer[4096];
    ssize_t bytesRead;
    string currentLine;

    // read file into lines vector
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0)
    {
        
        for (ssize_t i = 0; i < bytesRead; ++i)
        {
            
            if (buffer[i] == '\n')
            {
                lines.push_back(currentLine);
                currentLine.clear();
            }
            else
            {
                currentLine += buffer[i];
            }

        }
    }

    if (!currentLine.empty())
    {
        lines.push_back(currentLine); // push last line if no \n
    }

    // avoiding duplicates: only add if last com is diff
    if (lines.empty() || lines.back() != cmd)
    {
        lines.push_back(cmd);
    }

    // keep last MAX_HISTORY commands
    if (lines.size() > size_t(MAX_HISTORY))
    {
        lines.erase(lines.begin());
    }

    // rewrite file from scratch
    lseek(fd, 0, SEEK_SET);   // move file pointer to start
    ftruncate(fd, 0);         // clear the file

    for (auto &line : lines)
    {
        string toWrite = line + "\n";
        write(fd, toWrite.c_str(), toWrite.size());
    }

    close(fd);
}

// load history
void his_load() 
{
    string file = home_dir + "/history.txt";

    FILE* f = fopen(file.c_str(), "r"); // open in read mode
    if (!f) return;                     // file doesn't exist

    char* line = nullptr;
    size_t len = 0;
    ssize_t read;

    // read line by line
    while ((read = getline(&line, &len, f)) != -1)
    {

        if (read > 0 && line[read-1] == '\n') // remove trailing newline
        {
            line[read-1] = '\0';
        }
        add_history(line); // add readline history
    }

    if (line) 
    {
        free(line);
    }
    
    fclose(f);
}

// print n last commands
void his_print(int n = 10) 
{
    HIST_ENTRY **hist_list = history_list(); // get all in-memory history
   
    if (!hist_list)
    {
        return;                
    } 

    int total = history_length;              // total commands
    int start = max(0, total - n);           // start from last N commands

    for (int i = start; i < total; i++)
    {
        cout << hist_list[i]->line << "\n";
    }
    
}
