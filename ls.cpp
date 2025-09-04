#include "header.h"
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Returns the type of file as a single character
// '-' = regular, 'd' = directory, 'l' = symlink, etc.
char ftype(mode_t m)
{
    if (S_ISREG(m))
    {
        return '-';
    }
    else if (S_ISDIR(m))
    {
        return 'd';
    }
    else if (S_ISLNK(m))
    {
        return 'l';
    }
    else if (S_ISCHR(m))
    {
        return 'c';
    }
    else if (S_ISBLK(m))
    {
        return 'b';
    }
    else if (S_ISFIFO(m))
    {
        return 'p';
    }
    else if (S_ISSOCK(m))
    {
        return 's';
    }
    else
    {
        return '?';
    }
}

// Returns permission string (rwx for user/group/other)
string perm(mode_t m)
{
    string p = "";

    // User permissions
    if (m & S_IRUSR)
    {
        p += 'r';
    }
    else
    {
        p += '-';
    }

    if (m & S_IWUSR)
    {
        p += 'w';
    }
    else
    {
        p += '-';
    }

    if (m & S_IXUSR)
    {
        p += 'x';
    }
    else
    {
        p += '-';
    }

    // Group permissions
    if (m & S_IRGRP)
    {
        p += 'r';
    }
    else
    {
        p += '-';
    }

    if (m & S_IWGRP)
    {
        p += 'w';
    }
    else
    {
        p += '-';
    }

    if (m & S_IXGRP)
    {
        p += 'x';
    }
    else
    {
        p += '-';
    }

    // Others permissions
    if (m & S_IROTH)
    {
        p += 'r';
    }
    else
    {
        p += '-';
    }

    if (m & S_IWOTH)
    {
        p += 'w';
    }
    else
    {
        p += '-';
    }

    if (m & S_IXOTH)
    {
        p += 'x';
    }
    else
    {
        p += '-';
    }

    return p;
}

// Lists contents of a single directory
// d = directory path
// h = show hidden files
// l = long format
void lsdir(const string &d, bool h, bool l)
{
    string dir = "";
    if (d.empty())
    {
        dir = ".";
    }
    else
    {
        dir = d;
    }

    DIR *dp = opendir(dir.c_str());
    if (dp == nullptr)
    {
        cerr << "error: cannot open '" << dir << "'" << endl;
        return;
    }

    vector<string> f;
    struct dirent *e;

    while (true)
    {
        e = readdir(dp);
        if (e == nullptr)
        {
            break;
        }

        string n = e->d_name;

        if (!h)
        {
            if (n[0] == '.')
            {
                continue;
            }
        }

        f.push_back(n);
    }

    closedir(dp);

    sort(f.begin(), f.end());

    for (size_t i = 0; i < f.size(); i++)
    {
        string p = dir + "/" + f[i];
        struct stat s;

        if (lstat(p.c_str(), &s) < 0)
        {
            cerr << "error: cannot access '" << p << "'" << endl;
            continue;
        }

        if (l)
        {
            cout << ftype(s.st_mode);
            cout << perm(s.st_mode);
            cout << "\t";

            cout << s.st_nlink << "\t";

            struct passwd *pw = getpwuid(s.st_uid);
            if (pw != nullptr)
            {
                cout << pw->pw_name << "\t";
            }
            else
            {
                cout << "unknown" << "\t";
            }

            struct group *gr = getgrgid(s.st_gid);
            if (gr != nullptr)
            {
                cout << gr->gr_name << "\t";
            }
            else
            {
                cout << "unknown" << "\t";
            }

            cout << s.st_size << "\t";
            cout << f[i] << endl;
        }
        else
        {
            cout << f[i] << "\t";
        }
    }

    if (!l)
    {
        cout << endl;
    }
}

// Main ls command handler for multiple directories
// dirs = vector of directories
// h = show hidden files
// l = long format
void ls(vector<string> dirs, bool h, bool l)
{
    if (dirs.empty())
    {
        dirs.push_back(".");
    }

    for (size_t i = 0; i < dirs.size(); i++)
    {
        if (dirs.size() > 1)
        {
            cout << dirs[i] << ":" << endl;
        }

        lsdir(dirs[i], h, l);

        if (i != dirs.size() - 1)
        {
            cout << endl;
        }
    }
}