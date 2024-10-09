/*
    GridShell (https://github.com/invpe/GridShell) V011 Task
    Retrieve inputpayload from `arg` then hash it with SHA1 and store outputpayload file.  
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>    
#include "esp32/rom/sha.h" 

void perform_sha1(const unsigned char *input, uint32_t input_length, unsigned char *output) {
    SHA_CTX ctx;

    // Enable the SHA hardware
    ets_sha_enable();

    // Step 1: Initialize the SHA1 context    
    ets_sha_init(&ctx);

    // Step 2: Update the SHA1 context with the input data    
    ets_sha_update(&ctx, SHA1, input, input_length * 8);  // Pass input length in bits

    // Step 3: Finalize the SHA1 hash and get the output digest    
    ets_sha_finish(&ctx, SHA1, output);

    // Disable the SHA hardware
    ets_sha_disable();
}
// Convert SHA1 output to a hex string
void sha1_to_hex(const unsigned char *digest, char *hex_output) {    
    for (int i = 0; i < 20; i++) {
        sprintf(hex_output + (i * 2), "%02x", digest[i]);
    }
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

        unsigned char output[20];  // SHA1 produces a 20-byte (160-bit) digest
        char hex_output[41];       // SHA1 digest in hex (40 chars + null terminator)

        // Perform SHA1 hashing using input argument
        perform_sha1((const unsigned char*)arg, len, output);

        // Convert the digest to a hex string
        sha1_to_hex(output, hex_output);
        hex_output[40] = '\0';  // Ensure null termination


        printf("Output file opened\n");
        fprintf(output_file,"Input: %s\n", arg);
        fprintf(output_file,"SHA1: %s\n",hex_output);
        fclose(output_file);
    } 
}
