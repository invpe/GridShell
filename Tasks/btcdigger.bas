'+--------------------------------------------------------------+
' Very slow BTC Block miner :-) 1000 nonces in 60seconds
' Limited with the max nonce value of int (2147483647)
' INPUTPAYLOAD=SERIALIZED_BLOCK(152)TARGET_HEX(64)START_POS
' EXAMPLE: "0100000050120119172a610421a6c3011dd330d9df07b63616c2cc1f1cd00200000000006657a9252aacd5c0b2940996ecff952228c3067cc38d4885efb5a4ac4247e9f337221b4d4c86041b0000000000000000000000000000000000000000000000004c86040000000000274148012"
' NONCE TO SOLVE: 274148111
'+--------------------------------------------------------------+
hex_chars = LIST("0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f")
LENP=LEN(INPUTPAYLOAD)-1
INPUT_BLOCK=MID(INPUTPAYLOAD,0,152)
INPUT_TARGET=MID(INPUTPAYLOAD,152,64)
START_POS=VAL(RIGHT(INPUTPAYLOAD,LENP-152-63))
END_POS=START_POS+100
DEF I2HEX(lValue)
hexString = ""    
For i = 0 To 3
byteValue = FLOOR(lValue / (256 ^ i)) MOD 256
high = FLOOR(byteValue / 16)
If high < 10 Then
hexString = hexString + CHR(48 + high)
Else
hexString = hexString + CHR(87 + high)
EndIf
low = byteValue MOD 16
If low < 10 Then
hexString = hexString + CHR(48 + low)   
Else
hexString = hexString + CHR(87 + low) 
EndIf
Next i
Return hexString
ENDDEF
DEF LITTLE_ENDIAN_BIT_COMPARISON(binA, binB)  
If LEN(binA) <> LEN(binB) THEN
RETURN -2  
ENDIF
FOR i = LEN(binA) - 1 TO 0 STEP -1
byteA = GET(binA, i)
byteB = GET(binB, i) 
IF byteA < byteB THEN
RETURN -1
ELSEIF byteA > byteB THEN
RETURN 1
ENDIF
NEXT i
RETURN 0
ENDDEF 
OUTPUTPAYLOAD="-1" 
FOR Z = START_POS TO END_POS 
    NONCE = Z
    NONCE_HEX = I2HEX(NONCE)
    SER_BLOCK=INPUT_BLOCK+NONCE_HEX
    DHASH=SHA256H(SHA256H(SER_BLOCK))
    A=HEXTOBIN(DHASH)
    B=HEXTOBIN(INPUT_TARGET)
    RET=LITTLE_ENDIAN_BIT_COMPARISON(A,B)
    CLEAR(A)
    CLEAR(B)
    IF RET <-1 THEN
        OUTPUTPAYLOAD=STR(NONCE)
        EXIT
    ENDIF
NEXT Z