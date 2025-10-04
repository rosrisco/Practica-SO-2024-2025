#ifndef CMDS_H
#define CMDS_H

//Incluimos librerías:
#include "historic.h"
#include "openfiles.h"
#include "memory.h"
#include "aux.h"
#include "jobs.h"
#include "dirs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>      // Para getpwuid()
#include <grp.h>      //para getgrgid()
#include <errno.h>
#include <ctype.h>

//authros: imprime los autores y sus logins.
void Cmd_authors(int num, char * tr[], tListH *H);

//pid: imprime el pid del proceso ejecutado por la shell.
void Cmd_pid();

//ppid: imprime el pid del padre del proceso ejecutado por la shell.
void Cmd_ppid();

//cd/cd [dir]: imprime el directorio en el que se está o cambia al directorio pedido.
void Cmd_cd(int num, char * tr[]);

//date: imprime la fecha y la hora.
void Cmd_date(int num, char * tr[], tListH *H);

//historic/historic [N]/historic[-N]: imprime el historial de comandos, imprime el comando pedido (N) o imprime los últimos N comandos.
void Cmd_historic(int num, char * tr[], tListH H, tListF F, tListM M, tListJ J, tListD D, char *envp[]);

//open/open [file] mode: imprime la lista de ficheros abiertos, abre un fichero con el modo y nombre introducido.
void Cmd_open (int num, char * tr[], tListF *F);

//close: cierra el fichero dado su df.
void Cmd_close (char *tr[], tListF *F);

//dup: duplica un fichero dado su df.
void Cmd_dup (char * tr[], tListF *F);

//infosys: imprime los datos de la maquina.
void Cmd_infosys ();

//help/help [cmd]: imprime los comandos que hay en la shell, imprime el funcionamient del comando pedido.
void Cmd_help (int num, char *tr[], tListH *H);

//makefile [name]: crea un archivo de nombre name.
void Cmd_makefile (char *tr[]);

//makedir [name]: crea un directario de nombre name.
void Cmd_makedir (char *tr[]);

//listfile [-long][-link][-acc] name1 name2 ...: lista los archivos de los directorios pedidos.
void Cmd_listfile (char *tr[]);

//cwd: imprime el directorio en el que estamos ubicados.
void Cmd_cwd (int num);

//lisdir [-long][-link][-acc] name1 name2 ...: lista los contenidos de los directorios pedidos.
void Cmd_listdir (char *tr[]);

//reclist [-long][-hic][-link][-acc] name1 name2 ...: lista recursivamente los contenidos de los directorios pedidos (subdir después).
void Cmd_reclist (char *tr[]);

//revlist [-long][-hic][-link][-acc] name1 name2 ...: lista recursivamente los contenidos de los directorios pedidos (subdir primero).
void Cmd_revlist (char *tr[]);

//delrec [name1, name2, ...]: borra ficheros o directorios no vacios recursivamente.
void Cmd_erase (char *tr[]);

//delrec [name1, name2, ...]: borra ficheros o directorios no vacios recursivamente.
void Cmd_delrec (char *tr[]);

//allocate [-malloc] [-shared] [-createshared] [-mmap]...: asigna un bloque de memoria
void Cmd_allocate (char *tr[], int num, tListM *M, tListH H, tListF F);

//deallocate [-malloc] [-shared] [-delkey] [-mmap]...: desasigna un bloque de memoria
void Cmd_deallocate (char *tr[], int num, tListM *M, tListH H, tListF F);

//memfill addr cont byte: llena la memoria a partir de addr con byte
void Cmd_memfill(char *tr[]);

//memdump addr cont: vuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr
void Cmd_memdump(char *tr[]);

//memory [-blocks|-funcs|-vars|-all|-pmap]...:	muestra detalles de la memoria del proceso
void Cmd_memory(char *tr[], tListM M, tListH H);

//readfile fich addr cont: lee cont bytes desde fich a la direccion addr
void Cmd_readfile (char *tr[]);

//writefile [-o] fich addr cont: escribe cont bytes desde la direccion addr a fich
void Cmd_writefile (char *tr[]);

//read df addr cont: lo mismo que readfile pero usamos un descriptor de un archivo (ya abierto)
void Cmd_read (char *tr[], tListF F);

//write df addr cont: lo mismo que writefile pero usamos un descriptor de un archivo (ya abierto)
void Cmd_write (char *tr[], tListF F);

//recurse n: ejecutamos la funcion recursiva n veces
void Cmd_recurse (char *tr[]);

//getuid: muestra las credenciales del proceso que ejecuta la shell
void Cmd_getuid ();

//setuid [-l] id: cambia las credenciales del proceso que ejecuta la shell
void Cmd_setuid (char *tr[], tListH *H);

//showvar var1 var2...: muestra el valor y las direcciones de las variables de entorno dadas
void Cmd_showvar (char *tr[], char *envp[], tListH *H); //, char *e[]

//changevar [-a | -e | -p] var val...: cambia el valor de una variable de entorno
void Cmd_changevar (char *tr[], char *envp[], tListH *H);

