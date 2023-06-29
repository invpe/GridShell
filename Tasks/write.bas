  SVERSION="1.0"
  PRINT "STARTING ",SVERSION," NOW";
  'INPUTPAYLOAD="FNAME,APPEND_FLAG,FROM_HERE,ALL,IS,WRITTEN\n'
  '''''''''''''''''''''''''''''''''''
  DEF PTOKENIZER( TO_SPLIT , CHARR, ARG)  
  IDX = 0 
  CUR_STRING = ""
  FOR ICT = 0 TO LEN(TO_SPLIT)-1
    LET CHARV = MID( TO_SPLIT, ICT , 1 )
    IF CHARV <> CHARR THEN 
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
  DEF SREPLACE(SWHERE,SWHAT,SWITH)
    NSTR=""
    FOR I = 0 TO LEN(SWHERE)-1
      IF MID(SWHERE,I,1) = SWHAT THEN
        NSTR = NSTR + SWITH
      ELSE
        NSTR = NSTR + MID(SWHERE,I,1)
      ENDIF
    NEXT I
  RETURN NSTR
  ENDDEF 
  '''''''''''''''''''''''''''''''''''  
  TFNAME=PTOKENIZER(INPUTPAYLOAD,",",0)
  TAPPEND=PTOKENIZER(INPUTPAYLOAD,",",1)

  '' Bad
  IF LEN(TFNAME) <= 0 THEN
  OUTPUTPAYLOAD=""
  EXIT
  ENDIF

  IF LEN(TAPPEND) <> 1 THEN
  OUTPUTPAYLOAD=""
  EXIT
  ENDIF

  CTR=0
  POS=-1
  AFLAG=FALSE
  FOR I = 0 TO LEN(INPUTPAYLOAD) - 1
   IF MID(INPUTPAYLOAD,I,1) = "," THEN
    CTR = CTR +1
   ENDIF

   IF CTR = 2 THEN
    POS = I+1
    PRINT POS;
    EXIT
   ENDIF
  NEXT I

  IF POS = -1 THEN
   OUTPUTPAYLOAD=""
  ELSE
   OUTPUTPAYLOAD=MID(INPUTPAYLOAD,POS,LEN(INPUTPAYLOAD)-POS)
  ENDIF

  IF LEN(OUTPUTPAYLOAD) <= 0 THEN
  EXIT
  ENDIF

  IF TAPPEND = "1" THEN
    AFLAG=TRUE
  ELSE 
    AFLAG=FALSE
  ENDIF

WRITE(TFNAME,OUTPUTPAYLOAD,AFLAG)
SHA = SHA1(OUTPUTPAYLOAD)
OUTPUTPAYLOAD = SHA