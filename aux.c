#include "aux.h"

#define MAXVAR 1024


//Función auxiliar:
char letraTF (mode_t m){
    switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
    }
}

int trocearCadena(char * cadena, char * trozos[]) {
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL) {
        return 0;
    }
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL) {
        i++;
    }
    return i;
}

void historic_aux(char *comando, tListH *H, tListF *F, tListM *M, tListJ *J, tListD *D, char *envp[]) {
    char *tr[MAXNAME];
    int numtr = trocearCadena(comando, tr);

    if (strcmp("authors", tr[0]) == 0) {
        Cmd_authors(numtr, tr, H);
    } else if (strcmp("pid", tr[0]) == 0) {
        Cmd_pid();
    } else if (strcmp("ppid", tr[0]) == 0) {
        Cmd_ppid();
    } else if (strcmp("cd", tr[0]) == 0) {
        Cmd_cd(numtr, tr);
    } else if (strcmp("date", tr[0]) == 0) {
        Cmd_date(numtr, tr, H);
    } else if (strcmp("historic", tr[0]) == 0) {
        Cmd_historic(numtr, tr, *H, *F, *M, *J, *D, envp);
    } else if (strcmp("open", tr[0]) == 0) {
        Cmd_open(numtr, tr, F);
    } else if (strcmp("close", tr[0]) == 0) {
        Cmd_close(tr, F);
    } else if (strcmp("dup", tr[0]) == 0) {
        Cmd_dup(tr, F);
    } else if (strcmp("infosys", tr[0]) == 0) {
        Cmd_infosys();
    } else if (strcmp("help", tr[0]) == 0) {
        Cmd_help(numtr, tr, H);
    } else if (strcmp("makefile", tr[0]) == 0) {
        Cmd_makefile(tr);
    } else if (strcmp("makedir", tr[0]) == 0) {
        Cmd_makedir(tr);
    } else if (strcmp("listfile", tr[0]) == 0) {
        Cmd_listfile(tr);
    } else if (strcmp("cwd", tr[0]) == 0) {
        Cmd_cwd(numtr);
    } else if (strcmp("listdir", tr[0]) == 0) {
        Cmd_listdir(tr);
    } else if (strcmp("reclist", tr[0]) == 0) {
        Cmd_reclist(tr);
    } else if (strcmp("revlist", tr[0]) == 0) {
        Cmd_revlist(tr);
    } else if (strcmp("erase", tr[0]) == 0) {
        Cmd_erase(tr);
    } else if (strcmp("delrec", tr[0]) == 0) {
        Cmd_delrec(tr);
    } else if (strcmp("allocate", tr[0]) == 0){
        Cmd_allocate(tr, numtr, M, *H, *F);
    } else if (strcmp("deallocate", tr[0]) == 0){
            Cmd_deallocate(tr, numtr, M, *H, *F);
    } else if (strcmp("memfill", tr[0]) == 0){
            Cmd_memfill(tr);
    } else if (strcmp("memdump", tr[0]) == 0){
            Cmd_memdump(tr);
    } else if (strcmp("memory", tr[0]) == 0){
            Cmd_memory(tr, *M, *H);
    } else if (strcmp("readfile", tr[0]) == 0){
            Cmd_readfile(tr);
    } else if (strcmp("writefile", tr[0]) == 0){
            Cmd_writefile(tr);
    } else if (strcmp("read", tr[0]) == 0){
            Cmd_read(tr, *F);
    } else if (strcmp("write", tr[0]) == 0){
            Cmd_write(tr, *F);
    } else if (strcmp("recurse", tr[0]) == 0) {
            Cmd_recurse (tr);
    } else if (strcmp("getuid", tr[0]) == 0) {
            Cmd_getuid();
    } else if (strcmp("setuid", tr[0]) == 0) {
            Cmd_setuid(tr, H);
    } else if (strcmp("showvar", tr[0]) == 0) {
            Cmd_showvar(tr, envp, H);
    } else if (strcmp("changevar", tr[0]) == 0) {
            Cmd_changevar(tr, envp, H);
    } else if (strcmp("subsvar", tr[0]) == 0) {
            Cmd_subsvar(tr, envp, H);
    } else if (strcmp("environ", tr[0]) == 0) {
            Cmd_environ(tr, envp, H);
    } else if (strcmp("fork", tr[0]) == 0) {
            Cmd_fork(J);
    } else if (strcmp("search", tr[0]) == 0) {
            Cmd_search(tr, D, H);
    } else if (strcmp("exec", tr[0]) == 0) {
            Cmd_exec(tr, *D);
    } else if (strcmp("execpri", tr[0]) == 0) {
            Cmd_execpri(tr, *D);
    } else if (strcmp("fg", tr[0]) == 0) {
            Cmd_fg(tr, *D);
    } else if (strcmp("fgpri", tr[0]) == 0) {
            Cmd_fgpri(tr, *D, J);
    } else if (strcmp("back", tr[0]) == 0) {
            Cmd_back(tr, *D, J);
    } else if (strcmp("backpri", tr[0]) == 0) {
            Cmd_backpri(tr, *D, J);
    } else if (strcmp("listjobs", tr[0]) == 0) {
            Cmd_listjobs(*J);
    } else if (strcmp("deljobs", tr[0]) == 0) {
            Cmd_deljobs(tr, J);
    }else {
        errno = ENOENT;
        perror("Comando desconocido en historial");
    }
}

