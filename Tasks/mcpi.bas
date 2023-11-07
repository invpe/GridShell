' GridShell Node Script
' Generate random points and count those inside the quarter-circle
' INPUTPAYLOAD will be in the format "num_points,seed,"
' OUTPUTPAYLOAD will be in the format "inside_count, total_count"
' Created by ChatGPT 

DEF TOK(WHR,TSP)
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

' Tokenize input payload
LAL=LIST()
TOK(LAL,INPUTPAYLOAD)

NUM_POINTS = GET(LAL,0)
NUM_POINTS = VAL(NUM_POINTS)

' Apply random seed
SR = GET(LAL,1)
SR = VAL(SR)
SRND(SR)

INSIDE_COUNT = 0

' Calculate
FOR I = 1 TO NUM_POINTS
  X = RND
  Y = RND
  IF X*X + Y*Y <= 1 THEN INSIDE_COUNT = INSIDE_COUNT + 1
NEXT I

TOTAL_COUNT = NUM_POINTS

PRINT "Inside count: ", INSIDE_COUNT;
PRINT "Total count: ", TOTAL_COUNT;

' Return
OUTPUTPAYLOAD = STR(INSIDE_COUNT) + "," + STR(TOTAL_COUNT)
