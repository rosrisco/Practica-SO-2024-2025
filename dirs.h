#ifndef DIRS_H
#define DIRS_H

#define DNULL NULL

//Incluimos librerías:
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Definición de estructuras:
typedef char *tItemD;
typedef struct tNodeD *tPosD;
struct tNodeD {
 tItemD dir;
 tPosD next;
 tPosD prev;
};
typedef tPosD tListD;


//Funciones de la lista:
void createEmptyListD(tListD *D);
void emptyListD(tListD *D);
void deleteDir(tListD *D, tPosD posD);

//printListD: si la lista no está vacía, imprimimos todos sus elementos
void printListD(tListD D);

//Inserta un elemento en la lista y devuelve true (si la inserción fue exitosa) o false (si no se pudo insertar).
bool insertDir (tListD *D, tItemD itemD);

//Devuelven la posición del elemento buscado
tPosD findDir(tListD D, tItemD itemD);

#endif //DIRS_H
