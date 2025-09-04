# Custom Linux Shell in C++

## Author

**Parv Shah**  
**2025202019**

## Features

1. **Built-in Commands**  
   - `cd`: Change the current working directory.  
   - `pwd`: Print the current working directory.  
   - `echo`: Display text or variables.  
   - `history`: Display last N commands.  
   - `pinfo`: Display process information.  
   - `ls`: List files and directories with support for `-a`, `-l`, `-al`, and `-la`.  
   - `search`: Search for a file or directory in the current directory and its subdirectories.  

2. **External Commands**  
   - Supports running any system command available in `$PATH`.  
   - Background execution using `&`.  
   - Foreground execution by default.  

3. **Redirection**  
   - Input (`<`), output (`>`), and append (`>>`) redirection supported.  
   - Works in combination with commands and pipelines.  

4. **Pipelines**  
   - Execute multiple commands connected by `|`.  
   - Proper handling of intermediate output between commands.  

5. **Signal Handling**  
   - `Ctrl+C` (SIGINT) stops the foreground process without killing the shell.  
   - `Ctrl+Z` (SIGTSTP) stops and moves the foreground process to background.  

6. **Command History**  
   - Maintains commands in a file (`~/history.txt`).  
   - Load history at shell start and save new commands.  
   - Supports `history_update` internally.  

7. **Autocompletion**  
   - Built-in command completion using **Readline** library.  
   - File and directory name completion for command arguments.  

---

## Project Structure

```
.
├── header.h           # Common headers, global variables, and function declarations
├── shell.cpp          # Main shell loop and input handling
├── executor.cpp       # Handles command execution (foreground/background)
├── cd.cpp             # Implementation of 'cd' command
├── cwd.cpp            # Implementation of 'pwd' command
├── echo.cpp           # Implementation of 'echo' command
├── ls.cpp             # Implementation of 'ls' command
├── history.cpp        # History save/load/print functions
├── search.cpp         # Recursive search implementation
├── autocomplete.cpp   # Readline autocompletion support
├── Makefile           # Build instructions
└── README.md          # This file
```

---

## Compilation

```bash
sudo apt-get install libreadline-dev
```

Compile using `make`:

```bash
make
```

This will generate the `shell` executable.

---

## Usage

Run the shell:

```bash
./shell
```
