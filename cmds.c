//Incluimos librerías:
#include "cmds.h"


void Cmd_authors(int num, char *tr[], tListH *H) {
    if (num == 1) { //En caso de que el comando no tenga argumentos:
        printf("Xxx Xxx: xxxxx@udc.es\n"
               "Xxx Xxx: xxxxx@udc.es\n");
    } else if (num == 2) { //Si el comando tiene argumentos, comprobamos cuál es y hacemos que ejecute su función:
        if (strcmp("-l", tr[1]) == 0) {
            printf("xxxxx@udc.es\n"
                   "xxxxx@udc.es\n");
        } else if (strcmp("-n", tr[1]) == 0) {
            printf("Xxx Xxx\n"
                   "Xxx Xxx\n");
        } else { //En caso de que el segundo argumento sea inválido, se elimina del historial.
            delHistoric_aux(H);
        }
    } else { //En caso de que lleguen más argumentos, se elimina del historial.
        delHistoric_aux(H);
    }
}

void Cmd_pid(){
    printf("· pid: %d\n", getpid());
}

void Cmd_ppid(){
    printf("· pid: %d\n", getppid());
}

void Cmd_cd(int num, char * tr[]) {
    char dir[MAXNAME];
    if(num == 1) {
        printf("%s\n", getcwd(dir, MAXNAME)); //getcwd() obtiene la ruta del directorio de trabajo actual
    }
    else if(num == 2) {
        chdir(tr[1]); //Cambia el directorio de trabajo actual y se mueve al introducido
    }
}

