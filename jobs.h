#ifndef JOBS_H
#define JOBS_H

#define JNULL NULL

//Incluimos las librerias:
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>
#include <time.h>

#include <sys/resource.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>


//Definición de estructuras:
typedef struct Job {
 int pid;
 struct tm * time;
 enum Status {FINISHED, STOPPED, SIGNALED, ACTIVE} status;
 char *command_line;
 char username[20];
 int exit;
} tJob;

typedef tJob tItemJ;
typedef struct tNodeJ *tPosJ;

struct tNodeJ {
 tItemJ job;
 tPosJ next;
 tPosJ prev;
};

typedef tPosJ tListJ;

struct SEN {
 char *nombre;
 int senal;
};

//Funciones de la lista:
void createEmptyListJ (tListJ *J);
void emptyListJ (tListJ *J);
void printListJ (tListJ J);
void deleteJob (tListJ *J, enum Status status);

//Inserta un elemento en la lista y devuelve true (si la inserción fue exitosa) o false (si no se pudo insertar)
bool insertJob (tListJ *J, int pid, char *command_line);

//Busca el item en la lista y devuelve su posición
tPosJ findJob(tListJ J, int pid);

//Actualiza el estado de la lista de procesos y devuelve true (si la actualzación fue exitosa) o false (si no se completa la actualización)
bool updateJob(tListJ *J);

#endif //JOBS_H
