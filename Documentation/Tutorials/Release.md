# ⏩ Setting up from a release file
If you're not planning to work with sources, the most convienient way to setup a GS Node is to upload release to the board.

Follow the tutorial below, to program your board and configure the node in few simple steps.

## Download latest merged release file 
Click [https://github.com/invpe/GridShell/releases/latest/download/gridshellmerged.bin](https://github.com/invpe/GridShell/releases/latest/download/gridshellmerged.bin) and download 
all in one binary file, which you can simply upload to your esp32 device over WEB (simplest) or command line (advanced).


## Use web flasher - flashing from the web page (Chrome)
1. Open [https://espressif.github.io/esptool-js/](https://espressif.github.io/esptool-js/)

![image](https://github.com/invpe/GridShell/assets/106522950/8fe54ed2-7c97-4e25-802f-f3aef100e364)

   
2. Plug in your ESP32 device to USB, set port speed to 115200 and click connect.
  

3. Select **Serial port**

 ![image](https://github.com/invpe/GridShell/assets/106522950/b4c4cc90-5754-4c20-a9ef-2ad43951313f)

4. Provide files to flash (gridshellmerged.bin)

 ![image](https://github.com/invpe/GridShell/assets/106522950/72c01d34-1de1-42a0-8099-ea1fb0357021)

 
7. Click **Erase Flash**

 ![image](https://github.com/invpe/GridShell/assets/106522950/f3eacfce-4dc5-47b7-a103-3987213284c3)

8. Set flash address to **0x0**

![image](https://github.com/invpe/GridShell/assets/106522950/bae179f6-e717-4e95-9634-e1216239cab5)

9. Click **Program**

![image](https://github.com/invpe/GridShell/assets/106522950/2878acfe-2b8c-4f07-b4ea-b2e78f7a8baa)

    
10. Wait for the flasher to complete
11. Reset the device (remove from USB, plug in again)



## Use esptool - flashing from command line (advanced)
Simply execute: 

```
python3 esptool.py --chip esp32 --port "/dev/ttyUSB1" --baud 1500000  erase_flash

python3 esptool.py --chip esp32 --port "/dev/ttyUSB1" --baud 1500000  --before default_reset --after hard_reset write_flash  -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x0 gridshellmerged.bin
```

Remembering to replace `/dev/ttyUSB1` with your port.



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
Let us know [here](https://github.com/invpe/GridShell/issues), we will find a solution for sure!


