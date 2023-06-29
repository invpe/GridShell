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

## Update source code with details

🛑 If you don't have your own GridShell User account, you need it now - go [here](https://github.com/invpe/gridshell/blob/main/Documentation/Tutorials/Join.md) to create it.


We have to update exactly three things to make it work.
Fill necessary details in definitions as commented:

`#define GRID_U  ""      // << Provide your Grid UserHash`

`#define WIFI_A  ""      // << Provide your WiFi Access Point Name`

`#define WIFI_P  ""      // << Provide your WiFi Access Point Password`


Once done, we're ready to move forward, you just provided your UserHash, and WiFi Credentials for your ESP32 to connect to the GridShell server.


## Optionally change filename and text to write

By default the sketch will write (not append) to **MYTELEMETRY** file every **MINUTE**, you can easily change that by updating these lines in the code:

`bool bAppend = false;               // << File append flag (true = yes, false = no)`

`String strFileName = "MYTELEMETRY"; // << Filename`

`String strTextToWrite = "1,3,5,6,777,\n"; // << Text to write`

`bAppend` - is a flag telling we want to overwrite every minute, not append to the file.

`strFileName` - is a filename, simply that

`strTextToWrite` - is what you will write to the file. 


This translates to a simple table:

|columnA|columnB|columnC|columnD|columnE|
|-------|-------|-------|-------|-------|
|1|3|5|6|777|


We keep it comma separated (CSV format) to easily visualize it at the end.
Feel free to update it as you like or leave it default.

Good stuff, if you are here you deserve another slice of :pizza:

## Compile the sketch

We are ready to compile the sketch, before we do it we need to ensure that you have:

- Proper board selected
- Proper SPIFFS (flash size) selected

For this example i am using ESP32 M5-Stack ATOM board and will use this as a board for compilation.

1. In Arduino, go to `Tools->Boards` and select the proper ESP32 board

   ![image](https://github.com/invpe/GridShell/assets/106522950/901d1ca1-2e2f-4890-8d9d-8695dffea84c)


2. Use Minimal Spiffs partition
 
![image](https://github.com/invpe/GridShell/assets/106522950/2a331df5-9d04-440c-9217-e44821155b66)

3. Go to `Sketch->Verify/Compile`

   ![image](https://github.com/invpe/GridShell/assets/106522950/07584afe-537b-4def-850a-56404ec508df)

The compilation process will start, observe the bottom window for the results.
At the end of it, you should be presented with a summary message.

![image](https://github.com/invpe/GridShell/assets/106522950/f2a880ca-60d0-48ad-ba52-04a10809efd2)

4. Time to upload the sketch to the board, click `Sketch->Upload`

![image](https://github.com/invpe/GridShell/assets/106522950/205eeb73-8e32-4a9b-b0ab-8616f0020357)

Now hold for a few seconds for Arduino to program your sketch onto the ESP32.
Once completed you will be presented following output

![image](https://github.com/invpe/GridShell/assets/106522950/80a48260-5830-4b50-9c27-d5ececc2e209)


Grab another 🍕, we burned few calories here 😄


## Monitoring

Once your device is programmed with the example sketch, hit `Tools->Serial Monitor` so we can observe it working.
It is good to confirm all is well before we close Arduino and focus on visualizing data.
What you should see is ESP32 pushing `WRITE` task to the grid, with your filename and its contents every minute.

![image](https://github.com/invpe/GridShell/assets/106522950/343dbbee-028f-4ecd-8601-ead78ce06d9e)


`WRITE Task ID: 58559` - this is the id of your task on the GridShell network, go on and check it out via [api](https://api.gridshell.net/task/58559.json)

Now it's time for a 🍺, job done ! Your device is now contributing to the network and using it 🥇


## Final thoughts

You can see your tasks being submitted and executed but also your device being part of the execution chain (shells earning) via [API](https://api.gridshell.net/status) endpoint or [Explorer](https://explorer.gridshell.net:3000/)

Feel free to click throuh, for example on your user - where you can check balance of shells and number of connected nodes.
The returned format from API is always `JSON` to help you integrate with other systems more easily.


[API Handbook](https://github.com/invpe/GridShell/blob/main/Documentation/API/README.md)

[EXPLORER Handbook](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Explorer.md)