//subsvar [-a | -e] var1 var2 val: sutituye la variable de entorno var1 con nombre var2 y valor val
void Cmd_subsvar (char *tr[], char *envp[], tListH *H);

//environ [-environ | -addr]: muestra el entorno de proceso
void Cmd_environ (char *tr[], char *envp[], tListH *H);

//fork: la shell hace fork y espera a que su hijo termine
void Cmd_fork (tListJ *J);

//search [-clear | -path] [-add | -del] dir: cambia o muestra la lista de directorios de la shell
void Cmd_search (char *tr[], tListD *D, tListH *H);

//exec command: ejecuta, sin crear un proceso, command
void Cmd_exec (char *tr[], tListD D);

//execpri prio command: ejecuta, sin crear un proceso, command con prioridad prio
void Cmd_execpri (char *tr[], tListD D);

//fg command: crea un proceso que ejecuta command en primer plano
void Cmd_fg (char *tr[], tListD D);

//fgpri prio command: crea un proceso que ejecuta command en primer plano con prioridad prio
void Cmd_fgpri (char *tr[], tListD D, tListJ *J);

//back command: crea un proceso que ejecuta command en segundo plano
void Cmd_back (char *tr[], tListD D, tListJ *J);

//backpri prio command: crea un proceso que ejecuta command en segundo plano con prioridad prio
void Cmd_backpri (char *tr[], tListD D, tListJ *J);

//listjobs: imprime la lista de procesos en segundo plano
void Cmd_listjobs (tListJ J);

//deljobs [-term | -sig]: elimina los procesos de la lista de procesos en segundo plano
void Cmd_deljobs (char *tr[], tListJ *J);
#endif //CMDS_H

/*
struct tm {
    int tm_sec;   //Segundos [0, 60] (60 permite segundos intercalares)
    int tm_min;   //Minutos [0, 59]
    int tm_hour;  //Horas [0, 23]
    int tm_mday;  //Día del mes [1, 31]
    int tm_mon;   //Mes [0, 11] (0 = enero, 11 = diciembre)
    int tm_year;  //Años desde 1900 (por ejemplo, 122 para el año 2022)
    int tm_wday;  //Día de la semana [0, 6] (0 = domingo)
    int tm_yday;  //Día del año [0, 365]
    int tm_isdst; //Indicador de horario de verano (1 si está en horario de verano, 0 si no, y negativo si no está disponible)
};

struct utsname {
    char sysname[];    //Nombre del sistema operativo (por ejemplo, "Linux")
    char nodename[];   //Nombre de la red del sistema (nombre del host)
    char release[];    //Versión de lanzamiento del sistema operativo (por ejemplo, "5.10.0-8-amd64")
    char version[];    //Versión del sistema operativo (por ejemplo, "#1 SMP Debian 5.10.40-1 (2021-05-28)")
    char machine[];    //Arquitectura del hardware (por ejemplo, "x86_64")
    char domainname[]; //Dominio del sistema (GNU extension)
};

struct stat {
        dev_t     st_dev;     %ld         //ID del dispositivo
        ino_t     st_ino;     %lu         //Inodo
        mode_t    st_mode;    %o          //Permisos y tipo
        nlink_t   st_nlink;   %lu         //Número de enlaces
        uid_t     st_uid;     %u          //ID del propietario
        gid_t     st_gid;     %u          //ID del grupo
        dev_t     st_rdev;    %ld         //ID del dispositivo
        off_t     st_size;    %lld        //Tamaño
        blksize_t st_blksize; %ld         //Tamaño de bloque preferido para operaciones de E/S
        blkcnt_t  st_blocks;  %ld         //Número de bloques asignados
        time_t    st_atime;   ctime() %s  //Última fecha de acceso
        time_t    st_mtime;   ctime() %s  //Última fecha de modificación
        time_t    st_ctime;   ctime() %s  //Última fecha de cambio de metadatos (inode)
}

struct passwd {
    char   *pw_name;   //Nombre de usuario
    char   *pw_passwd; //Contraseña encriptada (generalmente no se usa por seguridad)
    uid_t   pw_uid;    //ID del usuario (UID)
    gid_t   pw_gid;    //ID del grupo (GID)
    char   *pw_gecos;  //Información del usuario (nombre real, entre otros)
    char   *pw_dir;    //Directorio home del usuario
    char   *pw_shell;  //Shell predeterminada del usuario
};

struct group {
    char   *gr_name;   //Nombre del grupo
    char   *gr_passwd; //Contraseña del grupo (generalmente no se usa)
    gid_t   gr_gid;    //ID del grupo (GID)
    char  **gr_mem;    //Lista de miembros del grupo
};

struct dirent {
    ino_t d_ino;             //Número de inodo del archivo (identificador único dentro del sistema de archivos)
    off_t d_off;             //Desplazamiento al siguiente `dirent` en el directorio (usado internamente)
    unsigned short d_reclen; //Longitud del registro de esta entrada
    unsigned char d_type;    //Tipo de archivo (por ejemplo, DT_REG para archivo regular, DT_DIR para directorio)
    char d_name[];           //Nombre de la entrada (nombre del archivo o subdirectorio)
};
*/
