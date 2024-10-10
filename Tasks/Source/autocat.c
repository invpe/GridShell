/*
    GridShell (https://github.com/invpe/GridShell) V011 Task
    GIP0068 Perform a choosen number of iterations of SHA256 on a given input and return as outputpayload
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>    
#include "esp32/rom/sha.h" 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>    
#include "esp32/rom/sha.h" 

void perform_sha256(const unsigned char *input, uint32_t input_length, unsigned char *output) {
    SHA_CTX ctx;

    ets_sha_enable();
    ets_sha_init(&ctx);
    ets_sha_update(&ctx, SHA2_256, input, input_length * 8);  // Pass input length in bits
    ets_sha_finish(&ctx, SHA2_256, output);
    ets_sha_disable();
}
// Convert SHA256 output to a hex string
void sha256_to_hex(const unsigned char *digest, char *hex_output) {
    for (int i = 0; i < 32; i++) {  // 32 bytes for SHA256
        sprintf(hex_output + (i * 2), "%02x", digest[i]);
    }
}
// Convert SHA1 output to a hex string
void sha1_to_hex(const unsigned char *digest, char *hex_output) {    
    for (int i = 0; i < 20; i++) {
        sprintf(hex_output + (i * 2), "%02x", digest[i]);
    }
}    
void local_main(char* arg,size_t len) 
{ 
    FILE* output_file = fopen("/spiffs/GSoutput", "w");
    if (output_file) 
    {   

        unsigned char output[32]; 
        char hex_output[65];      

        // Perform SHA256 hashing using input argument
        perform_sha256((const unsigned char*)arg, len, output);

        // Go on with more iterations
        for(int x =0; x < 100; x++)
            perform_sha256(output, 32, output);

        // Convert the digest to a hex string
        sha256_to_hex(output, hex_output);
        hex_output[64] = '\0';
        
        fprintf(output_file,"Input: %s\n", arg);
        fprintf(output_file,"SHA256: %s\n",hex_output);        
        fclose(output_file);
    } 
}