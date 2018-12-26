//Ran Shem Tov  -   206059586
//Hodaya Simhi  -   313613226
//
#include "myShell.h"
//
//MAIN
int main(int argc, char **argv)
{
  //allocate history
  char **myHistory;
  myHistory = (char**) malloc(sizeof(char*) * histSize);
  if(myHistory == NULL){
    perror("malloc err in main()");
    fprintf(stderr, "history buffer err, show_history won't work!\n");
  }
  //allocate pids history
  int *myPids, sTmp, i, sizePids = 0;
  int *sizePidsPtr = &sizePids;
  myPids = (int*) malloc(sizeof(int) * histSize);
  if(myPids == NULL){
    perror("malloc err in main()");
    fprintf(stderr, "pids buff err, return won't work!\n");
  }
  //
  // Run command loop.
  shellLoop(myHistory, myPids, sizePidsPtr);
  //
  //free memory
  sTmp = argsCount(myHistory);
  for(i = 0; i< sTmp; i++){
    if(myHistory[i] != NULL) free(myHistory[i]);
  }
  if(myHistory != NULL) free (myHistory);
  if(myPids != NULL)  free(myPids);
  return 0;
}