#ifndef __MYSHELL
#define __MYSHELL

#include "commands.h"

void shellLoop(char **history, int *pids);
int runCommand(char **args, int *pids);
int appLaunch(char **args, int *pids);

#endif