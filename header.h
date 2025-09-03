#ifndef HEADER_H
#define HEADER_H

#include <bits/stdc++.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

using namespace std;

// ---------------- Globals ----------------
extern char input[4096];
extern char cur_dir[PATH_MAX];
extern string home_dir;
extern char cwd[PATH_MAX];
extern char pre_dir[PATH_MAX];
extern char name[256];
extern struct passwd *password;
extern string currentDir;
extern string previousDir;
extern pid_t fg_pid;


// ---------------- echo.cpp ----------------
void echo(char input[4096]);

// ---------------- ls.cpp ----------------
string filePermissions(mode_t mode);
void listDirectoryContents(const string &directoryPath, bool showHidden, bool longFormat);
void lsCommand(vector<string> paths, bool showHidden, bool longFormat);

// ---------------- history.cpp ----------------
void load_history_file();
void save_history(const string &cmd);
void print_history(int n = 10);

// ---------------- pipeline.cpp ----------------
vector<char*> execute_piped_commands(char *line);
void execute_command(vector<char*> args, int input_fd, int output_fd);
int execute_pipeline(vector<vector<char*>> commands);

// ---------------- procinfo.cpp ----------------
void printprocinfo(pid_t pid);

// ---------------- prompt.cpp ----------------
void display();

// ---------------- redir.cpp ----------------
void execute_command_with_redirection(vector<char*> args, string &home_dir);

// ---------------- search.cpp ----------------
bool recursiveSearch(const string& directory, const string& target);
bool searchInCurrentDirectory(const string& target);

// ---------------- shell.cpp helpers ----------------
void execom(char* cmd);
void execomredir(vector<char*> args, string& home_dir);
vector<char*> exepipcom(char* line);
void execom(vector<char*> args, int input_fd, int output_fd);
int exepip(vector<vector<char*>> commands);
char* trim(char* str);
vector<char*> token(char* str, const char* delimiter);

// ---------------- autocomplete.cpp ----------------
char* command_generator(const char* text, int state);
char** my_completion(const char* text, int start, int end);

void cd_func(std::vector<char*> args);
void pwd(int arg);

void printProcessInfo(pid_t pid);

void executeCommand(char* cmd) ;
// ---------------- completion ----------------
char** my_completion(const char* text, int start, int end);

#endif // HEADER_H