bool imprimirRuta(char *tr[], char ruta[MAXNAME]) {
    if(tr[1] == NULL) { //Si no nos dan un parametro imprimimos la ruta
        if (realpath(".", ruta) != NULL) { //Guarda en ruta la ruta actual en la que nos encontramos
            printf( "************%s\n", ruta);
            return true;
        }
    }
    return false;
}

void delHistoric_aux(tListH *H) {
    tPosH posH = *H, nextPos;
    while (posH->next != HNULL) { //LLegamos al final del historial
        nextPos = posH->next;
        posH = nextPos;
    }
    deleteAtPositionH(H, posH); //Eliminamos el ultimo comando del historial
}

char * convierteModo (mode_t m, char *permisos){
    strcpy (permisos,"---------- ");

    permisos[0]=letraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}

void reclist_aux(char *rutaabs, int hid, int acc, int l_long, int link) {
    DIR * dir;

    int tam;
    char permisos[20], fyh[20], ruta[MAXNAME], origen[1024];

    //struct stat info;
    struct dirent *contenido;
    struct stat archivo;
    struct tm *tm_info;
    struct passwd *usuario;
    struct group *grupo;
    

    dir = opendir(rutaabs); //Abrimos el directorio
    if (!dir) {
        perror("Error al abrir el directorio.");
        return;
    }

    printf( "************%s\n", rutaabs);

    while ((contenido = readdir(dir)) != NULL) { //Recorremos el directorio

        if (hid == 0 && contenido->d_name[0] == '.') {
            continue;
        }
        //Metemos el nombre del archivo/directorio en la ruta para tener la ruta completa
        snprintf(ruta, sizeof(ruta), "%s/%s", rutaabs, contenido->d_name);

        //Obtenemos la información del archivo/directorio
        if (lstat(ruta, &archivo) != 0) {
            perror("Error en la obtencion de la informacion del archivo");
            return;
        }

        usuario = getpwuid(archivo.st_uid);
        grupo = getgrgid(archivo.st_gid);

        tm_info = localtime(&archivo.st_atime);
        strftime(fyh, 20, "%Y/%m/%d - %H:%M", tm_info);
        convierteModo(archivo.st_mode, permisos);

        if (l_long == 1) {
            if (link == 1) {
                if (S_ISLNK(archivo.st_mode)) {
                    tam = readlink(rutaabs, origen, sizeof(origen) - 1);
                    if (tam == -1) {
                        perror("Error al leer el enlace simbólico \n");
                        return;
                    }
                    origen[tam] = '\0';
                    printf("%s   %lu ( %ld)   %s%s   %s   %lld   %s -> %s \n", fyh, archivo.st_nlink, archivo.st_ino,
                                    grupo->gr_name, usuario->pw_name, permisos, (long long)archivo.st_size, contenido->d_name, origen);
                }
                else {
                    printf("%s   %lu ( %ld)   %s%s   %s   %lld   %s\n", fyh, archivo.st_nlink, archivo.st_ino,
                                    grupo->gr_name, usuario->pw_name, permisos, (long long)archivo.st_size, contenido->d_name);//Comprobamos que argumentos están activos e imprimimos lo que hace falta:);
                }
            } else {
                printf("%s   %lu ( %ld)   %s%s   %s   %lld   %s\n", fyh, archivo.st_nlink, archivo.st_ino,
                                    grupo->gr_name, usuario->pw_name, permisos, (long long)archivo.st_size, contenido->d_name);//Comprobamos que argumentos están activos e imprimimos lo que hace falta:);
            }
        } else if (acc == 1) {
            if (link == 1) {
                if (S_ISLNK(archivo.st_mode)) {
                    tam = readlink(rutaabs, origen, sizeof(origen) - 1);
                    if (tam == -1) {
                        perror("Error al leer el enlace simbólico \n");
                        return;
                    }
                    origen[tam] = '\0';
                    printf("%10lld   %s   %s -> %s \n", (long long) archivo.st_size, fyh, contenido->d_name, origen);
                }
                else {
                    printf("%10lld   %s   %s\n", (long long) archivo.st_size, fyh, contenido->d_name);
                }
            } else {
                printf("%10lld   %s   %s\n", (long long) archivo.st_size, fyh, contenido->d_name);
            }
        } else if (link == 1){
            if (S_ISLNK(archivo.st_mode)) {
                tam = readlink(rutaabs, origen, sizeof(origen) - 1);
                if (tam == -1) {
                    perror("Error al leer el enlace simbólico \n");
                    return;
                }
                origen[tam] = '\0';
                printf("%10lld      %s -> %s \n", (long long) archivo.st_size, contenido->d_name, origen);
            } else {
                printf("%10lld      %s\n", (long long) archivo.st_size, contenido->d_name);
            }
        } else {
            printf("%10lld      %s\n", (long long) archivo.st_size, contenido->d_name);
        }
    }
    rewinddir(dir);  //rewinddir() vuelve al comienzo del directorio

    //Recorremos los subdirectorios
    while ((contenido = readdir(dir)) != NULL) {
        //Ignoramos este directorio y el directorio padre
        if (strcmp(contenido->d_name, ".") == 0 || strcmp(contenido->d_name, "..") == 0) {
            continue;
        }

        //Metemos el nombre del archivo/directorio en la ruta para tener la ruta completa
        snprintf(ruta, sizeof(ruta), "%s/%s", rutaabs, contenido->d_name);

        //Obtenemos la información del archivo/directorio
        if (lstat(ruta, &archivo) != 0) {
            perror("Error en la obtencion de la informacion del archivo");
            return;
        }

        if (S_ISDIR(archivo.st_mode)) { //Si es un directorio volvemos a llamar a la función con la nueva ruta
            reclist_aux(ruta, hid, acc, l_long, link);
        }
    }

    closedir(dir);
}

