//Ran Shem Tov      -   206059586
//Hodaya Simhi      -   313613226
//
//
#include "commands.h"
char **history = NULL, **myEnvs = NULL;
int sizePids, *myPids, *pidsStrHistory, histNum;

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
    //allocate envs
    myEnvs = (char**) malloc(sizeof(char*) * histSize);
    if(myEnvs == NULL){
        perror("malloc err in initSharedVars()");
        fprintf(stderr, "envs buffer err\n");
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
    "print_env" ,    //args[0] = "print_env",
    "show_history", //args[0] = "show_history"
    "exit",          //args[0] = 'exit'
    "!",
    "<"
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
    &myExit,
    &runByHistory,
    &redirectIn
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
        return errOk;
    }
    if(argsCount(args) > 1){
        fprintf(stderr, "too many argumants recieved at myMan()\n");
        return errOk;
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
        printf("> SUCCESS: no apps in background\n");
        return 1;
    }
    char **argsTmp, histCpy[2048];
    int currPid = -1, i, len;

    for(i = 0; i < sizePids; i++){
        currPid = myPids[i];
        strcpy(histCpy, history[pidsStrHistory[i]]);
        argsTmp = splitLine(histCpy);
        //print
        printf("> %s:%d\n", argsTmp[1], currPid);
        currPid = -1;
    }
    return 1;
}
int return_pid(char **args){
    if(args[1] == NULL){
        fprintf(stderr, "no pid for command return\n");
        return errOk;
    }
    char *tmpS;
    int len = strlen(args[1]);
    tmpS = (char*) malloc(sizeof(char) * (len+1));
    //check
    strcpy(tmpS, args[1]);
    int currPid = betterAtoi(tmpS);
    if(currPid == 0){
        printf("> ERR: no valid pid for args[1] = %s\n", args[1]);
        return errOk;
    }
    int i, flag = 0, pidI, status, wpid;
    for(i = 0; i < sizePids; i++){
        if(myPids[i] == currPid){
            do
            {
                wpid = waitpid(currPid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
            flag = 1;
            pidI = i;
            break;
        }
    }
    if(flag == 0){
        printf("> ERR: requested pid = %d wasn't found\n", currPid);
        return errOk;
    }
    if(flag == 1){
        //remove myPids[i] & pidsStrHistory[i]
        for(i = pidI; i < sizePids - 1; i++){
            myPids[i] = myPids[i+1];
            pidsStrHistory[i] = pidsStrHistory[i+1];
        }
        myPids[sizePids-1] = -1;
        sizePids--;
        //print success message
        printf("> SUCCESS: pid = %d finished\n", currPid);
        return 1;
    }
}
int redirectOut(char **args){
    return 1;
}
int redirectIn(char **args){
    return 1;
}
int mySetEnv(char **args){
    if(args == NULL || argsCount(args) != 3){
        fprintf(stderr, "not enough args in redirectOut\n");
        return errOk;
    }
    char *key, *val;
    int keyLen, valLen, envsSize = 0, envReturn;
    keyLen = strlen(args[1]);
    valLen = strlen(args[2]);
    key = (char*)malloc(sizeof(char)*(keyLen+1));
    if(key == NULL){
        perror("malloc error in mySetEnv()");
        return errBad;
    }
    strcpy(key, args[1]);
    val = (char*)malloc(sizeof(char)*(valLen+1));
    if(val == NULL){
        perror("malloc error in mySetEnv()");
        return errBad;
    }
    strcpy(val, args[2]);
    //add to myEnvs
    envsSize = argsCount(myEnvs);
    myEnvs[envsSize] = (char*)malloc(sizeof(char)*(keyLen + 1));
    if(val == NULL){
        perror("malloc error in mySetEnv()");
        return errBad;
    }
    strcpy(myEnvs[envsSize], key);
    //terminate myEnvs
    myEnvs[envsSize+1] = NULL;
    //set
    envReturn = setenv(key, val, 1);
    if(envReturn == -1){
        perror("setenv() err");
        return errOk;
    }
    printf("> SUCCESS: %s was added with value %s\n", key, val);
    return 1;
}
int myPrintEnv(char **args){
    int i, envsSize = 0;
    envsSize = argsCount(myEnvs);
    if(envsSize == 0){
        printf("> SUCCESS: no enviorment variables set\n");
        return 1;
    }
    printf("> SUCCESS: printing enviorment variables\n> ");
    for(i = 0; i < envsSize; i++){
        printf("%s, ", myEnvs[i]);
    }
    printf("\n");
    return 1;
}
int showHistory(char **args){
    if(history == NULL){
        fprintf(stderr, "no valid history pointer\n");
        return errOk;
    }
    int size = argsCount(history), i;
    if(size == 0){
        printf("> SUCCESS: no history\n");
        return 1;
    }
    for(i = 0; i < size; i++){
        printf("> %d) %s", i, history[i]);
    }
    return 1;
}
int runByHistory(char **args){
    return 1;
}
int myExit(char **args){
    printf("> EXITING:\n");
    showHistory(NULL);
    printf("> THANK YOU\n");
    printf("> GOOD BYE:)\n");
    return 1;
}
