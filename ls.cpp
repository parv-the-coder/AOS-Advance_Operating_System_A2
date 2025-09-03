#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <grp.h>
#include <pwd.h>
#include <ctime>
#include <iomanip>

using namespace std;

#include"header.h"
// Helper: Convert file type + permissions to string
string filePermissions(mode_t mode) {
    string perms = "";

    // File type
    if (S_ISREG(mode)) perms += '-';
    else if (S_ISDIR(mode)) perms += 'd';
    else if (S_ISLNK(mode)) perms += 'l';
    else if (S_ISCHR(mode)) perms += 'c';
    else if (S_ISBLK(mode)) perms += 'b';
    else if (S_ISFIFO(mode)) perms += 'p';
    else if (S_ISSOCK(mode)) perms += 's';
    else perms += '?';

    // User permissions
    perms += (mode & S_IRUSR) ? 'r' : '-';
    perms += (mode & S_IWUSR) ? 'w' : '-';
    perms += (mode & S_IXUSR) ? 'x' : '-';

    // Group permissions
    perms += (mode & S_IRGRP) ? 'r' : '-';
    perms += (mode & S_IWGRP) ? 'w' : '-';
    perms += (mode & S_IXGRP) ? 'x' : '-';

    // Others permissions
    perms += (mode & S_IROTH) ? 'r' : '-';
    perms += (mode & S_IWOTH) ? 'w' : '-';
    perms += (mode & S_IXOTH) ? 'x' : '-';

    return perms;
}

// List directory contents
void listDirectoryContents(const string &directoryPath, bool showHidden, bool longFormat) {
    DIR *directory = opendir(directoryPath.empty() ? "." : directoryPath.c_str());
    if (!directory) {
        cerr << "Cannot access " << directoryPath << ": No such directory" << endl;
        return;
    }

    vector<string> names;
    struct dirent *entry;
    while ((entry = readdir(directory)) != nullptr) {
        string name = entry->d_name;
        if (!showHidden && name[0] == '.') continue;
        names.push_back(name);
    }

    closedir(directory);

    // Sort alphabetically
    sort(names.begin(), names.end());

    for (const auto &name : names) {
        string fullPath = directoryPath + "/" + name;
        struct stat fileStat;
        if (lstat(fullPath.c_str(), &fileStat) < 0) {
            perror("stat");
            continue;
        }

        if (longFormat) {
            string perms = filePermissions(fileStat.st_mode);
            struct passwd *pw = getpwuid(fileStat.st_uid);
            struct group *gr = getgrgid(fileStat.st_gid);

            // Format modification time
            char timebuf[80];
            struct tm *tm_info = localtime(&fileStat.st_mtime);
            strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm_info);

            cout << perms << "\t";
            cout << fileStat.st_nlink << "\t";
            cout << (pw ? pw->pw_name : "unknown") << "\t";
            cout << (gr ? gr->gr_name : "unknown") << "\t";
            cout << fileStat.st_size << "\t";
            cout << timebuf << "\t";
            cout << name << endl;
        } else {
            cout << name << "\t";
        }
    }

    if (!longFormat) cout << endl;
}

// Main ls command handler
void lsCommand(vector<string> paths, bool showHidden, bool longFormat) {
    if (paths.empty()) paths.push_back(".");

    for (size_t i = 0; i < paths.size(); i++) {
        if (paths.size() > 1) {
            cout << paths[i] << ":" << endl;
        }
        listDirectoryContents(paths[i], showHidden, longFormat);
        if (i != paths.size() - 1) cout << endl;
    }
}
