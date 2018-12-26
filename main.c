//Ran Shem Tov  -   206059586
//Hodaya Simhi  -   313613226
//
#include "myShell.h"
//
//MAIN
int main(int argc, char **argv)
{
  
  //
  // Run command loop.
  shellLoop();
  //
  //free memory
  int sTmp = argsCount(history), i;
  for(i = 0; i< sTmp; i++){
    if(history[i] != NULL) free(history[i]);
  }
  if(history != NULL) free (history);
  if(myPids != NULL)  free(myPids);
  return 0;
}