void revlist_aux(char *rutaabs, int hid, int acc, int l_long, int link) {
    DIR * dir;

    int tam;
    char permisos[20], fyh[20], ruta[MAXNAME], origen[1024];

    struct dirent *contenido;
    struct stat archivo;
    struct tm *tm_info;
    struct passwd *usuario;
    struct group *grupo;
    

    dir = opendir(rutaabs); //Abrimos el directorio
    if (dir == NULL) {
        perror("Error al abrir el directorio.");
        return;
    }


    while ((contenido = readdir(dir)) != NULL) { //Recorremos el directorio
        //Omitimos el directorio actual y el directorio padre
        if (strcmp(contenido->d_name, ".") == 0 || strcmp(contenido->d_name, "..") == 0) {
            continue;
        }

        //Metemos el nombre del archivo/directorio en la ruta para tener la ruta completa
        snprintf(ruta, sizeof(ruta), "%s/%s", rutaabs, contenido->d_name);

        //Obtenemos la información del archivo/directorio
        if (lstat(ruta, &archivo) != 0) {
            perror("Error en la obtencion de la informacion del archivo");
            return;
        }

        if (S_ISDIR(archivo.st_mode)) { //Si es un directorio, volvemos a llamar a la función con la nueva ruta
            revlist_aux(ruta, hid, acc, l_long, link);
        }
    }
    printf( "************%s\n", rutaabs);
    rewinddir(dir); //rewinddir() vuelve al comienzo del directorio

    while ((contenido = readdir(dir)) != NULL) { //Volvemos a recorrer el directorio

        if (hid == 0 && contenido->d_name[0] == '.') {
            continue;
        }

        //Metemos el nombre del archivo/directorio en la ruta para tener la ruta completa
        snprintf(ruta, sizeof(ruta), "%s/%s", rutaabs, contenido->d_name);

        //Obtenemos la información sobre el archivo/directorio
        if (lstat(ruta, &archivo) != 0) {
            perror("Error en la obtencion de la informacion del archivo");
            return;
        }

        usuario = getpwuid(archivo.st_uid);
        grupo = getgrgid(archivo.st_gid);

        tm_info = localtime(&archivo.st_atime);
        strftime(fyh, 20, "%Y/%m/%d - %H:%M", tm_info);
        convierteModo(archivo.st_mode, permisos);

        if (l_long == 1) {
            if (link == 1) {
                if (S_ISLNK(archivo.st_mode)) {
                    tam = readlink(rutaabs, origen, sizeof(origen) - 1);
                    if (tam == -1) {
                        perror("Error al leer el enlace simbólico \n");
                        return;
                    }
                    origen[tam] = '\0';
                    printf("%s   %lu ( %ld)   %s%s   %s   %lld   %s -> %s \n", fyh, archivo.st_nlink, archivo.st_ino,
                                    grupo->gr_name, usuario->pw_name, permisos, (long long)archivo.st_size, contenido->d_name, origen);
                }
                else {
                    printf("%s   %lu ( %ld)   %s%s   %s   %lld   %s\n", fyh, archivo.st_nlink, archivo.st_ino,
                                    grupo->gr_name, usuario->pw_name, permisos, (long long)archivo.st_size, contenido->d_name);//Comprobamos que argumentos están activos e imprimimos lo que hace falta:);
                }
            } else {
                printf("%s   %lu ( %ld)   %s%s   %s   %lld   %s\n", fyh, archivo.st_nlink, archivo.st_ino,
                                    grupo->gr_name, usuario->pw_name, permisos, (long long)archivo.st_size, contenido->d_name);//Comprobamos que argumentos están activos e imprimimos lo que hace falta:);
            }
        } else if (acc == 1) {
            if (link == 1) {
                if (S_ISLNK(archivo.st_mode)) {
                    tam = readlink(rutaabs, origen, sizeof(origen) - 1);
                    if (tam == -1) {
                        perror("Error al leer el enlace simbólico \n");
                        return;
                    }
                    origen[tam] = '\0';
                    printf("%10lld   %s   %s -> %s \n", (long long) archivo.st_size, fyh, contenido->d_name, origen);
                }
                else {
                    printf("%10lld   %s   %s\n", (long long) archivo.st_size, fyh, contenido->d_name);
                }
            } else {
                printf("%10lld   %s   %s\n", (long long) archivo.st_size, fyh, contenido->d_name);
            }
        } else if (link == 1){
            if (S_ISLNK(archivo.st_mode)) {
                tam = readlink(rutaabs, origen, sizeof(origen) - 1);
                if (tam == -1) {
                    perror("Error al leer el enlace simbólico \n");
                    return;
                }
                origen[tam] = '\0';
                printf("%10lld      %s -> %s \n", (long long) archivo.st_size, contenido->d_name, origen);
            } else {
                printf("%10lld      %s\n", (long long) archivo.st_size, contenido->d_name);
            }
        } else {
            printf("%10lld      %s\n", (long long) archivo.st_size, contenido->d_name);
        }
    }

    closedir(dir);
}

