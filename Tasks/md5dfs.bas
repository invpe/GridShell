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
OUTPUTPAYLOAD = FMD5(TCHUNK)

