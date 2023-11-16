'' This modification ensures that if the end of the file is reached and no newline character is found, the remaining buffer is treated as the last line. This should '' solve the issue with the last line not being read correctly in your dataset.
OUTPUTPAYLOAD = ""
CURS = 0
CSZ = 64 
BUF = "" 

DEF FNL(STRS)
    SLEN = LEN(STRS) - 1
    FOR ICT = 0 TO SLEN
        CCC = MID(STRS, ICT, 1)
        IF ASC(CCC) = 10 THEN RETURN ICT
    NEXT ICT
    RETURN -1
ENDDEF 

DEF RLN(FLEN)
    NLN = ""
    NBF = ""
    IF CURS+CSZ > FLEN THEN CSZ = FLEN - CURS
    IF LEN(BUF) < CSZ THEN
        NBF = READ(CURS, CSZ)
        CURS = CURS + LEN(NBF)  
    ENDIF
    IF NBF <> "" THEN BUF = BUF + NBF
    POS = FNL(BUF)
    IF POS <> -1 THEN
        NLN = LEFT(BUF, POS)
        BUF = RIGHT(BUF, LEN(BUF) - POS - 1)
    ELSEIF CURS >= FLEN THEN
        NLN = BUF
        BUF = ""
    ENDIF
    RETURN NLN
ENDDEF 
 
FNAM = INPUTPAYLOAD
GETIT = DOWNLOAD(FNAM)
IF GETIT <= 0 THEN EXIT 

WHILE TRUE
    LINE = RLN(GETIT)
    IF LINE = "" AND CURS >= GETIT THEN EXIT
    PRINT ">", LINE;
WEND
OUTPUTPAYLOAD="OK"
