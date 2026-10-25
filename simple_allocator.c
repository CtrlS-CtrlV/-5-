#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define POOL_SIZE (4096)
#define ALIGNMENT 8
#define ALIGN(x) (((x) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

typedef struct BlockHeader {
    size_t size;                /* Розмір блоку даних (без заголовка) */
    int    is_free;             /* 1 — вільний, 0 — зайнятий */
    struct BlockHeader *next;   /* Наступний блок у списку */
} BlockHeader;

#define HEADER_SIZE (ALIGN(sizeof(BlockHeader)))

static uint8_t memory_pool[POOL_SIZE];
static BlockHeader *free_list = NULL;
static int initialized = 0;

static void pool_init(void)
{
    free_list = (BlockHeader *)memory_pool;
    free_list->size = POOL_SIZE - HEADER_SIZE;
    free_list->is_free = 1;
    free_list->next = NULL;
    initialized = 1;
}

void *my_malloc(size_t size)
{
    if (!initialized) pool_init();
    if (size == 0) return NULL;

    size = ALIGN(size);

    BlockHeader *curr = free_list;
    while (curr != NULL) {
        if (curr->is_free && curr->size >= size) {
            /* Розподіл блоку (splitting), якщо залишок достатній */
            if (curr->size >= size + HEADER_SIZE + ALIGNMENT) {
                BlockHeader *next_block = (BlockHeader *)((uint8_t *)curr + HEADER_SIZE + size);
                next_block->size = curr->size - size - HEADER_SIZE;
                next_block->is_free = 1;
                next_block->next = curr->next;

                curr->size = size;
                curr->next = next_block;
            }

            curr->is_free = 0;
            return (void *)((uint8_t *)curr + HEADER_SIZE);
        }
        curr = curr->next;
    }

    return NULL; /* Немає вільної пам'яті */
}

void my_free(void *ptr)
{
    if (ptr == NULL) return;

    BlockHeader *block = (BlockHeader *)((uint8_t *)ptr - HEADER_SIZE);
    block->is_free = 1;

    BlockHeader *curr = free_list;
    while (curr != NULL && curr->next != NULL) {
        if (curr->is_free && curr->next->is_free) {
            curr->size += HEADER_SIZE + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}

void pool_dump(void)
{
    printf("=== Стан пулу пам'яті ===\n");
    BlockHeader *curr = free_list;
    int block_num = 0;
    while (curr != NULL) {
        printf("  Блок %d: адреса=%p, розмір=%zu, %s\n",
               block_num,
               (void *)curr,
               curr->size,
               curr->is_free ? "ВІЛЬНИЙ" : "ЗАЙНЯТИЙ");
        curr = curr->next;
        block_num++;
    }
    printf("  Всього блоків: %d\n\n", block_num);
}

int main(void)
{
    printf("=== Демонстрація власного алокатора ===\n");
    printf("Розмір пулу: %d байтів\n", POOL_SIZE);
    printf("Розмір заголовка: %zu байтів\n\n", HEADER_SIZE);

    pool_dump();

    /* Виділення блоків */
    printf("--- Виділення блоків ---\n");
    int *a = my_malloc(10 * sizeof(int));
    printf("a = my_malloc(%zu) → %p\n", 10 * sizeof(int), (void *)a);
    pool_dump();

    char *b = my_malloc(100);
    printf("b = my_malloc(100) → %p\n", (void *)b);

    int *c = my_malloc(200);
    printf("c = my_malloc(200) → %p\n", (void *)c);
    pool_dump();

    /* Використання виділеної пам'яті */
    if (a) {
        for (int i = 0; i < 10; i++) a[i] = i * 100;
        printf("a[5] = %d\n", a[5]);
    }
    if (b) {
        strcpy(b, "Власний алокатор працює!");
        printf("b = \"%s\"\n", b);
    }

    /* Звільнення та злиття */
    printf("\n--- Звільнення блоків ---\n");
    my_free(a);
    printf("my_free(a)\n");
    pool_dump();

    my_free(c);
    printf("my_free(c)\n");
    pool_dump();

    my_free(b);
    printf("my_free(b) — очікується злиття суміжних блоків\n");
    pool_dump();

    /* Повторне виділення для перевірки повторного використання */
    printf("--- Повторне виділення ---\n");
    int *d = my_malloc(500);
    printf("d = my_malloc(500) → %p\n", (void *)d);
    pool_dump();

    my_free(d);

    return EXIT_SUCCESS;
}