void delrec_aux(char *path) {
    DIR *dir;
    struct dirent *contenido;
    char fullpath[MAXNAME];  // origen para la ruta completa
    struct stat statbuf;

    dir = opendir(path); //Abrimos el directorio
    if (dir == NULL) {
        //Si no se puede abrir como directorio, intentamos eliminar como archivo
        if (unlink(path) == -1) {
            perror("Error al eliminar el archivo");
        }
    }else {
        while ((contenido = readdir(dir)) != NULL) { //Recorremos el directorio
            //Omitimos el directorio actual y el directorio padre
            if (strcmp(contenido->d_name, ".") == 0 || strcmp(contenido->d_name, "..") == 0) {
                continue;
            }

            //Metemos el nombre del archivo/directorio en la ruta para tener la ruta completa
            snprintf(fullpath, sizeof(fullpath), "%s/%s", path, contenido->d_name);

            //Obtenemos la información sobre el archivo/directorio
            if (lstat(fullpath, &statbuf) == -1) {
                perror("Error al obtener información del archivo/directorio");
                continue;
            }

            if (S_ISDIR(statbuf.st_mode)) { //Si es un directorio, volvemos a llamar a la función con la nueva ruta
                delrec_aux(fullpath);
            } else {
                if (unlink(fullpath) == -1) { //Intentamos borrarlo si es un archivo
                    perror("Error al eliminar el archivo");
                }
            }
        }
    }
    closedir(dir); //Cerramos el directorio

    if (rmdir(path) == -1) { //Intentamos eliminar el directorio una vez vaciado
        perror("Error al eliminar el directorio");
    }
}