void Cmd_date(int num, char * tr[], tListH *H) {
    time_t fyh; //Declaramos fyh como una variable de tiempo en segundos.

    //Esta estructura guarda la fecha (días, meses, años) y la hora (horas, minutos, segundos) desglosada cada una de sus respectivas partes.
    struct tm * tm_info;

    //Obtiene la hora del sistema en segundos y la almacena en fyh
    time(&fyh);
    tm_info = localtime(&fyh); //localtime() convierte la información de fyh y la almacena en el struct tm, al cual apuntará tm_info

    if(num == 1) { //En caso de que el comando no tenga argumentos:
        printf("%02d/%02d/%04d\n", tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
        printf("%02d:%02d:%02d\n", tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
    }
    else if (num == 2) { //En caso de que el comando tenga argumentos, comprobamos cuál es y hacemos que realice su función:
        if (strcmp("-t", tr[1]) == 0) {
            printf("%02d:%02d:%02d\n", tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
        }
        else if (strcmp("-d", tr[1]) == 0) {
            printf("%02d/%02d/%04d\n", tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
        } else { //En caso de que el segundo argumento sea inválido, se elimina del historial.
            delHistoric_aux(H);
        }
    }else { //En caso de que lleguen más argumentos, se elimina del historial.
        delHistoric_aux(H);
    }
}

void Cmd_historic(int num, char * tr[], tListH H, tListF F, tListM M, tListJ J, tListD D, char *envp[]) {
    int i;
    tPosH pos = H, q = H;

    if(num == 1) { //Si el comando no tiene argumentos:
        int a = 0;

        //Imprimimos la lista de comandos por orden:
        while (pos != HNULL) {
            a++;
            printf("%d. %s\n", a, pos->data);
            pos = pos->next;
        }
    } else if (num == 2) { //Si el comando tiene argumentos:
        if(strncmp(tr[1],"-", 1) == 0) { //Comprobamos si el argumento empieza por un "-", en caso de que sí:

            int total = 0, b = 1, first;
            i = atoi(tr[1] + 1); //Pasamos la cadena después del "-" a enteros, devuelve 0 en caso de no ser un número
            if(i == 0) {
                delHistoric_aux(&H);
            }
            //Calculamos el número de elementos que hay en la lista de ficheros abiertos:
            while (q != HNULL) {
                total++;
                q = q->next;
            }
            first = total - i + 1; //Calculamos el elemento desde el que se tiene que empezar a imprimir la lista de archivos abiertos
            while (pos != HNULL) { //Recorremos la lista entera.
                if (b >= first) { //Mientras b sea mayor que first:
                    printf("%d. %s\n", b, pos->data);
                }
                pos = pos->next;
                b++;
            }
        } else { //En caso de que se introduzca solo un número como argumento:
            int c = 1;
            i = atoi(tr[1]); //Convertimos la cadena en número entero, atoi() devuelve 0 en caso de que la cadena no sea un número
            if(i != 0) {
                //Recorremos la lista hasta que el contador sea igual a i y llamamos al comando en esa posición:
                while(pos != HNULL && c != i) {
                    pos = pos->next;
                    c ++;
                }
                if(pos != HNULL) {
                    printf("Ejecutando %d. %s", i, pos->data);
                    historic_aux(pos->data, &H, &F, &M, &J, &D, envp); //LLamamos a la auxiliar para ejecutar los comandos
                }
            }else{
                errno = EINVAL;
                perror("Error");
                delHistoric_aux(&H);
            }
        }
    } else { //En caso de argumento inválido, se elimina del historial.
        delHistoric_aux(&H);
    }
}

void Cmd_open (int num, char * tr[], tListF *F) {
    int i,df, mode = 0;
    char *name = tr[1];

    if (num == 1 && *F != FNULL) { //Si el comando no tiene argumentos:
        printListF(*F);

    } else if (num == 3) { //En caso de que sí que tengo argumentos:
        for(i=1; tr[i] != NULL; i++) { //Comprobamos cuál es el modo introducido:
            if (!strcmp(tr[i],"cr")) mode|=O_CREAT;
            else if (!strcmp(tr[i],"ex")) mode|=O_EXCL;
            else if (!strcmp(tr[i],"ro")) mode|=O_RDONLY;
            else if (!strcmp(tr[i],"wo")) mode|=O_WRONLY;
            else if (!strcmp(tr[i],"rw")) mode|=O_RDWR;
            else if (!strcmp(tr[i],"ap")) mode|=O_APPEND;
            else if (!strcmp(tr[i],"tr")) mode|=O_TRUNC;
        }
        //open() abre el archivo con su modo y dándole permisos (0777) y devuelve el file descriptor una vez que se abre el archivo.
        if ((df=open(tr[1],mode,0777))== -1) { //Si el df es -1, el archivo no se puede crear.
            perror ("Imposible abrir fichero");
        }
        else{ //En caso contrario, insertamos el archivo en la lista de archivos abiertos:
            insertItemF(F, FNULL, name, mode, df);
            printf ("Anadida entrada a la tabla ficheros abiertos:\n");
            printListF(*F);
        }
    }
}

void Cmd_close (char *tr[], tListF *F) {
    int df;

    if (tr[1] == NULL || (df = atoi(tr[1])) < 0) { //Si no hay argumento o es inválido (df<0):
        printListF(*F);
        return;
    }

    //close() cierra el archivo dado su df.
    if (close(df) == -1) { //En caso de que devuelva -1, el archivo no se puede eliminar.
        perror("Inposible cerrar descriptor");
    }
    tPosF posF = findItemF(df, *F);
    deleteAtPositionF(F ,posF);
}

void Cmd_dup (char * tr[], tListF *F){
    int df, duplicado;
    tItemF itemF;
    tPosF posF, last;

    if (tr[1] == NULL || (df = atoi(tr[1])) < 0) { //no hay parametro
        printListF(*F);
        return;
    }

    //dup() crea el duplicado dado el df, si no se puede duplicar, devuelve -1
    duplicado = dup(df);

    if(duplicado == -1) {
        perror ("Imposible duplicar fichero");
    } else { //En caso de posible duplicado, metemos sus respectivos datos en cada parte y añadimos el archivo a la lista:
        for(tPosF p = *F; p != FNULL ; p = p -> next) { //Buscamos la última posición:
            if(df == p->file.df) {
                itemF = p->file;
                break;
            }
        }

        for(last = *F; last != FNULL ; last = last -> next){}
        posF = last; //La posición del item será el último
        insertItemF(F, posF, itemF.name,itemF.mode, duplicado);
    }
}

void Cmd_infosys() {
    struct utsname info;
    /*
    Creamos un struct de tipo utsname, que contiene los campos:
        sysname -> nombre del sistema operativo
        nodename -> nombre del nodo (máquina)
        release -> versión del sistema operativo
        version -> número de versión
        machine -> arquitectura del hardware
    */
    uname(&info); //Llenamos los campos anteriores con la información de la máquina (haciendo una llamada al sistema)
    printf("Sistema operativo: %s\n", info.sysname);
    printf("Release: %s\n", info.release);
    printf("Versión: %s\n", info.version);
    printf("Máquina: %s\n", info.machine);
    printf("Hostname: %s\n", info.nodename);
}

void Cmd_help(int num, char *tr[], tListH *H) {
    if (num == 1) { //Si la cadana no tiene argumentos:
        printf("authors [-l] [-n]\n"
            "pid\n"
            "ppid\n"
            "cd [dir]\n"
            "date [-t | -d]\n"
            "historic [N | -N]\n"
            "open filename [cr] [ap] [ex] [ro] [rw] [wo] [tr]\n"
            "close [df]\n"
            "dup [df]\n"
            "infosys\n"
            "help [cmd]\n"
            "makefile [name]\n"
            "makedir [name]\n"
            "listfile [-long][-link][-acc] name1 name2 ...\n"
            "cwd\n"
            "reclist [-hid][-long][-link][-acc] name1 name2 ...\n"
            "revlist [-hid][-long][-link][-acc] name1 name2 ...\n"
            "erase [name1 name2 ...]\n"
            "delrec [name1 name2 ...]\n"
            "allocate [-malloc | -mmap | -createshared | -shared]\n"
            "deallocate [-malloc | -mmap | -shared | -delkey | addr]\n"
            "memfill [addr][cont][ch]\n"
            "memdump [addr][cont]\n"
            "memory [-funcs | -vars | -blocks | -all | -pmap]\n"
            "readfile [name][addr][cont]\n"
            "writefile [name][addr][cont]\n"
            "read [df][addr][cont]\n"
            "write [df][addr][cont]\n"
            "recurse [n]\n"
            "getuid\n"
            "setuid [-l] id\n"
            "showvar var1 var2 ...\n"
            "changevar [-a | -e | -p] var val\n"
            "subsvar [-a | -e] var1 var2 val\n"
            "environ [-environ | -addr]\n"
            "fork\n"
            "search [-clear | -path] [-add | -del] dir\n"
            "exec command\n"
            "execpri prio command\n"
            "fg command\n"
            "fgpri prio command\n"
            "back command\n"
            "backpri prio command\n"
            "listjobs\n"
            "deljobs [-term | -sig]\n"
            "quit\n"
            "exit\n"
            "bye\n");
    }
    else if (num == 2) { //Si tiene argumentos, comprobamos cuál es y hacemos que realice su función:
        if (strcmp("authors", tr[1]) == 0) {
            printf("authors    imprime los nombre y logins de las autoras del programa\n   -n      imprime los nombres\n   -l      imprime los logins\n");
        } else if (strcmp("pid", tr[1]) == 0) {
            printf("pid    imprime el pid del proceso ejecutado\n");
        } else if (strcmp("ppid", tr[1]) == 0) {
            printf("ppid   imprime el pid del padre del proceso\n");
        } else if (strcmp("cd", tr[1]) == 0) {
            printf("cd   muestra el directorio actual\n   [dir]   cambia el directorio en el que se esta a dir\n");
        } else if (strcmp("date", tr[1]) == 0) {
            printf("date    imrpime la fecha y la hora\n   -d   imprime la fecha\n   -t   imprime la hora\n");
        } else if (strcmp("historic", tr[1]) == 0) {
            printf("historic     escribe los comandos que se han usado ordenados numericamente\n     [N]     repite el comando numero N\n    [-N]     imprime los ultimos N comandos\n");
        } else if (strcmp("open", tr[1]) == 0) {
            printf("open    muestra una lista de archivos abiertos\nopen [file] mode    abre un archivo\n"
                   "[cr]     crea el archivo en caso de que no exista\n"
                   "[ap]     al escribir, lo hace al final del archivo\n"
                   "[ex]     se usa con cr, se asegura de que no se sobreescriba un archivo\n"
                   "[ro]     se abre en modo lectura\n"
                   "[rw]     se abre en modo lectura y escritura\n"
                   "[wo]     se abre en modo escritura\n"
                   "[tr]     si el archivo existe, se abre modo escritura y se vacia\n");
        } else if (strcmp("close", tr[1]) == 0) {
            printf("close [df]    cierra el descriptor del archivo df\n");
        } else if (strcmp("dup", tr[1]) == 0) {
            printf("dup [df]    duplica el descriptor del archivo df\n");
        } else if (strcmp("infosys", tr[1]) == 0) {
            printf("infosys    imprime la informacion de la maquina en la que se ejecuta la shell\n");
        } else if (strcmp("help", tr[1]) == 0) {
            printf("help   imprime lista de comandos\n     [cmd]     imprime el uso del comando\n");
        } else if (strcmp("quit", tr[1]) == 0 || strcmp("exit", tr[1]) == 0 || strcmp("bye", tr[1]) == 0) {
            printf("quit, exit, bye    cierra la shell\n");
        } else if (strcmp("makefile", tr[1]) == 0) {
            printf("makefile [name]   crea un fichero de nombre name\n");
        } else if (strcmp("makedir", tr[1]) == 0) {
            printf("makedir [name]   crea un directorio de nombre name\n");
        } else if (strcmp("listfile", tr[1]) == 0) {
            printf("listfile [-long][-link][-acc] name1 name2 ...   lista los ficheros de un directorio\n"
                   "[-long]    listado largo\n"
                   "[-acc]     tiempo de acceso\n"
                   "[-link]    si es enlace simbólico, el path contenido\n");
        } else if (strcmp("cwd", tr[1]) == 0) {
            printf("cwd   muestra el directorio actual\n");
        } else if (strcmp("listdir", tr[1]) == 0) {
            printf("listfdir [-long][-hic][-link][-acc] name1 name2 ...   lista los contenidos de un directorio\n"
                   "[-long]    listado largo\n"
                   "[-hid]     incluye los ficheros ocultos\n"
                   "[-acc]     tiempo de acceso\n"
                   "[-link]    si es enlace simbólico, el path contenido\n");
        } else if (strcmp("reclist", tr[1]) == 0) {
            printf("reclist [-long][-hic][-link][-acc] name1 name2 ...   lista recursivamente los contenidos de un directorios (subdir después)\n"
                   "[-long]    listado largo\n"
                   "[-hid]     incluye los ficheros ocultos\n"
                   "[-acc]     tiempo de acceso\n"
                   "[-link]    si es enlace simbólico, el path contenido\n");
        } else if (strcmp("revlist", tr[1]) == 0) {
            printf("revlist [-long][-hic][-link][-acc] name1 name2 ...   lista recursivamente los contenidos de directorios (subdir primero)\n"
                   "[-long]    listado largo\n"
                   "[-hid]     incluye los ficheros ocultos\n"
                   "[-acc]     tiempo de acceso\n"
                   "[-link]    si es enlace simbólico, el path contenido\n");
        } else if (strcmp("erase", tr[1]) == 0) {
            printf("erase [name1, name2, ...]   borra directorios o ficheros vacíos\n");
        } else if (strcmp("delrec", tr[1]) == 0) {
            printf("delrec [name1, name2, ...]   borra ficheros o directorios no vacios recursivamente\n");
        } else if (strcmp("allocate", tr[1]) == 0){
            printf("allocate [-malloc | -mmap | -createshared | -shared]   asigna un bloque de memoria\n"
                    "[-malloc] tam: asigna un bloque malloc de tamano tam\n"
                    "[-createshared] cl tam: asigna (creando) el bloque de memoria compartida de clave cl y tamano tam\n"
                    "[-shared] cl: asigna el bloque de memoria compartida (ya existente) de clave cl\n"
                    "[-mmap] fich perm: mapea el fichero fich, con los permisos perm\n");
        } else if (strcmp("deallocate", tr[1]) == 0){
            printf("deallocate [-malloc | -shared | delkey | -mmap| addr]   desasigna un bloque de memoria"
                   "[-malloc] tam: desasigna el bloque malloc de tamano tam\n"
                   "[-shared] cl: desasigna (desmapea) el bloque de memoria compartida de clave cl\n"
                   "[-delkey] cl: elimina del sistema (sin desmapear) la clave de memoria cl\n"
                   "[-mmap] fich: desmapea el fichero mapeado fich\n"
                   "addr: desasigna el bloque de memoria en la direccion addr\n");
        } else if (strcmp("memfill", tr[1]) == 0){
            printf("memfill [addr][cont][ch]   llena la memoria cont bytes con ch a partir de la direccion addr\n");
        } else if (strcmp("memdump", tr[1]) == 0){
            printf("memdump addr cont 	vuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr\n");
        } else if (strcmp("memory", tr[1]) == 0){
            printf("memory [-blocks | -funcs | -vars | -all | -pmap]   muestra detalles de la memoria del proceso\n"
                   "[-blocks]: muestra los bloques de memoria asignados\n"
                   "[-funcs]: muestra las direcciones de las funciones\n"
                   "[-vars]: muestra las direcciones de las variables\n"
                   "[-all]: muestra todo\n"
                   "[-pmap]: muestra la salida del comando pmap(o similar)\n");
        } else if (strcmp("readfile", tr[1]) == 0){
            printf("readfile [name][addr][cont]   lee cont bytes desde el fichero hasta la direccion addr\n");
        } else if (strcmp("writefile", tr[1]) == 0){
            printf("writefile [name][addr][cont]   escribe cont bytes desde la direccion addr a fichero\n");
        } else if (strcmp("read", tr[1]) == 0){
            printf("read [df][addr][cont]   transfiere cont bytes desde el fichero del df a la direccion addr\n");
        } else if (strcmp("write", tr[1]) == 0){
            printf("write [df][addr][cont]   transfiere cont bytes desde la direccion addr hasta el fichero del df\n");
        } else if (strcmp("recurse", tr[1]) == 0) {
            printf("recurse [n]   ejecuta la funcion recursiva n veces\n");
        }else if (strcmp("getuid", tr[1]) == 0) {
            printf("getuid   muestra las credenciales del proceso que ejecuta la shell\n");
        }else if (strcmp("setuid", tr[1]) == 0){
            printf("setuid [-l] id   cambia las credenciales del proceso que ejecuta la shell\n"
                    "id: establece la credencial al valor id \n"
                    "[-l] id: establece la credencial al usuario con nombre id \n");
        }else if (strcmp("showvar", tr[1]) == 0){
            printf("showvar var1 var2...   muestra el valor y las direcciones de las variables de entorno var1, var2, ...\n");
        }else if (strcmp("changevar", tr[1]) == 0){
            printf("changevar [-a | -e | -p] var val...   cambia el valor de una variable de entorno\n"
                    "[-a]: accede por el tercer argumento de main\n"
                    "[-e]: accede mediante environ\n"
                    "[-p]: accede mediante putenv\n");
        }else if (strcmp("subsvar", tr[1]) == 0){
            printf("subsvar [-a | -e] var1 var2 val   sutituye la variable de entorno var1 con nombre var2 y valor val\n"
                    "[-a]: accede por el tercer argumento de main\n"
                    "[-e]: accede mediante environ\n");
        }else if (strcmp("environ", tr[1]) == 0){
            printf("environ [-environ | -addr]   muestra el entorno de proceso\n"
                   "[-environ]: accede usando environ (en vez del tercer argumento de main)\n"
                   "[-addr]: muestra el valor y donde se almacena environ y el tercer argumento de main\n");
        }else if (strcmp("fork", tr[1]) == 0){
            printf("fork   la shell hace fork y espera a que su hijo termine\n");
        }else if (strcmp("search", tr[1]) == 0){
            printf("search [-clear | -path] [-add | -del] dir   cambia o muestra la lista de directorios de la shell\n"
                   "[-clear]: vacia la lista de directorios\n"
                   "[-path]: importa el PATH en la lista de directorios\n"
                   "[-add] dir: añade dir a la lista de directorios\n"
                   "[-del] dir: elimina dir de la lista de diretorios\n");
        }else if (strcmp("exec", tr[1]) == 0){
            printf("exec command   ejecuta, sin crear un proceso, command (para mas info help command)\n");
        }else if (strcmp("execpri", tr[1]) == 0){
            printf("execpri prio command   ejecuta, sin crear un proceso, command (para mas info help command) con prioridad prio\n");
        }else if (strcmp("fg", tr[1]) == 0){
            printf("fg command   crea un proceso que ejecuta command (para mas info help command) en primer plano\n");
        }else if (strcmp("fgpri", tr[1]) == 0){
            printf("fgpri prio command   crea un proceso que ejecuta command (para mas info help command) en primer plano con prioridad prio\n");
        }else if (strcmp("back", tr[1]) == 0){
            printf("back command   crea un proceso que ejecuta command (para mas info help command) en segundo plano\n");
        }else if (strcmp("backpri", tr[1]) == 0){
            printf("backpri prio command   crea un proceso que ejecuta command (para mas info help command) en segundo plano con prioridad prio\n");
        }else if (strcmp("listjobs", tr[1]) == 0){
            printf("listjobs   imprime la lista de procesos en segundo plano\n");
        }else if (strcmp("deljobs", tr[1]) == 0){
            printf("deljobs [-term | -sig]   elimina los procesos de la lista de procesos en segundo plano\n"
                   "[-term]: los terminados\n"
                   "[-sig]: los terminados por senal\n");
        }else if (strcmp("command", tr[1]) == 0){
            printf("command [var1 var2 var3...] ejecutable [argumento1 argumento2...]\n"
                   "ejecutable: nombre del comando a ejecutar\n"
                   "[var1 var2 var3...]: si se incluyen, serán las variables del nuevo entorno\n"
                   "[argumento1 argumento2...]: argumentos del comando ejecutable\n");
        } else { //En caso de argumento inválido, se elimina del historial.
            delHistoric_aux(H);
        }
    } else { //En caso de argumento inválido, se elimina del historial.
        delHistoric_aux(H);
    }
}

void Cmd_makefile(char *tr[]) {
    char ruta[MAXNAME], permisos[20], *nombre = tr[1];
    int archivo;
    struct stat info; //Esta estructura contiene la información sobre el archivo

    imprimirRuta(tr, ruta);

    if(tr[1] != NULL) {
        //Creamos el archivo con sus flags y permisos completos
        archivo = open(nombre, O_CREAT | O_WRONLY, 0775);

        if (archivo == -1) { //Si no se pudo crear el archivo (open devuelve -1)
            perror("Error al crear el archivo");
            return;
        }

        close(archivo); //Si se crea correctamente, se cierra el archivo

        if (lstat(nombre, &info) == 0) { //lstat obtiene la información del archivo y se guarda esta en info
            convierteModo(info.st_mode, permisos);
            printf("Permisos del archivo: %s\n", permisos);
        } else { //Si no se puede acceder a la información del archivo:
            perror("Error obteniendo permisos del archivo");
        }
    }
}

void Cmd_makedir (char *tr[]){
    char ruta[MAXNAME];
    imprimirRuta(tr, ruta);
    if(tr[1] != NULL) {
        if (mkdir(tr[1], 0755) == -1) { //Crea el directorio con los permisos completos (en caso de no poder, mkdir devuelve -1)
            perror ("Imposible crear directorio.");
        }
    }
}

void Cmd_listfile (char *tr[]) {
    char rutaabs[MAXNAME], *ruta, permisos[20], fyh[20];
    int  acc = 0, l_long = 0;

    //Definimos las estructuras:
    struct stat archivo;
    struct passwd *usuario; //Para el nombre del user
    struct group *grupo; //Para el nombre del grupo
    struct tm *tm_info;

    imprimirRuta(tr, rutaabs);

    if(tr[1] != NULL) {
        for (int i = 1; tr[i] != NULL; i++) { //Recorremos las palabras de la cadena:
            //Activamos los argumentos válidos
            if (strcmp(tr[i], "-acc") == 0) {
                acc = 1;
            } else if (strcmp(tr[i], "-long") == 0) {
                l_long = 1;
            } else{ //Si no es argumento:
                ruta = realpath(tr[i], rutaabs); //Pasamos tr[i] a rutaabas y lo almacenamos en ruta

                if(ruta == NULL) { //realpath devuelve nulo si falla y por tanto no hay ruta
                    perror("Error obteniendo ruta");
                }

                if(stat(ruta, &archivo) == 0) { //Obtenemos la información del archivo (lstat devuelve 0 en caso de fallo)

                    tm_info = localtime(&archivo.st_atime); //locaLtime convierte el tiempo de acceso (st_atime) en un formato de fecha y hora

                    usuario = getpwuid(archivo.st_uid); //getpwuid() obtiene el nombre de user del archivo (st_uid)
                    grupo = getgrgid(archivo.st_gid); //getgrgid() obtiene el nombre del grupo del archivo (st_gid)

                    strftime(fyh, 20, "%Y/%m/%d - %H:%M", tm_info); //Formateamos la fecha y la convierte en una cadena legible
                    convierteModo(archivo.st_mode, permisos);

                    //Comprobamos que argumentos están activos e imprimimos lo que hace falta:
                    if(l_long == 1) {
                        printf("%s   %lu ( %ld)   %s%s   %s   %lld   %s\n", fyh, archivo.st_nlink, archivo.st_ino,
                            grupo->gr_name, usuario->pw_name, permisos, (long long)archivo.st_size, tr[i]);
                        //Para imprimir el tamaño del archivo, convertimos la variable en long long para asegurarnos de poder
                        //imprimirlo correctamente dado que puede haber archivos de gran tamaño
                    } else if(acc == 1) {
                        printf("%lld   %s   %s\n", (long long)archivo.st_size, fyh,tr[i]);
                    } else {
                        printf("%lld   %s\n", (long long)archivo.st_size, tr[i]);
                    }
                } else {
                    perror("Error en la obtencion de la informacion del archivo");
                }
            }
        }
    }
}

void Cmd_cwd (int num){
    char dir[MAXNAME];
    if(num == 1) {
        printf("%s\n", getcwd(dir, MAXNAME)); //getcwd() obtiene la ruta del directorio actual
    }
}

void Cmd_listdir (char *tr[]) {
    DIR * dir;

    char rutaabs[MAXNAME], permisos[20], fyh[20], origen[1024];
    int hid = 0, acc = 0, l_long = 0, link = 0, tam;

    //Definimos las estructuras:
    struct dirent *contenido; //Para el contenido de los directorios
    struct stat archivo;
    struct tm *tm_info;
    struct passwd *usuario;
    struct group *grupo;


    imprimirRuta(tr, rutaabs);

    if(tr[1] != NULL) {
        for (int i = 1; tr[i] != NULL; i++) { //Recorremos las palabras de la cadena:
            //Activamos los argumentos válidos:
            if (strcmp(tr[i], "-hid") == 0) {
                hid = 1;
            } else if (strcmp(tr[i], "-acc") == 0) {
                acc = 1;
            } else if (strcmp(tr[i], "-long") == 0) {
                l_long = 1;
            } else if(strcmp(tr[i], "-link") == 0) {
                link = 1;
            } else { //Si no es un argumento:
                dir = opendir(tr[i]); //Abrimos el directorio tr[i] y guardamos su contenido en dir
                if (dir) { //Si se ha podido abrir: (opendir() devuelve nulo en caso de fallo)
                    printf( "************%s\n", tr[i]);

                    while ((contenido = readdir(dir)) != NULL) {
                    //readdir() lee la entrada de cada directorio y la asigna al contenido hasta que no queden elementos en dir

                        if (hid == 0 && contenido->d_name[0] == '.') {
                            continue;
                        }
                        //Metemos el nombre del archivo/directorio en la rutaabs para tener la ruta completa
                        snprintf(rutaabs, sizeof(rutaabs), "%s/%s", tr[i], contenido->d_name);

                        if (lstat(rutaabs, &archivo) == 0) { //Obtenemos la información del archivo (lstat devuelve -1 en caso de fallo)

                            usuario = getpwuid(archivo.st_uid); //getpwuid() obtiene el nombre de user del archivo (st_uid)
                            grupo = getgrgid(archivo.st_gid); //getgrgid() obtiene el nombre del grupo del archivo (st_gid)

                            tm_info = localtime(&archivo.st_atime); //locaLtime convierte el tiempo de acceso (st_atime) en un formato de fecha y hora
                            strftime(fyh, 20, "%Y/%m/%d - %H:%M", tm_info); //Formateamos la fecha y la convierte en una cadena legible
                            convierteModo(archivo.st_mode, permisos);

                            //Comprobamos que argumentos están activos e imprimimos lo que hace falta:
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
                            } else { // Asignamos el nombre aquí
                                printf("%10lld      %s\n", (long long) archivo.st_size, contenido->d_name);
                            }
                        } else {
                            perror("Error en la obtencion de la informacion del archivo");
                        }
                    }
                    closedir(dir);
                } else {
                    perror("Error al abrir el directorio.");
                }
            }
        }
    }
}

void Cmd_reclist (char *tr[]){
    char rutaabs[MAXNAME];
    int hid = 0, acc = 0, l_long = 0, link = 0;

    imprimirRuta(tr, rutaabs);

    if(tr[1] != NULL) {
        for (int i = 1; tr[i] != NULL; i++) { //Recorremos las palabras de la cadena:
            //Activamos los argumentos válidos:
            if (strcmp(tr[i], "-hid") == 0) {
                hid = 1;
            } else if (strcmp(tr[i], "-acc") == 0) {
                acc = 1;
            } else if (strcmp(tr[i], "-long") == 0) {
                l_long = 1;
            } else if(strcmp(tr[i], "-link") == 0) {
                link = 1;
            }else { //Si no es un argumento:
                strncpy(rutaabs, tr[i], MAXNAME); //Guardamos tr[i] en rutaabs
                reclist_aux(rutaabs, hid, acc, l_long, link);
            }
        }
    }
}

void Cmd_revlist (char *tr[]){
    char rutaabs[MAXNAME];
    int hid = 0, acc = 0, l_long = 0, link = 0;

    imprimirRuta(tr, rutaabs);
    if(tr[1] != NULL) {
        for (int i = 1; tr[i] != NULL; i++) { //Recorremos las palabras de la cadena:
            //Activamos los argumentos válidos:
            if (strcmp(tr[i], "-hid") == 0) {
                hid = 1;
            } else if (strcmp(tr[i], "-acc") == 0) {
                acc = 1;
            } else if (strcmp(tr[i], "-long") == 0) {
                l_long = 1;
            } else if(strcmp(tr[i], "-link") == 0) {
                link = 1;
            }
            else {
                strncpy(rutaabs, tr[i], MAXNAME); //Guardamos tr[i] en rutaabs
                revlist_aux(rutaabs, hid, acc, l_long, link);
            }
        }
    }
}

void Cmd_erase(char *tr[]) {
    char rutaabs[MAXNAME];
    //Si no nos pasan parametro se imprime la ruta
    if (!imprimirRuta(tr,rutaabs)){ //Si nos pasan parámetros
        for (int i = 1; tr[i] != NULL; i++) { //Recorremos la entrada desde tr[1]
            if (realpath(tr[i], rutaabs) == NULL) { //Intentamos conseguir la ruta del archivo
                perror("Error al obtener la ruta");
                continue;
            }

            if (remove(tr[i]) == 0) {  //Eliminamos el archivo o directorio si está vacío
                printf("Eliminado: %s\n", tr[i]);
            } else {
                perror("Error al eliminar");
            }
        }
    }
}

void Cmd_delrec (char *tr[]) {
    char rutaabs[MAXNAME];

    //Si no nos pasan parametro se imprime la ruta
    if(!imprimirRuta(tr, rutaabs)) { //Si nos pasan parámetros
        for(int i = 1; tr[i] != NULL; i++) { //Recorremos la entrada desde tr[1]
            delrec_aux(tr[i]); //Función auxiliar de delrec
        }
    }
}

void Cmd_allocate (char *tr[], int num, tListM *M, tListH H, tListF F) {
    if (num == 1) { //En caso de que el comando no tenga argumentos:
        printf("Lista de bloques asignados para el proceso %d:\n",getpid());
        printListM(*M);
    } else { //Si el comando tiene argumentos, comprobamos cuál es y hacemos que ejecute su función:
        if (strcmp("-malloc", tr[1]) == 0) {
            if (num == 2) {
                printf("Lista de bloques malloc asignados para el proceso %d:\n",getpid()); //imprimimos también el proceso en el que estamos
                printMalloc(*M);
            } else {
                do_AllocateMalloc(tr, M);
            }
        } else if (strcmp("-mmap", tr[1]) == 0){
            if (num == 2) {
                printf("Lista de bloques mmap asignados para el proceso %d:\n",getpid());
                printMmap(*M);
            } else {
                do_AllocateMmap(tr, M, &F);
            }
        } else if (strcmp("-createshared", tr[1]) == 0) {
            if(num == 2) {
                printf("Lista de bloques shared asignados para el proceso %d:\n",getpid());
                printShared(*M);
            } else {
                do_AllocateCreateshared(tr, M);
            }
        } else if (strcmp("-shared", tr[1]) == 0) {
            if(num == 2) {
                printf("Lista de bloques shared asignados para el proceso %d:\n",getpid());
                printShared(*M);
            } else {
                do_AllocateShared(tr, M);
            }
        } else {
            delHistoric_aux(&H);
        }
    }
}

void Cmd_deallocate (char *tr[], int num, tListM *M, tListH H, tListF F) {
    if (num == 1) { //En caso de que el comando no tenga argumentos:
        printf("Lista de bloques asignados para el proceso %d:\n",getpid());
        printListM(*M);
    } else { //Si el comando tiene argumentos, comprobamos cuál es y hacemos que ejecute su función:
        if (strcmp("-malloc", tr[1]) == 0) {
            if (num == 2) {
                printf("Lista de bloques malloc asignados para el proceso %d:\n",getpid());
                printMalloc(*M);
            } else {
               do_DeallocateMalloc(tr, M);
            }
        } else if (strcmp("-mmap", tr[1]) == 0){
            if (num == 2) {
                printf("Lista de bloques mmap asignados para el proceso %d:\n",getpid());
                printMmap(*M);
            } else {
                do_DeallocateMmap(tr, M, F);
            }
        } else if (strcmp("-shared", tr[1]) == 0) {
            if(num == 2) {
                printf("Lista de bloques shared asignados para el proceso %d:\n",getpid());
                printShared(*M);
            } else {
                do_DeallocateShared(tr, M);
            }
        } else if (strcmp("-delkey", tr[1]) == 0) {
            if(num == 2) {
                printf("delkey necesita una clave valida\n");
            } else {
                do_DeallocateDelkey(tr);
            }
        } else { //Si nos dan la dirección de memoria, la buscamos en la lista y comprobamos su tipo para saber como desasignarla:
            tPosM posM;
            void* addr;
            addr = (void*) strtoul(tr[1], NULL, 16);
            posM = findBlock(addr,*M);

            if (posM != NULL) {
                if (posM->block.type == MALLOC) {
                    free(addr);
                    deleteBlockM(M, posM);
                } else if (posM->block.type == SHARED) {
                    int id;
                    if (shmdt(addr)==-1) {
                        perror ("shmdt: imposible desvincular memoria compartida\n");
                    }
                    if ((id=shmget(posM->block.key,0,0666))==-1) {
                        perror ("shmget: imposible obtener memoria compartida\n");
                        return;
                    }
                    if (shmctl(id,IPC_RMID,NULL)==-1) {
                        perror ("shmctl: imposible eliminar memoria compartida\n");
                    }
                    deleteBlockM(M, posM);
                } else if (posM->block.type == MMAP) {
                    if (munmap(posM->block.address, posM->block.size) == -1) {
                        perror("munmap: imposible liberar memoria mapeada\n");
                    }
                    free(posM->block.file);
                    deleteBlockM(M, posM);

                    tPosF posF = findItemF(posM->block.df, F);
                    if (posF != NULL) {
                        deleteAtPositionF(&F, posF);
                    }
                }
            } else {
                printf("No se encontro un bloque de direccion de memoria %p\n", addr);
            }
        }
    }
}

void Cmd_memfill (char *tr[]) {
    if (tr[1] != NULL && tr[2] != NULL && tr[3]!=NULL) {
        //Convertimos las cadenas al tipo respectivo para cada una:
        size_t cont = (size_t) strtoul(tr[2], NULL, 10);
        void* addr = (void*) strtoul(tr[1], NULL, 16);
        int ch = (int) strtoul(tr[3], NULL, 10);

        llenarMemoria(addr, cont, ch);
        printf("Llenando %ld bytes de memoria con el byte %d (%c) a partir de la direccion %p\n", cont, ch, ch, addr);
    }
}

void Cmd_memdump (char *tr[]) {
    if (tr[1] != NULL) {
        void* addr = (void*) strtoul(tr[1], NULL, 16);
        unsigned char* addr_c = (unsigned char *)addr;
        size_t cont;

        if (tr[2] == NULL) {
            cont = 25;
        } else {
            cont = (size_t) strtoul(tr[2], NULL, 10);
        }

        printf("Volcando %ld bytes desde la direccion %p\n", cont, addr);
        //Recorremos los bytes de la dirección e imprimimos sus caracteres:
        for (size_t i = 0; i < cont; i++) {
            if (isprint(addr_c[i])) { //isprint() comprueba si el caracter dado es imprimible
                printf("%-c\t", addr_c[i]);
            } else { //Si no lo es, imprime un espacio
                printf("%-3c\t",' ');
            }
        }
        printf("\n");

        //Recorremos los bytes de la dirección e imprimimos sus valores en hexadecimal:
        for (size_t i = 0; i < cont; i++) {
            printf("%02x\t", addr_c[i]);
        }
        printf("\n");
    }
}

//Creamos las variables globales:
int global1,global2, global3;
int global_i1 = 666, global_i2 = 13, global_i3 = 33;
void Cmd_memory (char *tr[], tListM M, tListH H) {
    //Creamos las variables locales:
    int local1;
    char local2;
    float local3;

    //Creamos las variables estáticas:
    static float static1, static2, static3;
    static double g = 9.80665, e = 2.718281828459, pi = 3.14159265358979;

    if(tr[1] == NULL || strcmp("-all", tr[1]) == 0) {
        //imprimir variables
        printf("Variables locales (automaticas):   %p,   %p,   %p\n", (void*)&local1, (void*)&local2, (void*)&local3);
        printf("Variables globales:                %p,   %p,   %p\n", (void*)&global_i1, (void*)&global_i2, (void*)&global_i3);
        printf("Var (N.I) globales:                %p,   %p,   %p\n", (void*)&global1, (void*)&global2, (void*)&global3);
        printf("Variables estaticas:               %p,   %p,   %p\n", (void*)&g, (void*)&e, (void*)&pi);
        printf("Variables (N.I) estaticas:         %p,   %p,   %p\n", (void*)&static1, (void*)&static2, (void*)&static3);
        printf("\n");
        printf("Funciones de programa:   %p,  %p,  %p\n", (void*)Cmd_authors, (void*)Cmd_listdir, (void*)Cmd_writefile);
        printf("Funciones de libreria:   %p,  %p,  %p\n", (void*)printf, (void*)strtoul, (void*)perror);
        printf("\n******Lista de bloques asignados para el proceso %d:\n",getpid());
        printListM(M);
    } else {
        if (strcmp("-funcs", tr[1]) == 0) {
            printf("Funciones de programa:   %p,  %p,  %p\n", (void*)Cmd_authors, (void*)Cmd_listdir, (void*)Cmd_writefile);
            printf("Funciones de libreria:   %p,  %p,  %p\n", (void*)printf, (void*)strtoul, (void*)perror);
        } else if (strcmp("-vars", tr[1]) == 0) {
            printf("Variables locales (automaticas):   %p,   %p,   %p\n", (void*)&local1, (void*)&local2, (void*)&local3);
            printf("Variables globales:                %p,   %p,   %p\n", (void*)&global_i1, (void*)&global_i2, (void*)&global_i3);
            printf("Var (N.I) globales:                %p,   %p,   %p\n", (void*)&global1, (void*)&global2, (void*)&global3);
            printf("Variables estaticas:               %p,   %p,   %p\n", (void*)&g, (void*)&e, (void*)&pi);
            printf("Variables (N.I) estaticas:         %p,   %p,   %p\n", (void*)&static1, (void*)&static2, (void*)&static3);
        } else if (strcmp("-blocks", tr[1]) == 0) {
            printf("Lista de bloques asignados para el proceso %d:\n",getpid());
            printListM(M);
        } if (strcmp("-pmap", tr[1]) == 0) {
            do_pmap();
        } else {
            delHistoric_aux(&H);
        }
    }
}

void Cmd_readfile (char *tr[]) {
    void *p;
    size_t cont = -1;  //Si no pasamos tamaño leemos el archivo entero
    ssize_t n;
    if (tr[1] == NULL || tr[2] == NULL){
        printf ("Faltan parametros\n");
        return;
    }
    p = cadtop(tr[2]);  //Convertimos la cadena en un puntero

    if (tr[3] != NULL) { //Si nos pasan tamaño
        cont = (size_t) atoll(tr[3]); //Conseguimos el tamaño
    }

    if ((n = leerFichero(tr[1], p, cont)) == -1) { //Llamamos a la función auxiliar para leer el archivo
        perror ("Imposible leer fichero");
    }else{
        printf ("Leidos %lld bytes de %s en la dirección %p\n",(long long) n, tr[1], p);
    }
}

void Cmd_writefile (char *tr[]) {
    void *p;
    FILE *archivo;
    size_t cont; //En este caso nos tienen que pasar el tamaño
    ssize_t n;

    if (tr[1] == NULL || tr[2] == NULL || tr[3] == NULL){
        printf ("Faltan parametros\n");
        return;
    }

    p = cadtop(tr[2]); //Convertimos la cadena a puntero
    cont = (size_t) atoll(tr[3]); //Conseguimos el tamaño

    if (access(tr[1], F_OK) == 0) { //Comprobamos si el archivo ya existe
        errno = EEXIST;
        perror("Imposible escribir fichero");
        return;
    }

    archivo = fopen(tr[1], "wb"); //Creamos el archivo en modo escritura binaria

    if (archivo == NULL){ //Si no se ha creado correctamente
        perror ("Error al crear el archivo");
        return;
    }

    if ((n = escribirFichero(tr[1], p, cont)) == -1){ //Llamamos a la función auxiliar para escribir el archivo
        perror ("Imposible escribir fichero");
    }else {
        printf ("Escritos %lld bytes de %s en la dirección %p\n",(long long) n, tr[1], p);
    }
}

void Cmd_read (char *tr[], tListF F) {
    void *p;
    tPosF posF;
    size_t cont = -1;  //Si no pasamos tamaño leemos el archivo entero
    ssize_t n;
    if (tr[1] == NULL || tr[2] == NULL){
        printf ("Faltan parametros\n");
        return;
    }
    p = cadtop(tr[2]);  //Convertimos la cadena en un puntero

    posF = findItemF(atoi(tr[1]), F); //Buscamos el archivo dado su descriptor
    if(posF == FNULL) {
        printf("Error al leer el descriptor %s a la direccion %p: Bad file descriptor\n", tr[1], p);
        return;
    }
    if (tr[3] != NULL) { //Si nos pasan tamaño
        cont = (size_t) atoll(tr[3]); //Conseguimos el tamaño
    }
    if ((n = leerFichero(posF->file.name, p, cont)) == -1) { //Llamamos a la función auxiliar para leer el archivo
        perror ("Imposible leer fichero");

    }else{
        printf ("Leidos %lld bytes del descriptor %d en la dirección %p\n",(long long) n, posF->file.df, p);

    }
}

void Cmd_write (char *tr[], tListF F) {
    void *p;
    tPosF posF;
    size_t cont; //En este caso nos tienen que pasar el tamaño
    ssize_t n;
    char *modo;

    if (tr[1] == NULL || tr[2] == NULL || tr[3] == NULL){
        printf ("Faltan parametros\n");
        return;
    }

    p = cadtop(tr[2]);  //Convertimos la cadena en un puntero
    cont = (size_t) atoll(tr[3]); //Asignamos el tamaño a escribir

    posF = findItemF(atoi(tr[1]), F); //Buscamos el archivo dado su descriptor
    if(posF == FNULL) {
        printf("Error al escribir en el descriptor %s a la direccion %p: Bad file descriptor\n", tr[1], p);
        return;
    }

    modo = mtos(posF->file.mode); //Convertimos el modo en un string
    if (strcmp(modo, "write-only") == -1 && strcmp(modo, "read-write") == -1 ) { //Si no está en modo escritura, no podemos escribir en el archivo
        printf("Error al escribir en el descriptor %s a la direccion %p: Bad file descriptor\n", tr[1], p);
        return;
    }
    if ((n = escribirFichero(posF->file.name, p, cont)) == -1) { //Llamamos a la función auxiliar
        perror ("Imposible escribir fichero");
    }else {
        printf ("Escritos %lld bytes del descriptor %d en la dirección %p\n",(long long) n, posF->file.df, p);
    }
}

void Cmd_recurse (char *tr[]) {
    if(tr[1] != NULL) {
        int n = atoi(tr[1]);
        recursiva(n); //Llamamos a la función recursiva n veces
    }
}

void Cmd_getuid () {
    struct passwd *user_real, *user_efectivo; //Para el nombre del user

    user_real = getpwuid(getuid()); //Hace que user_real apunte a un struct passwd que contiene la info del usuario
    printf("Credencial real: %d (%s)\n", getuid(), user_real->pw_name);

    user_efectivo = getpwuid(geteuid()); //Hace que user_efectivo apunte a un struct passwd que contiene la info del usuario
    printf("Credencial efectiva: %d (%s)\n", geteuid(), user_efectivo->pw_name);
}

void Cmd_setuid (char *tr[], tListH *H) {
    if (tr[1] != NULL) {
        //Cambia las credenciales del user efectivo a las pedidas mediante el login
        if (strcmp(tr[1], "-l") == 0) {
            struct passwd *user;
            user = getpwnam(tr[2]); //getpwnam() -> encuentra el user
            if (user != NULL) {
                if (seteuid(user->pw_uid) == -1) {
                    perror("Imposible cambiar credencial");
                    errno = EPERM;
                }
            } else {
                printf("Usuario '%s' no existente.\n", tr[2]);
            }
        } else {
            //Cambia las credenciales del user efectivo a las pedidas mediante el uid
            if (seteuid(strtoul(tr[1], NULL, 10)) == -1) {
                perror("Imposible cambiar credencial");
                errno = EPERM;
            }
        }
    } else {
        delHistoric_aux(H);
    }
}

extern char **environ;  //Definimos el entorno mediante environ
void Cmd_showvar (char *tr[], char *envp[], tListH *H) {
    int p = 0;
    if (tr[1] == NULL) { //Si entra solo showvar, devuelve todas las variables del entorno
        //Recorremos el entorno (arg3) e imprimimos todas las variables
        int i = 0;
        for (char **env = envp; *env != NULL; env++) {
            printf("%p -> main arg3[%d] = (%p)  %s\n",(void*)env, i, (void*)*env, *env);
            i++;
        }
    } else {
        for (int i = 1; tr[i] != NULL; i++) { //Recorremos todos los argumentos que son variables y vamos imprimiendo la info de cada uno
            //Con arg3:
            p = buscarVariable(tr[i], envp);
            if (p != -1) {
                printf("Variable %s:\n", tr[i]);
                printf("\tCon main arg3   %s (%p) @%p\n",envp[p], envp[p], &envp[p]);
            }
            //Con environ:
            p = buscarVariable(tr[i], environ);
            if (p != -1) {
                printf("\tCon environ     %s (%p) @%p\n",environ[p], environ[p], &environ[p]);
            }

            //Con getenv:
            if (getenv(tr[i]) != NULL) {
                printf("\tCon getenv      %s=%s (%p)\n", tr[i], getenv(tr[i]), getenv(tr[i]));
            }
        }
    }
}

void Cmd_changevar (char *tr[], char *envp[], tListH *H) {
    if (tr[1] != NULL) {
        int pos;
        if (strcmp(tr[1], "-a") == 0) { //Cambiamos la variable por el arg3 del main
            if (cambiarVariable(tr[2], tr[3], envp) == -1) {
                perror("Variable no encontrada en el entorno");
                errno = EBADR;
            }
        } else if (strcmp(tr[1], "-e") == 0) { //Cambiamos la variable por environ
            if (cambiarVariable(tr[2], tr[3], environ) == -1) {
                perror("Variable no encontrada en el entorno");
                errno = EBADR;
            }
        } else if (strcmp(tr[1], "-p") == 0) { //Añadimos variables usando putenv
            char *v2 = malloc(strlen(tr[2]) + strlen(tr[3]) + 2); //v2=val\0
            if (v2 == NULL) {
                perror("Error al asignar memoria");
                errno = ENOMEM;
                return;
            }

            //Guarda en v2 el contenido de los strings dados en el formato solicitado
            snprintf(v2, strlen(tr[2]) + strlen(tr[3]) + 2, "%s=%s", tr[2], tr[3]);
            if (unsetenv(tr[2]) != 0) {
                perror("Error eliminando la variable antigua");
                return;
            }
            if (putenv(v2) != 0) {
                perror("Error añadiendo la nueva variable");
            }
            free(v2);
        } else {
            perror("Variable no encontrada en el entorno");
            errno = EBADR;
        }
    } else {
        delHistoric_aux(H);
    }
}

void Cmd_subsvar (char *tr[], char *envp[], tListH *H) {
    if (tr[1] != NULL) {
        char *v2 = malloc(strlen(tr[3]) + strlen(tr[4]) + 2); //v2=val\0
        if (v2 == NULL) {
            perror("Error al asignar memoria");
            errno = ENOMEM;
            return;
        }

        //Guarda en v2 el contenido de los strings dados en el formato solicitado
        snprintf(v2, strlen(tr[3]) + strlen(tr[4]) + 2, "%s=%s", tr[3], tr[4]);

        if (strcmp(tr[1], "-a") == 0) { //Cambiamos en el entorno desde el arg3
            int pos;
            if ((pos = buscarVariable(tr[2], envp)) == -1) {
                printf("No se encontró la variable %s\n", tr[2]);
                free(v2);
                return;
            }
            envp[pos] = strdup(v2);
        } else if (strcmp(tr[1], "-e") == 0) { //Cambiamos en el entorno desde environ
            if (unsetenv(tr[2]) != 0) {
                perror("Error eliminando la variable antigua");
                return;
            }
            if (putenv(v2) != 0) {
                perror("Error añadiendo la nueva variable");
            }
        } else {
            delHistoric_aux(H);
        }
    } else {
        delHistoric_aux(H);
    }
}

void Cmd_environ (char *tr[], char *envp[], tListH *H) {
    if (tr[1] == NULL) {
        //Recorremos el entorno (arg3) e imprimimos todas las variables
        int i = 0;
        for (char **env = envp; *env != NULL; env++) {
            printf("%p -> main arg3[%d] = (%p)  %s\n",(void*)env, i, (void*)*env, *env);
            i++;
        }
    } else if  (strcmp(tr[1], "-environ") == 0) {
        //Recorremos el entorno (environ) e imprimimos todas las variables
        int i = 0;
        for (char **env = environ; *env != NULL; env++) {
            printf("%p -> environ[%d] = (%p)  %s\n",(void*)env, i, (void*)*env, *env);
            i++;
        }
    } else if (strcmp(tr[1], "-addr") == 0) {
        //Imprimimos las direcciones de memoria de las variables
        printf("environ: %p (almacenado en %p)\n", environ, (void*)&environ);
        printf("main arg3: %p (almacenado en %p)\n", envp, (void*)&envp);
    } else {
        delHistoric_aux(H);
    }
}

void Cmd_fork(tListJ *J) {
    pid_t pid;

    if ((pid = fork()) == 0) {
        emptyListJ(J); //VaciarListaProcesos(&LP); Depende de la implementación de cada uno
        printf("ejecutando proceso %d\n", getpid());
    } else if (pid != -1) {
        waitpid(pid,NULL, 0);
    }
}

void Cmd_search(char *tr[], tListD *D, tListH *H) {
    if (tr[1] == NULL) {
        printListD(*D);
    } else {
        if (tr[2] != NULL) {
            //Comprobamos que la ruta sea válida y, en caso de que lo sea, nos aseguramos de que es un directorio
            struct stat dir;
            if (stat(tr[2], &dir) != 0) {
                perror("Ruta no valida");
                return;
            }
            if (S_ISDIR(dir.st_mode)) {
                //Miramos en que caso estamos:
                if (strcmp(tr[1], "-add") == 0) {
                    insertDir(D, tr[2]);
                } else if (strcmp(tr[1], "-del") == 0) {
                    tPosD posD = findDir(*D, tr[2]);
                    deleteDir(D, posD);
                }
            } else {
                perror("La ruta no pertenece a un directorio");
                errno = ENOTDIR;
            }
        } else {
            if (strcmp(tr[1], "-clear") == 0) {
                emptyListD(D);
            } else if (strcmp(tr[1], "-path") == 0) {
                char *path = getenv("PATH"); //getenv() -> devuelve el entorno del PATH
                if (path != NULL) {
                    int num = 0;
                    char *dirs = strdup(path), *trpath = strtok(dirs, ":");
                    //Hacemos un duplicado del entorno del path para evitar problemas con el strtok
                    while (trpath != NULL) {
                        //Vamos insertando uno a uno los dir del path
                        num++;
                        insertDir(D, trpath);
                        trpath = strtok(NULL, ":");
                    }
                    free(dirs);
                    printf("Importados %d directorios en la ruta de busqueda\n", num);
                } else {
                    perror("PATH no definido");
                }
            } else {
                delHistoric_aux(H);
            }
        }
    }
}

void Cmd_exec (char *tr[], tListD D) {
    char **cad;
    char **env;
    int numvars = 0;
    if (tr[1] == NULL) {
        return;
    }

    for (int i = 1; tr[i] != NULL; i++) {
        if (isVar(tr[i])) {
            numvars++; //Contamos las variables del entorno dadas
        } else {
            break;
        }
    }
    cad = &tr[numvars + 1]; //Guardamos el comando después de las variables de entorno
    env = malloc((numvars + 1) * sizeof(char *));
    if (env == NULL) {
        return; //Si el malloc falla
    }
    int k = 0;

    for (int j = 1; tr[j] != NULL; j++) {
        if (isVar(tr[j])) {
            env[k] = tr[j]; //Copiar la variable de entorno en el array
            k++;
        } else {
            break;
        }
    }
    env[k] = NULL; //El último valor debe ser null

    char *rutaabs = Ejecutable(D, cad[0]);

    if (rutaabs == NULL || strcmp(rutaabs, cad[0]) == 0) { //Si no lo encontramos
        errno = ENOENT;
        perror("No ejecutado");
    }
    if (numvars > 0) { //Ejecutamos el programa con las variables
        execve(rutaabs, cad, env);
    } else {
        execv(rutaabs, cad);
    }
    free(env);
}

void Cmd_execpri (char *tr[], tListD D) {
    char **cad;
    char **env;
    int prio, numvars = 0;

    if (tr[1] == NULL || tr[2] == NULL) {
        return;
    }

    for (int i = 2; tr[i] != NULL; i++) { //Comenzamos el bucle después de la prio
        if (isVar(tr[i])) {
            numvars++; //Contamos las variables del entorno dadas
        } else {
            break;
        }
    }
    prio = atoi(tr[1]); //Conseguimos la prioridad
    cad = &tr[numvars + 2]; //Guardamos el comando después de las variables de entorno y la prioridad

    char *rutaabs = Ejecutable(D, cad[0]); //Comprobamos si es un comando ejecutable

    if (rutaabs == NULL || strcmp(rutaabs, cad[0]) == 0) { //Si no lo encontramos
        errno = ENOENT;
        perror("No ejecutado");
    }

    if (setpriority(PRIO_PROCESS, 0, prio) == -1) { //Cambiamos la prioridad del proceso
        perror("Error al establecer la prioridad");
        exit(-1);  //Si el comando falla, exit code -1
    }

    if (numvars == 0) {
        execv(rutaabs, cad); //Ejecutamos el comando normal
    } else{
        env = malloc((numvars + 1) * sizeof(char *)); //Reservamos memoria para el nuevo entorno
        if (env == NULL) {
            return;
        }

        for (int j = 0; j < numvars; j++) {
            int pos = buscarVariable(tr[j + 2], environ); //Buscamos la variable después de execpri y de prioridad
            env[j] = environ[pos];
        }
        env[numvars] = NULL; //Terminamos el entorno con NULL
        execve(rutaabs, cad, env); //Ejecutamos el programa con las variables
        free(env);
    }
}

void Cmd_fg (char *tr[], tListD D) {
    pid_t pid;
    char **cad;
    char **env;
    int numvars = 0;

    for (int i = 0; tr[i] != NULL; i++) { //Recorremos tr[] buscando variables de entorno
        if (strcmp(tr[i], "fg") == 0) { //Saltamos el fg
            continue;
        }
        if (isVar(tr[i])) {
            numvars++; //Contamos las variables del entorno dadas
        } else {
            break;
        }
    }

    if (strcmp(tr[0], "fg") == 0) {
        if (tr[1] == NULL) { //Si no nos pasan comando
            errno = EFAULT;
            perror("No ejecutado");
            return;
        }
        cad = &tr[numvars + 1]; //Si el primer comando es fg, el comando empieza después de las variables de entorno +1
    } else {
        cad = &tr[numvars]; //Si el primer comando no es fg, el comando empieza después de las variables de entorno
    }

    char *rutaabs = Ejecutable(D, cad[0]); //Comprobamos si es un comando ejecutable

    if (rutaabs == NULL || strcmp(rutaabs, cad[0]) == 0) { //Si no lo encontramos
        errno = ENOENT;
        perror("No ejecutado");
        return;
    }

    pid = fork(); //Creamos un proceso hijo
    if (pid < 0) {
        errno = ENOENT;
        perror("No ejecutado");
    } else if (pid == 0) { //En el proceso hijo
        if (numvars == 0) {
            execv(rutaabs, cad); //Ejecutamos el comando normal
        } else {
            env = malloc((numvars + 2) * sizeof(char *)); //Reservamos memoria para el nuevo entorno
            if (env == NULL) {
                return;
            }

            for (int j = 0; j < numvars; j++) {
                int pos;
                if (strcmp(tr[0], "fg") == 0) {
                    pos = buscarVariable(tr[j + 1], environ);//Buscamos las variables después de fg
                } else {
                    pos = buscarVariable(tr[j], environ);
                }
                env[j] = environ[pos];
            }
            env[numvars] = NULL; //Terminamos el entorno con NULL
            execve(rutaabs, cad, env); //Ejecutamos el programa con las variables
            free(env);
        }
    } else { //En el proceso padre
        int status;
        if (waitpid(pid, &status, 0) == -1) { //Esperamos a que termine el hijo
            perror("Error esperando al hijo");
        }
    }
}

void Cmd_fgpri (char *tr[], tListD D, tListJ *J) {
    pid_t pid;
    char **cad;
    char **env;
    int prio, numvars = 0;

    if (tr[1] == NULL || tr[2] == NULL) { //Si no nos pasan comando
        errno = EFAULT;
        perror("No ejecutado");
        return;
    }

    for (int i = 2; tr[i] != NULL; i++) { //Buscamos las variables después de la prioridad
        if (isVar(tr[i])) {
            numvars++; //Contamos las variables del entorno dadas
        } else {
            break;
        }
    }

    prio = atoi(tr[1]); //Conseguimos el valor de la prioridad
    cad = &tr[numvars + 2]; //Después de fg y de la prioridad

    char *rutaabs = Ejecutable(D, cad[0]); //Comprobamos si es un comando ejecutable

    if (rutaabs == NULL || strcmp(rutaabs, cad[0]) == 0) {
        errno = ENOENT;
        perror("No ejecutado");
        return;
    }

    pid = fork(); //Creamos el proceso hijo
    if (pid < 0) { //Error
        errno = ENOENT;
        perror("No ejecutado");
    } else if (pid == 0) { //En el proceso hijo

        if (setpriority(PRIO_PROCESS, 0, prio) == -1) {
            perror("Error al establecer la prioridad");
            _exit(EXIT_FAILURE);  //Terminamos el hijo si setpriority falla
        }
        if (numvars == 0) {
            execv(rutaabs, cad); //Ejecutamos el programa normal
        } else {
            env = malloc((numvars + 1) * sizeof(char *)); //Reservamos memoria para el nuevo entorno
            if (env == NULL) {
                return;
            }
            for (int j = 0; j < numvars; j++) {
                int pos = buscarVariable(tr[j + 2], environ); //Buscamos las variables después de fg y prioridad
                env[j] = environ[pos];
            }
            env[numvars] = NULL; //Terminamos el entorno con NULL
            execve(rutaabs, cad, env); //Ejecutamos el programa con las variables
            free(env);
        }
    } else { //En el proceso padre
        int status;
        if (waitpid(pid, &status, 0) == -1) { //Espera a que el hijo termine
            perror("Error esperando al hijo");
        }
    }
}

void Cmd_back (char *tr[], tListD D, tListJ *J) {
    pid_t pid;
    char **cad;
    char **env;
    int numvars = 0;

    if (tr[1] == NULL) { //Si no nos pasan comando
        errno = EFAULT;
        perror("No ejecutado");
        return;
    }

    for (int i = 1; tr[i] != NULL; i++) {
        if (isVar(tr[i])) {
            numvars++; //Contamos las variables del entorno dadas
        } else {
            break;
        }
    }

    cad = &tr[numvars + 1];

    char *rutaabs = Ejecutable(D, cad[0]); //Comprobamos si es un comando ejecutable

    if (rutaabs == NULL || strcmp(rutaabs, cad[0]) == 0) {
        errno = ENOENT;
        perror("No ejecutado");
    }

    pid = fork(); //Creamos un proceso hijo

    if (pid < 0) { //Error en fork
        errno = ENOENT;
        perror("No ejecutado");
    }
    if (pid == 0) { //En el proceso hijo
        if (numvars == 0) {
            if (execv(rutaabs, cad) == -1) { //Ejecutamos el proceso hijo
                exit(-1);  //Si no se ejecuta, exit code -1
            }
        }else{
            env = malloc((numvars + 1) * sizeof(char *)); //Reservamos memoria para el nuevo entorno
            if (env == NULL) {
                return;
            }

            for (int j = 0; j < numvars; j++) {
                int pos = buscarVariable(tr[j + 1], environ); //Buscamos las variables después de back
                env[j] = environ[pos];
            }
            env[numvars] = NULL; //Terminamos el entorno con NULL
            if (execve(rutaabs, cad, env) == -1) { //Ejecutamos el programa con las variables
                exit(-1); //Si no se ejecuta, exit code -1
            }
            free(env);
        }

    }else{ //En el proceso padre
        if (!insertJob(J, pid, cad[0])) {
            perror("Error en la lista de procesos");
        }
    }
}

void Cmd_backpri (char *tr[], tListD D, tListJ *J) {
    pid_t pid;
    char **cad;
    char **env;
    int prio, numvars = 0;

    if (tr[1] == NULL || tr[2] == NULL) { //Si no nos pasan comando
        errno = EFAULT;
        perror("No ejecutado");
        return;
    }

    for (int i = 2; tr[i] != NULL; i++) { //Buscamos las variables después de la prioridad
        if (isVar(tr[i])) {
            numvars++; //Contamos las variables del entorno dadas
        } else {
            break;
        }
    }

    prio = atoi(tr[1]); //Conseguimos el valor de la prioridad
    cad = &tr[numvars + 2]; //Después de la prioridad

    char *rutaabs = Ejecutable(D, cad[0]); //Comprobamos si es un comando ejecutable

    if (rutaabs == NULL || strcmp(rutaabs, cad[0]) == 0) {
        errno = ENOENT;
        perror("No ejecutado");
    }

    pid = fork(); //Creamos un proceso hijo

    if (pid < 0){ //Error en fork
        errno = ENOENT;
        perror("No ejecutado");
    } else if (pid == 0) { //En el proceso hijo

        if (setpriority(PRIO_PROCESS, 0, prio) == -1) {
            perror("Error al establecer la prioridad");
            exit(EXIT_FAILURE);  //Terminamos el hijo si setpriority falla
        }
        if (numvars == 0) {
            if (execv(rutaabs, cad) == -1) { //Ejecutamos el proceso hijo
                exit(-1);  //Si no se ejecuta, exit code -1
            }
        } else {
            env = malloc((numvars + 1) * sizeof(char *)); //Reservamos memoria para el nuevo entorno
            if (env == NULL) {
                return;
            }
            for (int j = 0; j < numvars; j++) {
                int pos = buscarVariable(tr[j + 2], environ); //Buscamos la variable después de back y prioridad
                env[j] = environ[pos];
            }
            env[numvars] = NULL; //Terminamos el entorno con NULL
            execve(rutaabs, cad, env); //Ejecutamos el programa con las variables
            free(env);
        }

    } else { //En el proceso padre
        if (!insertJob(J, pid, cad[0])) {
            perror("Error en la lista de procesos");
        }
    }
}

void Cmd_listjobs (tListJ J) {
    updateJob(&J); //Actualizamos la lista antes de imprimirla
    printListJ(J);
}

void Cmd_deljobs (char *tr[], tListJ *J) {
    if (tr[1] == NULL) { //Si no nos pasan parametros vaciamos la lista
        emptyListJ(J);
    } else {
        if (strcmp (tr[1], "-term") == 0) {
            deleteJob(J, FINISHED);
        } else if (strcmp (tr[1], "-sig") == 0) {
            deleteJob(J, SIGNALED);
        }
    }
}
