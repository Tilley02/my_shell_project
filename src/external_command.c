// Conor Tilley, 21307283
// I acknowledge and understand the DCU Acedemic Integrity Policy

// This file is run if a command has not been made by me, a child process is made to run the command

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

extern int errno;
void syserr(char* );

int external(char ** args)
{
   pid_t pid;

   pid = fork();

   if (pid == -1)
   {
      printf("System failed to fork\n");
      syserr("fork");
      return 0;
   }
   else if (pid == 0)
   {
      execvp(args[0], args);
      syserr("exec"); // If execvp fails return system error exec
   }
   for(int i = 0; args[i]; i++) {
      if(!strcmp(args[i], "&")) { //Should return to command prompt immediately after command run
         args[i] = NULL;
         return 1;
      }
   }
   wait(NULL);

   return 1;
}

void syserr(char * msg)
{
   fprintf(stderr, "%s: %s", strerror(errno), msg);
   abort();
}
