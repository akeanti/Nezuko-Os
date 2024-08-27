#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

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
    printf("\033[34mAvailable commands:\033[0m\n");
    printf("  \033[33mcd <dir>\033[0m          - Change directory\n");
    printf("  \033[33mpwd\033[0m               - Print working directory\n");
    printf("  \033[33mexit\033[0m              - Exit the shell\n");
    printf("  \033[33mhello\033[0m             - Print a hello message\n");
    printf("  \033[33mfortune\033[0m           - Print a random fortune\n");
    printf("  \033[33mcowsay <text>\033[0m     - Print text with a cow ASCII art\n");
    printf("  \033[33mneofetch\033[0m          - Display system information\n");
    printf("  \033[33mascii\033[0m             - Show Nezuko-themed ASCII art\n");
    printf("  \033[33mclock\033[0m             - Show the current time in ASCII art\n");
    printf("  \033[33mosinfo\033[0m            - Show OS info in a window\n");
    printf("  \033[33myay <args>\033[0m        - Run AUR commands\n");
    printf("  \033[33mpacman <args>\033[0m     - Run pacman commands\n");
}

void local_cowsay(char *text) {
    printf("\033[36m"); // Set text color to cyan
    printf(" _______\n");
    printf("< %s >\n", text);
    printf(" -------\n");
    printf("        \\   ^__^\n");
    printf("         \\  (oo)\\_______\n");
    printf("            (__)\\       )\\/\\\n");
    printf("                ||----w |\n");
    printf("                ||     ||\n");
    printf("\033[0m"); // Reset text color
}

void print_nezuko_ascii() {
    printf("\033[36m> \033[35mNezuko Os \033[36mV 1.0.0\n");
    printf("\033[0m"); // Reset text color
}

void print_clock() {
    time_t now;
    struct tm *tm_info;
    char buffer[26];

    time(&now);
    tm_info = localtime(&now);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);

    printf("\033[35m"); // Set text color to pink
    printf("   _____     \n");
    printf("  /     \\    \n");
    printf(" |  %s |   \n", buffer);
    printf("  \\_____/    \n");
    printf("\033[0m"); // Reset text color
}

