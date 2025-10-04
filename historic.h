#ifndef HISTORIC_H

#define HISTORIC_H
#define HNULL NULL

//Incluimos librería:
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Definicion de estructuras
typedef char *tItemH;
typedef struct tNodeH *tPosH;
struct tNodeH {
    tItemH data;
    tPosH next;
};
typedef tPosH tListH;

//Funciones de la lista:
void createEmptyListH(tListH *H);
void emptyListH(tListH *H);
void deleteAtPositionH(tListH *H, tPosH posH);

//Inserta un elemento en la lista y devuelve true (si la inserción fue exitosa) o false (si no se pudo insertar)
bool insertItemH(tListH *H, tPosH posH, tItemH itemH);

//Busca el item en la lista y devuelve su posición
tPosH findItemH(tItemH itemH, tListH H);

//Devulve el item dada su posición
tItemH getItemH(tPosH posH, tListH H);

#endif //HISTORIC_H
