# 🧪  Bencharking BAS execution 

Here i will store some benchmark results i performed as part of playing with the BAS execution.
To keep benchmark consistent, following setup was used:

- Software [Playground](https://github.com/invpe/GridShell/tree/main/Sources/ESPPlayground) sketch
- `GS_DEBUG` disabled
- No Serial printing
- Hardware `M5 Stack ATOM ESP32 Board`
- Compiled with `Minimal SPIFFS (Large APP with OTA)`
  
## PRIME
Find prime numbers between `0 and 100`

```
FOR N = 0 TO 100
M = 2
ISP = 1
WHILE M < N
IF N MOD M = 0 THEN ISP = 0 ELSE M = M + 1
IF ISP = 0 THEN EXIT
WEND
IF ISP = 1 THEN PRINT STR(N);
NEXT N 
```
Time of execution 	`5731/300000 ms`


## SHA1
Generate SHA1 from a string

```
HASH = SHA1("THIS IS A PLAIN TEXT") 
```

Time of execution 	`46/300000 ms`


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


## TOKENIZE string to list optimized 1
Move `LEN(TO_SPLIT)` outside of the `FOR` loop

```
DEF TOKENIZE(GDZIE, TO_SPLIT )   
  CUR_STRING = "" 
  SLEN = LEN(TO_SPLIT)-1 ''<<<<<<<<
  ICT = 0
  FOR ICT = 0 TO SLEN
    CHARV = MID( TO_SPLIT, ICT , 1 )
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
Time of execution 	`659/300000 ms`

## TOKENIZE empty FOR loop
Empty FOR loop

```
 
DEF TOKENIZE(GDZIE, TO_SPLIT )   
  CUR_STRING = "" 
  SLEN = LEN(TO_SPLIT)-1
  ICT = 0
  FOR ICT = 0 TO SLEN
   
  NEXT ICT 
ENDDEF 
CSVLIST=LIST()
STRCSV="THIS,IS,A,DUMMY,COMMA,SEPARATED,TEXT,WITH,SOME,RANDOM VALUES,LIKE,2,43,5,56,7,3,45,234,"
TOKENIZE(CSVLIST,STRCSV)
```
Time of execution 	`120/300000 ms`


## TOKENIZE Add MID in the FOR loop
Adding MID to the FOR loop

``` 
DEF TOKENIZE(GDZIE, TO_SPLIT )   
  CUR_STRING = "" 
  SLEN = LEN(TO_SPLIT)-1
  ICT = 0
  FOR ICT = 0 TO SLEN
    CHARV = MID( TO_SPLIT, ICT , 1 )   
  NEXT ICT 
ENDDEF 
CSVLIST=LIST()
STRCSV="THIS,IS,A,DUMMY,COMMA,SEPARATED,TEXT,WITH,SOME,RANDOM VALUES,LIKE,2,43,5,56,7,3,45,234,"
TOKENIZE(CSVLIST,STRCSV)
```
Time of execution 	`348/300000 ms`


## TOKENIZE Add MID in the FOR loop, disable MB_ENABLE_UNICODE
Remove UNICODE Support from mybasic.h

`//#	define MB_ENABLE_UNICODE`


``` 
DEF TOKENIZE(GDZIE, TO_SPLIT )   
  CUR_STRING = "" 
  SLEN = LEN(TO_SPLIT)-1
  ICT = 0
  FOR ICT = 0 TO SLEN
    CHARV = MID( TO_SPLIT, ICT , 1 )   
  NEXT ICT 
ENDDEF 
CSVLIST=LIST()
STRCSV="THIS,IS,A,DUMMY,COMMA,SEPARATED,TEXT,WITH,SOME,RANDOM VALUES,LIKE,2,43,5,56,7,3,45,234,"
TOKENIZE(CSVLIST,STRCSV)
```
Time of execution 	`298/300000 ms`
