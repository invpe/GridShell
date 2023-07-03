# 🧪  Bencharking BAS execution 

Here i will store some benchmark results i performed as part of playing with the BAS execution.
To keep benchmark consistent, following setup was used:

- Software [Playground](https://github.com/invpe/GridShell/tree/main/Sources/ESPPlayground) sketch
- Hardware `M5 Stack ATOM ESP32 Board`
- Compiled with `Minimal SPIFFS (Large APP with OTA)`
  

## DOWNLOAD
Download `3757` bytes of `TELEMETRY` to local `SPIFFS`
File is downloaded from api endpoint.

```
FNAME="PocNetGroupMiners00000000000000000000001POOLT083AF25746D8202372"
BYTES = DOWNLOAD(FNAME)
IF BYTES = 0 THEN
PRINT "ERROR";
EXIT
ENDIF
```
Time of execution 	`5030/300000 ms`

## READ byte per byte
Reading `3757` bytes of `TELEMETRY` from `SPIFFS` one by one byte.

```
BYTES=TSIZE() 
FOR I = 0 TO BYTES
TXT = READ(I,1)
NEXT I
```
Time of execution 	42751/300000 ms

## READ in 500b chunks
Reading `7x500` (3.5k) bytes from `TELEMETRY` file stored on `SPIFFS`
```
FOR I = 0 TO 7
TXT = READ(I*500,500) 
NEXT I
```
 
Time of execution 	`187/300000 ms`

## TOKENIZE string to list
Split string by `,` to LIST object

```
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

CSVLIST=LIST()
STRCSV="THIS,IS,A,DUMMY,COMMA,SEPARATED,TEXT,WITH,SOME,RANDOM VALUES,LIKE,2,43,5,56,7,3,45,234,"
TOKENIZE(CSVLIST,STRCSV)
```

Time of execution 	`844/300000 ms`

## TOKENIZE string to list `10 times`
Split string by `,` to LIST objects and do it 10 times

```
CSVLIST=LIST()
FOR I = 0 TO 10
CLEAR(CSVLIST)
STRCSV="THIS,IS,A,DUMMY,COMMA,SEPARATED,TEXT,WITH,SOME,RANDOM VALUES,LIKE,2,43,5,56,7,3,45,234,"
TOKENIZE(CSVLIST,STRCSV) 
NEXT I
```
Time of execution 	`8170/300000 ms`