void recursiva (int n) {
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

    if (n>0) {
        recursiva(n-1);
    }
}

void llenarMemoria (void *p, size_t cont, unsigned char byte) {
    unsigned char *arr=(unsigned char *) p;
    size_t i;

    for (i=0; i<cont;i++)
        arr[i]=byte;
}

ssize_t leerFichero (char *f, void *p, size_t cont) {
    struct stat s;
    ssize_t  n;
    int df, aux;

    if (stat (f, &s) == -1 || (df = open(f,O_RDONLY)) == -1)
        return -1;
    if (cont == -1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
        cont = s.st_size;
    if ((n = read(df, p, cont)) == -1){
        aux = errno;
        close(df);
        errno = aux;
        return -1;
    }
    close (df);
    return n;
}

ssize_t escribirFichero (char *f, void *p, size_t cont) {
    ssize_t  n;
    int df, aux;

    if ((df=open(f, O_WRONLY, 0777))==-1){
        return -1;
    }
    if ((n = write(df, p, cont)) == -1){
        aux = errno;
        close(df);
        errno = aux;
        return -1;
    }
    close (df);
    return n;
}

void do_AllocateMalloc(char *tr[], tListM *M) {
    size_t tam;
    void *p;
    tam = (size_t) strtoul(tr[2], NULL, 10);

    if (tam == 0) {
        printf ("No se asignan bloques de 0 bytes\n");
        return;
    }
    if ((p = malloc(tam)) != NULL) { //Asignamos un bloque de memoria de tamaño tam y comprobamos si se creo correctamente
        printf ("Asignados %lu bytes en %p\n", (unsigned long) tam, p);
    } else {
        printf ("Imposible asignar memoria:%s\n", strerror(errno));
    }

    //Insertamos en la lista de memoria:
    insertMalloc(M, MNULL, p, tam);
}

void * obtenerMemoriaShmget (key_t clave, size_t tam, tListM *M){
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;

    if (tam) {    //tam distito de 0 indica crear
        flags = flags | IPC_CREAT | IPC_EXCL; //cuando no es crear pasamos de tamaño 0
    }
    if (clave == IPC_PRIVATE) {
        errno = EINVAL;
        return NULL;
    }
    if ((id = shmget(clave, tam, flags)) == -1) {
        return (NULL);
    }
    if ((p = shmat(id, NULL, 0)) == (void*) -1){
        aux = errno;
        if (tam) {
            shmctl(id,IPC_RMID,NULL);
            errno = aux;
            return (NULL);
        }
    }
    shmctl (id,IPC_STAT,&s); //si no es crear, necesitamos el tamano, que es s.shm_segsz
    insertShared(M, MNULL, p, s.shm_segsz, clave);
    return (p);
}

void do_AllocateCreateshared (char *tr[],tListM *M){
    key_t cl;
    size_t tam;
    void *p;

    cl = (key_t)  strtoul(tr[2], NULL, 10);
    tam = (size_t) strtoul(tr[3], NULL, 10);
    if (tam == 0) {
        printf ("No se asignan bloques de 0 bytes\n");
        return;
    }
    if ((p = obtenerMemoriaShmget(cl, tam, M)) != NULL) {
        printf ("Asignados %lu bytes en %p\n", (unsigned long) tam, p);
    } else {
        printf ("Imposible asignar memoria compartida clave %lu:%s\n", (unsigned long) cl, strerror(errno));
    }
}

void do_AllocateShared (char *tr[], tListM *M){
    key_t cl;
    void *p;

    cl = (key_t)  strtoul(tr[2], NULL, 10);

    if ((p=obtenerMemoriaShmget(cl, 0, M))!=NULL) {
        printf ("Asignada memoria compartida de clave %lu en %p\n",(unsigned long) cl, p);
    } else {
        printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
    }
}

void * mapearFichero (char * fichero, int protection, tListM *M, tListF *F) {
    int df, map = MAP_PRIVATE, modo = O_RDONLY;
    struct stat s;
    void *p;

    if (protection&PROT_WRITE)
        modo = O_RDWR;
    if (stat(fichero,&s) == -1 || (df = open(fichero, modo)) == -1)
        return NULL;
    if ((p = mmap (NULL, s.st_size, protection, map, df, 0)) == MAP_FAILED)
        return NULL;
    insertMmap(M, MNULL, p, s.st_size, df, fichero);
    /*Guardas en la lista de descriptores usados df, fichero*/
    insertItemF(F, FNULL, fichero, modo, df);
    return p;
}

void do_AllocateMmap(char *tr[], tListM *M, tListF *F) {
    char *perm;
    void *p;
    int protection=0;

    if ((perm = tr[3]) != NULL && strlen(perm) < 4) {
        if (strchr(perm,'r') != NULL) protection |= PROT_READ;
        if (strchr(perm,'w') != NULL) protection |= PROT_WRITE;
        if (strchr(perm,'x') != NULL) protection |= PROT_EXEC;
    }
    if ((p=mapearFichero(tr[2],protection, M, F))==NULL) {
        perror ("Imposible mapear fichero");
    } else {
        printf ("fichero %s mapeado en %p\n", tr[0], p);
    }
}

void do_DeallocateMalloc (char *tr[], tListM *M) {
    size_t tam;
    tam = (size_t) strtoul(tr[2], NULL, 10);

    if (tam == 0) {
        printf ("No se desasignan bloques de 0 bytes\n");
        return;
    }
    tPosM posM = findMallocBlock(tam, *M);
    if (posM == MNULL) {
        printf("No se encontró un bloque de tamaño %lu\n", tam);
        return;
    }

    //Si la posición buscada es válida liberamos la dirrección de memoria y eliminamos el bloque de la lista:
    if (posM->block.address != MNULL) {
        free(posM->block.address);
        deleteBlockM(M, posM);
    }
}

void do_DeallocateShared (char *tr[], tListM *M) {
    key_t clave;
    int id;
    char *key=tr[2];
    tPosM posM;

    if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE) { //IC_PRIVATE: quiere decir que la clave es solo accesible para el proceso que lo crea, por tanto, no podemos desasignarla
        printf ("      shared necesita clave_valida\n");
        return;
    }
    posM = findSharedBlock(clave, *M);
    if (posM == MNULL) {
        printf("No se encontró un bloque de clave %d\n", clave);
        return;
    }
    if (shmdt(posM->block.address)==-1) { //shmdt(): desvincula la dirección de memoria compartida
        perror ("shmdt: imposible desvincular memoria compartida\n");
    }
    if ((id=shmget(clave,0,0666))==-1) { //shmget(): dada la key, el tamaño y los permisos (lectura/escritura), consigue el id de la memoria compartida
        perror ("shmget: imposible obtener memoria compartida");
        return;
    }
    if (shmctl(id,IPC_RMID,NULL)==-1) { //shmctl(): al ser llamado con el comando IPC_RMID, elimina la memoria compartida dada el id
        perror ("shmctl: imposible eliminar memoria compartida\n");
    }

    //Eliminamos el bloque de la lista de la lista:
    deleteBlockM(M, posM);
}

