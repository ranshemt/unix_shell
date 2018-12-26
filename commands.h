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


  ////////////////////////////
 //   commands functions   //
////////////////////////////
//"myMan" only
//print manual
int myMan(char **args, int *pids);
//
//path + arguments(...)
//launch the app and wait for it to end
int launchWait(char **args, int *pids);
//
//path + arguments(...) + '&' at the end
//launch the app and continue loop
int launchContinue(char **args, int *pids);
//
//"tasks" only
//print all background apps (pids)
int tasks(char **args, int *pids);
//
//"return" at the beginnig and pid
//wait for pid to finish all background pids
int return_pid(char **args, int *pids);
//
//path + arguments(...) + '>' + "output_file_name"
//redirect STDOUT of app to "output_file_name"
int redirectOut(char **args, int *pids);
//
//KEY=VALUE
//sets enviorment variable key to value
int mySetEnv(char **args, int *pids);
//
//"print_env" only
//prints EV that were set
int myPrintEnv(char **args, int *pids);
//
//"show_history" only
//prints history of commands
int showHistory(char **args, int *pids);
//
//"exit" only
//activating show_history + exit loop
int myExit(char **args, int *pids);
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
extern const int (*commsFuncs[]) (char **, int*);
//
//util func
int commsNum();
//
//
#endif 