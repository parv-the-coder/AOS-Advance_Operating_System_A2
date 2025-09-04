#ifndef HEADER_H
#define HEADER_H

#include <bits/stdc++.h> 
#include <unistd.h>         
#include <pwd.h>            
#include <sys/types.h>      
#include <dirent.h>         
#include <sys/stat.h>       
#include <grp.h>            
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

extern char input[4096];
extern char cur_dir[PATH_MAX];
extern string home_dir;
extern char cwd[PATH_MAX];
extern char pre_dir[PATH_MAX];
extern char name[256];
extern struct passwd *password;
extern string currdir;
extern string prev_dir;
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
void his_print(int n);

// ---------------- pipeline.cpp ----------------
vector<char*> splitpipe(char *line);
void runpipe(vector<char*> args, int input_fd, int output_fd);
void pipeline(vector<vector<char*>> &piped, int n);

// ---------------- procinfo.cpp ----------------
void pinfo(pid_t pid);

// ---------------- redir.cpp ----------------
void redir(vector<char*> args, string &home_dir);

// ---------------- search.cpp ----------------
bool search(const string& target);


char* trim(char* str);
vector<char*> token(char* str, const char* delimiter);

// ---------------- autocomplete.cpp ----------------
char* com_gen(const char* text, int state);
char** tab_com(const char* text, int start, int end);

void cd(std::vector<char*> args);
void pwd(int arg);


void executeCommand(char* cmd) ;


// ---------------- foreground/background functions ----------------
int foreground(vector<char*>& args);
int background(vector<char*>& args);


#endif // HEADER_H
