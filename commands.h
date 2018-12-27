//Ran Shem Tov  -   206059586
//Hodaya Simhi  -   313613226
//
#ifndef __COMMS
#define __COMMS 
#include "utils.h"
/*
l   -   exec_wait         no implemntation
&   -   exec_continue     no implemenatation
>   -   change STDOUT and exec_wait implement change stdout
*/

//
//shared constants
extern char **history;
extern char **myEnvs;
extern int *myPids, sizePids, *pidsStrHistory, histNum;
int initSharedVars();

  ////////////////////////////
 //   commands functions   //
////////////////////////////
//"myMan" only
//print manual
int myMan(char **args);
//
//path + arguments(...)
//launch the app and wait for it to end
int launchWait(char **args);
//
//path + arguments(...) + '&' at the end
//launch the app and continue loop
int launchContinue(char **args);
//
//"tasks" only
//print all background apps (pids)
int tasks(char **args);
//
//"return" at the beginnig and pid
//wait for pid to finish all background pids
int return_pid(char **args);
//
//path + arguments(...) + '>' + "output_file_name"
//redirect STDOUT of app to "output_file_name"
int redirectOut(char **args);
//
//KEY=VALUE
//sets enviorment variable key to value
int mySetEnv(char **args);
//
//"print_env" only
//prints EV that were set
int myPrintEnv(char **args);
//
//"show_history" only
//prints history of commands
int showHistory(char **args);
//!+number
//run that command from history
int runByHistory(char **args);
//
//"exit" only
//activating show_history + exit loop
int myExit(char **args);
  ///////////////////////////
 //   commands settings   //
///////////////////////////
//
//commands codes array (args[0])
extern const char *commsCodes[] /* = {
    "myMan",        //args[0] = "myMan"
    "l",            //args[0] = l, args[1] = path, args[..] = arguments
    "&",            //args[0] = '&', args[1] = path
    "tasks",        //args[0] = "tasks"
    "return",       //args[0] = "return" args[1] = pid
    ">",            //args[0] = '>', args[1] = output, args[2] = path, args[..] = arguments
    "=",            //args[0] = '=', args[1] = KEY, args[2] = VALUE
    "print_env"     //args[0] = "print_env",
    "show_history", //args[0] = "show_history"
    "exit"          //only 'exit'
    }*/;
//
//commands functions
extern const int (*commsFuncs[]) (char **);
//
//util func
int commsNum();
//
//
#endif 