A = INPUTPAYLOAD 
FOR I = 0 TO 20000
A = SHA256(A)
NEXT I 
OUTPUTPAYLOAD=A