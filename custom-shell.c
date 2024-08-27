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
        perror("\033[31mError\033[0m: \033[1mFork Failed\033[0m");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("\033[31mError\033[0m: \033[1mCommand Failed\033[0m");
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
    printf("\033[1;35mAvailable Commands:\033[0m\n");
    printf("  \033[1;33mcd <dir>\033[0m          - Change directory\n");
    printf("  \033[1;33mpwd\033[0m               - Print working directory\n");
    printf("  \033[1;33mexit\033[0m              - Exit the shell\n");
    printf("  \033[1;33mhello\033[0m             - Print a hello message\n");
    printf("  \033[1;33mfortune\033[0m           - Print a random fortune\n");
    printf("  \033[1;33mcowsay <text>\033[0m     - Print text with a cow ASCII art\n");
    printf("  \033[1;33mascii\033[0m             - Show Nezuko-themed ASCII art\n");
    printf("  \033[1;33mclock\033[0m             - Show the current time in ASCII art\n");
    printf("  \033[1;33mosinfo\033[0m            - Show OS info in a window\n");
    printf("  \033[1;33myay <args>\033[0m        - Run AUR commands\n");
    printf("  \033[1;33mpacman <args>\033[0m     - Run pacman commands\n");
    printf("  \033[1;33mweather <city>\033[0m     - Display current weather for a city\n");
    printf("  \033[1;33mnews\033[0m               - Display latest news headlines\n");
    printf("  \033[1;33mgame\033[0m               - Play a text-based game\n");
    printf("  \033[1;33mquote\033[0m              - Display a random inspirational quote\n");
    printf("  \033[1;33msysteminfo\033[0m        - Display detailed system information\n");
    printf("  \033[1;33mlistprocs\033[0m         - List all running processes\n");
}

void local_cowsay(char *text) {
    printf("\033[1;36m"); // Set text color to cyan
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
    printf("\033[1;35m> \033[1;36mNezuko OS \033[1;35mV 1.0.0 \033[1;32m~ \033[1;37m%s\033[0m", getenv("PWD"));
    printf("\033[0m"); // Reset text color
}

void print_clock() {
    time_t now;
    struct tm *tm_info;
    char buffer[26];

    time(&now);
    tm_info = localtime(&now);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);

    printf("\033[1;35m"); // Set text color to pink
    printf("   _____     \n");
    printf("  /     \\    \n");
    printf(" |  %s |   \n", buffer);
    printf("  \\_____/    \n");
    printf("\033[0m"); // Reset text color
}

void show_os_info() {
    // Display Nezuko ASCII art on the left and system info on the right
    printf("\033[1;35m"); // Set text color to pink
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
    printf("⠀⠀⢸⣿⣄⣲⣃⣡⣤⣤⣤⣀⣀⣐⡂⡇⢠⣼⠸⣿⣿⣿⣿⣿⣿⣿⡆⠀⠀⠀\n");
    printf("⠀⠀⣾⣿⣿⡿⣻⣿⣿⠇⠈⠩⡀⠉⡩⠜⠉⢩⢷⣿⣿⣽⣿⣿⣿⣟⣿⣿⡄⠀\n");
    printf("⠀⣠⣿⣿⣽⣾⣾⡟⠁⢃⠀⠀⠠⠊⠀⢀⠔⡡⢺⣷⣿⣿⣿⣿⣿⣯⣿⠘⠃⠀\n");
    printf("⣾⣿⣿⣿⠏⠀⢁⡤⠶⠚⠠⠀⠀⠀⣀⣪⣤⣤⡙⣶⣿⣿⣮⠟⣿⠋⠈⢠⣠⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠈⠀⠀⠀⠀⠉⠛⠋⠋⠈⠀⠀⠀⠈⠀⠀⠀⠈⠀⠀⠀⠉⠀\n");
    printf("\033[0m"); // Reset text color

    // OS Information
    FILE *fp;
    char buffer[256];

    printf("\n\033[1;36mOS Information:\033[0m\n");
    fp = popen("uname -a", "r");
    if (fp != NULL) {
        fgets(buffer, sizeof(buffer) - 1, fp);
        printf("\033[1;36mOS:\033[0m \033[1;37m%s\033[0m", buffer);
        pclose(fp);
    }

    fp = popen("lscpu | grep 'Model name:'", "r");
    if (fp != NULL) {
        fgets(buffer, sizeof(buffer) - 1, fp);
        printf("\033[1;36mCPU:\033[0m \033[1;37m%s\033[0m", buffer);
        pclose(fp);
    }

    fp = popen("free -h | grep Mem", "r");
    if (fp != NULL) {
        fgets(buffer, sizeof(buffer) - 1, fp);
        printf("\033[1;36mMemory:\033[0m \033[1;37m%s\033[0m", buffer);
        pclose(fp);
    }
}

