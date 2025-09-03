#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <pwd.h>

extern std::string currentDir;
extern std::string home_dir;
extern struct passwd *password;
extern char name[256];
extern pid_t fg_pid;   // you already used this in procinfo

#endif
