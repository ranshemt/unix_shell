//Ran Shem Tov      -   206059586
//Hodaya Simhi      -   313613226
//
//
#include "commands.h"
char **history = NULL;
int sizePids, *myPids, *pidsStrHistory;

//init shared variables
int initSharedVars(){
    int i;
    //allocate history
    history = (char**) malloc(sizeof(char*) * histSize);
    if(history == NULL){
        perror("malloc err in initSharedVars()");
        fprintf(stderr, "history buffer err\n");
        return 0;
    }
    //allocate pids history
    pidsStrHistory = (int*) malloc(sizeof(int) * histSize);
    if(pidsStrHistory == NULL){
        perror("malloc err in initSharedVars()");
        fprintf(stderr, "pids buff err, return won't work!\n");
        return 0;
    }
    //allocate pids
    myPids = (int*) malloc(sizeof(int) * histSize);
    if(myPids == NULL){
        perror("malloc err in initSharedVars()");
        fprintf(stderr, "pids buff err, return won't work!\n");
        return 0;
    }
    //init myPids to -1
    for(i = 0; i < histSize; i++)   myPids[i] = -1;
    //pids size
    sizePids = 0;
    return 1;
}
  //////////////////////////
 //   commands settings  //
//////////////////////////
//
//commands names (order is important!)
const char *commsCodes[]  = {
    "myMan",        //args[0] = "myMan"
    "l",            //args[0] = l, args[1] = path, args[..] = arguments
    "&",            //args[0] = '&', args[1] = path
    "tasks",        //args[0] = "tasks"
    "return",       //args[0] = "return" args[1] = pid
    ">",            //args[0] = '>', args[1] = output, args[2] = path, args[..] = arguments
    "=",            //args[0] = '=', args[1] = KEY, args[2] = VALUE
    "print_env"     //args[0] = "print_env",
    "show_history", //args[0] = "show_history"
    "exit"          //args[0] = 'exit'
    };
//commands matching function (order is important!)
const int (*commsFuncs[]) (char **) = {
    &myMan,
    &launchWait,
    &launchContinue,
    &tasks,
    &return_pid,
    &redirectOut,
    &mySetEnv,
    &myPrintEnv,
    &showHistory,
    &myExit
    };
//
int commsNum()
{
    return sizeof(commsCodes) / sizeof(char *);
}
  ////////////////////////////////
 //   commands implementation  //
////////////////////////////////
//
//print manual
int myMan(char **args){
    if(args == NULL){
        fprintf(stderr, "no command recieved at myMan()\n");
        return 0;
    }
    if(argsCount(args) > 1){
        fprintf(stderr, "too many argumants recieved at myMan()\n");
        return 0;
    }
    printf("> 'myMan' for manual\n---------\n");
    printf("> e.g. 'myMan'\n---------\n");
    printf("> fullPath/command + arg + arg +.. to run app and wait for it to end\n");
    printf("> e.g. /bin/ls -1 -t -1\n---------\n");
    printf("> fullPath/command + arg + arg + .. + & to run app in the background\n");
    printf("> e.g. /usr/bin/mousepad &\n---------\n");
    printf("> tasks to print all running backgorund apps\n");
    printf("> e.g. tasks\n---------\n");
    printf("> 'return' + pid to wait for background app to end and then continue\n");
    printf("> e.g. return 1921\n---------\n");
    printf("> fullPath/command + arg + arg + .. + > + outputFile to redirect STDOUT of requsted app and run it\n");
    printf("> e.g. /usr/ls -1 -t -a > output\n---------\n");
    printf("> KEYNAME=VALUE to set enviorment variable\n");
    printf("> e.g. KEY=VALUE\n---------\n");
    printf("> 'print_env' to show all modified enviorment variables\n");
    printf("> e.g. print_env\n---------\n");
    printf("> 'show_history' to show previous commands\n");
    printf("> e.g. show_history\n---------\n");
    printf("> 'exit' to show history and close the shell program\n");
    printf("> e.g. exit\n---------\n");
    return 1;
}
int launchWait(char **args){
    //all required job done in launchApp()
    return 1;
}
int launchContinue(char **args){
    //all required job done in launchApp()
    return 1;
}
int tasks(char **args){
    if(sizePids == 0){
        return -1;
    }
    char *fPath = NULL, **argsTmp;
    int currPid = -1, i, len;

    for(i = 0; i < sizePids; i++){
        currPid = myPids[i];
        argsTmp = splitLine(history[i]);
        //print
        printf("> %s:%d\n", argsTmp[1], currPid);
        fPath = NULL;
        currPid = -1;
    }
    //free argsTmp
    int sTmp = argsCount(argsTmp);
    for(i = 0; i < sTmp; i++){
        if(argsTmp[i] != NULL) free(argsTmp[i]);
    }
    if(argsTmp != NULL) free(argsTmp);
    return 1;
}
int return_pid(char **args){

}
int redirectOut(char **args){

}
int mySetEnv(char **args){

}
int myPrintEnv(char **args){

}
int showHistory(char **args){

}
int myExit(char **args){

}
//
//
//
//
//cd
int lsh_cd(char **args, int *pids)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("lsh");
        }
    }
    return 1;
}
//
//help
int lsh_help(char **args, int *pids)
{
    int i;
    printf("Stephen Brennan's LSH\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (i = 0; i < commsNum(); i++)
    {
        printf("  %s\n", commsCodes[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}
//
//exit
int lsh_exit(char **args, int *pids)
{
    return 0;
}