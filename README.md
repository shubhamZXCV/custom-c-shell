click me to view
<a href="https://karthikv1392.github.io/cs3301_osn/mini-projects/mp1">PROBLEM STATEMENT</a>

# Custom C Shell

## How to Run

1. **Compile the Shell**  
   Run the following command to compile the shell:
   ```bash
   make
   ```

2. **Execute the Shell**  
   Once compiled, run the shell using:
   ```bash
   ./a.out
   ```

3. **Clean Up**  
   To remove the object files generated during compilation, use:
   ```bash
   make clean
   ```

### Commands Supported: 
- **hop**: Change the current working directory and prints the absolute path.
    - *Syntax*: `hop <directory>`
        - if `directory` is `"~"`, it changes to the home directory.
        - if `directory` is `"-"`, it changes to the last working directory.
        - otherwise, it changes to the specified directory.
        - in case of no arguments, it changes to the home directory.
        - muliple arguments leads to the final directory till no error and all paths are printed.
- **reveal**: Reveals information about the system, user, and current directory.
    - *Syntax*: `reveal <flag> <directory>`
        - if `flag` is `"-a"`, it reveals all files and directories.
        - if `flag` is `"-l"`, it reveals detailed information about the directory.
        - if `flag` is `"-a*l*"` or `"-l*a*"` or `any order of a and l (both inclusive)`, it reveals all files and directories with detailed information.
        - if `directory` is not provided, it reveals information about the current directory.
        - prints the executables in green, directories in blue, and other files in white.
        - prints the permissions of the files in symbolic format.
- **log**: Handles logging functionalities.
    - *Syntax*: `log`
        - displays the contents of the `command_log.txt` file.
    - *Syntax*: `log purge`
        - purges the contents of the `command_log.txt` file.
    - *Syntax*: `log execute <number>`
        - executes the last `number` commands from the `command_log.txt` file.
    - *Note*: The `log` does not logs the most recent command.
        - It logs all the commands (erreneous too) except the most recent one.
        - The input directly entered in the terminal is logged irrespective of the output.
- **proclore**: Handles process lore.
    - *Syntax*: `proclore <process ID>`
        - if `process ID` is provided, it prints the process information for the given process ID.
        - if no `process ID` is provided, it prints the process information for the current process (terminal).
        - prints the process information for the given process ID.
- **seek**: Handles search functionalities.
    - *Syntax*: `seek <flag> <search query> <directory>`
        - Given the flags are provided, in the form:
            `seek -d <search query> <directory>` or `seek -f <search query> <directory>` or `seek -e <search query> <directory>`
            `seek -d -f <search query> <directory>` or `seek -f -d <search query> <directory>`or `seek -d -e <search query> <directory>`
            `seek -e -d <search query> <directory>` or `seek -f -e <search query> <directory>` or `seek -e -f <search query> <directory>`
            `seek -e*f*d* -f*e*d* -d*f*e* <search query> <directory>` 
            i.e. at max 3 flags are allowed. 
        - if 'flag' is `"-d"`, it searches for only directories based on the search query.
        - if 'flag' is `"-f"`, it searches for only files based on the search query.
        - if both `"-d"` and `"-f"` are provided, leads to an error.
        - if no flag `-d` or `-f` is provided, it searches for both files and directories based on the search query.
        - if 'search query' is not provided, it throws an error.
        - if 'directory' is not provided, it searches in the current directory.
        - if 'directory' is provided, it searches in the specified directory.
        - prints the search results based on the flags and search query.
        - prints the relative path of the search results based on the base directory.
        - prints the search results in color-coded format (directories in blue, files in green).
        - if flag is `"-e"`, if search contains exactly one file (0 directories) it prints the content of the file.
        - if flag is `"-e"`, if search contains more than one file it just prints the names of the files.
        - if flag is `"-e"`, if search contains only one directory (0 files) it prints the exact path of the directory and hops to it.
        - if flag is `"-e"`, if search contains more than one directory it just prints the names of the directories.
- **q**: Quits the shell.
- **fg**: Brings the background process to the foreground.
    - *Syntax*: `fg <PID>`
        - if `PID` is provided, it brings the specified process to the foreground.
        - if no `PID` is provided, it throws an error.

- **bg**: Continues the stopped background process.
    - *Syntax*: `bg <PID>`
        - if `PID` is provided, it continues the specified process in the background.
        - if no `PID` is provided, it throws an error.
- **activities**: Displays the activities of the processes.
    - *Syntax*: `activities`
        - displays the activities of the processes.
- **ping**: Sends a signal to a process based on the provided PID.
    - *Syntax*: `ping <PID> <signal>`
        - if `PID` is provided, it sends the specified signal to the process with the given PID.
        - if no `PID` is provided, it throws an error.
        - if no `signal` is provided, it throws an error.
        - if the `PID` is not found, it throws an error.
        - if the `signal` is not found, it throws an error.
        - if the `signal` is not a number, it throws an error.
- **neonate**: Displays the most recent process ID or continuously updates the process ID.
    - *Syntax*: `neonate <time>`
        - if `time` is provided, it continuously updates the process ID every `time` seconds.
        - if no `time` is provided, it displays the most recent process ID.
        - press `x` to stop the continuous update for both cases.
        - if `time` is not a number, it throws an error.
- **iMan**: Displays the interactive manual for a given command.
    - *Syntax*: `iMan <command>`
        - if `command` is provided, it displays the interactive manual for the given command.
        - if no `command` is provided, it throws an error.

- **Other Commands**: Any other command is executed as a system command using `execvp`.
    - if the provided command is not found, it prints an error message.