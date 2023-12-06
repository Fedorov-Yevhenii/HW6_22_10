#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "time.h"
#include <unistd.h>

/*
 * Напишіть програму, яка
 * + залежить від значення опції -f (--file)
 * + встановлює (якщо при запуску програми вказано опцію чи ні — якщо не вказано) значення системної змінної FILE_NAME.
 * + Крім того, вона запускає задану кількість (опція -n (--number)) «Дочірніх програм».
 *
 * Створіть «дочірню» програму, яка
 * + перевіряє значення цієї системної змінної.
 * + Якщо змінна задана, то базова частина Ім'я результуючого файлу береться з неї.
 * + Якщо змінна не задана, то як базової частини імені результуючого файлу береться значення за промовчанням.
 * + Крім цього, Ім'я файлу має містити «номер» програми, отриманий з командного рядка.
 * + У файл збережіть задану кількість (номер програми) випадкових чисел в діапазоні [0,1].
*/

double getRandomNumber()
{
    double num = (rand() % 10001)/ 10000.0;
    return num;
}

void childThreadRoutine(int count, const char *file_name) {

    FILE *file = fopen(file_name, "w");
    srand(time(NULL));

    if (file == NULL) {
        perror("Помилка читання файла");
        exit(1);
    }

    for (int i = 0; i < count; i++) { // в залежності від номера програми, кількість згенерованих чисел відрізняється для файла
        fprintf(file, "%f\n", getRandomNumber());
    }

    fclose(file);
}

int main(int argc, char *argv[]) {

    char *file_name = "result.txt";
    int count = 0;

    int opt;
    while ((opt = getopt(argc, argv, "f:n:")) != -1) { // повертає значення опції або -1, якщо всі опції оброблені
        switch (opt) {
            case 'f': // опція -f знаходиться у командному рядку,
                file_name = optarg; // присвоюємо file_name значення аргументу опції
                break;
            case 'n':
                count = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-f file_name] [-n count]\n", argv[0]);
                exit(1);
        }
    }

    if (setenv("FILE_NAME", file_name, 1) != 0) { // Встановлюємо системну змінну FILE_NAME
        perror("Error setting environment variable");
        exit(1);
    }

    for (int i = 1; i <= count; i++) { // Створюємо -n # дочірніх процесів
        pid_t pid = fork();
        if (pid == -1) {
            perror("Помилка при виклику fork");
            exit(1);
        }
        else if (pid == 0) {
            char childFileName[256];
            snprintf(childFileName, sizeof(childFileName), "%s-%d.txt", file_name, i);
            childThreadRoutine(i, childFileName);
            exit(0);
        }
    }

    for (int i = 0; i < count; i++) {
        wait(NULL);
    }

    return 0;
}