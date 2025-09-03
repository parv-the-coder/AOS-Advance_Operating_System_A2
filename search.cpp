#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <cstring>
#include <string>
#include <sys/stat.h>
#include <dirent.h>

using namespace std;

// Recursive function to search for a file/directory in current + all subdirectories
bool recursiveSearch(const string& directory, const string& target) {
    DIR *dir = opendir(directory.c_str());
    if (!dir) {
        // skip if cannot open directory
        return false;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        string name = entry->d_name;

        // Skip "." and ".."
        if (name == "." || name == "..") continue;

        // Found match
        if (name == target) {
            closedir(dir);
            return true;
        }

        // If it's a directory, recurse into it
        // if (entry->d_type == DT_DIR) {
        //     string newPath = directory + "/" + name;
        //     if (recursiveSearch(newPath, target)) {
        //         closedir(dir);
        //         return true;
        string newPath = directory + "/" + name;
        struct stat st;
        if (lstat(newPath.c_str(), &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                if (recursiveSearch(newPath, target)) {
                    closedir(dir);
                    return true;
            }
        }
        }
    }

    closedir(dir);
    return false;
}

// Wrapper: search starting from current working directory
bool searchInCurrentDirectory(const string& target) {
    char currentPath[PATH_MAX];
    if (getcwd(currentPath, sizeof(currentPath)) == nullptr) {
        perror("Error: Unable to get current working directory");
        return false;
    }
    return recursiveSearch(currentPath, target);
}
