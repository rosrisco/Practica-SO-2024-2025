# Practica-SO-2024-2025
NOTA: 2,31/2,5

## Descripción de los archivos:
p3.c: contiene la base de la shell.  

cmds.h/.c: contienen todos los comandos que nos piden a lo largo de toda la práctica.  

aux.h/.c: algunas funciones de ayuda que se usan en los diferentes archivos del proyecto (era más cómodo para nosotras ponerlos en su propia librería en vez de repetirlos en cada archivo).  

historic.h/.c: contienen una lista enlazada simple en la que se van guardando los comandos que se meten en la shell.  

openfiles.h/.c: contienen una lista doblemente enlazada en la que se van guardando los archivos que se abren.  

dirs.h/.c: se usan para Cmd_search, contienen una lista donde se guardan los directorios de búsqueda.  

jobs.h/.c: contienen una lista doblemente enlazada que gestiona los trabajos en ejecución.  

memory.h/.c: contienen una lista doblemente enlazada que gestiona el uso de la memoria de la shell.  