void show_os_info() {
    // Display Nezuko ASCII art on the left and system info on the right
    printf("\033[35m"); // Set text color to pink
    printf("Nezuko OS v1.0.0\n");
    printf("========================\n");

    printf("           ⢉⣭⣿⣿⣿⣿⣿⣿⣿⣿⣭⣉⠉⡹⠉⠛⠍     \n");
    printf("⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣯⣿⣶⣶⣾⣿⣿⣿⣟⣿⣷⡃⠀⡐⠁⠀⠀ ⠀\n");
    printf("⠀⠀⠀⠀⠀⢀⣴⣿⣿⡿⠻⠟⠛⠋⠉⠛⠿⢿⣿⣿⡿⡋⠤⢾⡀⠤⠒⡆⠀⠀\n");
    printf("⠀⠀⠀⠀⢀⣾⣿⠟⡁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢷⣧⣄⣀⠀⠀⣠⠁⠀⠀\n");
    printf("⠀⠀⠀⠀⣼⣿⠏⠀⠐⠀⠀⠀⠀⠀⠀⠀⢀⠔⠀⠀⠈⣿⣿⣿⣿⣯⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⢠⣿⡟⠀⣐⣠⣔⡓⠀⠀⠀⢰⣾⢤⣄⣂⢄⠀⣿⣿⣿⣷⢿⡄⠀⠀⠀\n");
    printf("⠀⠀⠀⢸⣿⡧⠞⡥⡒⡐⢌⠀⠀⠀⢨⢔⠠⠐⢉⠳⡤⣿⣿⢿⣿⣿⡇⠀⠀⠀\n");
    printf("⠀⠀⠀⢸⣿⣿⠸⡄⠰⠇⢰⠀⠀⠀⢨⠈⠛⠀⢸⠀⠁⣿⣿⡂⢻⣷⣧⠀⠀⠀\n");
    printf("⠀⠀⠀⣸⣿⢿⣸⢯⠀⠐⠂⠀⠀⠀⠀⠁⢒⡢⣕⡀⠀⣿⣿⠀⣾⣿⣿⠀⠀⠀\n");
    printf("⠀⠀⠀⣿⠁⠎⡆⡘⠀⠀⠀⠀⠉⠉⠉⡹⢡⢻⢁⣮⣼⣿⣿⣾⣿⣿⣿⠀⠀⠀\n");
    printf("⠀⠀⢸⣿⣄⣲⣃⣡⣤⣤⣤⣀⣀⣐⡂⡇⢠⣼⠸⣿⣾⣿⣿⣿⣿⣿⣿⡆⠀⠀\n");
    printf("⠀⠀⣾⣿⣿⡿⣻⣿⣿⠇⠈⠩⡀⠉⡩⠜⠉⢩⢷⣿⣿⣽⣿⣿⣿⣟⣿⣿⡄⠀\n");
    printf("⠀⣠⣿⣿⣽⣾⣾⡟⠁⢃⠀⠀⠠⠊⠀⢀⠔⡡⢺⣷⣿⣿⣿⣿⣯⣿⠘⣿⣿⡄\n");
    printf("⣴⣿⣟⣿⣿⣿⠟⠀⠀⠀⠈⠒⠊⠉⠐⠁⠀⠀⠀⠘⠿⠟⠻⠿⠉⠀⠀⠈⠉⠀\n");
    printf("\033[0m"); // Reset text color

        // System information on the right
    char buffer[128];
    FILE *fp;

    // OS Information
    fp = popen("uname -sr", "r");
    if (fp != NULL) {
        fgets(buffer, sizeof(buffer) - 1, fp);
        printf("OS: %s", buffer);
        pclose(fp);
    }

    // Kernel Version
    fp = popen("uname -r", "r");
    if (fp != NULL) {
        fgets(buffer, sizeof(buffer) - 1, fp);
        printf("Kernel: %s", buffer);
        pclose(fp);
    }

    // Hostname
    fp = popen("hostname", "r");
    if (fp != NULL) {
        fgets(buffer, sizeof(buffer) - 1, fp);
        printf("Hostname: %s", buffer);
        pclose(fp);
    }

    // Uptime
    fp = popen("uptime -p", "r");
    if (fp != NULL) {
        fgets(buffer, sizeof(buffer) - 1, fp);
        printf("Uptime: %s", buffer);
        pclose(fp);
    }
}

int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARG_SIZE];

    while (1) {
        // Print prompt with Nezuko-themed style
        print_nezuko_ascii();
        printf("\033[35m🌸 [Linux User] \033[0m"); // Prompt with pink color
        printf("\033[36m$ \033[0m"); // Command input color

        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            perror("fgets");
            exit(EXIT_FAILURE);
        }

        input[strcspn(input, "\n")] = '\0'; // Remove newline character

        if (strcmp(input, "exit") == 0) {
            printf("Exiting Nezuko OS. Have a great day!\n");
            break;
        }

        if (strcmp(input, "help") == 0) {
            print_help();
            continue;
        }

        if (strcmp(input, "cowsay") == 0) {
            printf("Enter text for cowsay: ");
            if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
                perror("fgets");
                exit(EXIT_FAILURE);
            }
            input[strcspn(input, "\n")] = '\0'; // Remove newline character
            local_cowsay(input);
            continue;
        }

        if (strcmp(input, "ascii") == 0) {
            print_nezuko_ascii();
            continue;
        }

        if (strcmp(input, "clock") == 0) {
            print_clock();
            continue;
        }

        if (strcmp(input, "osinfo") == 0) {
            show_os_info();
            continue;
        }

        if (strncmp(input, "cd ", 3) == 0) {
            if (chdir(input + 3) != 0) {
                perror("chdir");
            }
            continue;
        }

        if (strcmp(input, "pwd") == 0) {
            char cwd[MAX_INPUT_SIZE];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
            } else {
                perror("getcwd");
            }
            continue;
        }

        if (strncmp(input, "yay ", 4) == 0) {
            char *args[MAX_ARG_SIZE];
            parse_input(input + 4, args);
            execute_command(args);
            continue;
        }

        if (strncmp(input, "pacman ", 7) == 0) {
            char *args[MAX_ARG_SIZE];
            parse_input(input + 7, args);
            execute_command(args);
            continue;
        }

        if (strcmp(input, "fortune") == 0) {
            system("fortune");
            continue;
        }

        printf("Command not found. Type 'help' for a list of commands.\n");
    }

    return 0;
}
