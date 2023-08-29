# ⏩ Setting up from a release file
If youre not planning to work with sources, the most convienient way to setup a GS Node is to upload release to the board.
Follow the tutorial below, to program your board and configure the node in few simple steps.

1. Download latest release files
2. Open [https://esp.huhn.me/](https://esp.huhn.me/)
3. Plug in your ESP32 device
4. Select **Connect**

   ![image](https://github.com/invpe/GridShell/assets/106522950/a34b65f8-e819-4e6b-a45e-9bbb7cb38ec5)


5. Select **Serial port**

![image](https://github.com/invpe/GridShell/assets/106522950/b8681ded-7dc3-4a4e-a4a9-6afa42fdcf36)


6. Connect

![image](https://github.com/invpe/GridShell/assets/106522950/6217a0fc-a1f9-4575-97ca-cdf6eed8110c)

7. Click **ERASE**

![image](https://github.com/invpe/GridShell/assets/106522950/7ebc1077-c6a5-4e7e-9479-1dd27ef8a34b)




Open the serial monitor [https://serial.huhn.me/](https://serial.huhn.me/)

Reset your ESP32

![image](https://github.com/invpe/GridShell/assets/106522950/eeaca9d8-475a-4c31-9531-816f41eae5f5)

Send "HELLO" via Serial terminal 

![image](https://github.com/invpe/GridShell/assets/106522950/b22120d6-3bcc-450b-85a5-e3b14f542e80)

The Node will ask config questions, provide all the answers

![image](https://github.com/invpe/GridShell/assets/106522950/77c5b265-a98a-40db-8bbf-9ed6e0dbca19)


The configuration is now saved to SPIFFS, you are good to go 👍
