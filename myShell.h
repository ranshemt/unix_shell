#ifndef __MYSHELL
#define __MYSHELL

#include "commands.h"

void shellLoop();
int runCommand(char **args);
int appLaunch(char **args);

#endif