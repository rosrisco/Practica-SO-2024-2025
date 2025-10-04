#include "jobs.h"

static struct SEN sigstrnum[]={
    {"HUP", SIGHUP},
    {"INT", SIGINT},
    {"QUIT", SIGQUIT},
    {"ILL", SIGILL},
    {"TRAP", SIGTRAP},
    {"ABRT", SIGABRT},
    {"IOT", SIGIOT},
    {"BUS", SIGBUS},
    {"FPE", SIGFPE},
    {"KILL", SIGKILL},
    {"USR1", SIGUSR1},
    {"SEGV", SIGSEGV},
    {"USR2", SIGUSR2},
    {"PIPE", SIGPIPE},
    {"ALRM", SIGALRM},
    {"TERM", SIGTERM},
    {"CHLD", SIGCHLD},
    {"CONT", SIGCONT},
    {"STOP", SIGSTOP},
    {"TSTP", SIGTSTP},
    {"TTIN", SIGTTIN},
    {"TTOU", SIGTTOU},
    {"URG", SIGURG},
    {"XCPU", SIGXCPU},
    {"XFSZ", SIGXFSZ},
    {"VTALRM", SIGVTALRM},
    {"PROF", SIGPROF},
    {"WINCH", SIGWINCH},
    {"IO", SIGIO},
    {"SYS", SIGSYS},
   /*senales que no hay en todas partes*/
   #ifdef SIGPOLL
    {"POLL", SIGPOLL},
   #endif
   #ifdef SIGPWR
    {"PWR", SIGPWR},
   #endif
   #ifdef SIGEMT
    {"EMT", SIGEMT},
   #endif
   #ifdef SIGINFO
    {"INFO", SIGINFO},
   #endif
   #ifdef SIGSTKFLT
    {"STKFLT", SIGSTKFLT},
   #endif
   #ifdef SIGCLD
    {"CLD", SIGCLD},
   #endif
   #ifdef SIGLOST
    {"LOST", SIGLOST},
   #endif
   #ifdef SIGCANCEL
    {"CANCEL", SIGCANCEL},
   #endif
   #ifdef SIGTHAW
    {"THAW", SIGTHAW},
   #endif
   #ifdef SIGFREEZE
    {"FREEZE", SIGFREEZE},
   #endif
   #ifdef SIGLWP
    {"LWP", SIGLWP},
   #endif
   #ifdef SIGWAITING
    {"WAITING", SIGWAITING},
   #endif
     {NULL,-1},
    };    /*fin array sigstrnum */


char * enumTocharJ(enum Status status) {
    switch (status){
        case FINISHED:
            return "finished";
        case STOPPED:
            return "stopped";
        case SIGNALED:
            return "signaled";
        case ACTIVE:
            return "active";
        default: return "error";
    }
}

char *NombreSenal(int sen){  /*devuelve el nombre senal a partir de la senal*/
    /* para sitios donde no hay sig2str*/
    int i;
    for (i=0; sigstrnum[i].nombre!=NULL; i++)
        if (sen==sigstrnum[i].senal)
            return sigstrnum[i].nombre;
    return ("SIGUNKNOWN");
}


void createEmptyListJ (tListJ *J) {
    *J = JNULL;
}

void emptyListJ (tListJ *J) {
    tPosJ posJ = *J; //Declaramos el puntero a la lista
    tPosJ next;

    while (posJ != JNULL) {
        next = posJ->next; //Guardamos el siguiente nodo
        free(posJ->job.command_line);
        free(posJ); //Liberamos el nodo
        posJ = next;
    }
    *J = NULL;
}

void printListJ (tListJ J) {
    tPosJ posJ;
    char fyh[20];
    int prio;
    char senal[32];

    for (posJ = J; posJ != JNULL; posJ = posJ->next) {
        strftime(fyh, sizeof(fyh), "%Y/%m/%d - %H:%M", posJ->job.time); //Guardamos la fecha y hora en fyh con el formato pedido

        if (posJ->job.status == FINISHED){ //Si un proceso ha terminado su prioridad siempre va a ser -1
            prio = -1;
        }else{
            prio = getpriority(PRIO_PROCESS, posJ->job.pid); //Conseguimos la prioridad con el pid del proceso
        }

        if (posJ->job.status == SIGNALED){ //Si un proceso ha terminado por señal imprimimos la señal por la que terminó
            snprintf(senal, sizeof(senal), "%s", NombreSenal(posJ->job.exit));
        }else{
            snprintf(senal, sizeof(senal), "%03d", posJ->job.exit); //Si no ha terminado por señal, imprimimos su codigo de salida
        }

        printf("%d\t %s p = %d %s %s (%s) %s \n", posJ->job.pid, posJ->job.username, prio, fyh, enumTocharJ(posJ->job.status), senal,  posJ->job.command_line);
    }
}

