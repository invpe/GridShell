# 🧪  Bencharking BAS execution 

Here i will store some benchmark results i performed as part of playing with the BAS execution.
All benchmarks were performed on the [Playground](https://github.com/invpe/GridShell/tree/main/Sources/ESPPlayground) sketch, using M5 Stack ATOM ESP32 Board.

## READ
Reading `5x500` (2.5k) bytes from `TELEMETRY` file stored on `SPIFFS`
```
FOR I = 0 TO 5
TXT = READ(I,500)
NEXT I
```
 
Time of execution: `187/300000 ms`



