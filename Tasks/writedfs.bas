OUTPUTPAYLOAD=""
SVERSION="2.0"
PAYLOAD=LIST()
'''''''''''''''''''''''''''''''''''
DEF TOKENIZE(GDZIE, TO_SPLIT )   
CUR_STRING = ""
SLEN = LEN(TO_SPLIT)-1
FOR ICT = 0 TO SLEN
  CHARV = MID( TO_SPLIT, ICT , 1 )
  IF CHARV <> "," THEN 
    CUR_STRING = CUR_STRING + CHARV 
  ELSE  
    PUSH(GDZIE,CUR_STRING)
    CUR_STRING = "" 
  ENDIF
NEXT ICT 
ENDDEF
'''''''''''''''''''''''''''''''''''  
TOKENIZE(PAYLOAD,INPUTPAYLOAD)
TCHUNK=GET(PAYLOAD,0)
TDATA=GET(PAYLOAD,1)
IF LEN(TDATA) <= 0 THEN EXIT
TTXT=B64D(TDATA)
IF WRITE(TCHUNK,TTXT) = 1 THEN
OUTPUTPAYLOAD = FMD5(TCHUNK)