void deleteJob(tListJ *J, enum Status status) {
    tPosJ prev = JNULL;
    tPosJ next;

    for (tPosJ posJ = *J; posJ != JNULL; ) { //Recorremos la lista
        if (posJ->job.status == status) { //Si el estado es igual eliminamos ese proceso
            next = posJ->next;

            if (prev == JNULL) {
                *J = next;
            } else {
                prev->next = next;
            }
            free(posJ->job.command_line);
            free(posJ);
            posJ = next;
        } else {
            prev = posJ;
            posJ = posJ->next;
        }
    }
}



bool insertJob (tListJ *J, int pid, char *command_line) {
    tPosJ q, r;
    time_t fyh; //Declaramos fyh como una variable de tiempo en segundos.
    struct tm * tm_info; //Esta estructura guarda la fecha (días, meses, años) y la hora (horas, minutos, segundos) desglosada cada una en sus respectivas partes.
    struct passwd *user;
    struct stat job_stat;
    char ruta[256];

    q = malloc(sizeof(struct tNodeJ)); //Inicializamos el nodo
    if (q == JNULL) {
        return false;
    }

    //Obtiene la hora del sistema en segundos y la almacena en fyh
    time(&fyh);
    tm_info = localtime(&fyh); //localtime() convierte la información de fyh y la almacena en el struct tm, al cual apuntará tm_info

    //Inicializamos los valores del bloque:
    q->job.pid = pid;
    q->job.time = tm_info;
    q->job.status = ACTIVE;
    q->job.command_line = strdup(command_line);

    snprintf(ruta, sizeof(ruta), "/proc/%d", pid); //Conseguimos la ruta del proceso
    stat(ruta, &job_stat);

    user = getpwuid(job_stat.st_uid);
    if (user == NULL) {
        perror("Error obteniendo el usuario");
    }else{
        strcpy(q->job.username, user->pw_name);
    }


    //Inicializamos los valores de la posición:
    q->next = JNULL;
    q->prev = JNULL;

    if (*J == JNULL) {
        //Si la lista está vacía, q es la primera posición.
        *J = q;
    } else {
        for (r = *J; r->next != JNULL; r = r->next){}
        r->next = q;
        q->prev = r;
    }
    return true;
}

bool updateJob(tListJ *J) {
    tPosJ posJ;
    int info;
    pid_t status;
    int pid;

    for (posJ = *J; posJ != JNULL; posJ = posJ->next){ //Recorremos la lista de procesos

        if (posJ->job.status == FINISHED || posJ->job.status == SIGNALED){ //Si un proceso ya ha terminado, lo saltamos
            continue;
        }
        pid = posJ->job.pid; //Guardamos el pid del proceso
        status = waitpid(pid, &info, WUNTRACED | WNOHANG); //Comprobamos el estado del proceso con waitpid
        //WNOHANG no bloquea el proceso y WUNTRACED comprueba si un proceso está detenido

        if (status == -1) {
            perror("Error en waitpid");
            return false;
        }

        if (status == 0) { //El proceso sigue ejecutándose
            posJ->job.status = ACTIVE;
            posJ->job.exit = 0;

        }else{
            if (WIFSTOPPED(info)) { //Comprueba si el proceso hijo se ha parado
                posJ->job.status = STOPPED;
                posJ->job.exit = WSTOPSIG(info);
            } else if (WIFSIGNALED(info)) { //Comprueba si el proceso hijo se ha parado por una señal
                posJ->job.status = SIGNALED;
                posJ->job.exit = WTERMSIG(info);
            } else if (WIFEXITED(info)) { //Comprueba si el proceso hijo ha finalizado
                posJ->job.status = FINISHED;
                posJ->job.exit = WEXITSTATUS(info);
            } else {    //En otro caso, el proceso sigue activo
                posJ->job.status = ACTIVE;
                posJ->job.exit = 0;
            }
        }
    }
    return true;
}

tPosJ findJob(tListJ J, int pid) {
    tPosJ posJ;
    for (posJ = J; posJ != JNULL; posJ = posJ->next) { //Recorremos la lista
        if (posJ->job.pid == pid) { //Si el pid es igual devolvemos la posición
            return posJ;
        }
    }
    return JNULL;
}
