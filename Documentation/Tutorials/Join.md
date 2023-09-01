<p align="center">
<img src=https://gridshl.files.wordpress.com/2022/06/img_3487.png>
</p>  

----------------

# What you will need ?
This tutorial assumes that you have an ESP32 device ready to join the GridShell and that you have Arduino application installed.
Arduino needs to be able to upload sketches to your ESP32, therefore ensure that this is all working before moving forward.

# Create account on the testnet
[https://api.gridshell.net/getuser/](https://api.gridshell.net/getuser/)

# Setup from a release file
Open [Documentation/Tutorials/Release.md](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Release.md) to simply upload release binaries to your ESP32.

# Setup from sources
Lets try to make it in three simple steps to get you up and running.

1. Download the sketch https://github.com/invpe/GridShell/tree/main/Sources/Node/ClientESP32
2. Download GridShell library [here](https://github.com/invpe/GridShell/tree/main/Sources/GridShell) and put all files into sketch folder
3. Update the sketch with account details, WiFi credentials & compile 

# Monitoring
Access your account details with the expolorer page here:

[https://explorer.gridshell.net:3000/d/c67e1c66-8ac3-480e-860b-7d1ba05aa8f8/gridshell-user?orgId=1](https://explorer.gridshell.net:3000/d/c67e1c66-8ac3-480e-860b-7d1ba05aa8f8/gridshell-user?orgId=1)



To obtain your user latest and greatest.
Of course the name will differ, this is just an example :-)

# Important
When creating account, you have been given three values, ensure to keep them noted
as it is impossible to recreate them if lost.

![Account](https://github.com/invpe/GridShell/blob/main/Resources/Tut6A.png?raw=true)

* Username (Public) - this is cool to share with others
* UserHash (Private) - never share it - used for the sketch to ident
* UserKey (Private) - never share it - might be used later on
