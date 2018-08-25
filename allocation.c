#include <stdio.h>
#ifndef MEMORY_SIZE
    #define MEMORY_SIZE 4096
#endif
#ifndef uint8_t
    #include <stdint.h>
#endif

// creating in heap memory
static uint8_t memory[MEMORY_SIZE];

typedef struct Pessoa {
    int idade;
    double altura;
} Pessoa_t;

void* memalloc(uint8_t size) {
    register unsigned int i = 0;
    while(i < MEMORY_SIZE) {
        if (memory[i] == 0) {
            if (MEMORY_SIZE - i > size) {
                for (register unsigned int j = i; j < (i + size); j++) {
                    if (memory[j] != 0) goto outer;
                }
                memory[i] = size;
                return &memory[i + 1];
            }
            goto l1;
        } else {
            i += memory[i];
        }
    outer: i = i + 1;
    }
l1: return NULL;
}

void memfree(void* a) {
    uint8_t *size = a - 1;
    uint8_t block_size = (*size / sizeof(uint8_t)) + 1;
    register unsigned int i = 0;
loop:if (i < block_size) {
        *size = 0;
        size = size + 1;
        i++;
        goto loop;
    }
}

void printmem() {
    register unsigned int i = 0;
    for (; i < MEMORY_SIZE; i++) {
        if (memory[i] != 0) {
            printf("Irregularidade no endereco: %p\n", &memory[i]);
            printf("\tValor encontrado: %d\n", memory[i]);
        }
    }
}

int main(int argc, char **argv) {
    register unsigned int i = 0;
    for (; i < MEMORY_SIZE; i++) {
        memory[i] = 0;
    }
    Pessoa_t* a;
    a = (Pessoa_t *) memalloc(sizeof(Pessoa_t));
    a->idade = 10;
    a->altura = 50;
    memfree(a);
    printmem();
    return 0;
}