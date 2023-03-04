REM Genearte hourly averages on daily basis   
'''''''''''''''''''''''''''''''''''
SVERSION="1.4"
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


DIM HRCOUNTER(24)
DIM DBIAVG(24) 
STRIN = ""
VAR CURSOR = 0
VAR TOTALCOUNT = 0 



LISTA = LIST()   
 
' Take the filename
FNAME$  = INPUTPAYLOAD
 
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
    PRINT "[",TOKENIZED,"]";   
 
    CLEAR(LISTA)
    TOKENIZE(LISTA, TOKENIZED) 
    VAR EPOCH   = GET(LISTA,0)
    VAR HR      = GET(LISTA,1)
    VAR MT      = GET(LISTA,2)
    VAR DBI     = GET(LISTA,3) 
    VAR GODZINA = VAL(HR)   

    ' Add count of samples per hour 
    HRCOUNTER(GODZINA) = HRCOUNTER(GODZINA)+1
   
    ' Sum samples together, we will divdie later  
    DBIAVG(GODZINA)  = DBIAVG(GODZINA)+VAL(DBI) 
 
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
''''''''''''''''''''''''
''''''''''''''''''''''''
TEXT$       = ""
STROUTPUT$  = ""
FOR I = 0 TO 23 

  '' No data from this hour, zero out
  IF HRCOUNTER(I) = 0 THEN
    STROUTPUT$ = STR(I)+",0,"+CHR(10)
  ELSE
    STROUTPUT$ = STR(I)+","+STR(DBIAVG(I)/HRCOUNTER(I))+CHR(10)  
  ENDIF 

  '' Append final string to write
  TEXT$ = TEXT$ + STROUTPUT$
NEXT I
 
OUTPUTPAYLOAD=TEXT$
PRINT OUTPUTPAYLOAD;
 