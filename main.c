//Ran Shem Tov  -   206059586
//Hodaya Simhi  -   313613226
//
#include "myShell.h"
//
//MAIN
int main(int argc, char **argv)
{
  if(initSharedVars() == 0){
    fprintf(stderr, "error allocating memory for shared variables!\n");
    fprintf(stderr, "Program won't start\n");
    return 0;
  }
  //
  // Run command loop.
  shellLoop();
  
  return 0;
}