/*
    GridShell (https://github.com/invpe/GridShell) V011 Task
    Retrieve inputpayload from `arg` and store it with additional text in output file.    
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>    
    
void local_main(char* arg,size_t len) 
{

    printf("Hello world from a task!\n"); 
    printf("Inputpayload: %s\n", arg);  

    FILE* output_file = fopen("/spiffs/GSoutput", "w");
    if (!output_file) {
        printf("Failed to open the output file.\n");        
    }   
    else
    {   
        printf("Output file opened\n");
        fprintf(output_file,"Input: %s\n", arg);
        fprintf(output_file,"Some additional text from the task\n");
        fclose(output_file);
    } 
}
