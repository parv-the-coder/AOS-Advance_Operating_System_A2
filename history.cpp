#include <bits/stdc++.h>
#include <fcntl.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

extern string home_dir;
int MAX_HISTORY = 20;


// Save a command to history (file + readline)
void his_save(const string &cmd) 
{
    string file = home_dir + "/history.txt";
    vector<string> lines;

    int fd = open(file.c_str(), O_RDWR | O_CREAT, 0644);
    if (fd < 0)
    {
        perror("Failed to open history file");
        return;
    }

    char buffer[4096];
    ssize_t bytesRead;
    string currentLine;

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
        lines.push_back(currentLine);
    }

    if (lines.empty() || lines.back() != cmd)
    {
        lines.push_back(cmd);
    }

    if (lines.size() > MAX_HISTORY)
    {
        lines.erase(lines.begin());
    }

    lseek(fd, 0, SEEK_SET);
    ftruncate(fd, 0);

    for (auto &line : lines)
    {
        string toWrite = line + "\n";
        write(fd, toWrite.c_str(), toWrite.size());
    }

    close(fd);
}



// Load history from file into readline at shell start
void his_load() 
{
    string file = home_dir + "/history.txt";

    FILE* f = fopen(file.c_str(), "r");
    if (!f)
    {
        return;
    }

    char* line = nullptr;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, f)) != -1)
    {
        if (read > 0 && line[read-1] == '\n')
        {
            line[read-1] = '\0';
        }

        add_history(line);
    }

    if (line)
    {
        free(line);
    }

    fclose(f);
}



// Print last N history commands (like your existing print_history)
void his_print(int n = 10) 
{
    HIST_ENTRY **hist_list = history_list();
    if (!hist_list)
    {
        return;
    }

    int total = history_length;
    int start = max(0, total - n);

    for (int i = start; i < total; i++)
    {
        cout << hist_list[i]->line << "\n";
    }
}
