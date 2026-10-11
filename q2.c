#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void special_handler(int sig)
{
    if (sig == SIGINT)
        printf("This is a special signal handler for SIGINT\n");
    else if (sig == SIGTSTP)
        printf("This is a special signal handler for SIGTSTP\n");
    else if (sig == SIGQUIT)
        printf("This is a special signal handler for SIGQUIT\n");
}

int main(void)
{
    char s[200];

    if (signal(SIGINT, special_handler) == SIG_ERR ||
        signal(SIGTSTP, special_handler) == SIG_ERR ||
        signal(SIGQUIT, special_handler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }

    printf("Enter a string:\n");

    if (fgets(s, 200, stdin) == NULL)
        perror("gets");
    else
        printf("you entered: %s\n", s);

    return 0;
}
