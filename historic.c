//Incluimos librerías:
#include "historic.h"


void createEmptyListH(tListH *H) {
    *H = HNULL;
}

void emptyListH(tListH *H) {
    tPosH p = *H; // Usamos el puntero a la lista
    tPosH next;

    while (p != NULL) {
        next = p->next; // Guardar el siguiente nodo
        free(p->data); // Liberar la cadena del historial
        free(p); // Liberar el nodo actual
        p = next; // Mover al siguiente nodo
    }

    *H = NULL; // Reiniciar el puntero a la lista
}

void deleteAtPositionH(tListH *H, tPosH posH) {
    tPosH p;

    if (posH == *H) { //Si se elimina la primera posición:
        *H = (*H)->next;
    } else {
        //Buscamos el nodo anterior para actualizar su siguiente
        for (p = *H; p->next != posH && p->next != HNULL; p = p->next);

        if (posH->next == HNULL) { //Si se libera la última posición:
            p->next = HNULL;
        } else { //Si eliminamos una posición intermedia:
            p->next = posH->next;
        }
    }
    //Liberamos el comando y el nodo:
    free(posH->data);
    free(posH);
}

bool insertItemH(tListH *H, tPosH posH, tItemH itemH) {
    tPosH q, r;
    //Creamos nodo:
    q = malloc(sizeof(struct tNodeH));
    if (q == HNULL) {
        return false;
    } else { //En caso de poder crear un nodo:
        q->data = strdup(itemH); //Duplicar el contenido de itemH y lo mete en data
        if (q->data == NULL) { //Comprobar si strdup falló
            free(q); //Liberar el nodo si strdup falla
            return false;
        }
        q->next = HNULL;

        if (*H == HNULL) { //Si la lista está vacía
            *H = q;
        } else if (posH == HNULL) { //Se inserta el nodo al final de la lista:
            for (r = *H; r->next != HNULL; r = r->next);
            r->next = q;
        } else if (posH == *H) { //Se inserta el nodo al principio de la lista:
            q->next = posH;
            *H = q;
        }
        return true;
    }
}

tPosH findItemH(tItemH itemH, tListH H) {
    tPosH p;
    if (H == HNULL) {
        return HNULL;
    } else {
        //Recorremos la lista hasta que encontremos un item igual al que entra por la función.
        for (p = H; (p != HNULL) && (strcmp(p->data, itemH) != 0); p = p->next); //Cambiar a strcmp para comparar cadenas
        return p;
    }
}

tItemH getItemH(tPosH posH, tListH H) {
    return posH -> data;
}
