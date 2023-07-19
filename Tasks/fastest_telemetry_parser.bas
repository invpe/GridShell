'' FASTEST TELEMETRY PARSER I COULD WRITE ''
'' DOWNLOAD THE FILE FIRST 

BYTES = DOWNLOAD("----_TELEMETRY_---FILE_----")
IF BYTES = 0 THEN EXIT

CURSOR = 0 
CHUNKSIZE = 500
LINE_AS_LIST = LIST()
BUFFER = ""
 
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

DEF FINDNEWLINEPOS(STRINGUS)
  SLEN=LEN(STRINGUS)-1 
  FOR ICT = 0 TO SLEN
    CCC = MID(STRINGUS,ICT,1)
    IF ASC(CCC) = 10 THEN RETURN ICT
  NEXT ICT
RETURN -1
ENDDEF  


DEF READLINE(FILE_LEN)
  NEWLINE=""
  NBUFFER = "" 

  IF LEN(BUFFER) < CHUNKSIZE THEN
    NBUFFER = READ(CURSOR,CHUNKSIZE)
    CURSOR = CURSOR + CHUNKSIZE 
  ENDIF

  IF CURSOR+CHUNKSIZE > FILE_LEN THEN CHUNKSIZE = FILE_LEN - CURSOR

  IF NBUFFER <> "" THEN  
    BUFFER=BUFFER+NBUFFER
  ENDIF

  POS = FINDNEWLINEPOS(BUFFER)
  IF POS <> -1 THEN
    NEWLINE = LEFT(BUFFER,POS-1)
    ZOSTAW = LEN(BUFFER) - POS - 1
    IF ZOSTAW > 0 THEN 
    BUFFER=RIGHT(BUFFER,ZOSTAW)
    ELSE 
    BUFFER=""
    ENDIF
  ENDIF 
   
  RETURN NEWLINE
ENDDEF

WHILE TRUE
  LINE = READLINE(BYTES) 
  IF LINE = "" THEN EXIT
  CLEAR(LINE_AS_LIST)
  LINE=LINE+","
  PRINT LINE;
''  TOKENIZE(LINE_AS_LIST,LINE)
''  PRINT GET(LINE_AS_LIST,3);
WEND  
