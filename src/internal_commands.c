// Conor Tilley, 21307283
// I acknowledge and understand the DCU Acedemic Integrity Policy

#include "internal_commands.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


extern char **environ;
pid_t pid;

// Cd command that requires an argument to cd to
void cd(char **args) {
   if (args[1] == NULL) {
      printf("Enter a directory to cd to");
   }
   // If directory does not exist shows error message below
   else {
      if(chdir(args[1]) == -1) {
         printf("Directory %s does not exist\n", args[1]);
      }
   }
}

// Clear clears the users terminal, error handeling for fork in it
void clear() {
      pid = fork();
      if (pid == -1) {
         perror("Fork failed");
      }
      else if (pid == 0) {
         execl("/usr/bin/clear", "clear", (char* ) NULL);
         exit(1);
      }
      else {
         waitpid(pid, NULL, 0);
      }
}

// dir lists all the files and directories in a current directory in long format
void dir() {
   pid = fork();
   if (pid == -1) {
      perror("Fork failed");
   }
   else if (pid == 0) {
      execl("/bin/ls", "ls", "-al", (char* ) NULL);
      exit(1); //If execl fails
   }
   else {
      waitpid(pid, NULL, 0); // Parent process waits for child process to exit
   }
}

// Lists all the enviroment variables
void environment() {
   for (int i = 0; environ[i] != NULL; i++)
      printf("%s\n", environ[i]);
}

// Gets an enviroment vairable
void get_env(char **args) {
   char* input = getenv(args[1]);
   if (input == NULL) {
      printf("Environment variable not set, please set it.\n");
   }
   else {
      printf("Enviroment set: %s=%s\n", args[1], input);
   }
}

// Can set an enviroment variable to whatever
void set_env(char **args) {
   if (args[1] == NULL || args[2] == NULL) {
      printf("Please enter variables at position 1 and 2.\n");
   }
   else {
      setenv(args[1], args[2], 1); // The 1 at the end here overrides the previous set variable if there is one set
   }
}

// Prints out whatever the users types out after echo
void echo(char *argv[]) {
   for (int i = 1; argv[i]; i++)
   {
      printf("%s ", argv[i]);
   }
   printf("\n");
}

// Brings the user to the help manual
void help() {
   system("more ../manual/readme.md");
}

// Waits for the users to press the enter key
void pause_shell() {
   printf("Shell paused. Please press Enter to continue\n");
   getchar();
}
