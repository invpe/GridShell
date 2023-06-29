# :turtle: The simplest way to start using GridShell

In this article, we will set you up, step by step so that you can start using GridShell like a pro 👍
You will write your first ESP32 Arduino sketch, which will write telemetry to Grid which later we will visualize 🥇

Follow each point, take small steps straight to the 🏁 finish line.


## Your workplace

We begin this tutorial by creating a folder on your desktop and adding necessary files to it, so you've got all things in one place.


1. Create a folder called **Simple** on your desktop
2. Download all files from [Library](https://github.com/invpe/GridShell/tree/main/Sources/GridShell) and put them into your newly created **Simple** folder
3. Download [Simple](https://github.com/invpe/GridShell/blob/main/Sources/Integrations/Simple/Simple.ino) sketch and put it into your newly  created **Simple** folder

Once all above steps have been completd, your folder should have following files inside

![image](https://github.com/invpe/GridShell/assets/106522950/405e6371-fbfb-42c0-bee6-95f6891eb6b6)

If all is good, you deserve a slice of 🍕

## Arduino

We will now open the sketch file with Arduino and compile it.
Prepare for a easy ride but don't take another slice of 🍕 yet  😄

1. Open your newly created **Simple** folder
2. Double click on **Simple.ino** which is a sketch file
3. Arduino IDE will open

![image](https://github.com/invpe/GridShell/assets/106522950/0fb04ad5-1ecb-411a-aa96-094e09da8b15)

4. You will be presented with the **source code** of the Simple sketch

🛑 If you don't have your own GridShell User account, you need it now - go [here](https://github.com/invpe/gridshell/blob/main/Documentation/Tutorials/Join.md) to create it.


We have to update exactly three things to make it work.
Fill necessary details in definitions as commented:

#define GRID_U  ""      // << Provide your Grid UserHash
#define WIFI_A  ""      // << Provide your WiFi Access Point Name
#define WIFI_P  ""      // << Provide your WiFi Access Point Password

Once done, we're ready to move forward, just as an example my setup is as follows:

#define GRID_U  f3y2hssbcvxert243hrhsddfasd24dsfas2323348282a3bd4b""      // << Provide your Grid UserHash
#define WIFI_A  "superinternet"      // << Provide your WiFi Access Point Name
#define WIFI_P  "superpasswordyes"      // << Provide your WiFi Access Point Password



