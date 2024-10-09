/*
    GridShell (https://github.com/invpe/GridShell) V011 Task
    Obtain the input payload given to the task

    *Warrning* This task will fail, as no output is returned
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>    
    
void local_main(char* arg,size_t len) 
{

    printf("Hello world from a task!\n"); 
    printf("Inputpayload: %s\n", arg);     
 
}
