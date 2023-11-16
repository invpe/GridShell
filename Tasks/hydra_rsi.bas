''Exponential Moving Average
''Dataset File: EPOCH,CLOSING_PRICE,
OUTPUTPAYLOAD = ""
CURS = 0
CSZ = 64 
BUF = "" 
LIST_PRICE=LIST() 
LIST_RSI=LIST() 

DEF DO_RSI(LISTA,PERIOD) 
	UPGAINS = 0
    DOWNGAINS = 0
    FIRST_RUN = 1
    FOR I = 1 TO LEN(LISTA) - 1
        CHANGE = GET(LISTA, I) - GET(LISTA, I - 1)
        IF CHANGE > 0 THEN
            UPGAINS = UPGAINS + CHANGE
        ELSE
            DOWNGAINS = DOWNGAINS - CHANGE  
        ENDIF
        IF I >= PERIOD THEN
            IF FIRST_RUN = 1 THEN
                AVG_UPGAIN = UPGAINS / PERIOD
                AVG_DOWNGAIN = DOWNGAINS / PERIOD
                FIRST_RUN = 0
            ELSE              
                 IF CHANGE > 0 THEN
                    AVG_UPGAIN = ((AVG_UPGAIN * (PERIOD - 1)) + CHANGE) / PERIOD
                ELSE
                    AVG_UPGAIN = (AVG_UPGAIN * (PERIOD - 1)) / PERIOD
                ENDIF
                
                IF CHANGE < 0 THEN
                    AVG_DOWNGAIN = ((AVG_DOWNGAIN * (PERIOD - 1)) - CHANGE) / PERIOD
                ELSE
                    AVG_DOWNGAIN = (AVG_DOWNGAIN * (PERIOD - 1)) / PERIOD
                ENDIF            
            ENDIF

            RS = AVG_UPGAIN / AVG_DOWNGAIN
            RSI = 100 - (100 / (1 + RS))
            PUSH(LIST_RSI, RSI)
        ELSE
            PUSH(LIST_RSI, 0)  
        ENDIF
    NEXT I
ENDDEF

DEF TOK(WHR,TSP)
    CLEAR(WHR)
	CRS=""
	SLEN=LEN(TSP)-1
	FOR ICT=0 TO SLEN
	CHARV=MID(TSP,ICT,1)
	IF CHARV<>"," THEN
	CRS=CRS+CHARV
	ELSE
	PUSH(WHR,CRS)
	CRS=""
	ENDIF
	NEXT ICT
ENDDEF 
DEF FNL(STRS)
    SLEN = LEN(STRS) - 1
    FOR ICT = 0 TO SLEN
        CCC = MID(STRS, ICT, 1)
        IF ASC(CCC) = 10 THEN RETURN ICT
    NEXT ICT
    RETURN -1
ENDDEF 
DEF RLN(FLEN)
    NLN = ""
    NBF = ""
    IF CURS+CSZ > FLEN THEN CSZ = FLEN - CURS
    IF LEN(BUF) < CSZ THEN
        NBF = READ(CURS, CSZ)
        CURS = CURS + LEN(NBF)  
    ENDIF
    IF NBF <> "" THEN BUF = BUF + NBF
    POS = FNL(BUF)
    IF POS <> -1 THEN
        NLN = LEFT(BUF, POS)
        BUF = RIGHT(BUF, LEN(BUF) - POS - 1)
    ELSEIF CURS >= FLEN THEN
        NLN = BUF
        BUF = ""
    ENDIF
    RETURN NLN
ENDDEF 

PAYLOAD=LIST() 
TOK(PAYLOAD,INPUTPAYLOAD)
FNAM = GET(PAYLOAD,0)
PERIOD = VAL(GET(PAYLOAD,1))

GETIT = DOWNLOAD(FNAM)
IF GETIT <= 0 THEN EXIT 

LAL=LIST()
WHILE TRUE
    LINE = RLN(GETIT)
    IF LINE = "" OR CURS >= GETIT THEN EXIT	
	TOK(LAL,LINE)
	EPOCH=GET(LAL,0)	
	VVV=GET(LAL,1)
	PUSH(LIST_PRICE,VAL(VVV))	
    PRINT ">", EPOCH;
WEND

DO_RSI(LIST_PRICE,PERIOD)
CLEAR(LIST_PRICE)

FOR I = 0 TO LEN(LIST_RSI) - 1
V = GET(LIST_RSI,I)
OUTPUTPAYLOAD=OUTPUTPAYLOAD+STR(V)+CHR(10)
NEXT I