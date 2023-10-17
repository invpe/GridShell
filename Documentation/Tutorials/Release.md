# ⏩ Setting up from a release file
If youre not planning to work with sources, the most convienient way to setup a GS Node is to upload release to the board.
Follow the tutorial below, to program your board and configure the node in few simple steps.

1. Download latest release files [https://github.com/invpe/GridShell/releases](https://github.com/invpe/GridShell/releases)

## Use esptool
Simply execute: 

`python3 esptool.py --chip esp32 --port "/dev/ttyUSB1" --baud 1500000  --before default_reset --after hard_reset write_flash  -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x10000 latest.bin`

Remembering to replace `/dev/ttyUSB1` with your port.

## Use web flasher 
1. Open [https://esp.huhn.me/](https://esp.huhn.me/)
2. Plug in your ESP32 device

![image](https://github.com/invpe/GridShell/assets/106522950/81c7c593-cba8-4829-95ed-0ca9fe7d6fea)

_Image by Sparkfun_


3. Select **Connect**

   ![image](https://github.com/invpe/GridShell/assets/106522950/a34b65f8-e819-4e6b-a45e-9bbb7cb38ec5)


4. Select **Serial port**

![image](https://github.com/invpe/GridShell/assets/106522950/b8681ded-7dc3-4a4e-a4a9-6afa42fdcf36)


5. Connect

![image](https://github.com/invpe/GridShell/assets/106522950/6217a0fc-a1f9-4575-97ca-cdf6eed8110c)
 
6. Provide files to flash

![Screenshot from 2023-10-08 15-05-22](https://github.com/invpe/GridShell/assets/106522950/67f47bf0-49d6-4abe-aefa-260ea24fcd44)
 
7. Click **PROGRAM**

![Screenshot from 2023-08-29 09-43-40](https://github.com/invpe/GridShell/assets/106522950/ac02ce31-1c87-4009-b0f5-75bfb0920b80)

8. Reset the device (remove from USB, plug in again)

![image](https://github.com/invpe/GridShell/assets/106522950/14812e2d-0b9b-4d1d-ad14-d386ac10e669)


# Configuring the node

1. Open the serial monitor [https://serial.huhn.me/](https://serial.huhn.me/)

![image](https://github.com/invpe/GridShell/assets/106522950/eeaca9d8-475a-4c31-9531-816f41eae5f5)

2. Send "HELLO" via Serial terminal 

![image](https://github.com/invpe/GridShell/assets/106522950/b22120d6-3bcc-450b-85a5-e3b14f542e80)

3. The Node will ask config questions, provide all the answers


![image](https://github.com/invpe/GridShell/assets/106522950/77c5b265-a98a-40db-8bbf-9ed6e0dbca19)

# 🍏 Success
The configuration is now saved, you are good to go 👍

# 🤬 Made a mistake?
Don't worry, just follow the steps again ! 🤝

# :thought_balloon: Issues?
Let us know [here](https://github.com/invpe/GridShell/discussions), we will find a solution for sure!


