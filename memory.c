//Incluimos librerías:
#include "memory.h"


char * enumTocharM(enum Type type) {
    switch (type) {
        case MALLOC:
            return "malloc";
        case SHARED:
            return "shared";
        case MMAP:
            return "mmap";
        default: return "error";
    }
}

void createEmptyListM(tListM *M) {
    *M = MNULL;
}

void emptyListM(tListM *M) {
    tPosM posM = *M; //Declaramos el puntero a la lista
    tPosM next;

    while (posM != MNULL) {
        next = posM->next; //Guardamos el siguiente nodo

        //Comprobamos el tipo del bloque para saber como eliminarlo:
        if(posM->block.type == SHARED) {
            int id;
            if (shmdt(posM->block.address)!=-1 && (id=shmget(posM->block.key,0,0666))!=-1) {
                shmctl(id,IPC_RMID, MNULL);
            }
        } else if (posM->block.type == MMAP) {
            if (munmap(posM->block.address, posM->block.size) != -1) {
                free(posM->block.file);
            }
        } else {
            free(posM->block.address);
        }

        //Liberamos el nodo
        free(posM);
        posM = next;
    }
    *M = MNULL;
}

void printListM(tListM M) {
    char fyh[20];
    if(M != MNULL) {
        for (tPosM posM = M; posM != MNULL; posM = posM->next) {
            strftime(fyh, 20, "%Y/%m/%d - %H:%M", posM->block.time); //Formateamos la fecha y la convierte en una cadena legible
            if(posM->block.type == SHARED) {
                printf("%p       %s   %s   (key %d)\n", posM->block.address, fyh, enumTocharM(posM->block.type), posM->block.key);
            } else if (posM->block.type == MMAP) {
                printf("%p       %s   %s   %s (descriptor %d)\n", posM->block.address, fyh, enumTocharM(posM->block.type), posM->block.file, posM->block.df);
            } else {
                printf("%p       %s   %s\n", posM->block.address, fyh, enumTocharM(posM->block.type));
            }
        }
    }
}

void printMalloc(tListM M) {
    char fyh[20];
    if(M != MNULL) {
        for (tPosM posM = M; posM != MNULL; posM = posM->next) {
            if(posM->block.type == MALLOC) {
                strftime(fyh, 20, "%Y/%m/%d - %H:%M", posM->block.time); //Formateamos la fecha y la convierte en una cadena legible
                printf("%p       %s   %s\n", posM->block.address, fyh, enumTocharM(posM->block.type));
            }
        }
    }
}
void printShared(tListM M) {
    char fyh[20];
    if(M != MNULL) {
        for (tPosM posM = M; posM != MNULL; posM = posM->next) {
            if(posM->block.type == SHARED) {
                strftime(fyh, 20, "%Y/%m/%d - %H:%M", posM->block.time); //Formateamos la fecha y la convierte en una cadena legible
                printf("%p       %s   %s   (key %d)\n", posM->block.address, fyh, enumTocharM(posM->block.type), posM->block.key);
            }
        }
    }
}
void printMmap(tListM M) {
    char fyh[20];
    if(M != MNULL)
        for (tPosM posM = M; posM != MNULL; posM = posM->next) {
            if(posM->block.type == MMAP) {
                strftime(fyh, 20, "%Y/%m/%d - %H:%M", posM->block.time); //Formateamos la fecha y la convierte en una cadena legible
                printf("%p       %s   %s   %s (descriptor %d)\n", posM->block.address, fyh, enumTocharM(posM->block.type), posM->block.file, posM->block.df);
            }
        }
    }

void deleteBlockM(tListM *M, tPosM posM) {
    if (posM == NULL) { //Si posF es NULL, no hay nada que eliminar
        return;
    }
    if (posM == *M) { //Si eliminamos la primera posición:
        *M = (*M)->next;
        if (*M != MNULL) { //Si la lista no está vacía, actualizamos la posición:
            (*M)->prev = MNULL;
        }
    } else { // Si eliminamos otra posición que no sea la primera:
        posM->prev->next = posM->next;
        if (posM->next != MNULL) { //Si la lista no se queda vacía, actualizamos la posición:
            posM->next->prev = posM;
        }
    }
    free(posM); //Liberar el bloque
}

