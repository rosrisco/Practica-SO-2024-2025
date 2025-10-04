//Incluimos librerías:
#include "cmds.h"
#include "historic.h"
#include "openfiles.h"
#include "aux.h"
#include "memory.h"
#include "jobs.h"
#include "dirs.h"

int main(int argc, char *argv[], char *envp[]) {
    char * tr[MAXNAME];
    char cadena[MAXNAME];
    int numtr;

    //Inicializamos las listas:
    tListH H;
    tListF F;
    tListM M;
    tListJ J;
    tListD D;

    createEmptyListH(&H);
    createEmptyListF(&F);
    createEmptyListM(&M);
    createEmptyListJ(&J);
    createEmptyListD(&D);

    insertItemF(&F, FNULL, "entrada estandar", O_RDWR, 0);
    insertItemF(&F, FNULL, "salida estandar", O_RDWR, 1);
    insertItemF(&F, FNULL, "error estandar", O_RDWR, 2);

    //Sacamos el directorio en el que está el ejecutable:
    char dir[MAXNAME], *carpeta;
    getcwd(dir, sizeof(dir)); //Nos devuelve el path completo
    carpeta = strrchr(dir,'/'); //strrchr() encuentra la última aparición del caracter '/' y devuelve el puntero de dicha posición

    //Sacamos la información de la máquina:
    struct utsname so;
    uname(&so);

    //La shell:
    while (1) {

        printf("%s@%s(%s)--> ", so.sysname, so.nodename, carpeta);
        fgets(cadena, MAXNAME, stdin);

        if (strcmp(cadena, "\n") == 0) {
            continue; //Si no ponemos nada, continua el bucle
        }

        //Insertamos cada comando en la lista del historial
        insertItemH(&H, HNULL, cadena);

        numtr = trocearCadena(cadena, tr);

        if (strcmp("authors", tr[0]) == 0) {
            Cmd_authors(numtr, tr, &H);
        } else if (strcmp("pid", tr[0]) == 0) {
            Cmd_pid();
        } else if (strcmp("ppid", tr[0]) == 0) {
            Cmd_ppid();
        } else if (strcmp("cd", tr[0]) == 0) {
            Cmd_cd(numtr, tr);
        } else if (strcmp("date", tr[0]) == 0) {
            Cmd_date(numtr, tr, &H);
        } else if (strcmp("historic", tr[0]) == 0) {
            Cmd_historic(numtr, tr, H, F, M, J, D, envp);
        } else if (strcmp("open", tr[0]) == 0) {
            Cmd_open(numtr, tr, &F);
        } else if (strcmp("close", tr[0]) == 0) {
            Cmd_close(tr, &F);
        } else if (strcmp("dup", tr[0]) == 0) {
            Cmd_dup(tr, &F);
        } else if (strcmp("infosys", tr[0]) == 0) {
            Cmd_infosys();
        } else if (strcmp("help", tr[0]) == 0) {
            Cmd_help(numtr, tr, &H);
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
            Cmd_allocate(tr, numtr, &M, H, F);
        } else if (strcmp("deallocate", tr[0]) == 0){
            Cmd_deallocate(tr, numtr, &M, H, F);
        } else if (strcmp("memfill", tr[0]) == 0){
            Cmd_memfill(tr);
        } else if (strcmp("memdump", tr[0]) == 0){
            Cmd_memdump(tr);
        } else if (strcmp("memory", tr[0]) == 0){
            Cmd_memory(tr, M, H);
        } else if (strcmp("readfile", tr[0]) == 0){
            Cmd_readfile(tr);
        } else if (strcmp("writefile", tr[0]) == 0){
            Cmd_writefile(tr);
        } else if (strcmp("read", tr[0]) == 0){
            Cmd_read(tr, F);
        } else if (strcmp("write", tr[0]) == 0){
            Cmd_write(tr, F);
        } else if (strcmp("recurse", tr[0]) == 0) {
            Cmd_recurse (tr);
        } else if (strcmp("getuid", tr[0]) == 0) {
            Cmd_getuid();
        } else if (strcmp("setuid", tr[0]) == 0) {
            Cmd_setuid(tr, &H);
        } else if (strcmp("showvar", tr[0]) == 0) {
            Cmd_showvar(tr, envp, &H);
        } else if (strcmp("changevar", tr[0]) == 0) {
            Cmd_changevar(tr, envp, &H);
        } else if (strcmp("subsvar", tr[0]) == 0) {
            Cmd_subsvar(tr, envp, &H);
        } else if (strcmp("environ", tr[0]) == 0) {
            Cmd_environ(tr, envp, &H);
        } else if (strcmp("fork", tr[0]) == 0) {
            Cmd_fork(&J);
        } else if (strcmp("search", tr[0]) == 0) {
            Cmd_search(tr, &D, &H);
        } else if (strcmp("exec", tr[0]) == 0) {
            Cmd_exec(tr, D);
        } else if (strcmp("execpri", tr[0]) == 0) {
            Cmd_execpri(tr, D);
        } else if (strcmp("fg", tr[0]) == 0) {
            Cmd_fg(tr, D);
        } else if (strcmp("fgpri", tr[0]) == 0) {
            Cmd_fgpri(tr, D, &J);
        } else if (strcmp("back", tr[0]) == 0) {
            Cmd_back(tr, D, &J);
        } else if (strcmp("backpri", tr[0]) == 0) {
            Cmd_backpri(tr, D, &J);
        } else if (strcmp("listjobs", tr[0]) == 0) {
            Cmd_listjobs(J);
        } else if (strcmp("deljobs", tr[0]) == 0) {
            Cmd_deljobs(tr, &J);
        } else if(strcmp("quit", tr[0]) == 0 || strcmp("bye", tr[0]) == 0 || strcmp("exit", tr[0]) == 0 ){
            break;
        } else { //En caso de meter un comando inválido, se elimina de la lista del historial
            Cmd_fg(tr, D);
        }
    }

    //Vaciamos las listas para evitar fugas de memoria
    emptyListH(&H);
    emptyListF(&F);
    emptyListM(&M);
    emptyListJ(&J);
    emptyListD(&D);

    return 0;
}
