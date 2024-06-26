# Vanilla nodes for GS

This repository contains the source code for pure vanilla nodes in GridShell. Recently, I have added support for the ESP8266, enabling these boards to join the network as well.

The compilation process will determine the appropriate version to compile, utilizing #ifdef directives to manage this.

Specific items regarding devices supported by the source code.


## ESP8266

### Base64
There's no (or i coudln't find) base64 encoder/decoder, so i had to install the base64 library to get that part of the code done.
Within the core SDK there is base64 encoder, but i couldn't find decoder so we have to use an external lib for esp8266.


## ESP32

Nothing atm.
