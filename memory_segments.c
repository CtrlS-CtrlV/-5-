#include <stdio.h>
#include <stdlib.h>

int global_init = 42;         /* Сегмент DATA */
int global_uninit;            /* Сегмент BSS  */

void show_stack_direction(int depth)
{
    int local_var;
    printf("   Рівень рекурсії %d: &local_var = %p\n", depth, (void *)&local_var);
    if (depth < 3) {
        show_stack_direction(depth + 1);
    }
}

int main(void)
{
    /* Локальні змінні (стек) */
    int local_a = 10;
    int local_b = 20;

    /* Статична змінна */
    static int static_var = 100;

    /* Динамічна пам'ять (купа) */
    int *heap_1 = malloc(sizeof(int));
    int *heap_2 = malloc(sizeof(int));
    int *heap_3 = malloc(1024 * sizeof(int));

    printf("=== Розміщення пам'яті процесу ===\n\n");

    printf("TEXT  (код):    main         = %p\n", (void *)main);
    printf("DATA  (ініц.):  &global_init = %p\n", (void *)&global_init);
    printf("DATA  (ініц.):  &static_var  = %p\n", (void *)&static_var);
    printf("BSS   (неініц.):&global_uninit= %p\n", (void *)&global_uninit);
    printf("HEAP  (купа):   heap_1       = %p\n", (void *)heap_1);
    printf("HEAP  (купа):   heap_2       = %p\n", (void *)heap_2);
    printf("HEAP  (купа):   heap_3       = %p\n", (void *)heap_3);
    printf("STACK (стек):   &local_a     = %p\n", (void *)&local_a);
    printf("STACK (стек):   &local_b     = %p\n", (void *)&local_b);

    printf("\n--- Напрямок росту стеку ---\n");
    show_stack_direction(0);

    printf("\n--- Напрямок росту купи ---\n");
    printf("heap_1 = %p\n", (void *)heap_1);
    printf("heap_2 = %p\n", (void *)heap_2);
    printf("heap_3 = %p\n", (void *)heap_3);

    /* Звільнення динамічної пам'яті */
    free(heap_1);
    free(heap_2);
    free(heap_3);

    return EXIT_SUCCESS;
}
