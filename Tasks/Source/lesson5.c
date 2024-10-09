/*
    GridShell (https://github.com/invpe/GridShell) V011 Task
    Retrieve inputpayload from `arg` then perform MD5 and store outputpayload file.  
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>    
#include "esp32/rom/md5_hash.h"

// Function to convert a binary digest to a hex string
void convert_to_hex_string(const unsigned char *digest, char *hex_output) {    
    for (int i = 0; i < 16; i++) {  // MD5 is 16 bytes, so loop for 16 iterations
        sprintf(hex_output + (i * 2), "%02x", digest[i]);
    }
    hex_output[32] = '\0';  // Null-terminate the hex string
}
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
        struct MD5Context context;
        MD5Init(&context);
        MD5Update(&context, (const char*)arg, len);
        
        unsigned char digest[16];  // MD5 produces a 16-byte digest
        MD5Final(digest, &context);

        // Convert digest to a hex string
        char hex_output[33];  // 32 characters for MD5 hex + null terminator
        convert_to_hex_string(digest, hex_output);
 
        printf("Output file opened\n");
        fprintf(output_file,"Input: %s\n", arg);
        fprintf(output_file,"MD5: %s\n",hex_output);
        fclose(output_file);
    } 
}
