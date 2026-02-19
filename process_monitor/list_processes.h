#ifndef LIST_PROCESSES
#define LIST_PROCESSES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>
#include <sys/types.h>

#define MAX_PROCESSES 1024
#define STRING_LEN 32

typedef struct process {
    int pid;
    int ppid;
    char name[STRING_LEN];
    char path[STRING_LEN];
} process;

/*
#
#   The purpose of this library
#   is to obtain the processes
#   currently active in the
#   linux OS
#
#   |   |   |   |   |   |   |
#   V   V   V   V   V   V   V
*/

process * getProcesses();

/*
#   A   A   A   A   A   A   A
#   |   |   |   |   |   |   |
*/

/// Testing functions

void clearScreen();

void runtimeTestLoop();


#endif /* LIST_PROCESSES */ 