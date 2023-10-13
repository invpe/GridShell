# 🔠 Library of tasks

This is a library of tasks created for executing on the network.

With community contributions, we hope to expand this list of versatile tasks that can be easily utilized by anyone, eventually creating a comprehensive resource for a wide range of purposes.



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

## widgeridoo - Geneate a simple JSON for use with [Widgeridoo](https://apps.apple.com/us/app/widgeridoo/id1531359008)

Input: `TITLE,VALUE,`

Example: `My Sensor, 123,`
