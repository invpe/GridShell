'Average selected column values
'INPUTPAYLOAD="file,colidx," 
'''''''''''''''''''''''''''''''''''
DEF TOKENIZE(GDZIE, TO_SPLIT )   
  CUR_STRING = ""
  FOR ICT = 0 TO LEN(TO_SPLIT)-1
    LET CHARV = MID( TO_SPLIT, ICT , 1 )
    IF CHARV <> "," THEN 
      CUR_STRING = CUR_STRING + CHARV 
    ELSE  
      PUSH(GDZIE,CUR_STRING)
      CUR_STRING = "" 
    ENDIF
  NEXT ICT 
ENDDEF 
'''''''''''''''''''''''''''''''''''  
TUCURSOR = 0 
''''''''''''''''''''''''''''''''''' 
DEF READ_UNTIL(ASC_CHARACTER)
STRO=""
WHILE TRUE
    NEWS = READ(TUCURSOR,1)
    '' END
    IF NEWS = "" THEN
    RETURN ""
    ENDIF
    '' NEW LINE
    IF ASC(NEWS) = ASC_CHARACTER THEN
        TUCURSOR = TUCURSOR + 1
        RETURN STRO
    ELSE 
        STRO = STRO + NEWS
    ENDIF
    TUCURSOR=TUCURSOR+1
WEND  
ENDDEF
''''''''''''''''''''''''''''''''''' 
PAYLOAD=LIST() 
TOKENIZE(PAYLOAD,INPUTPAYLOAD)
FNAME = GET(PAYLOAD,0)
VIDX = GET(PAYLOAD,1) 

BYTES = DOWNLOAD(FNAME)
IF BYTES = 0 THEN
EXIT
ENDIF

VTOTAL = 0
VTOTALCOUNT = 0

LISTA = LIST()  

WHILE TRUE
 LINE = READ_UNTIL(10)
 IF LINE<>"" THEN
  LINE=LINE+","
  CLEAR(LISTA)
  TOKENIZE(LISTA, LINE)   
  COLVALUE = GET(LISTA,VAL(VIDX))   
  VTOTAL = VTOTAL+VAL(COLVALUE)
  VTOTALCOUNT = VTOTALCOUNT + 1
 ELSE
  EXIT
 ENDIF
WEND

VRESULT = VTOTAL/VTOTALCOUNT
OUTPUTPAYLOAD=STR(VRESULT)  