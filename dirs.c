#include "dirs.h"

//Funciones de la lista:
void createEmptyListD(tListD *D) {
    *D = DNULL;
}

void emptyListD(tListD *D) {
    tPosD p = *D; //Declaramos el puntero a la lista
    tPosD next;

    while (p != DNULL) {
        next = p->next; //Guardamos el siguiente nodo
        //Liberamos el nombre del archivo y el nodo
        free(p->dir);
        free(p);
        p = next;
    }

    *D = DNULL;
}

void deleteDir(tListD *D, tPosD posD) {
    if (posD == NULL) { //Si posD es NULL, no hay nada que eliminar
        return;
    }
    if (posD == *D) { //Si eliminamos la primera posición:
        *D = posD->next;
        if (*D != DNULL) { //Si la lista no está vacía, actualizamos la posición:
            (*D)->prev = DNULL;
        }
    } else { //Si eliminamos otra posición que no sea la primera:
        posD->prev->next = posD->next;
        if (posD->next != DNULL) {
            posD->next->prev = posD->prev;
        }
    }
    free(posD->dir);
    free(posD); //Liberar el bloque
}

void printListD(tListD D) {
    if(D != DNULL) {
        for (tPosD posD = D; posD != DNULL; posD = posD->next) { //Recorremos la lista
            printf("%s\n", posD->dir);  //Imprimimos el directorio
        }
    }
}

bool insertDir (tListD *D, tItemD itemD) {
    tPosD q, r;
    //Creamos nodo:
    q = malloc(sizeof(struct tNodeD));
    if (q == DNULL) {
        return false;
    } else { //En caso de poder crear un nodo:
        q->dir = strdup(itemD); //Duplicar el contenido de itemH y lo mete en data
        if (q->dir == NULL) { //Comprobar si strdup falló
            free(q); //Liberar el nodo si strdup falla
            return false;
        }
        q->next = DNULL;

        if (*D == DNULL) { //Si la lista está vacía
            *D = q;
        } else { //Se inserta el nodo al final de la lista:
            for (r = *D; r->next != DNULL; r = r->next){}
            r->next = q;
        }
        return true;
    }
}

tPosD findDir(tListD D, tItemD itemD) {
    tPosD d;
    if (D == DNULL) {
        return DNULL;
    }
    for (d = D; d != DNULL; d = d->next) {  //Recorremos la lista
        if (strcmp(d->dir, itemD) == 0) { //Si el directorio se encuentra dentro de la lista, lo devolvemos
            return d;
        }
    }
    return DNULL;
}
