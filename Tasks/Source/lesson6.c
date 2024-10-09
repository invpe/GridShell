/*
    GridShell (https://github.com/invpe/GridShell) V011 Task
    Retrieve inputpayload from `arg` then base64 encode it and store in outputpayload file.
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>     
#include "mbedtls/md.h"

unsigned char* base64_decode(const unsigned char *src, size_t len, size_t *out_len);
unsigned char* base64_encode(const unsigned char* src, size_t len, size_t* out_len);

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
        size_t encoded_len;
        char* pEncoded = base64_encode(arg, len, &encoded_len);   
    
        printf("Output file opened\n");
        fprintf(output_file,"Input: %s\n", arg);
        fprintf(output_file,"BASE64Encoded: %s\n",pEncoded);

        free(pEncoded);
        fclose(output_file);
    } 
}