void print_weather(char *city) {
    char command[256];
    snprintf(command, sizeof(command), "curl -s 'http://wttr.in/%s?format=3'", city);
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("\033[31mError\033[0m: \033[1mCommand Failed\033[0m");
        return;
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer) - 1, fp);
    printf("\033[1;36mWeather in %s:\033[0m \033[1;37m%s\033[0m\n", city, buffer);
    pclose(fp);
}

void print_news() {
    // Note: News API requires a key. Replace 'YOUR_API_KEY' with your actual API key.
    // Example URL: https://newsapi.org/v2/top-headlines?country=us&apiKey=YOUR_API_KEY
    char command[] = "curl -s 'https://newsapi.org/v2/top-headlines?country=us&apiKey=YOUR_API_KEY'";
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("\033[31mError\033[0m: \033[1mCommand Failed\033[0m");
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer) - 1, fp) != NULL) {
        printf("%s", buffer);
    }
    pclose(fp);
}

void print_quote() {
    char *quotes[] = {
        "The only way to do great work is to love what you do. - Steve Jobs",
        "Life is what happens when you're busy making other plans. - John Lennon",
        "The purpose of our lives is to be happy. - Dalai Lama",
        "Get busy living or get busy dying. - Stephen King",
        "You only live once, but if you do it right, once is enough. - Mae West"
    };

    int num_quotes = sizeof(quotes) / sizeof(quotes[0]);
    srand(time(NULL));
    int index = rand() % num_quotes;
    printf("\033[1;36mQuote:\033[0m \033[1;37m%s\033[0m\n", quotes[index]);
}

void play_game() {
    printf("\033[1;35mWelcome to the Number Guessing Game!\033[0m\n");
    printf("Guess a number between 1 and 100.\n");

    int target = rand() % 100 + 1;
    int guess = 0;
    int attempts = 0;

    while (guess != target) {
        printf("Enter your guess: ");
        scanf("%d", &guess);
        attempts++;

        if (guess < target) {
            printf("Too low! Try again.\n");
        } else if (guess > target) {
            printf("Too high! Try again.\n");
        } else {
            printf("Congratulations! You've guessed the number in %d attempts.\n", attempts);
        }
    }
}

void print_systeminfo() {
    char command[] = "uname -a && lscpu && free -h";
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("\033[31mError\033[0m: \033[1mCommand Failed\033[0m");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer) - 1, fp) != NULL) {
        printf("%s", buffer);
    }
    pclose(fp);
}

void list_processes() {
    char command[] = "ps aux";
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("\033[31mError\033[0m: \033[1mCommand Failed\033[0m");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer) - 1, fp) != NULL) {
        printf("%s", buffer);
    }
    pclose(fp);
}

int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARG_SIZE];
    int running = 1;

    while (running) {

        // Print prompt with Nezuko-themed style
        print_nezuko_ascii();
        printf("\n \033[35m🌸 [Linux User] \033[0m"); // Prompt with pink color
        printf("\033[36m$ \033[0m"); // Command input color
        fflush(stdout);

        // Get user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("\033[31mError\033[0m: \033[1mInput Failed\033[0m");
            continue;
        }

        // Remove newline character
        input[strcspn(input, "\n")] = '\0';

        // Exit condition
        if (strcmp(input, "exit") == 0) {
            running = 0;
            continue;
        }

        // Help command
        if (strcmp(input, "help") == 0) {
            print_help();
            continue;
        }

        // Special commands
        if (strncmp(input, "cowsay ", 7) == 0) {
            local_cowsay(input + 7);
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

        if (strncmp(input, "weather ", 8) == 0) {
            print_weather(input + 8);
            continue;
        }

        if (strcmp(input, "news") == 0) {
            print_news();
            continue;
        }

        if (strcmp(input, "quote") == 0) {
            print_quote();
            continue;
        }

        if (strcmp(input, "game") == 0) {
            play_game();
            continue;
        }

        if (strcmp(input, "systeminfo") == 0) {
            print_systeminfo();
            continue;
        }

        if (strcmp(input, "listprocs") == 0) {
            list_processes();
            continue;
        }

        // Parse input and execute command
        parse_input(input, args);
        execute_command(args);
    }

    printf("\033[1;35mGoodbye!\033[0m\n");
    return 0;
}
