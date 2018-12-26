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
  int *myPids;
  myPids = (int*) malloc(sizeof(int) * histSize);
  if(myPids == NULL){
    perror("malloc err in main()");
    fprintf(stderr, "pids buff err, return won't work!\n");
  }
  //
  // Run command loop.
  shellLoop(myHistory, myPids);
  //
  //free memory
  if(myHistory != NULL){
    while(*myHistory != NULL) free(*myHistory++);
    free(myHistory);
  }
  if(myPids != NULL)  free(myPids);
  return 0;
}