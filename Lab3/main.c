#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#define IN 1024
#define ARGUMENTS 64
#define MAX_BG_JOBS 128

struct {
    pid_t bg_pids[MAX_BG_JOBS];
    int bg_count;
} shell_state = { .bg_count = 0 };

void sw(const char *str) {
    size_t len = 0;
    while (str[len]) len++;
    write(STDOUT_FILENO, str, len);
}

void int_(int num) {
    char buffer[16];
    int i = 0;
    if (num == 0) {
        sw("0");
        return;
    }
    while (num > 0 && i < 15) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }
    while (i > 0) {
        char c = buffer[--i];
        write(STDOUT_FILENO, &c, 1);
    }
}

void child_handler(int sig) {
    int saved_errno = errno;
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        sw("\nPID: ");
        int_(pid);
        if (WIFEXITED(status)) {
            sw(" code: ");
            int_(WEXITSTATUS(status));
            sw("\n>>>>> ");
        }
        for (int i = 0; i < shell_state.bg_count; i++) {
            if (shell_state.bg_pids[i] == pid) {
                shell_state.bg_pids[i] = 0;
                break;
            }
        }
    }
    errno = saved_errno;
}

void add_bg_job(pid_t pid) {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    if (shell_state.bg_count < MAX_BG_JOBS) {
        shell_state.bg_pids[shell_state.bg_count++] = pid;
    } else {
        sw("Warning: Limit.\n");
    }
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
}

void cmd_help(char **args) {
    puts("Zaimplementowane komendy:");
    puts("  Help                  - wyswietla ta liste");
    puts("  Pwd                   - wyswietla biezacy katalog roboczy");
    puts("  Run program <params>  - uruchamia program i czeka na jego zakonczenie");
    puts("  Bg program <params>   - uruchamia program w tle");
    puts("  Exit                  - zamyka interpreter i czysci procesy potomne");
}

void PWD(char **args) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd))) {
        puts(cwd);
    } else perror("Pwd failed");
}

void EXIT(char **args) {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    for (int i = 0; i < shell_state.bg_count; i++) {
        if (shell_state.bg_pids[i] > 0) {
            kill(shell_state.bg_pids[i], SIGTERM);
            waitpid(shell_state.bg_pids[i], NULL, 0);
        }
    }
    exit(EXIT_SUCCESS);
}

void external(char **args, int is_background) {
    if (!args[1]) return;
    pid_t pid = fork();
    if (pid < 0) return;
    if (pid == 0) {
        execvp(args[1], &args[1]);
        perror("Error execvp");
        _exit(EXIT_FAILURE);
    }
    if (!is_background) {
        int status;
        if (waitpid(pid, &status, 0) != -1) {
            if (WIFEXITED(status)) {
                printf("Main process end %d code: %d\n", pid, WEXITSTATUS(status));
            }
        } else perror("Error waitpid");
    } else {
        printf("Background process PID: %d\n", pid);
        add_bg_job(pid);
    }
}

void run(char **args) { external(args, 0); }
void bg(char **args)  { external(args, 1); }

typedef void (*cmd_handler_t)(char **);
struct {
    const char *name;
    cmd_handler_t handler;
} builtins[] = {
    {"help", cmd_help},
    {"pwd",  PWD},
    {"exit", EXIT},
    {"run",  run},
    {"bg",   bg},
};

int Parser(char *line, char **args) {
    int count = 0;
    char *saveptr;
    char *token = strtok_r(line, " \t\n\r", &saveptr);
    while (token && count < ARGUMENTS - 1) {
        args[count++] = token;
        token = strtok_r(NULL, " \t\n\r", &saveptr);
    }
    args[count] = NULL;
    return count;
}

int main(void) {
    char input[IN];
    char *args[ARGUMENTS];
    struct sigaction sa = {0};
    sa.sa_handler = child_handler;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);

    while (1) {
        printf(">>>>> ");
        fflush(stdout);
        if (!fgets(input, sizeof(input), stdin)) EXIT(NULL);
        if (Parser(input, args) == 0) continue;

        int found = 0;
        size_t num_builtins = sizeof(builtins) / sizeof(builtins[0]);
        for (size_t i = 0; i < num_builtins; i++) {
            if (i == 0) {
                size_t len = strlen(args[0]);
                for (size_t j = 0; j < len; j++) args[i][j] = tolower(args[i][j]);
            }
            if (strcmp(args[0], builtins[i].name) == 0) {
                builtins[i].handler(args);
                found = 1;
                break;
            }
        }
        if (!found) fprintf(stderr, "Unknown: %s\n", args[0]);
    }
    return EXIT_SUCCESS;
}