bool insertMalloc(tListM *M, tPosM posM, void* address, size_t n) {
    tPosM q, r;
    tItemM itemM;

    time_t fyh; //Declaramos fyh como una variable de tiempo en segundos.

    //Esta estructura guarda la fecha (días, meses, años) y la hora (horas, minutos, segundos) desglosada cada una de sus respecivas partes.
    struct tm * tm_info;

    //Obtiene la hora del sistema en segundos y almacena en fyh
    time(&fyh);
    tm_info = localtime(&fyh); //localtime() convierte la información de fyh y la almacena en el struct tm, al cual apuntará tm_info

    //Inicializamos los valores del bloque:
    itemM.address = address;
    itemM.size = n;
    itemM.type = MALLOC;
    itemM.time = tm_info;

    //Creamos un nodo:
    q = malloc(sizeof(struct tNodeM));
    if (q == MNULL) {
        free(itemM.address);
        free(q);
        return false;
    }

    //Inicializamos los valores de la posición:
    q->block = itemM;
    q->next = MNULL;
    q->prev = MNULL;

    if (*M == MNULL) { //Si la lista está vacía, q es la primera posición.
        *M = q;
    } else if (posM == MNULL) { //Insertamos el nodo al final de la lista:
        for (r = *M; r->next != MNULL; r = r->next);
        r->next = q;
        q->prev = r;
    } else if (posM == *M) { //Insertamos el nodo al principio de la lista:
        q->next = posM;
        posM->prev = q;
        *M = q;
    } else { //Insertamos el nodo en una posición intermedia:
        q->next = posM;
        q->prev = posM->prev;

        if (posM->prev != MNULL) {
            posM->prev->next = q;
        }

        posM->prev = q;
    }
    return true;
}
bool insertShared(tListM *M, tPosM posM, void* address, size_t n, int cl) {
    tPosM q, r;
    tItemM itemM;

    time_t fyh; //Declaramos fyh como una variable de tiempo en segundos.

    //Esta estructura guarda la fecha (días, meses, años) y la hora (horas, minutos, segundos) desglosada cada una de sus respecivas partes.
    struct tm * tm_info;

    //Obtiene la hora del sistema en segundos y almacena en fyh
    time(&fyh);
    tm_info = localtime(&fyh); //localtime() convierte la información de fyh y la almacena en el struct tm, al cual apuntará tm_info

    //Inicializamos los valores del bloque:
    itemM.address = address;
    itemM.size = n;
    itemM.type = SHARED;
    itemM.time = tm_info;
    itemM.key = cl;

    //Creamos un nodo:
    q = malloc(sizeof(struct tNodeM));
    if (q == MNULL) {
        free(itemM.address);
        return false;
    }

    //Inicializamos los valores de la posición:
    q->block = itemM;
    q->next = MNULL;
    q->prev = MNULL;

    if (*M == MNULL) { //Si la lista está vacía, q es la primera posición.
        *M = q;
    } else if (posM == MNULL) { //Insertamos el nodo al final de la lista:
        for (r = *M; r->next != MNULL; r = r->next);
        r->next = q;
        q->prev = r;
    } else if (posM == *M) { //Insertamos el nodo al principio de la lista:
        q->next = posM;
        posM->prev = q;
        *M = q;
    } else { //Insertamos el nodo en una posición intermedia:
        q->next = posM;
        q->prev = posM->prev;

        if (posM->prev != MNULL) {
            posM->prev->next = q;
        }

        posM->prev = q;
    }
    return true;
}
bool insertMmap(tListM *M, tPosM posM, void* address, size_t n, int df, char* file) {
    tPosM q, r;
    tItemM itemM;

    time_t fyh; //Declaramos fyh como una variable de tiempo en segundos.

    //Esta estructura guarda la fecha (días, meses, años) y la hora (horas, minutos, segundos) desglosada cada una de sus respecivas partes.
    struct tm * tm_info;

    //Obtiene la hora del sistema en segundos y almacena en fyh
    time(&fyh);
    tm_info = localtime(&fyh); //localtime() convierte la información de fyh y la almacena en el struct tm, al cual apuntará tm_info

    //Inicializamos los valores del bloque:
    itemM.address = address;
    itemM.size = n;
    itemM.type = MMAP;
    itemM.time = tm_info;
    itemM.df = df;
    itemM.file = strdup(file);

    //Creamos un nodo:
    q = malloc(sizeof(struct tNodeM));
    if (q == MNULL) {
        free(itemM.address);
        return false;
    }

    //Inicializamos los valores de la posición:
    q->block = itemM;
    q->next = MNULL;
    q->prev = MNULL;

    if (*M == MNULL) { // Si la lista está vacía, q es la primera posición.
        *M = q;
    } else if (posM == MNULL) { //Insertamos el nodo al final de la lista:
        for (r = *M; r->next != MNULL; r = r->next);
        r->next = q;
        q->prev = r;
    } else if (posM == *M) { //Insertamos el nodo al principio de la lista:
        q->next = posM;
        posM->prev = q;
        *M = q;
    } else { //Insertamos el nodo en una posición intermedia:
        q->next = posM;
        q->prev = posM->prev;

        if (posM->prev != MNULL) {
            posM->prev->next = q;
        }

        posM->prev = q;
    }
    return true;
}

tItemM getBlockM (tPosM posM, tListM M) {
    return posM->block;
}

tPosM lastM (tListM M) {
    tPosM m;
    if(M == MNULL) {
        return MNULL;
    }
    for(m = M; m->next != MNULL; m = m->next);
    return m;
}

tPosM findBlock (void* address, tListM M) {
    tPosM m;
    if (M == MNULL) {
        return MNULL;
    }
    for (m = M; m != MNULL; m = m->next) {
        if (m->block.address == address) {
            return m;
        }
    }
    return MNULL;
}
tPosM findMallocBlock (size_t n, tListM M) {
    tPosM m;
    if (M == MNULL) {
        return MNULL;
    }
    for (m = M; m != MNULL; m = m->next) {
        if (m->block.type == MALLOC && m->block.size == n) {
            return m;
        }
    }
    return MNULL;
}
tPosM findSharedBlock (key_t key, tListM M) {
    tPosM m;
    if (M == MNULL) {
        return MNULL;
    }
    for (m = M; m != MNULL; m = m->next) {
        if (m->block.type == SHARED && m->block.key == key) {
            return m;
        }
    }
    return MNULL;
}

tPosM findMmapBlock (char* file, tListM M) {
    tPosM m;
    if (M == MNULL) {
        return MNULL;
    }
    for (m = M; m != MNULL; m = m->next) {
        if (m->block.type == MMAP && strcmp(m->block.file, file) == 0) {
            return m;
        }
    }
    return MNULL;
}
