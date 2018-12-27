#ifndef _UTILS_
#define _UTILS_
//
//c libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
//
//consts
#define tknsNum 64
#define tokDelim "!= \t\r\n\a"
#define histSize 128
#define errOk 9
#define errBad 10
#define Exit -55
//
//methods
int isAppCommand(const char *code);
char * readLine(void);
char ** splitLine(char *line);
void printArgs(char **args);
int argsCount(char **args);
int arrSize(int *a);
int betterAtoi(char *s);
int freeArgs(char **);
//
#endif