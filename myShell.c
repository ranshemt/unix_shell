#include "myShell.h"

//
//main shell loop
void shellLoop(char **history, int *pids)
{
    char *line;     //allocated in readLine
    char **args;    //allocated in splitLine
    int status, hisSize;
    hisSize = argsCount(history);
    do
    {
        printf("> ");
        //read line
        line = readLine();
        //save line in history
        history[hisSize] = (char*)malloc(sizeof(char)*(strlen(line)+1));
        if(history[hisSize == NULL]){
            perror("malloc err in shellLoop()");
            fprintf(stderr, "malloc err for history. %s won't be saved\n", line);
        }
        strcpy(history, line);
        //
        //split line into args
        args = splitLine(line);
            //printf("> calling runCommand with args: \n");
            //printArgs(args);
        //run the command
        status = runCommand(args, pids);
        //free memory
        if(line != NULL)    free(line);
        if(args != NULL){
            while(*args != NULL) free(*args++);
            free(args);  
        }
        free(line);
    } while (status);
}
//
//run relevant command function
int runCommand(char **args, int *pids)
{
    int i, r = -1;

    if (args[0] == NULL)
    {
        // An empty command was entered.
        return 1;
    }
    //
    //activate relevant command
    for (i = 0; i < commsNum(); i++)
    {
        //all commands
        if (strcmp(args[0], commsCodes[i]) == 0){
            r = (*commsFuncs[i])(args, pids);
            //printf("matching command: %s\n", commsCodes[i]);
            //app commands
            if(r == 1 && isAppCommand(commsCodes[i]))
            r = appLaunch(args, pids);
            break;
        }
    }
    //
    return r;
}
/*
    l   -   exec_wait         no implemntation
    &   -   exec_continue     no implemenatation
    >   -   change STDOUT and exec_wait implement change stdout
*/
//
//
//called by runCommand to activate program
int appLaunch(char **args, int *pids)
{
        //printf("> appLaunch() called with args:\n");
        //printArgs(args);
    //vars
    pid_t pid, wpid;
    int status, comm = -1, i, lenT;
    char **argv, *newOut;
    //argv for exec()
    argv = (char**)malloc(tknsNum * sizeof(char *));
    //create argv for runApp&Wait
    if(strcmp(args[0], "l") == 0){
        comm = 0;
        for(i = 1; i < argsCount(args); i++){
            lenT = strlen(args[i]);
            argv[i-1] = (char*)malloc(sizeof(char)*(lenT+1));
            if(argv[i-1] == NULL){
                perror("malloc err in appLaunch()");
            }
            strcpy(argv[i-1], args[i]);
        }
        argv[++i] = NULL;
    }
    //create argv for runApp&Continue
    if(strcmp(args[0], "&") == 0){
        comm = 1;
        for(i = 1; i < argsCount(args); i++){
            lenT = strlen(args[i]);
            argv[i-1] = (char*)malloc(sizeof(char)*(lenT+1));
            if(argv[i-1] == NULL){
                perror("malloc err in appLaunch()");
            }
            strcpy(argv[i-1], args[i]);
        }
        argv[++i] = NULL;
    }
    //create argv for runAppRedirectOutput&Wait
    if(strcmp(args[0], ">") == 0){
        comm = 2;
        for(i = 1; i < argsCount(args)-1; i++){
            lenT = strlen(args[i]);
            argv[i-1] = (char*)malloc(sizeof(char)*(lenT+1));
            if(argv[i-1] == NULL){
                perror("malloc err in appLaunch()");
            }
            strcpy(argv[i-1], args[i]);
        }
        argv[++i] = NULL;
        lenT = strlen(args[argsCount(args) - 1]);
        newOut = (char*)malloc(sizeof(char) * (lenT + 1));
        if(newOut == NULL){
            perror("malloc err in appLaunch()");
        }
        strcpy(newOut, args[argsCount(args) - 1]);
    }
        //printf("> > appLaunch()\n");
    //fork
    pid = fork();
    if (pid == 0)
    {
        // Child process
            //printf("> >callinc execv with path = args[1] = %s\n", args[1]);
            //printf("> >calling execv with argv: \n");
            //printArgs(argv);
        if (execv(args[1], argv) == -1)
        {
            perror("> >execv in appLaunch()");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // Error forking
        perror("> >fork in appLaunch()");
    }
    else
    {
        // Parent process
        //wait if needed
        if(comm == 0 || comm == 2){
            do
            {
                wpid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        //save pif if needed
        if(comm == 1){
            pids[arrSize(pids)] = pid;
        }
    }
    if(argv != NULL){
        while(*argv != NULL) free(*argv++);
        free(argv);   
    }
    if(newOut != NULL) free(newOut);
    return 1;
}