//Incluimos librerías:
#include "openfiles.h"

//mtos: convierte el modo en un string para poder imprimirlo.
char * mtos(int mode) {
    switch (mode) {
        case O_CREAT: return "create";
        case O_APPEND: return "append";
        case O_EXCL: return "execute";
        case O_RDONLY: return "read-only";
        case O_WRONLY: return "write-only";
        case O_RDWR: return "read-write";
        case O_TRUNC: return "truncate";
        default: return "desconocido";
    }
}

void createEmptyListF(tListF *F) {
    *F = FNULL;
}

void emptyListF(tListF *F) {
    tPosF p = *F; //Declaramos el puntero a la lista
    tPosF next;

    while (p != FNULL) {
        next = p->next; //Guardamos el siguiente nodo
        //Liberamos el nombre del archivo y el nodo
        free(p->file.name);
        free(p);
        p = next;
    }

    *F = FNULL;
}

void deleteAtPositionF(tListF *F, tPosF posF) {
    if (posF == FNULL) { //Si posF es NULL, no hay nada que eliminar
        return;
    }
    if (posF == *F) { //Si eliminamos la primera posición:
        *F = (*F)->next;
        if (*F != FNULL) { //Si la lista no está vacía, actualizamos la posición:
            (*F)->prev = FNULL;
        }
    } else { //Si eliminamos otra posición que no sea la primera:
        posF->prev->next = posF->next;
        if (posF->next != FNULL) { //Si la lista no se queda vacía, actualizamos la posición:
            posF->next->prev = posF->prev;
        }
    }
    free(posF->file.name); //Liberar el nombre del archivo
    free(posF); //Liberar el nodo
}

bool insertItemF(tListF *F, tPosF posF, char *name, int mode, int df) {
    tPosF q, r;
    tItemF itemF;

    //Inicializamos los valores del item:
    itemF.name = strdup(name); //Asignamos el nombre aquí
    itemF.mode = mode;
    itemF.df = df;

    //Creamos un nodo:
    q = malloc(sizeof(struct tNodeF));
    if (q == FNULL) {
        free(itemF.name); //Liberar el nombre si la asignación falla
        return false;
    }

    //Inicializamos los valores de la posición:
    q->file = itemF;
    q->next = FNULL;
    q->prev = FNULL;

    if (*F == FNULL) { //Si la lista está vacía, q es la primera posición.
        *F = q;
    } else if (posF == FNULL) { //Insertamos el nodo al final de la lista:
        for (r = *F; r->next != FNULL; r = r->next);
        r->next = q;
        q->prev = r;
    } else if (posF == *F) { //Insertamos el nodo al principio de la lista:
        q->next = posF;
        posF->prev = q;
        *F = q;
    } else { //Insertamos el nodo en una posición intermedia:
        q->next = posF;
        q->prev = posF->prev;

        if (posF->prev != FNULL) {
            posF->prev->next = q;
        }

        posF->prev = q;
    }
    return true;
}

void printListF(tListF F) {
    //Si la lista no está vacía, la recorremos entera e imprimimos todos sus elementos:
    if(F != FNULL) {
        for (tPosF posF = F; posF != FNULL; posF = posF->next) {
            char *mode = mtos(posF->file.mode);
            printf("%i  %s  %s\n", posF->file.df, posF->file.name, mode);
        }
    }
}

tPosF findItemF(int df, tListF F) {
    tPosF p;
    if(F == FNULL) { //Si la lista está vacía:
        return FNULL;
    } else {
        //Recorre toda la lista y si encuentra el item con el mismo descriptor devuelve la posicion.
        for(p = F; p != FNULL; p = p -> next) {
            if(df == p->file.df) {
                return p;
            }
        }
    }
    return FNULL;
}
