' This example will tokenize the comma separated string given in INPUTPAYLOAD
' Tokens will be stored in the LIST object called LISTA
' It will return first token from the tokenized list
' Example Input Payload: A,B,C,D,

' Define a function that will tokenize the given string and store it in a list object
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

' Create a list object
LISTA = LIST()
' Tokenize the input payload
TOK(LISTA,INPUTPAYLOAD)
' Return first token
OUTPUTPAYLOAD=GET(LISTA,0)
