#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Написати свою програму, яка
 * + отримує з командного рядка команди
 * + або одну команду,
 * + або одну команду з параметрами,
 * + або кілька команд,
 * + розділений точною з комою (;))
 * + і передає їх для виконання функції system().
 * + Вивести код завершення.
*/

int getExitCode(const char *cmd) {

    int status = system(cmd); // Зчитує cmd як команду та передає оболонці системи, яка виконує її

    if (WIFEXITED(status)) { // Повертає код завершення дочірнього процесу, якщо той завершився успіхом
        printf("Command '%s' exited with status %d\n", cmd, WEXITSTATUS(status));
        return WEXITSTATUS(status);
    }

    else {
        fprintf(stderr, "Command '%s' failed to execute\n", cmd);
        return -1;
    }
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <commands>\n", argv[0]);
        return 1;
    }

    char *commands = argv[1];
    char *token = strtok(commands, ";");

    while (token != NULL) {
        int exit_code = getExitCode(token);
        printf("Exit code: %d\n", exit_code); // Вивести код завершення.

        token = strtok(NULL, ";");
    }

    return 0;
}
