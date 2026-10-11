   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>
   #include <signal.h>
   #include <sys/types.h>
   #include <sys/wait.h>

   void sigint_handler(int sig)
   {
       const char msg[] = "\nSIGINT received. Exiting program...\n";
       write(STDOUT_FILENO, msg, sizeof(msg) - 1);
       _exit(0);
   }

   int is_prime(int n)
   {
       if (n < 2) return 0;
       for (int i = 2; (long)i * i <= n; i++)
           if (n % i == 0) return 0;
       return 1;
   }

   int main(void)
   {
       int fd[2];

       if (signal(SIGINT, sigint_handler) == SIG_ERR) {
           perror("signal");
           exit(EXIT_FAILURE);
       }
       if (pipe(fd) == -1) {
           perror("pipe");
           exit(EXIT_FAILURE);
       }

       pid_t pid = fork();
       if (pid < 0) {
           perror("fork");
           exit(EXIT_FAILURE);
       }

       if (pid == 0) {                      /* child */
           signal(SIGINT, SIG_DFL);         /* only the parent prints the SIGINT message */
           close(fd[0]);                    /* close read end */

           int num;
           printf("Enter a number: ");
           fflush(stdout);
           if (scanf("%d", &num) != 1) {
               fprintf(stderr, "Invalid input.\n");
               exit(EXIT_FAILURE);
           }
           write(fd[1], &num, sizeof(num));
           close(fd[1]);
           exit(EXIT_SUCCESS);
       }
       else {                               /* parent */
           close(fd[1]);                    /* close write end */

           int num;
           ssize_t n = read(fd[0], &num, sizeof(num));
           wait(NULL);
           close(fd[0]);

           if (n != (ssize_t)sizeof(num)) {
               printf("No number received from child.\n");
               return EXIT_FAILURE;
           }
           printf("Parent received %d from child: %d is %s.\n",
                  num, num, is_prime(num) ? "a prime number" : "NOT a prime number");
       }
       return EXIT_SUCCESS;
   }
