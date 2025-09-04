#include"header.h"

void pinfo(pid_t pid) 
{
    // route to /proc files for  PID
    char stat_path[256], exe_path[256];
    sprintf(stat_path, "/proc/%d/stat", pid);  // file containing process info
    sprintf(exe_path, "/proc/%d/exe", pid);    // symlink to executable

    // stat file in read-only mode
    int fd = open(stat_path, O_RDONLY);
    
    // check if process exists
    if (fd == -1) 
    {
        printf("Error: Process with PID %d does not exist.\n", pid);
        return;
    }

    // read cont stat file 
    char buffer[4096];
    int n = read(fd, buffer, sizeof(buffer) - 1); // leave space for null-termintor
    close(fd);

    // checking  reading  
    if (n <= 0) {
        printf("Error reading process info.\n");
        return;
    }

    buffer[n] = '\0';

    // to hold extracted info
    char *p = buffer;        
    char comm[256] = {0};      //  name
    char state = '?';           // state
    long rss = 0;               // memory

    // extracting comm name
    char *start = strchr(p, '(');  
    char *end = strrchr(p, ')');    
    
    if (start && end && end > start) 
    {
        strncpy(comm, start + 1, end - start - 1); // copy name
        comm[end - start - 1] = '\0';              // null-terminate
        p = end + 2;
    }

    // process state
    if (*p) 
    {
        state = *p;
    }

    //memory info
    int field = 3;           
    char *token = strtok(p, " "); // split string by spaces
    
    while (token) 
    {
        
        if (field == 24) 
        {
            rss = atol(token);
        }

        field++;
        token = strtok(nullptr, " "); // next field
    }

    //  path using readlink
    char exe_real[PATH_MAX];
    ssize_t len = readlink(exe_path, exe_real, sizeof(exe_real) - 1);
    
    if (len != -1) 
    {
        exe_real[len] = '\0'; // null-terminate
    }

    else
    {
        strcpy(exe_real, "N/A");         // invalid
    }

    // conv mem from pages to KB
    long page_size = sysconf(_SC_PAGESIZE) / 1024; // 1 page in KB
    long memory = rss * page_size;                  // total memory in KB

    // process info in format
    printf("pid -- %d\n", pid);
    printf("Process Name -- %s\n", comm);
    printf("Process Status -- %c\n", state);
    printf("memory -- %ld KB\n", memory);
    printf("Executable Path -- %s\n", exe_real);
}