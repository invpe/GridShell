'+-----------------------------------------------------+
' DUCO Block miner, experimental for computing purposes
' INPUT: prevhashnexthashdifficulty*100
' EXAMPLE: f4b6921e7758d30abb10fcdf385d0462b5344ac1f92ffe303304034517a62c19d03c50d05cca42138
'+-----------------------------------------------------+
LENP=LEN(INPUTPAYLOAD)-1
PREVH=LEFT(INPUTPAYLOAD,40)
NEXTH=MID(INPUTPAYLOAD,40,40)
DFCLTY=RIGHT(INPUTPAYLOAD,LENP-79)
FIND=-1
FF=VAL(DFCLTY) 
FOR I=0 TO FF 
IF SHA1(PREVH+STR(I)) = NEXTH THEN
FIND=I
EXIT
ENDIF
NEXT I
OUTPUTPAYLOAD=STR(FIND)