void do_DeallocateMmap (char *tr[], tListM *M, tListF F) {
    tPosM posM = findMmapBlock(tr[2], *M);

    if (posM == MNULL) {
        printf("No se encontró un bloque de fichero %s\n", tr[2]);
        return;
    }
    if (munmap(posM->block.address, posM->block.size) == -1) { //munmap(): libera la memoria mapeada en la dirección dada con el tamaño dado
        perror("munmap: imposible liberar memoria mapeada\n");
    }
    //Liberamos el archivo y eliminamos el bloque de la lista:
    free(posM->block.file);
    deleteBlockM(M, posM);

    //Si se encuentra en la lista, lo eliminamos
    tPosF posF = findItemF(posM->block.df, F);
    if (posF != NULL) {
        deleteAtPositionF(&F, posF);
    }
}

void do_DeallocateDelkey (char *args[]) {
    key_t clave;
    int id;
    char *key=args[2];

    if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE) {
        printf ("      delkey necesita clave_valida\n");
        return;
    }
    if ((id=shmget(clave,0,0666))==-1) {
        perror ("shmget: imposible obtener memoria compartida");
        return;
    }
    if (shmctl(id,IPC_RMID,NULL)==-1) {
        perror ("shmctl: imposible eliminar memoria compartida\n");
    }
}

