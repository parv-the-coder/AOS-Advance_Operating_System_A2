#include"header.h"

void pinfo(pid_t pid) {
    // Paths to /proc files for the given PID
    char stat_path[256], exe_path[256];
    sprintf(stat_path, "/proc/%d/stat", pid);  // file containing process info
    sprintf(exe_path, "/proc/%d/exe", pid);    // symlink to executable

    // Open the stat file in read-only mode
    int fd = open(stat_path, O_RDONLY);
    if (fd == -1) { // check if process exists
        printf("Error: Process with PID %d does not exist.\n", pid);
        return;
    }

    // Read the content of the stat file into a buffer
    char buffer[4096];
    int n = read(fd, buffer, sizeof(buffer) - 1); // leave space for null terminator
    close(fd); // always close file descriptor to avoid leaks

    if (n <= 0) { // error reading the file
        printf("Error reading process info.\n");
        return;
    }
    buffer[n] = '\0'; // null-terminate the buffer to safely treat as string

    // Variables to hold extracted info
    char *p = buffer;          // pointer for manual parsing
    char comm[256] = {0};      // process name
    char state = '?';           // process state
    long rss = 0;               // resident set size (memory)

    // Extract command name (process name) which is between parentheses
    char *start = strchr(p, '(');    // first '('
    char *end = strrchr(p, ')');     // last ')'
    if (start && end && end > start) {
        strncpy(comm, start + 1, end - start - 1); // copy name inside parentheses
        comm[end - start - 1] = '\0';              // null-terminate
        p = end + 2; // move pointer past ") " to start of next fields
    }

    // Extract process state: the single character right after ')'
    if (*p) state = *p;

    // Extract memory info (resident set size)
    // This is the 24th field in /proc/[pid]/stat
    int field = 3;           // we already read comm and state
    char *token = strtok(p, " "); // split remaining string by spaces
    while (token) {
        if (field == 24) rss = atol(token); // convert RSS from string to long
        field++;
        token = strtok(nullptr, " "); // get next field
    }

    // Get executable path using readlink on /proc/[pid]/exe
    char exe_real[PATH_MAX];
    ssize_t len = readlink(exe_path, exe_real, sizeof(exe_real) - 1);
    if (len != -1) exe_real[len] = '\0'; // null-terminate if valid
    else strcpy(exe_real, "N/A");         // if invalid, show N/A

    // Convert memory from pages to KB
    long page_size = sysconf(_SC_PAGESIZE) / 1024; // 1 page in KB
    long memory = rss * page_size;                  // total memory in KB

    // Print process info in the requested format
    printf("pid -- %d\n", pid);
    printf("Process Name -- %s\n", comm);
    printf("Process Status -- %c\n", state);
    printf("memory -- %ld KB\n", memory);
    printf("Executable Path -- %s\n", exe_real);
}