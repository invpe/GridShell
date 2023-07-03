# 🧪  Bencharking BAS execution 

Here i will store some benchmark results i performed as part of playing with the BAS execution.
All benchmarks were performed on the [Playground](https://github.com/invpe/GridShell/tree/main/Sources/ESPPlayground) sketch, using M5 Stack ATOM ESP32 Board.

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
Reading `5x500` (2.5k) bytes from `TELEMETRY` file stored on `SPIFFS`
```
FOR I = 0 TO 5
TXT = READ(I,500)
NEXT I
```
 
Time of execution: `187/300000 ms`



