
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int    id;
    char   name[32];
    double score;
} Student;

#define NUM_STUDENTS 5

void print_students(const char *label, const Student *arr, size_t count)
{
    printf("--- %s ---\n", label);
    printf("Адреса масиву: %p\n", (void *)arr);
    for (size_t i = 0; i < count; i++) {
        printf("  [%zu] id=%d, name=\"%s\", score=%.2f\n",
               i, arr[i].id, arr[i].name, arr[i].score);
    }
    printf("\n");
}

int main(void)
{
    printf("=== Порівняння malloc() та calloc() ===\n\n");

    /* Частина 1: malloc */
    Student *arr_malloc = malloc(NUM_STUDENTS * sizeof(Student));
    if (arr_malloc == NULL) {
        perror("Помилка malloc");
        return EXIT_FAILURE;
    }

    /* Вивід пам'яті ДО заповнення */
    print_students("malloc: ДО заповнення (можливе сміття)", arr_malloc, NUM_STUDENTS);

    /* Заповнення даних для malloc */
    for (int i = 0; i < NUM_STUDENTS; i++) {
        arr_malloc[i].id = i + 1;
        snprintf(arr_malloc[i].name, sizeof(arr_malloc[i].name), "Студент_%d", i + 1);
        arr_malloc[i].score = 75.0 + i * 2.5;
    }

    print_students("malloc: ПІСЛЯ заповнення", arr_malloc, NUM_STUDENTS);
    free(arr_malloc);

    Student *arr_calloc = calloc(NUM_STUDENTS, sizeof(Student));
    if (arr_calloc == NULL) {
        perror("Помилка calloc");
        return EXIT_FAILURE;
    }

    print_students("calloc: ДО заповнення (гарантовані нулі)", arr_calloc, NUM_STUDENTS);

    for (int i = 0; i < NUM_STUDENTS; i++) {
        arr_calloc[i].id = i + 1;
        snprintf(arr_calloc[i].name, sizeof(arr_calloc[i].name), "Студент_%d", i + 1);
        arr_calloc[i].score = 75.0 + i * 2.5;
    }

    print_students("calloc: ПІСЛЯ заповнення", arr_calloc, NUM_STUDENTS);
    free(arr_calloc);

    printf("Висновок: malloc() залишає виділену пам'ять неініціалізованою (містить випадкові дані/сміття),\n");
    printf("а calloc() автоматично зануляє (ініціалізує нулями) всю виділену пам'ять.\n");

    return EXIT_SUCCESS;
}
