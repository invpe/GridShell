' In this example, we will download telemetry file given by INPUTPAYLOAD
' Then read first line and return it in OUTPUTPAYLOAD
FSIZE=DOWNLOAD(INPUTPAYLOAD)

' Reset file position to 0
RESETFPOS()

' Read line
STRLINE=READLINE()

' Return it
OUTPUTPAYLOAD=STRLINE
