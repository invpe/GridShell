'Kalman Filter 
'INPUTPAYLOAD="filename_to_parse,KalmanQ,KalmanR,column_idx_to_filter"
SVERSION="1.0"
PRINT "STARTING ",SVERSION," NOW";
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
DEF PTOKENIZER( TO_SPLIT , ARG)  
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
''''''''''''''''''''''''''''''''''' 
' Find me a \n and return byte number
DEF FINDNEWLINEPOS(STRINGUS)
  FOR ICT = 0 TO LEN(STRINGUS)-1
    CHAR = ASC(MID(STRINGUS,ICT,1))
    IF CHAR = 10 THEN 
      RETURN ICT
    ENDIF
  NEXT ICT
  RETURN -1
ENDDEF  
'''''''''''''''''''''''''''''''''''  
TEXT$ = "" 
STRIN = ""
VAR CURSOR = 0
VAR TOTALCOUNT = 0 
''''''''''''''''''''''''''''''''''' 
x = 0
P = 1
Q = 1
R = 1
DEF KalmanUpdate(z)
K = P / (P + R)
x = x + K * (z - x)
P = (1 - K) * P
ENDDEF
'''''''''''''''''''''''''''''''''''
LISTA = LIST()   
 
' Take the filename
FNAME$ = PTOKENIZER(INPUTPAYLOAD,0) ' Filename to parse
qQ = PTOKENIZER(INPUTPAYLOAD,1) ' KalmanQ
rR = PTOKENIZER(INPUTPAYLOAD,2) ' KalmanR
vVALUE = PTOKENIZER(INPUTPAYLOAD,3) ' Which column to filter
Q = VAL(qQ)
R = VAL(rR)
VALUEPOSITION = VAL(vVALUE)

' Download the file from API server
PRINT "DOWNLOADING ",FNAME$;
VAR GETIT = DOWNLOAD(FNAME$)
PRINT "DOWNLOADED ",GETIT," BYTES"; 

' Continue until there is something to do
VAR CHUNKSIZE = 128 
WHILE READ(CURSOR,CHUNKSIZE) <> "" 
  NEWS =  READ(CURSOR,CHUNKSIZE)  
  STRIN = STRIN + NEWS  
  VAR NEWLINEPOS = FINDNEWLINEPOS(STRIN)  

  WHILE NEWLINEPOS > 0 
    TOKENIZED = LEFT(STRIN,NEWLINEPOS)
    TOKENIZED = TOKENIZED +"," 
 
    CLEAR(LISTA)
    TOKENIZE(LISTA, TOKENIZED)  
    VAR RAWVALUE = GET(LISTA,VALUEPOSITION)  

    ' Kalman
    KalmanUpdate(VAL(RAWVALUE)) 
    TEXT$ = TEXT$  + STR(x) + CHR(10)
 	
 	  '
    VAR THESIZE = LEN(STRIN) - NEWLINEPOS  

    ' We need to get new chunk
    IF THESIZE < 10 THEN
      NEWLINEPOS = -1
    ELSE
      STRIN       = MID(STRIN,NEWLINEPOS + 1,THESIZE - 1) 
      NEWLINEPOS  = FINDNEWLINEPOS(STRIN)     
    ENDIF
  WEND     
 CURSOR = CURSOR + CHUNKSIZE 
 TOTALCOUNT = TOTALCOUNT+CHUNKSIZE
WEND
''''''''''''''''''''''''
 
OUTPUTPAYLOAD=TEXT$
PRINT OUTPUTPAYLOAD;
 
 