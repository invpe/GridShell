DIM HRC(24)
DIM PM1S(24)
DIM PM25S(24)
DIM PM10S(24)
DIM HUS(24)
DIM TMS(24) 
CURS=0
TCT=0
CURS=0
IJP=0
GOD=0
CSZ=64
LINE=""
LAL=LIST()
BUF="" 

DEF MAX(a,b)
IF a>b THEN RETURN a
RETURN b
ENDDEF 
 
DEF PM2IJP(A,B)
R=MAX(A/60,B/100)*5
RETURN R
ENDDEF 

LAL=CSV2LIST(INPUTPAYLOAD)
FF=GET(LAL,0)
MAC=GET(LAL,1) 
CLEAR(LAL)

GETIT=DOWNLOAD(FF)
IF GETIT <= 0 THEN EXIT 

RESETFPOS()
TZONE=1
WHILE TRUE
LINE=READLINE()
IF LINE="" THEN EXIT
CLEAR(LAL)
LAL=CSV2LIST(LINE)
EPOCH=GET(LAL,0)
FMAC=GET(LAL,1)
IF FMAC = MAC THEN	
	PM1=GET(LAL,2)
	PM25=GET(LAL,3)
	PM10=GET(LAL,4)
	HUMI=GET(LAL,5)
	TEMP=GET(LAL,6)
	'' CONVERT TO DAY HOUR 
	GOD=VAL(EPOCH)/3600
	GOD=(GOD+TZONE) MOD 24
	HRC(GOD)=HRC(GOD)+1
	PM1S(GOD)=PM1S(GOD)+VAL(PM1)
	PM25S(GOD)=PM25S(GOD)+VAL(PM25)
	PM10S(GOD)=PM10S(GOD)+VAL(PM10)
	HUS(GOD)=HUS(GOD)+VAL(HUMI)
	TMS(GOD)=TMS(GOD)+VAL(TEMP)
ENDIF
WEND   
IJP=0
OUTPUTPAYLOAD=""
FOR I=0 TO 23
IF HRC(I)>0 THEN
P1A=PM1S(I)/HRC(I)
P25A=PM25S(I)/HRC(I)
P10A=PM10S(I)/HRC(I)
HA=HUS(I)/HRC(I)
TA=TMS(I)/HRC(I)
IJP=PM2IJP(P25A,P10A)
IJP=FLOOR(IJP) 
OUTPUTPAYLOAD=OUTPUTPAYLOAD+STR(I)+","+STR(IJP)+CHR(10)
ENDIF
NEXT I  
