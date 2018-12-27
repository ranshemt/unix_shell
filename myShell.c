//Ran Shem Tov  -   206059586
//Hodaya Simhi  -   313613226
//bonus history by Hodaya
//bonus redirect stdin by Ran
#include "myShell.h"

//
//main shell loop
void shellLoop()
{
    char *line;     //allocated in readLine
    char **args;    //allocated in splitLine
    int status, sTmp, i;
    char strTmp[2048];
    do
    {
        printf("> ");
        //
        //read line
        line = readLine();
        if(line == NULL){
            fprintf(stderr, "no line input!\n");
            continue;
        }
        //
        //split line into args
        strcpy(strTmp, line);
        args = splitLine(strTmp);
        if(args == NULL){
            fprintf(stderr, "couldn't splitLine\n");
            if(line != NULL) free(line);
            continue;
        }
        //
        sTmp = argsCount(history);
        if(sTmp == 0 && strcmp(args[0], "!") == 0){
            if(line != NULL) free(line);
            continue;
        }
        //
        //save line in history
        history[sTmp] = (char*)malloc(sizeof(char)*(strlen(line)+1));
        if(history[sTmp] == NULL){
            perror("malloc err in shellLoop()");
            fprintf(stderr, "malloc err for history. %s won't be saved\n", line);
            if(line != NULL) free(line);
            return;
        }
        strcpy(history[sTmp], line);
        history[sTmp + 1] = NULL;
        //
            // printf("> args: \n");
            // printArgs(args);
        //
        // ! run command by history
        while(strcmp("!", args[0]) == 0){
            histNum = -1;
            histNum = betterAtoi(args[1]);
            strcpy(strTmp, history[histNum]);
            args = splitLine(strTmp);
        }
        //
        //run the command
        status = runCommand(args);
        if(status == Exit){
            if(line != NULL)    free(line);
            freeArgs(args);
            break;
        }
        if(status == errOk){
            fprintf(stderr, "runCommand(%s) didn't work\n", line);
        }
        if(status == errBad){
            fprintf(stderr, "FATAL: runCommand(%s) didn't work\n", line);
        }
        //
        //free memory
        if(line != NULL)    free(line);
        freeArgs(args);
    } while (status != errBad);
    //
    //free memory
    if(myPids != NULL)  free(myPids);
    if(pidsStrHistory != NULL)  free(pidsStrHistory);
    freeArgs(myEnvs);
    freeArgs(history);
}
//
//run relevant command function
int runCommand(char **args)
{
    int i, r = 1;

    if (args[0] == NULL)
    {
        // An empty command was entered.
        return 0;
    }
    //
    //activate relevant command
    for (i = 0; i < commsNum(); i++)
    {
        //certain it's wrong command
        if(args == NULL){
            return -1;
        }
        //all commands
        if (strcmp(args[0], commsCodes[i]) == 0){
            r = (*commsFuncs[i])(args);
            //app commands
            if(r == 1 && isAppCommand(commsCodes[i]))
            r = appLaunch(args);
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
int appLaunch(char **args)
{
    //vars
    pid_t pid, wpid;
    int status, comm = -1, i, lenT, redirectFD, s;
    char **argv, *newOut = NULL;
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
                return 0;
            }
            strcpy(argv[i-1], args[i]);
        }
        argv[++i] = NULL;
    }
    //create argv for runAppRedirectOutput&Wait
    if(strcmp(args[0], ">") == 0 || strcmp(args[0], "<") == 0){
        if(strcmp(args[0], ">") == 0)   comm = 2;
        if(strcmp(args[0], "<") == 0)   comm = 3;
        for(i = 1; i < argsCount(args)-1; i++){
            lenT = strlen(args[i]);
            argv[i-1] = (char*)malloc(sizeof(char)*(lenT+1));
            if(argv[i-1] == NULL){
                perror("malloc err in appLaunch()");
                return 0;
            }
            strcpy(argv[i-1], args[i]);
        }
        argv[++i] = NULL;
        lenT = strlen(args[argsCount(args) - 1]);
        newOut = (char*)malloc(sizeof(char) * (lenT + 1));
        if(newOut == NULL){
            perror("malloc err in appLaunch()");
            return 0;
        }
        strcpy(newOut, args[argsCount(args) - 1]);
    }
    //
    //fork
    pid = fork();
    if (pid == 0)
    {
        //
        //out redirection
        if(comm == 2){
            s = argsCount(args);
            redirectFD = open(args[s-1], O_WRONLY | O_CREAT | O_TRUNC,S_IRWXU);
        }
        //
        //in redirection
        if(comm == 3){
            s = argsCount(args);
            redirectFD = open(args[s-1], O_RDONLY);
        }
        // Child process
        //
        //change output stream
        if(comm == 2){
            if(close(STDOUT_FILENO) < 0){
                perror("close in redirect");
                return 0;
            }
            dup(redirectFD);
        }
        //
        //change input stream
        if(comm == 3){
            if(close(STDIN_FILENO) < 0){
                perror("close in redirect");
                return 0;
            }
            dup(redirectFD);
        }
        if (execv(args[1], argv) == -1)
        {
            perror("> >execv in appLaunch()");
            return 0;
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // Error forking
        perror("> >fork in appLaunch()");
        return 0;
    }
    else
    {
        // Parent process
        //wait if needed
        if(comm == 0 || comm == 2 || comm == 3){
            do
            {
                wpid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        if(comm == 3)   printf("\n");
        //save pid if needed
        if(comm == 1){
            //save pid
            myPids[sizePids] = pid;
            //save index of matching line from history array
            pidsStrHistory[sizePids] = argsCount(history) - 1;
            //advance
            sizePids++;
        }
    }
    if(newOut != NULL) free(newOut);
    freeArgs(argv);
    return 1;
}
