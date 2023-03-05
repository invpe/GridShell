<p align="center">
<img src=https://gridshl.files.wordpress.com/2022/06/img_3487.png>
</p>  

----------------

# What you will need ?
This tutorial assumes that you have an ESP32 device ready to join the GridShell and that you have Arduino application installed.
Arduino needs to be able to upload sketches to your ESP32, therefore ensure that this is all working before moving forward.

# Setup
Lets try to make it in three simple steps to get you up and running.

1. Download the sketch https://github.com/invpe/GridShell/tree/main/Sources/Node/ClientESP32
2. Create Test Net account https://api.gridshell.net/getuser/
3. Update the sketch with account details, WiFi credentials & compile 

# Monitoring
Access your account details with Test Net API server, head to :

https://api.gridshell.net/user/your_new_grid_username.json

To obtain your user latest and greatest.
Of course the name will differ, this is just an example :-)

# Important
When creating account, you have been given three values, ensure to keep them noted
as it is impossible to recreate them if lost.

![Account](https://github.com/invpe/GridShell/blob/main/Resources/Tut6A.png?raw=true)

* Username (Public) - this is cool to share with others
* UserHash (Private) - never share it - used for the sketch to ident
* UserKey (Private) - never share it - might be used later on