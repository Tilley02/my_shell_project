// Conor Tilley, 21307283
// I acknowledge and understand the DCU Acedemic Integrity Policy

// Runs through commands I have made, if none there execute the external command, which runs a child process to execute the code
// This is executed in myshell.c to run through commands

#include <string.h>
#include "internal_commands.h"
#include "internal_commands.c"
#include "external_command.h"
#include "external_command.c"
#include "command.h"

char cwd[256];

// Every function until the external function is defined in the internal command file, external is defined in the external command file
int main_command(char **args) {

   if (!strcmp(args[0], "cd")) {
      cd(args);
      return 1;
   }

   else if (!strcmp(args[0], "clr")) {
      clr();
      return 1;
   }

   else if (!strcmp(args[0], "dir")) {
      dir();
      return 1;
   }

   else if (!strcmp(args[0], "environ")) {
      environment();
      return 1;
   }

   else if (!strcmp(args[0], "get_env")) {
      get_env(args);
      return 1;
   }
   

   else if (!strcmp(args[0], "set_env")) {
      set_env(args);
      return 1;
   }

   else if (!strcmp(args[0], "echo")) {
      echo(args);
      return 1;
   }

   else if (!strcmp(args[0], "help")) {
      help();
      return 1;
   }

   else if (!strcmp(args[0], "pause")) {
      pause_shell();
      return 1;
   }

   else if (!strcmp(args[0], "quit")) {
      return 0;
   }

   else {
      if (external(args) == 0) {
         return 0;
      }
      else {
         return 1;
      }
   }
}
