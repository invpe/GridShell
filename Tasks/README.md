# 🔠 Library of tasks

This is a library of tasks created for executing on the network.
Every task requires specific `INPUTPAYLOAD` and examples of usage are given below.



## writedfs - Submit a telemetry to the network.

Input: `FILENAME,APPEND?,BASE64(TEXT_TO_WRITE),`

Example: `MyTelemetry,0,BASE64("Ovewrites the file\n"),`
Example: `MyTelemetry,1,BASE64("Appends the file\n"),`

Cost: 🐚

## send - Submit a shell transfer to the network

Input: `AMOUNT,RECEIVER`

Example: `1000,PocNetGroupMiners00000000000000000000001`

Cost: 🐚



