#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 * Напишіть програму, яка
 * + реєструє вказане при старті програми командному рядку кількість функцій - обробників завершення процесу.
 * + Обробник має затримувати роботу на 1 сек.
 * + та виводити повідомлення з номером.
*/

#define TIME_TO_SLEEP 1

void exitHandler(int status) { // Оброблює завершення процесу
    sleep(TIME_TO_SLEEP); // Затримка на секунду
    printf("Exit handler: Process exited with status %d\n", status);
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_handlers>\n", argv[0]);
        return 1;
    }

    int num_handlers = atoi(argv[1]);  // Кількість обробників, що передається при запуску програми (./my_program num_handlers)

    for (int i = 0; i < num_handlers; ++i) {
        if (on_exit((void (*)(int, void *)) exitHandler, NULL) != 0) { // Еквівалент функції atexit()
            fprintf(stderr, "Error registering exit handler\n");
            return 1;
        }
    }
    printf("Registered %d exit handlers\n", num_handlers); // Реєстреація обробників

    pid_t child_pid = fork();
    if (child_pid == -1) { // Помилка виклику fork()
        perror("fork");
        return 1;
    }
    else if (child_pid == 0) { // Виконання коду в дочірньому процесі
        sleep(TIME_TO_SLEEP);
        return 111;
    }
    else { // У батьківському процесі, якщо повертає PID молодшого
        int status;
        waitpid(child_pid, &status, 0); // Очікує завершення дочірнього процесу
        if (WIFEXITED(status)) { // Якщо успішно завершений дочірній процес
            printf("Parent process: Child exited with status %d\n", WEXITSTATUS(status)); // Повідомлення з номером
        }
        else {
            fprintf(stderr, "Parent process: Child process did not exit normally\n");
        }
    }

    return 0;
}

