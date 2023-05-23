REM This script is used to do a rough estimation
REM of the GRID performance in floating point operations per second.
REM Beware! This is as rough as it can be :)
REM INPUTPAYLOAD="1,100,"
DEF TOKENIZE( TO_SPLIT , ARG)  
IDX = 0 
CUR_STRING = ""
FOR ICT = 0 TO LEN(TO_SPLIT)-1
  LET CHARV = MID( TO_SPLIT, ICT , 1 )
  IF CHARV <> "," THEN 
    CUR_STRING = CUR_STRING + CHARV 
  ELSE  
    IF IDX = ARG THEN
      RETURN CUR_STRING
    ENDIF
    CUR_STRING = "" 
    IDX = IDX + 1
  ENDIF
NEXT ICT 
ENDDEF  
AS = TOKENIZE(INPUTPAYLOAD,0)
BS = TOKENIZE(INPUTPAYLOAD,1)
A = VAL(AS)
B = VAL(BS)
FOR I = 0 TO B
  A = A * 1
NEXT I
OUTPUTPAYLOAD = STR(A) 
PRINT OUTPUTPAYLOAD;
