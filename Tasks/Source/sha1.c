// SHA1
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
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
// Main function that takes input and computes SHA1
char* local_main(const char* arg, size_t len) {
    printf("Hello world from a task!\n");
    printf("Input payload: %s\n", arg);

    unsigned char output[20];  // SHA1 produces a 20-byte (160-bit) digest
    char hex_output[41];       // SHA1 digest in hex (40 chars + null terminator)

    // Perform SHA1 hashing using input argument
    perform_sha1((const unsigned char*)arg, len, output);

    // Convert the digest to a hex string
    sha1_to_hex(output, hex_output);
    hex_output[40] = '\0';  // Ensure null termination

    printf("SHA1 Hash: %s\n", hex_output);

    // Return dynamically allocated memory, or NULL if nothing
    char* result = (char*)malloc(41 * sizeof(char));   
    if (result) {
        strcpy(result, hex_output);  // Copy the hex string into the dynamically allocated memory
    }
    return result;  
}