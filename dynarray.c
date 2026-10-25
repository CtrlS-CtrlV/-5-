
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int     *data;
    size_t  size;
    size_t  capacity;
} DynArray;

int dyn_array_init(DynArray *arr, size_t initial_capacity)
{
    arr->data = malloc(initial_capacity * sizeof(int));
    if (arr->data == NULL) {
        return -1;
    }
    arr->size = 0;
    arr->capacity = initial_capacity;
    return 0;
}

int dyn_array_push(DynArray *arr, int value)
{
    if (arr->size == arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        
        /* Правильний патерн realloc з використанням тимчасової змінної */
        int *tmp = realloc(arr->data, new_capacity * sizeof(int));
        if (tmp == NULL) {
            return -1; /* Оригінальний вказівник arr->data не втрачено! */
        }
        
        arr->data = tmp;
        printf("[realloc] Ємність збільшена: %zu -> %zu\n", arr->capacity, new_capacity);
        arr->capacity = new_capacity;
    }

    arr->data[arr->size] = value;
    arr->size++;
    return 0;
}

void dyn_array_print(const DynArray *arr)
{
    printf("DynArray [size=%zu, capacity=%zu]: ", arr->size, arr->capacity);
    for (size_t i = 0; i < arr->size; i++) {
        printf("%d ", arr->data[i]);
    }
    printf("\n");
}

void dyn_array_free(DynArray *arr)
{
    if (arr->data != NULL) {
        free(arr->data);
        arr->data = NULL;
    }
    arr->size = 0;
    arr->capacity = 0;
}

int main(void)
{
    DynArray arr;

    if (dyn_array_init(&arr, 4) != 0) {
        fprintf(stderr, "Помилка ініціалізації\n");
        return EXIT_FAILURE;
    }

    printf("=== Динамічний масив із realloc() ===\n\n");

    /* Додати 20 елементів (спровокувати кілька realloc) */
    for (int i = 1; i <= 20; i++) {
        if (dyn_array_push(&arr, i * 10) != 0) {
            fprintf(stderr, "Помилка додавання елемента %d\n", i);
            dyn_array_free(&arr);
            return EXIT_FAILURE;
        }
    }

    printf("\n");
    dyn_array_print(&arr);
    dyn_array_free(&arr);

    return EXIT_SUCCESS;
}
