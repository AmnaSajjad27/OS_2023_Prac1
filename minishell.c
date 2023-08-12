/*********************************************************************
Program : miniShell Version : 1.3
--------------------------------------------------------------------
skeleton code for linix/unix/minix command line interpreter
--------------------------------------------------------------------
File : minishell.c
Compiler/System : gcc/linux
********************************************************************/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>


#define NV 20 /* max number of command tokens */
#define NL 100 /* input buffer size */

char line[NL];
int processCounter = 0;
int child_Pids[100];
int children_stat;

void child_finished()
{
    for (int i = 0; i < processCounter; i++)
    {
        if (waitpid(child_Pids[i], &children_stat, WNOHANG) != 0)
        {
            printf("[%d]+ Done                     %s\n",i+1, line);
        }
    }
}

int main(int argk, char *argv[], char *envp[])
{
int frkRtnVal; /* value returned by fork sys call */
// int wpid; /* value returned by wait */
char *v[NV]; /* array of pointers to command line tokens */
char *sep = " \t\n";/* command line token separators */
int i; /* parse index */

while (1)
{ 
    fflush(stdout);
    // read in user command and store in line
    fgets(line, NL, stdin);
    fflush(stdin);

    // check if end of file, then exist 
    if (feof(stdin)) 
    { 
    /* non-zero on EOF */
        exit(0);
    }
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\000')
    {
        continue; /* to prompt */
    }

    // v[0] holds the command and v[i] hold its arguments 
    v[0] = strtok(line, sep);
    for (i = 1; i < NV; i++) 
    {
        // tokenised into indivual commands 
        v[i] = strtok(NULL, sep);
        if (v[i] == NULL)
            break;
    }

// chekc if its a background command 
bool background_cmd = false;

if (i > 1 && strcmp(v[i - 1], "&") == 0) 
{
    background_cmd = true;
    v[i - 1] = NULL; // Remove the "&" from token
    // i--; // minus the # of tokens
}

// cd implementation 
if (i > 1 && strcmp(v[0], "cd") == 0 && !background_cmd)
{
    chdir(v[1]);
    continue;
}


switch (frkRtnVal = fork()) 
{
    case -1: /* fork returns error to parent process */
    {
        // perror("fork");
        break;
    }
    case 0: /* code executed only by child process */
    {
        execvp(v[0], v);
    }
    default: /* code executed only by parent process */
    {
        signal(SIGCHLD, child_finished);
        if (background_cmd)
        {
            child_finished();
            wait(0);
            break;
        }
        else 
        {
            child_finished();
            processCounter++;
            printf("[%d] %d\n", processCounter, frkRtnVal);
            child_Pids[processCounter] = frkRtnVal;
        }
    }
}
}
}