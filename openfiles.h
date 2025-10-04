#ifndef OPENFILES_H

#define OPENFILES_H
#define FNULL NULL


//Incluimos librería:
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

//Definición de estructuras:
typedef struct File {
 char *name;
 int mode;
 int df;
}tFile;

typedef tFile tItemF;
typedef struct tNodeF *tPosF;
struct tNodeF {
 tItemF file;
 tPosF next;
 tPosF prev;
};
typedef tPosF tListF;

char * mtos(int mode);

//Funciones de la lista:
void createEmptyListF(tListF *F);
void emptyListF(tListF *F);
void deleteAtPositionF(tListF *F, tPosF posF);
void printListF(tListF F);

//Inserta un elemento en la lista y devuelve true (si la inserción fue exitosa) o false (si no se pudo insertar).
bool insertItemF(tListF *F, tPosF posF, char * name, int mode, int df);

//Busca el item en la lista y devuelve su posición.
tPosF findItemF(int df, tListF F);

#endif //OPENFILES_H