void do_pmap (void) { /*sin argumentos*/
    pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
    char elpid[32];
    char *argv[4]={"pmap",elpid,NULL};

    sprintf (elpid,"%d", (int) getpid());
    if ((pid = fork()) == -1){
        perror ("Imposible crear proceso");
        return;
    }
    if (pid==0){
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;
        if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
            perror("cannot execute procstat (FreeBSD)");

        argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;
        if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
            perror("cannot execute procmap (OpenBSD)");

        argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
        if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
            perror("cannot execute vmmap (Mac-OS)");
        exit(1);
    }
    waitpid (pid,NULL,0);
}

void* cadtop(char *tr) {
    return (void*) strtoul(tr, NULL, 16);
}

int buscarVariable (char * var, char *e[]) {
    /*busca una variable en el entorno que se le pasa como parámetro*/
    /*devuelve la posicion de la variable en el entorno, -1 si no existe*/
    int pos=0;
    char aux[MAXVAR];

    strcpy (aux,var);
    strcat (aux,"=");

    while (e[pos]!=NULL){
        if (!strncmp(e[pos],aux,strlen(aux))) {
            return (pos);
        }
        else {
            pos++;
        }
    }
    errno=ENOENT;   /*no hay tal variable*/
    return(-1);
}

int cambiarVariable(char * var, char * valor, char *e[]) {
    /*cambia una variable en el entorno que se le pasa como parámetro*/
    /*lo hace directamente, no usa putenv*/
    int pos;
    char *aux;

    if ((pos=buscarVariable(var,e))==-1) {
        return(-1);
    }


    if ((aux=(char *)malloc(strlen(var)+strlen(valor)+2))==NULL) {
        return -1;
    }
    strcpy(aux,var);
    strcat(aux,"=");
    strcat(aux,valor);
    e[pos]=aux;
    return (pos);
}

char *Ejecutable (tListD D, char *s) {
    static char path[MAXNAME];
    struct stat st;
    char *p;
    tPosD posD = D;

    if (s == NULL || (posD == DNULL) || (p = (*posD).dir) ==NULL){
        return s;
    }

    if (s[0]=='/' || !strncmp (s,"./",2) || !strncmp (s,"../",3)){
        return s;        /*is an absolute pathname*/
    }

    strncpy (path, p, MAXNAME-1);
    strncat (path,"/",MAXNAME-1);
    strncat(path,s,MAXNAME-1);

    if (lstat(path,&st)!=-1) {
        return path;
    }
    while ((posD = posD->next)!=NULL){
        p = posD->dir;
        if (p == NULL) {
            continue;
        }

        strncpy (path, p, MAXNAME-1);
        strncat (path,"/",MAXNAME-1);
        strncat(path,s,MAXNAME-1);
        
        if (lstat(path,&st)!=-1)
            return path;
    }
    return s;
}

extern char **environ;
bool isVar(char *var) {
    char **env;
    for (env = environ; *env != NULL; env++) {
        char *eq_pos = strchr(*env, '=');
        if (eq_pos != NULL) {
            size_t len = eq_pos - *env;
            if (strncmp(var, *env, len) == 0 && var[len] == '\0') {
                return true;
            }
        }
    }
    return false;
}
