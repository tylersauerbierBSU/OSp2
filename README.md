# Project 2

 ![Project Meme](meme.jpg)

**This project can only be done on Linux!**

In this project, we will be implementing a simple shell using the examples that
were presented in [chapter
5](https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-api.pdf). This assignment can only
be completed on GNU/Linux operating systems due to the system calls used. The
[glibc
manual](https://www.gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html)
has some awesome documentation for writing a job control shell which you should
read through before you start coding as it will help you understand the problem
space. Keep in mind that some of the code samples provided in the libc manual
are written in C89 so may look a bit strange.  Finally, be aware that the shell
described in the libc manual is much more complex that what we will be building
so don't panic, your shell will be significantly smaller.

## Important Links

- Review the [grading rubric](https://shanepanter.com/cs452/grading-rubric.html)

## Task 1 - Print Version

Let's start off simple and just have our shell print off its version. When the
shell is started with a command line argument -v, it prints out the version of
the project and then quits, you will need to leverage the header file
`labconfig.h` for the major and minor version. Parse the command line arguments
with
[getops](https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html).
The program should exit after printing the version.

## Task 2 - User Input

While we could use a function like
[scanf](https://cplusplus.com/reference/cstdio/scanf/) to get input from the
user a much more robust way would be to leverage the [GNU Readline
library](https://tiswww.case.edu/php/chet/readline/rltop.html). The GNU Readline
library allows a program to control the input line and adds a bunch of cool
functions that allow the user to edit the line, use TAB key for filename completion, and
the use of up arrow, down arrow, left arrow and right arrow keys to access the
history of commands typed in by the user. You will need to include the header
files `readline/readline.h` and `readline/history.h` to use the readline
functions.

```c
#include <readline/readline.h>
#include <readline/history.h>

char *line;

using_history();
while ((line=readline("$"))){
    printf("%s\n",line);
    add_history(line);
    free(line);
}
```

>You need to install both the readline header files and development
libraries for the above code to compile and link. All the correct libraries are
installed on the lab machines. On Redhat based machines development packages
end in devel. So to get all the readline development packages you would need
to install readline-devel as well as readline. On ubuntu the development files
are named readline-dev.

The readline documentation is a good starting point on how to use the readline
library. Pay close attention to memory ownership. Remember that C does not have
a garbage collector.

- [Readline Docs](https://tiswww.cwru.edu/php/chet/readline/readline.html)
- [History Docs](https://tiswww.cwru.edu/php/chet/readline/history.html)

### Custom Prompt

The default prompt for the shell may be anything you like. However the shell
checks for an environment variable `MY_PROMPT`. If the environment variable is
set, then it uses the value as the prompt. The environment variable can be set
inline  `MY_PROMPT="foo>" ./myprogram` so you can quickly test your program.

- Use the system call
  [getenv](https://man7.org/linux/man-pages/man3/getenv.3.html) to retrieve
  environment variables

## Task 3 - Built in Commands

Now that we can get input from users lets add in some built in commands. These
commands need to be handled by the shell itself, you should not create a new
process to handle these commands so it is good to implement these **before**
you add in the fork/exec code in a future task.

### Exit command

Include a built-in command named `exit` that terminates the shell normally. Your
shell should return a status of 0 when it terminates normally and a
non-zero status otherwise. Your shell should also terminate normally on
receiving the end of input [EOF](https://en.wikipedia.org/wiki/End-of-file)
(Under Linux and bash, this would normally be Ctl-d for you to test your
mini-shell). You are required to clean up any allocated memory before you exit.

- [exit](https://man7.org/linux/man-pages/man3/exit.3.html)

### Change Directory Command

Include a built-in command named `cd` to allow an user to change directories.
You will need to use the `chdir` system call. The `cd` command without any
arguments should change the working directory to the user's home directory. You
must first use `getenv` and if `getenv` returns NULL your program should fall
back to the system calls `getuid` and `getpwuid` to find out the home directory
of the user. Make sure to print an error message if the cd command fails.

- See [chdir](https://man7.org/linux/man-pages/man2/chdir.2.html)
- See [getenv](https://man7.org/linux/man-pages/man3/getenv.3.html)
- See [getuid](https://man7.org/linux/man-pages/man2/getuid.2.html)
- See [getpwuid](https://man7.org/linux/man-pages/man3/getpwuid.3p.html)

### History Command

Add a new built in command to your shell to print out a history of command
entered. You should leverage the [history
library](https://tiswww.cwru.edu/php/chet/readline/history.html) library to
accomplish this.

## Task 4 - Create a Process

Our shell will create a new process and wait for it to complete. The shell
accepts one command per line with arguments. It should accept at least `ARG_MAX`
arguments to any command. You will need to use the system call `sysconf` and
`_SC_ARG_MAX` to get the maximum length of arguments that the the `exec` family
of functions can accept. The shell will parse each line that is entered and then
attempt to execute the process using the `execvp` system call. The `execvp`
system call performs a search for the command using the `PATH` environment
variable. This will simplify your programming since you do not have to search
for the location of the command.

For our simple shell you can assume that all command line arguments will be
separated by spaces, you don't have to worry about quoted arguments. For example
given the command `ls -l -a` you would parse this string as an array of size 3
with the structure of `ls` -> `-l` -> and `-a`. The command `ls "-l -a"` would
parse out to be `ls` -> `"-l` -> `-a"`. If you want to write a parsing algorithm
that handles quotes like bash more information is available at [the linux
documentation
project](https://tldp.org/LDP/Bash-Beginners-Guide/html/sect_03_03.html).

If the user just presses the Enter key, then the shell displays another prompt.
If the user types just spaces and then presses the Enter key, then the shell
displays another prompt as this is also an empty command. Empty commands should
not cause a segfault or memory leak.

- [execvp](https://man7.org/linux/man-pages/man3/exec.3.html)
- [fork()](https://man7.org/linux/man-pages/man2/fork.2.html)
- [waitpid()](https://man7.org/linux/man-pages/man2/wait.2.html)

### Signals

The shell should ignore the signals listed below:

```c
signal(SIGINT, SIG_IGN);
signal(SIGQUIT, SIG_IGN);
signal(SIGTSTP, SIG_IGN);
signal(SIGTTIN, SIG_IGN);
signal(SIGTTOU, SIG_IGN);
```

In the child process don't forget to set these signals back to default!

```c
/*This is the child process*/
pid_t child = getpid();
setpgid(child, child);
tcsetpgrp(sh.shell_terminal,child);
signal (SIGINT, SIG_DFL);
signal (SIGQUIT, SIG_DFL);
signal (SIGTSTP, SIG_DFL);
signal (SIGTTIN, SIG_DFL);
signal (SIGTTOU, SIG_DFL);
execvp(cmd[0], cmd);
fprintf(stderr, "exec failed\n");
```

If there is no process being executed, then the shell should just display a new
prompt and ignore any input on the current line. You will need to use the
tcgetpgrp and tcsetpgrp system calls to get and set the foreground process group
of the controlling terminal and the signal system call to ignore and enable
signals.

>The glibc manual links below are describing a job control shell. You are not
required to implement a job control shell. Your shell only needs to handle one
process at a time. You can  to use the documentation linked below as a guide but
be aware you shell will probably fail to function correctly if you just copy and
paste the code examples without understanding what they do.You are free to use
code from the manual as long as you take the time to understand what it does and
why."

- [glibc - Initializing the Shell](https://www.gnu.org/software/libc/manual/html_node/Initializing-the-Shell.html)
- [glibc - Launching Jobs](https://www.gnu.org/software/libc/manual/html_node/Launching-Jobs.html)
- [Signal man page](https://man7.org/linux/man-pages/man7/signal.7.html)

## Task 5 - Job Control

This task is mandatory for **Grad Students**. Undergrads can attempt this task
for extra credit.

In addition to all the undergrad requirements grad students are required to also
implement job control. Your shell can start a process(job) in the background if
an ampersand(&) is the last character on the line. For each background job that
is started, it prints an id and the process id (pid) of the background job and
the full command that was given by the user. After starting a background job,
the shell comes back with a prompt ready to accept a new command from the user
without waiting for the background job to finish.  The user should not be
required to separate the & from the command by a space. For example, the
commands date & and date& are both valid. Additionally, blanks after the
ampersand are valid as well.

### Job List

The mini shell should keep track of commands running in the background. Whenever
the user starts a job in the background, it should assign and display the job
number, the associated process id and the full command (including the
ampersand).

`[n] process-id command`

It should also display an informative message when a background job is done.
That is, every time the user presses the ENTER key, the shell should report all
the background jobs that have finished since the last time the user pressed the
ENTER key.  The message should be of the following form:

`[n] Done command`

Where n is the job number and command is the actual command that was typed.  You
should use the WNOHANG option with the waitpid system call to determine the
status of background jobs.

A complete [job
control](https://www.gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html)
example is available in the libc manual. Your implementation will not be as
complex as what is described there.

### Jobs command

Add a new built-in command called jobs, that prints out all the background
commands that are running or are done but whose status has not yet been
reported.  Here is a sample output:

```bash
[1] 3451 Running sleep 100 &
[2] Done    sleep 5 &
[3] 3452 Running gargantuan &
```

The first job should be assigned the job number 1. Each additional job should be
assigned one higher number. If lower numbered jobs finish, then we do not reuse
them unless all jobs numbered higher than that number have also finished.

Add the built-in commands fg and bg that allow the last process stopped (with
SIGTSTP) to be restarted using the SIGCONT signal (check the man page for signal
in section 7 (man 7 signal).  The fg and bg commands apply to the last stopped
process.  You will probably have to use the [setpgid and
getpgrp](http://man7.org/linux/man-pages/man2/setpgid.2.html) system calls.

With an optional argument the fg and bg commands can apply to any stopped job or
job running in background. For example, fg 2 brings the second job to the
foreground (if there is a job number 2). Note that this implies that you can use
the command fg to bring any running background job into the foreground. Read
through the libc manual pages for a great explanation on foreground and
background jobs. However be careful with the libc manual pages as they describe
implementing pipelines so just copying code (without understanding) will give
your shell incorrect behavior.

Here are some example scenarios.  Suppose you have just stopped a foreground
process and then back-grounded it, the output of your built-in command jobs
would look something like the following (before and after the bg command):

```bash
> loop
<Ctrl>-z
[4] Stopped  loop
> jobs
[1] Running  sleep 101 &
[2] Running  sleep 103 &
[3] Running  sleep 105 &
[4] Stopped  loop
> bg
[4] loop
> jobs
[1] Running  sleep 101 &
[2] Running  sleep 103 &
[3] Running  sleep 105 &
[4] Running  loop
>
```

Note that the three sleep commands were already started in the background
earlier by the user so they aren't affected by the Ctrl-z.  The program loop is
just the following infinitely looping/sleeping program:

`main(){for(;;){sleep(100);}}`

## Task 6 - Complete the Retrospective

Once you have completed all the tasks open the file **Retrospective.md** and
complete each section that has a TODO label. Reference the grading rubric
for details on how this will be graded.

## Task 7 - Add, Commit, Push your code

Once you are finished you need to make sure that you have pushed all your code
to GitHub for grading! The Video walk through linked in the beginning of this
document will show you how to accomplish this task.
