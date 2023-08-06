// Task 1: takes the input parameter n from the command line and prints the first “n” even numbers

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>


int main (int argc, int *argv[])
{
    // print ouch and yeah
    void sig_hup_handler(signum)
    {
        printf("Ouch!");
    }
    void sig_int_handler(signum)
    {
        printf("Yeah!");
    }
    // Register signal handler
    signal(SIGHUP, sig_hup_handler);
    signal(SIGINT, sig_int_handler);

    if (argc != 2)
    {
        // no paramter passed 
        return 1;
    }

    int n; 
    // ASCII to integer 
    n = atoi(argv[1]);

    // for error checking 
    // printf("N passed is %d",n);

    for (int i = 1; n > 0; i++)
    {
        //Check if odd
        if (i % 2 == 0)
        {
            // Print the n
            printf("%d ",i);
            sleep(5);
            n--;
        }
    }

}
