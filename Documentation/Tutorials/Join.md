<p align="center">
<img src=https://gridshl.files.wordpress.com/2022/06/img_3487.png>
</p>  

----------------

# What you will need ?
This tutorial assumes that you have an ESP32 device ready to join the GridShell.

Then depending on the option you choose either [Setup from release file](https://github.com/invpe/GridShell/edit/main/Documentation/Tutorials/Join.md#setup-from-a-release-file) or [Setup from sources](https://github.com/invpe/GridShell/edit/main/Documentation/Tutorials/Join.md#setup-from-sources) you need either a browser or [Arduino IDE](https://www.arduino.cc/en/software/) installed for the sources option.

The simplest way to join a vanilla node is to use the Release setup process, but if you want to modify the code or join your existing sensor as a GridShell node, then definitely reach for the sources tutorial.
 
# Setup your account first

GridShell doesn't require any account creation process, you simply provide your 64 bytes long string, that is used as your account.
Feel free to use [this tool](http://www.unit-conversion.info/texttools/random-string-generator/) to generate the string for you, just don't forget it later on!

![image](https://github.com/invpe/GridShell/assets/106522950/3ed066e8-a72e-4654-941f-94a94bf8496b)

You can [read more](https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0070-V09Accountless.md) in the GIP about the details.


# Setup from a release file
Open [Documentation/Tutorials/Release.md](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Release.md) to simply upload release binaries to your ESP32.

# Setup from sources
Lets try to make it in three simple steps to get you up and running.

1. Download the sketch https://github.com/invpe/GridShell/tree/main/Sources/Node/ClientESP32
2. Download GridShell library [here](https://github.com/invpe/GridShell/tree/main/Sources/GridShell) and put all files into sketch folder
3. Set `Default partitioning`
4. Compile & Upload the code


# Configure the node
Once uploaded to your device, continue setup by following [release manual](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Release.md#configuring-the-node) 
to configure your node.


# Monitoring
Access your account details with the expolorer page here:

[https://explorer.gridshell.net:3000/d/cd4c7abe-f622-4384-8a6d-348f3bffde47/user?orgId=1](https://explorer.gridshell.net:3000/d/cd4c7abe-f622-4384-8a6d-348f3bffde47/user?orgId=1)

To obtain your user latest and greatest.
Of course the name will differ, this is just an example :-)
 
