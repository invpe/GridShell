' This example will obtain the telemetry file given as INPUTPAYLOAD
' It will then return the size (in bytes) of the telemetry in OUTPUTPAYLOAD

FSIZE = DOWNLOAD(INPUTPAYLOAD) 
OUTPUTPAYLOAD = STR(FSIZE)
