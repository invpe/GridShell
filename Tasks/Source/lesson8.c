/*
    GridShell (https://github.com/invpe/GridShell) V011 Task
    The task should be given a plain-text telemetry file which will be stored in /spiffs/GStelemetry
    It will read through up to the last line, and return arg and telemetry last non empty line as output.    
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>     

void local_main(char* arg, size_t len) 
{
    printf("Hello world from a task!\n"); 
    printf("Inputpayload: %s\n", arg);  

    FILE* input_file = fopen("/spiffs/GStelemetry", "r");

    if (input_file)     
    {
        char buffer[256]; 
        char last_line[256] = {0};

        // Read each line of the file until the end
        while (fgets(buffer, sizeof(buffer), input_file)) 
        {
            // Remove trailing newline characters if present
            size_t len = strlen(buffer);
            while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) 
            {
                buffer[--len] = '\0';
            }

            // If the line is not empty, copy it to last_line
            if (len > 0) 
            {
                strncpy(last_line, buffer, sizeof(last_line));
                last_line[sizeof(last_line) - 1] = '\0'; // Ensure null termination
            }
        }

        fclose(input_file);

        // Store the last non-empty line to the output payload
        FILE* output_file = fopen("/spiffs/GSoutput", "w");
        if (output_file) 
        {   
            fprintf(output_file, "Arg: %s\nLast non empty Line: %s\n", arg, last_line);            
            fclose(output_file);
        }
    }
    else
    {
        printf("Failed to open input file: /spiffs/GStelemetry\n");
    }
}
