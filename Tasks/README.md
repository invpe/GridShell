# 🔠 Library of tasks

This is a library of tasks created for executing on the network.
Every task requires specific `INPUTPAYLOAD` and examples of usage are given below.



## writedfs - Submit a telemetry to the network.

Input: `FILENAME,APPEND?,BASE64(TEXT_TO_WRITE),`

Example: `MyTelemetry,0,BASE64("Ovewrites the file\n"),`

Example: `MyTelemetry,1,BASE64("Appends the file\n"),`

Cost: 🐚
 
## l2daily - Calculate IJP (Air Quality Index) for the last hour

Input: `TELEMETRY FILE`

Example: `JadeChartreuseDromiceiomimusLOOKO24c7525a25d82023109`

Cost: 🐚
