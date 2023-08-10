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

// #include <stdbool.h>

#define NV 20 /* max number of command tokens */
#define NL 100 /* input buffer size */

// input command 
char line[NL]; /* command input buffer */

// to hold the background process count 
int background_counter = 0;

/* shell prompt, flushes the output stream */
void prompt(void)
{
    // fprintf(stdout, "\n msh> ");
    fflush(stdout);
}

// Main function 
/* argk - number of arguments
 argv - argument vector from command line
 envp - environment pointer */

int main(int argk, char *argv[], char *envp[])
{
int frkRtnVal; /* value returned by fork sys call */
int wpid; /* value returned by wait */
char *v[NV]; /* array of pointers to command line tokens */
char *sep = " \t\n";/* command line token separators */
int i; /* parse index */

/* prompt for and process one command line at a time */
// infinite loop keeps running asking for input 
while (1)
{ 
    /* do Forever */
    prompt();
    // read in user command and store in line
    fgets(line, NL, stdin);
    fflush(stdin);
    // check if end of file, then exist 
    if (feof(stdin)) 
    { 
    /* non-zero on EOF */
    // fprintf(stderr, "EOF pid %d feof %d ferror %d\n", getpid(),
    // feof(stdin), ferror(stdin));
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
int background_cmd = 0;

if (i > 1 && strcmp(v[i - 1], "&") == 0) 
{
    background_cmd = 1;
    v[i - 1] = NULL; // Remove the "&" from token
    i--; // minus the # of tokens
}

// built in command cd - changes work directory/ folder

    if (i > 1)
    {
        if (strcmp(v[1], "..") == 0)
        {
            if (chdir("..") == -1)
            {
                perror("cd");
            }
        }
        // arg given i.e. cd home
        // WRONG PARENTHIS WTF
        else if (chdir(v[1]) == -1)
        {
            perror("cd");
        }
        continue;
    }

/* assert i is number of tokens + 1 */
/* fork a child process to exec the command in v[0] */

switch (frkRtnVal = fork()) 
{
    case -1: /* fork returns error to parent process */
    {
        perror("fork");
        break;
    }
    case 0: /* code executed only by child process */
    {
    // excute the command 
    if (execvp(v[0], v) == -1)
    {
        // terminate if not properly excuted
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    break;
    }
    default: /* code executed only by parent process */
    {
        if (background_cmd)
        {
            background_counter++;
            printf("[%d] pid\n", background_counter);
        }
        else
        {
            // changed wait 0 to &wpid
            wpid = wait(&wpid);
            printf("[%d]+ Done                        %s\n", background_counter, v[0]);
        }
        break;
        // return frkRtnVal;
    }
}
}
}