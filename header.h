#ifndef HEADER_H
#define HEADER_H

#include <bits/stdc++.h>   // all standard C++ headers
#include <unistd.h>         // system calls
#include <pwd.h>            // user info
#include <sys/types.h>      // system types
#include <dirent.h>         // directory traversal
#include <sys/stat.h>       // file info
#include <grp.h>            // group info
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

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

void executeCommand(char* cmd);


// ---------------- echo.cpp ----------------
void echo(char input[4096]);

// ---------------- ls.cpp ----------------
string perm(mode_t m);
void lsdir(const string &d, bool h, bool l);
void ls(vector<string> dirs, bool h, bool l);

// ---------------- history.cpp ----------------
void his_load();
void his_save(const string &cmd);
void his_print(int n = 10);

// ---------------- pipeline.cpp ----------------
vector<char*> splitpipe(char *line);
void runpipe(vector<char*> args, int input_fd, int output_fd);
int pipeline(vector<vector<char*>> commands,int n);

// ---------------- procinfo.cpp ----------------
void pinfo(pid_t pid);

// ---------------- prompt.cpp ----------------
void display();

// ---------------- redir.cpp ----------------
void redir(vector<char*> args, string &home_dir);

// ---------------- search.cpp ----------------
bool dircheck(const string& directory, const string& target);
bool search(const string& target);

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

void cd(std::vector<char*> args);
void pwd(int arg);

void printProcessInfo(pid_t pid);

void executeCommand(char* cmd) ;
// ---------------- completion ----------------
char** my_completion(const char* text, int start, int end);


// ---------------- foreground/background functions ----------------
int foreground(vector<char*>& args);
int background(vector<char*>& args);


#endif // HEADER_H
