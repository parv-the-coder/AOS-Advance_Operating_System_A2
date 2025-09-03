// #include <bits/stdc++.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <sys/wait.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <dirent.h>

// using namespace std;

// void printProcessInfo(pid_t pid) 
// {
//     if (pid == 0) {
//         pid = getpid();  // default: current process
//     }

//     char statPath[256], exePath[256];
//     sprintf(statPath, "/proc/%d/stat", pid);
//     sprintf(exePath, "/proc/%d/exe", pid);

//     // Reading process status from /proc/[pid]/stat
//     ifstream statFile(statPath);
//     if (!statFile) {
//         perror("pinfo");
//         return;
//     }

//     string line, comm, state;
//     long rss = 0;
//     int field = 1;

//     getline(statFile, line);
//     statFile.close();

//     istringstream iss(line);
//     string token;
//     while (iss >> token) {
//         if (field == 2) comm = token;       // command name
//         else if (field == 3) state = token; // process state
//         else if (field == 24) rss = stol(token); // RSS
//         field++;
//     }

//     // Strip parentheses around comm
//     if (comm.size() > 2 && comm.front() == '(' && comm.back() == ')') {
//         comm = comm.substr(1, comm.size() - 2);
//     }

//     // Get the executable path
//     char exeRealPath[PATH_MAX];
//     ssize_t len = readlink(exePath, exeRealPath, sizeof(exeRealPath) - 1);
//     if (len != -1) exeRealPath[len] = '\0';
//     else strcpy(exeRealPath, "N/A");

//     // Convert memory from pages to KB
//     long pageSize = sysconf(_SC_PAGESIZE) / 1024;  
//     long memory = rss * pageSize;  

//     // Output
//     cout << "pid -- " << pid << endl;
//     cout << "Process Name -- " << comm << endl;
//     cout << "Process Status -- " << state << endl;
//     cout << "memory -- " << memory << " KB" << endl;
//     cout << "Executable Path -- " << exeRealPath << endl;
// }

#include <bits/stdc++.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "global.h"

using namespace std;

void printProcessInfo(pid_t pid) 
{
    if (pid == 0) {
        pid = getpid();  // default: current process
    }

    char statPath[256], exePath[256];
    sprintf(statPath, "/proc/%d/stat", pid);
    sprintf(exePath, "/proc/%d/exe", pid);

    // Reading process status from /proc/[pid]/stat
    ifstream statFile(statPath);
    if (!statFile) {
        perror("pinfo");
        return;
    }

    string line, comm, state;
    long rss = 0;
    int field = 1;

    getline(statFile, line);
    statFile.close();

    istringstream iss(line);
    string token;
    while (iss >> token) {
        if (field == 2) comm = token;       // command name
        else if (field == 3) state = token; // process state
        else if (field == 24) {
            try { rss = stol(token); } catch(...) { rss = 0; }
        }
        field++;
    }

    // Strip parentheses around comm
    if (comm.size() > 2 && comm.front() == '(' && comm.back() == ')') {
        comm = comm.substr(1, comm.size() - 2);
    }

    // Get the executable path
    char exeRealPath[PATH_MAX];
    ssize_t len = readlink(exePath, exeRealPath, sizeof(exeRealPath) - 1);
    if (len != -1) exeRealPath[len] = '\0';
    else strcpy(exeRealPath, "N/A");

    // Convert memory from pages to KB
    long pageSize = sysconf(_SC_PAGESIZE) / 1024;  
    long memory = rss * pageSize;  

    // Add '+' if pid matches current foreground process
    string stateWithPlus = state;
    if (pid == fg_pid) stateWithPlus += "+";

    // Output
    cout << "pid -- " << pid << endl;
    cout << "Process Name -- " << comm << endl;
    cout << "Process Status -- " << stateWithPlus << endl;
    cout << "memory -- " << memory << " KB" << endl;
    cout << "Executable Path -- " << exeRealPath << endl;
}
