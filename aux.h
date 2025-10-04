#ifndef AUX_H
#define AUX_H

#define MAXNAME 1024
#define TAMANO 2048

#include "historic.h"
#include "openfiles.h"
#include "memory.h"
#include "cmds.h"
#include "dirs.h"
#include "jobs.h"

#include <sys/stat.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>


//trocearCadena: divide la cadena en trozos
int trocearCadena(char * cadena, char * trozos[]);

//historic_aux: funcion auxiliar de historic para volver a llamar a los comandos
void historic_aux(char *comando, tListH *H, tListF *F, tListM *M, tListJ *J, tListD *D, char *envp[]);

//imprimirRuta: imprime la ruta del dir actual y comprueba si tr[1] es nulo(true si no es nulo y no existe ruta válida, false si es nulo)
bool imprimirRuta(char *tr[], char ruta[MAXNAME]);

//delHistoric_aux: funcion auxiliar para eliminar del historial
void delHistoric_aux(tListH *H);

//ConvierteModo: devuelve los permisos de un fichero en formato rwx----
char * convierteModo (mode_t m, char *permisos);

//reclist_aux: recorre los directorios recursivamente e imprime sus contenidos
void reclist_aux(char *rutaabs, int hid, int acc, int l_long, int link);

//revlist_aux: recorre los directorios recursivamente e imprime sus contenidos
void revlist_aux(char *rutaabs, int hid, int acc, int l_long, int link);

//delrec_aux: recorre recursivamente los directorios y los elimina
void delrec_aux(char *path);

//recursiva: esta funcion tiene un array automatico y un array estatico ambos de tamano 2048 e imprime las direcciones de ambos arrays
void recursiva (int n);

//llenarMemoria: llena la memoria con cont bytes a partir de la dirección con el caracter dado
void llenarMemoria (void *p, size_t cont, unsigned char byte);

//leerFichero: lee el fichero en la direccion p con cont bytes y lanza un error si no se ha podido leer
ssize_t leerFichero (char *f, void *p, size_t cont);

//escribirFichero: escribe el fichero en la direccion p con cont bytes y lanza un error si no se ha podido escribir
ssize_t escribirFichero (char *f, void *p, size_t cont);

//do_AllocateMalloc: asigna un bloque malloc del tamano dado
void do_AllocateMalloc(char *tr[], tListM *M);

//do_AllocaateCreateshared: asigna (creando) un bloque de memoria compartida con clave y tamano dados
void do_AllocateCreateshared (char *tr[],tListM *M);

//do_AllocateShared: asigna un bloque de memoria compartida (ya existente) con clave dada
void do_AllocateShared (char *tr[], tListM *M);

//do_AllocateMmap: mapea el fichero con los permisos dados
void do_AllocateMmap(char *tr[], tListM *M, tListF *F);

//do_DeallocateMalloc: desasigna un bloque de memoria de tipo malloc
void do_DeallocateMalloc (char *tr[], tListM *M);

//do_DeallocateShared: desasigna un bloque de memoria de tipo shared
void do_DeallocateShared (char *tr[], tListM *M);

//do_DeallocateMmap: desasigna un bloque de memoria de tipo mmap
void do_DeallocateMmap (char *tr[], tListM *M, tListF F);

//do_DeallocateDelkey: elimina del sistema (sin desmapear) la clave de memoria
void do_DeallocateDelkey (char *args[]);

//do_pmap: muestra la salida del comando pmap (un mapa
void do_pmap(void);

//cadtop: convierte la cadena a una dirección de memoria
void *cadtop(char *tr);

//buscarVariable: busca una variable en el entorno que se le pasa como parámetro
int buscarVariable (char * var, char *e[]);

//cambiarVariable: cambia una variable en el entorno que se le pasa como parámetro
int cambiarVariable(char * var, char * valor, char *e[]);

//Ejecutable: busca un comando en la lista de directorios, si no es valido devuelve el comando
char *Ejecutable (tListD D, char *s);

//isVar: comprueba si la variable var se encuentra en el entorno
bool isVar (char *var);
#endif //AUX_H

/*
(tipo)strtoul(char1, char, base): convierte char1 en la base pedida y lo pasa al tipo especificado antes

struct shmid_ds {
    struct ipc_perm shm_perm;  //Permisos de acceso y metadatos de la estructura ipc
    size_t shm_segsz;          //Tamaño del segmento de memoria compartida (en bytes)
    time_t shm_atime;          //Última vez que el segmento fue accedido
    time_t shm_dtime;          //Última vez que el segmento fue detachado
    time_t shm_ctime;          //Última vez que se cambió el segmento (por ejemplo, cambios en permisos)
    pid_t shm_cpid;            //PID del creador del segmento
    pid_t shm_lpid;            //PID del último proceso que hizo una operación en el segmento
    shmatt_t shm_nattch;       //Número de procesos que están adjuntos al segmento de memoria
};
 */
