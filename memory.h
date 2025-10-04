#ifndef MEMORY_H
#define MEMORY_H

#define MNULL NULL

//Incluimos librería:
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/shm.h>

//Definición de estructuras:
typedef struct Block {
    void *address;
    size_t size;
    enum Type {MALLOC, SHARED, MMAP} type;
    struct tm * time;
    key_t key;
    char *file;
    int df;
}tBlock;

typedef tBlock tItemM;
typedef struct tNodeM *tPosM;

struct tNodeM {
    tItemM block;
    tPosM next;
    tPosM prev;
};

typedef tPosM tListM;

//Funciones de la lista:
void createEmptyListM(tListM *M);
void emptyListM(tListM *M);
void deleteBlockM(tListM *M, tPosM posM);

//printListM: si la lista no está vacía, imprimimos todos sus elementos
void printListM(tListM M);
//printMalloc: si la lista no está vacía, imprimimos todos sus elementos tipo malloc
void printMalloc(tListM M);
//printShared: si la lista no está vacía, imprimimos todos sus elementos tipo shared
void printShared(tListM M);
//printMmap: si la lista no está vacía, imprimimos todos sus elementos tipo mmap
void printMmap(tListM M);

//Inserta un elemento en la lista y devuelve true (si la inserción fue exitosa) o false (si no se pudo insertar).
bool insertMalloc(tListM *M, tPosM posM, void* address, size_t n);
bool insertShared(tListM *M, tPosM posM, void* address, size_t n, int key);
bool insertMmap(tListM *M, tPosM posM, void* address, size_t n, int df, char* file);

//getBlock: devuelve el bloque de la posición dada
tItemM getBlockM (tPosM posM, tListM M);

//lastM: devuelve la posición del último elemento
tPosM lastM (tListM M);

//Devuelven la posición del elemento buscado de diferente forma según su tipo
tPosM findBlock (void* address, tListM M);
tPosM findMallocBlock(size_t n, tListM M);
tPosM findSharedBlock (key_t key, tListM M);
tPosM findMmapBlock (char* file, tListM M);

#endif //MEMORY_H
