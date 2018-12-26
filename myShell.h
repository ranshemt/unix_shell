#ifndef __MYSHELL
#define __MYSHELL

#include "commands.h"

void shellLoop(char **history, int *pids, int *sizePids);
int runCommand(char **args, int *pids, int *sizePids);
int appLaunch(char **args, int *pids, int *sizePids);
//wow

#endif