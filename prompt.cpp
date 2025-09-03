#include <iostream>
#include<pwd.h>
using namespace std;
extern string currentDir;
extern string home_dir;
extern struct passwd *password;
extern char name[256];

void display() {
    string promptDir = currentDir;

    // Replace home directory with ~
    if (promptDir.find(home_dir) == 0) {
        promptDir.replace(0, home_dir.length(), "~");
    }

    // ANSI escape codes for colors
    const string RED    = "\033[1;31m";
    const string GREEN  = "\033[1;32m";
    const string BLUE   = "\033[1;34m";
    const string YELLOW = "\033[1;33m";
    const string RESET  = "\033[0m";

    // Format: username in green, hostname in blue, directory in yellow
    cout << GREEN << password->pw_name << RESET
         << "@" 
         << BLUE << name << RESET
         << ":" 
         << YELLOW << promptDir << RESET
         << "> ";
    cout.flush();
}
