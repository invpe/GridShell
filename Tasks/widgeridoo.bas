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
TTITLE=PTOKENIZER(INPUTPAYLOAD,0)
TVALUE=PTOKENIZER(INPUTPAYLOAD,1) 
JSONPAYLOAD="{"+CHR(34)+"Title"+CHR(34)+":"+CHR(34)+TTITLE+CHR(34)+","+CHR(34)+"Value"+CHR(34)+":"+CHR(34)+TVALUE+CHR(34)+"}" 
OUTPUTPAYLOAD=JSONPAYLOAD