// shell.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100

void execute_command(char **args) {
    pid_t pid, wpid;
    int status;

    if ((pid = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

void parse_input(char *input, char **args) {
    char *token;
    int i = 0;

    token = strtok(input, " \t\r\n");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \t\r\n");
    }
    args[i] = NULL;
}

void print_help() {
    printf("Available commands:\n");
    printf("  cd <dir>          - Change directory\n");
    printf("  pwd               - Print working directory\n");
    printf("  exit              - Exit the shell\n");
    printf("  hello             - Print a hello message\n");
    printf("  fortune           - Print a random fortune\n");
    printf("  cowsay <text>     - Print text with a cow ASCII art\n");
    printf("  neofetch          - Display system information\n");
    printf("  ascii             - Show Nezuko-themed ASCII art\n");
    printf("  yay <args>        - Run AUR commands\n");
    printf("  pacman <args>     - Run pacman commands\n");
}

void local_cowsay(char *text) {
    printf(" _______\n");
    printf("< %s >\n", text);
    printf(" -------\n");
    printf("        \\   ^__^\n");
    printf("         \\  (oo)\\_______\n");
    printf("            (__)\\       )\\/\\\n");
    printf("                ||----w |\n");
    printf("                ||     ||\n");
}

void print_nezuko_ascii() {
    printf("\033[35m"); // Set text color to pink
    printf("  /\\_/\\ \n");
    printf(" ( o.o ) \n");
    printf(" > ^ < \n");
    printf(" Nezuko-chan\n");
    printf("\033[0m"); // Reset text color
}

void handle_cute_commands(char **args) {
    if (strcmp(args[0], "hello") == 0) {
        printf("Hello, welcome to Nezuko OS!\n");
    } else if (strcmp(args[0], "fortune") == 0) {
        system("fortune");
    } else if (strcmp(args[0], "cowsay") == 0) {
        if (args[1] != NULL) {
            // Print text with local cowsay
            local_cowsay(args[1]);
        } else {
            printf("Usage: cowsay <text>\n");
        }
    } else if (strcmp(args[0], "neofetch") == 0) {
        system("neofetch");
    } else if (strcmp(args[0], "ascii") == 0) {
        print_nezuko_ascii();
    } else {
        // Not a cute command, continue to normal execution
        execute_command(args);
    }
}

void shell_loop() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARG_SIZE];
    int should_run = 1;

    while (should_run) {
        printf("\033[35mNezuko OS> \033[0m"); // Pink prompt
        fflush(stdout);

        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            perror("fgets");
            continue;
        }

        printf("Input received: %s\n", input);

        if (strcmp(input, "exit\n") == 0) {
            should_run = 0;
            continue;
        }

        parse_input(input, args);

        if (args[0] != NULL) {
            // Built-in commands
            if (strcmp(args[0], "cd") == 0) {
                if (args[1] == NULL) {
                    fprintf(stderr, "cd: missing argument\n");
                } else if (chdir(args[1]) != 0) {
                    perror("cd");
                }
            } else if (strcmp(args[0], "pwd") == 0) {
                char cwd[MAX_INPUT_SIZE];
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    printf("%s\n", cwd);
                } else {
                    perror("pwd");
                }
            } else if (strcmp(args[0], "help") == 0) {
                print_help();
            } else if (strcmp(args[0], "yay") == 0 || strcmp(args[0], "pacman") == 0) {
                // AUR or pacman commands
                execute_command(args);
            } else {
                // Handle cute commands or external commands
                handle_cute_commands(args);
            }
        }
    }
}

int main() {
    shell_loop();
    return 0;
}
