#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid1 = fork();
        if (pid1 == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid1 == 0) { // Child process 1 (command)
            close(pipefd[0]); // Close read end
            dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
            close(pipefd[1]);

            char **args = malloc((argc - i + 1) * sizeof(char *));
            int arg_count = 0;
            char *token = strtok(argv[i], " ");
            while (token != NULL) {
                args[arg_count++] = token;
                token = strtok(NULL, " ");
            }
            args[arg_count] = NULL;

            execvp(args[0], args);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else { // Parent process
            pid_t pid2 = fork();
            if (pid2 == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (pid2 == 0) { // Child process 2 (wc)
                close(pipefd[1]); // Close write end
                dup2(pipefd[0], STDIN_FILENO); // Redirect stdin from pipe
                close(pipefd[0]);

                execlp("wc", "wc", "-c", NULL);
                perror("execlp");
                exit(EXIT_FAILURE);
            } else { // Grandparent process
                close(pipefd[0]);
                close(pipefd[1]);

                waitpid(pid1, NULL, 0);
                waitpid(pid2, NULL, 0);
            }
        }
    }
    return 0;
}
