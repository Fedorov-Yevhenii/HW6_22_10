#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * Напишіть версію спрощеного командного процесора на С.
 * + виводиться повідомлення про те, хто працює з командним процесором
 * + на екран виводиться запрошення виду [user]$
 * + якщо команду не виконано, то на екран виводиться повідомлення
 * + інакше результат виконаної команди
 * + Для завершення роботи програми призначена команда stop (на екран виводиться повідомлення Wait 3 seconds..., програма чекає 3 секунди, потім очищає екран та завершує свою роботу)
 * + (-h) і (--help) - виклик довідки, без аргумента
 * + (-b #) та (--buffer #) - визначення # розміром символьного буфера, + за замовченням 127
*/

#define DEFAULT_BUFFER_SIZE 127
#define TIME_TO_SLEEP 3

int main(int argc, char *argv[]) {

    char username[256];
    char command[DEFAULT_BUFFER_SIZE + 1];
    int buffer_size = DEFAULT_BUFFER_SIZE;

    if (getlogin_r(username, sizeof(username)) != 0) {
        perror("Помилка під час визначення юзернейму");
        return 0;
    }

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) { // Введена команда виклику довідки
            printf("ДОВІДКОВА ІНФОРМАЦІЯ:\n");
            printf("  -h, --help | Виведення довідкової інформації\n");
            printf("  -b, --buffer SIZE | Визначення розміру символьного буферу (За замовчуванням: 127)\n");
            return 0;
        }
        else if ((strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--buffer") == 0)) { // Введена команда визначення довжини буфера
            if(i + 1 < argc) // Якщо вказано другий параметр,
            buffer_size = atoi(argv[++i]); // Оновлюємо розмір буфера символів
            // Інакше - лишаємо за замовчуванням
        }
        else {
            fprintf(stderr, "Невизначена команда: %s\n", argv[i]);
            return 0;
        }
    }

    printf("Наразі %s працює з командним процесором (розмір символьного буфера: %d)\n", username, buffer_size); // Повідомлення про те, хто працює з командним процесором
    printf("Очікую на команду, [%s]$", username);

    while (1) {
        if (fgets(command, sizeof(command), stdin) == NULL) { // 0 символів зчитано перш ніж введена команда скінчилась
            perror("Некоректна команда");
            return 0;
        }

        size_t command_length = strlen(command);
        if (command_length > 0 && command[command_length - 1] == '\n') {
            command[command_length - 1] = '\0';
        }

        if (strcmp(command, "stop") == 0) { // Обробка команди stop
            printf("Wait %d seconds...\n", TIME_TO_SLEEP);
            sleep(TIME_TO_SLEEP);
            system("clear");
            break;
        }

        int result = system(command); // Виконуємо команду, що не є командою завершення програми або будь-якою некоректною => команду процесора
        if (result == -1) {
            perror("Помилка виконання команди");
        } else {
            printf("Команду виконано з кодом: %d\n", result);
        }

        printf("Очікую на команду: [%s]$", username);
    }

    return 1;
}
