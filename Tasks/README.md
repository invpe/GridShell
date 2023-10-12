# 🔠 Library of tasks

This is a library of tasks created for executing on the network.
Every task requires specific `INPUTPAYLOAD` and examples of usage are given below.



## writedfs - Submit a telemetry to the network.

Input: `FILENAME,APPEND?,BASE64(TEXT_TO_WRITE),`

Example: `MyTelemetry,0,BASE64("Ovewrites the file\n"),`

Example: `MyTelemetry,1,BASE64("Appends the file\n"),`

## avgc - Average column

Input: `FILENAME,IDX,`

Example: `UsernameTelemetryfile,6,`

## kalmanc - Apply Kalman filter on column

Input: `FILENAME,IDX,`

Example: `UsernameTelemetryFile,6,`

## sha1 - Generate SHA1 from input

Input: `THIS_IS_A_TEXT`

Example: `Generate SHA1 from this message` 

## l2daily - Calculate IJP (Air Quality Index) for the last hour

Input: `TELEMETRY FILE`

Example: `JadeChartreuseDromiceiomimusLOOKO24c7525a25d82023109`
