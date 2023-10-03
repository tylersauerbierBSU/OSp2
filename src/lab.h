/*Do NOT modify this file! */
#ifndef LAB_H
#define LAB_H
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <lab_export.h>

#define UNUSED(x) (void)x;

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Set the shell prompt. This function will attempt to load a prompt
     * from the requested environment variable, if the environment variable is
     * not set a default prompt of "shell>" is returned.  This function calls
     * malloc internally and the caller must free the resulting string.
     *
     * @param env The environment variable
     * @return const char* The prompt
     */
    LAB_EXPORT char *get_prompt(const char *env);

    /**
     * Changes the current working directory of the shell. Uses the linux system
     * call chdir. With no arguments the users home directory is used as the
     * directory to change to.
     *
     * @param dir The directory to change to
     * @return  On success, zero is returned.  On error, -1 is returned, and
     * errno is set to indicate the error.
     */
    LAB_EXPORT int change_dir(char **dir);

    /**
     * @brief Convert line read from the user into to format that will work with
     * execvp We limit the number of arguments to ARG_MAX loaded from sysconf.
     * This function allocates memory that must be reclaimed with the cmd_free
     * function.
     *
     * @param line The line to process
     *
     * @return The line read in a format suitable for exec
     */
    LAB_EXPORT char **cmd_parse(char const *line);

    /**
     * @brief Free the line that was constructed with parse_cmd
     *
     * @param line the line to free
     */
    LAB_EXPORT void cmd_free(char ** line);

    /**
     * @brief Trim the whitespace from the start and end of a string.
     * For example "   ls -a   " becomes "ls -a". This function modifies
     * the argument line so that all printable chars are moved to the
     * front of the string
     *
     * @param line The line to trim
     * @return The new line with no whitespace
     */
    LAB_EXPORT char *trim_white(char *line);

    /**
     * @brief Entry point for the main function
     *
     * @param argc The argument count
     * @param argv The argument array
     * @return The exit code
     */
    LAB_EXPORT int go(int argc, char **argv);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
