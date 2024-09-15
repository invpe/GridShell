// SHA256
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
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
// Main function that takes input and computes SHA256
char* local_main(const char* arg, size_t len) {
    printf("Hello world from a task!\n");
    printf("Input payload: %s\n", arg);

    unsigned char output[32];  // SHA256 produces a 32-byte (256-bit) digest
    char hex_output[65];       // SHA256 digest in hex (64 chars + null terminator)

    // Perform SHA256 hashing using input argument
    perform_sha256((const unsigned char*)arg, len, output);

    // Convert the digest to a hex string
    sha256_to_hex(output, hex_output);
    hex_output[64] = '\0';  // Ensure null termination

    printf("SHA256 Hash: %s\n", hex_output);

    // Return dynamically allocated memory, or NULL if nothing
    char* result = (char*)malloc(65 * sizeof(char));  // Allocate memory for the 64-character hash + null terminator
    if (result) {
        strcpy(result, hex_output);  // Copy the hex string into the dynamically allocated memory
    }
    return result;  
}