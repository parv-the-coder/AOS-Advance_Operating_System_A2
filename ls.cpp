#include "header.h"
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// return type of file  single character
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

// returns perm string
string perm(mode_t m)
{
    string p = "";

    // user perm
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

    // group perm
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

    // other perm
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

// list contents of one dir d = dir path h = hidden files l = long format
void lsdir(const string &d, bool h, bool l)
{
    // use . if no dir is specified
    string dir;
    if (d.empty()) 
    {
        dir = ".";
    }
    else 
    {
        dir = d;
    }


    // open  dir
    DIR *dp = opendir(dir.c_str());
    if (dp == nullptr)
    {
        cerr << "error: cannot open '" << dir << "'" << endl;
        return;
    }

    vector<string> f;      // store file names
    struct dirent *e;

    // read entries in dir
    while ((e = readdir(dp)) != nullptr)
    {
        string n = e->d_name;

        // ignore hidden files if h = false
        if (!h && n[0] == '.')
        {
            continue;
        }

        f.push_back(n); // add file name to list
    }

    closedir(dp);

    // sorting 
    sort(f.begin(), f.end());

    // printing each files
    for (size_t i = 0; i < f.size(); i++)
    {
        string p = dir + "/" + f[i];
        struct stat s;

        // get file info
        if (lstat(p.c_str(), &s) < 0)
        {
            cerr << "error: cannot access '" << p << "'" << endl;
            continue;
        }

        if (l)
        {
            cout << ftype(s.st_mode);   // file type
            cout << perm(s.st_mode);    // perm
            cout << "\t";

            cout << s.st_nlink << "\t"; // number of links

            // owner name
            struct passwd *pw = getpwuid(s.st_uid);
            cout << (pw ? pw->pw_name : "unknown") << "\t";

            // group name
            struct group *gr = getgrgid(s.st_gid);
            cout << (gr ? gr->gr_name : "unknown") << "\t";

            cout << s.st_size << "\t";   // file size
            cout << f[i] << endl;        // file name
        }
        else
        {
            cout << f[i] << "\t";   
        }
    }

    // adding newline if not long format
    if (!l)
    {
        cout << endl;
    }
}




// ls comm
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