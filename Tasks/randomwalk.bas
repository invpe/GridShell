OUTPUTPAYLOAD=""
DIM inputs(3)

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
LAL=LIST()
TOK(LAL,INPUTPAYLOAD)

inputs(0)=VAL(GET(LAL,0))
inputs(1)=VAL(GET(LAL,1))
inputs(2)=VAL(GET(LAL,2))
num_steps = VAL(GET(LAL,3))
SRND(VAL(GET(LAL,4))) 

x = inputs(0)
y = inputs(1)
z = inputs(2) 

FOR i = 1 TO num_steps 
    stepX = RND * 2 - 1
    stepY = RND * 2 - 1
    stepZ = RND * 2 - 1
    x = x + stepX
    y = y + stepY
    z = z + stepZ
    x = FLOOR(x)
    y = FLOOR(y)
    z = FLOOR(z)
    out = x+y+z
    OUTPUTPAYLOAD=OUTPUTPAYLOAD+STR(out)+CHR(10)
NEXT i
 