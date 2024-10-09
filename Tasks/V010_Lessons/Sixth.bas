' In this example, we will download telemetry file given by INPUTPAYLOAD
' Then read the whole file and return last line
FSIZE=DOWNLOAD(INPUTPAYLOAD)

' Check if not zero
IF FSIZE <= 0 THEN EXIT

' Clear the file position
RESETFPOS()

LASTLINE = ""

' Read line by line to the end of the file
WHILE TRUE
STRLINE=READLINE()

IF STRLINE = "" THEN 
EXIT
ELSE
LASTLINE = STRLINE
ENDIF

WEND

' Return last line
OUTPUTPAYLOAD=LASTLINE
