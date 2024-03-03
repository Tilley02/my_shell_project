// Conor Tilley, 21307283
// I acknowledge and understand the DCU Acedemic Integrity Policy

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h> //For getting the last directory of the path for the prompt
#include "command.h"

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token separators

// Initializing functions
void parse_input(char **args, char **arg, char *buff);
void batchfile(char **argv, char **args, char *buff, char **arg);

int main(int argc, char **argv)
{
    char buff[MAX_BUFFER];                      // line buffer
    char * args[MAX_ARGS];                     // pointers to arg strings
    char ** arg = NULL;                               // working pointer thru args
    char cwd[256];                             // used in the cd command and pwd command
    char * prompt = " --> " ;                   // shell prompt
    int background_checker = 0;
    // FILE *input_file = stdin;
    // FILE *output_file = stdout;

// Checks for file
    if(argc == 2) {
        batchfile(argv, args, buff, arg);
    }


// I/O redirection
    // for (int i = 1; i < argc - 1; i++) {
    //     if (!strcmp(argv[i], "<")) {
    //         input_file = fopen(argv[i+1], "r");
    //         if (!input_file) {
    //             printf("Failed to open input file, exiting\n");
    //             exit(1);
    //         }
    //         i++;
    //     }
    //     else if (!strcmp(argv[i], ">")) {
    //         output_file = fopen(argv[i+1], "w");
    //         if (!output_file) {
    //             printf("Failed to open output file, exiting\n");
    //             exit(1);
    //         }
    //         i++;
    //     } else if (!strcmp(argv[i], ">>")) {
    //         output_file = fopen(argv[i+1], "a");
    //         if (!output_file) {
    //             printf("Failed to open output file, exiting\n");
    //             exit(1);
    //         }
    //         i++;
    //     }
    // }

    // Redirects the input and output files to location, gets file descriptors with fileno, dup2 then redirects the files
//    int input_file_var = fileno(input_file);
//     int output_file_var = fileno(output_file);

    // Redirect input and output to the specified files
    // if (dup2(input_file_var, STDIN_FILENO) == -1) {
    //     printf("Failed to redirect input, exiting\n");
    //     exit(1);
    // }
    // if (dup2(output_file_var, STDOUT_FILENO) == -1) {
    //     perror("Failed to redirect output, exiting\n");
    //     exit(1);
    // }

    // // close unnecessary file descriptors
    // if (input_file_var != STDIN_FILENO) {
    //     close(input_file_var);
    // }
    // if (output_file_var != STDOUT_FILENO) {
    //     close(output_file_var);
    // }

    // Reads from command line input and puts to output file then
    // while (fgets(buff, sizeof(buff), stdin)) {
    //     fputs(buff, stdout);
    // }

    // closeing the input and output files
    // fclose(input_file);
    // fclose(output_file);


    while(!feof(stdin))
    {
        getcwd(cwd, sizeof(cwd));
        char *dir = basename(cwd); // basename is in the libgen library, gets the last directory to use for prompt
        printf("%s%s", dir, prompt);
        // Initializing arguments
        for (int i = 0; i < MAX_ARGS; i++) {
            args[i] = NULL;
        }

// Parses input from command line then gives them to main_command functions which checks it for commands
        if(fgets(buff, MAX_BUFFER, stdin)) {
            parse_input(args, arg, buff);
            if(args[0]) {
                
                if (arg > args && arg[-1][strlen(arg[-1]) - 1] == '&') {
                    arg[-1][strlen(arg[-1]) - 1] = '\0';
                    background_checker = 1; // If this is changed to one process gets forked and user is returned back to 
                                            // Prompt where they can enter another command while the other runs in the back
                }

                if (args[0] && background_checker) {
                    pid_t pid = fork();
                    if (pid == -1) {
                        printf("Fork failed\n");
                    }
                    else if (pid == 0) {
                        execvp(args[0], args);
                        printf("Fork faile, exiting\n");
                        exit(1);
                    }
                }

                // If cannot be found breaks loop and exits
                if(main_command(args)==0) {
                    break;
                }
                // Otherwise continue
                else {
                    continue;
                }

                arg = args;
                while (*arg) {
                    fprintf(stdout,"%s ",*arg++);
                    fputs ("\n", stdout);

                }
            }
        }
    }
}

// Parses the command line into arguments like argv for the command line to read
void parse_input(char **args, char **arg, char *buff) {
    arg = args;
    *arg++ = strtok(buff, SEPARATORS);
    while ((*arg++ = strtok(NULL,SEPARATORS)));
}



// Checks if a file is entered and to read it and its contents
void batchfile(char **argv, char **args, char *buff, char **arg) {
    FILE *file;
    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error couldn't open file %s\n", argv[1]);
        return;
    }
    else {
        while (fgets(buff, MAX_BUFFER, file)) {
            parse_input(args, arg, buff);
            if (args[0]) {
                if (main_command(args) == 0) {
                    break;
                }
            }
            // Freeing space that was allocated for args
            for (int i = 0; args[i]; i++) {
            free(args[i]);
            args[i] = NULL;
        }
        }
        fclose(file);
    }
}