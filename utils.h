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
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
//
//consts
#define tknsNum 64
#define tokDelim "= \t\r\n\a"
#define histSize 128
//
//methods
int isAppCommand(const char *code);
char * readLine(void);
char ** splitLine(char *line);
void printArgs(char **args);
int isCaps(char *str);
int argsCount(char **args);
int arrSize(int *a);
//
#endif