SV="3.0"
DIM HRCOUNTER(24)
DIM PM1AVG(24)
DIM PM25AVG(24)
DIM PM10AVG(24)
DIM HUMIAVG(24)
DIM TEMPAVG(24)  
STRIN=""
VAR CURS=0
VAR TCT=0 
CURS=0 
CSZ=64
LAL=LIST()
BUFFER=""
LISTA=LIST()   

DEF TOK(GDZIE, TO_SPLIT )   
CUR_STRING=""
SLEN=LEN(TO_SPLIT)-1
FOR ICT=0 TO SLEN
CHARV=MID( TO_SPLIT, ICT , 1 )
IF CHARV <> "," THEN 
CUR_STRING=CUR_STRING+CHARV 
ELSE  
PUSH(GDZIE,CUR_STRING)
CUR_STRING="" 
ENDIF
NEXT ICT 
ENDDEF

DEF FNL(STRS)
SLEN=LEN(STRS)-1 
FOR ICT=0 TO SLEN
CCC=MID(STRS,ICT,1)
IF ASC(CCC)=10 THEN RETURN ICT
NEXT ICT
RETURN -1
ENDDEF  

DEF RLN(FLEN)
NEWLINE=""
NBUFFER="" 
IF LEN(BUFFER)<CSZ THEN
NBUFFER=READ(CURS,CSZ)
CURS=CURS+CSZ 
ENDIF
IF CURS+CSZ > FLEN THEN CSZ=FLEN - CURS
IF NBUFFER <> "" THEN  
BUFFER=BUFFER+NBUFFER
ENDIF
POS=FNL(BUFFER)
IF POS <> -1 THEN
NEWLINE=LEFT(BUFFER,POS-1)
ZOSTAW=LEN(BUFFER) - POS - 1
IF ZOSTAW > 0 THEN 
BUFFER=RIGHT(BUFFER,ZOSTAW)
ELSE 
BUFFER=""
ENDIF
ENDIF 
RETURN NEWLINE
ENDDEF
PRINT "STARTING ",SV," NOW"; 
FNAME$ =INPUTPAYLOAD 
VAR GETIT=DOWNLOAD(FNAME$)
IF GETIT <= 0 THEN EXIT

WHILE TRUE
LINE=RLN(GETIT) 
IF LINE="" THEN EXIT
CLEAR(LAL)
LINE=LINE+","
TOK(LAL,LINE)
VAR EPOCH  =GET(LAL,0)
VAR HR     =GET(LAL,1)
VAR PM1    =GET(LAL,3)
VAR PM25   =GET(LAL,4)
VAR PM10   =GET(LAL,5)
VAR HUMI   =GET(LAL,6)
VAR TEMP   =GET(LAL,7)
VAR GODZINA=VAL(HR)   
HRCOUNTER(GODZINA)=HRCOUNTER(GODZINA)+1
PM1AVG(GODZINA) =PM1AVG(GODZINA)+VAL(PM1)
PM25AVG(GODZINA)=PM25AVG(GODZINA)+VAL(PM25)
PM10AVG(GODZINA)=PM10AVG(GODZINA)+VAL(PM10)
HUMIAVG(GODZINA)=HUMIAVG(GODZINA)+VAL(HUMI)
TEMPAVG(GODZINA)=TEMPAVG(GODZINA)+VAL(TEMP)
WEND  
TXT="{"+CHR(34)+"HOURS"+CHR(34)+": ["
STRO=""
FOR I=0 TO 23 
IF HRCOUNTER(I)=0 THEN    
STRO="{"+CHR(34)+"PM1"+CHR(34)+": 0, "+CHR(34)+"PM25"+CHR(34)+": 0, "+CHR(34)+"PM10"+CHR(34)+": 0, "+CHR(34)+"HCHO"+CHR(34)+": 0, "+CHR(34)+"TEMP"+CHR(34)+": 0}"    
ELSE 
STRO="{"+CHR(34)+"PM1"+CHR(34)+": "+STR(PM1AVG(I)/HRCOUNTER(I))+","
STRO=STRO+CHR(34)+"PM25"+CHR(34)+": "+STR(PM25AVG(I)/HRCOUNTER(I))+","
STRO=STRO+CHR(34)+"PM10"+CHR(34)+": "+STR(PM10AVG(I)/HRCOUNTER(I))+","
STRO=STRO+CHR(34)+"HCHO"+CHR(34)+": "+STR(HUMIAVG(I)/HRCOUNTER(I))+","
STRO=STRO+CHR(34)+"TEMP"+CHR(34)+": "+STR(TEMPAVG(I)/HRCOUNTER(I))+"}"
ENDIF
IF I+1<24 THEN STRO=STRO+"," 
TXT=TXT+STRO
NEXT I
TXT=TXT+"]}"

OUTPUTPAYLOAD=TXT
PRINT OUTPUTPAYLOAD;