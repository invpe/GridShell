/*
    GridShell (https://github.com/invpe/GridShell) V011 Task
    This tests if a task execution timeout reacts on the node.
*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>   
#include <sys/unistd.h>
#include <sys/stat.h>

void local_main(char* arg,size_t len) {
    
    printf("Welcome to task\n");
    printf("Inputpayload: %s\n", arg); 
    while(1)
    {
        // The main sketch should reboot after task execution
        // time exceeds given value.
    }
}
