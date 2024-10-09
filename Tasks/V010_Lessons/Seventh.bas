' In this example we will use CS2LIST function to convert comma separated string to a list object
' This is faster than the TOK we used previously.
' Example payload: A,B,C,D,E,F,G

LISTA = LIST()
LISTA = CSV2LIST(INPUTPAYLOAD)
OUTPUTPAYLOAD=GET(LISTA,0)
