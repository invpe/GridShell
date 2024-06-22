<img src="https://github.com/invpe/GridShell/assets/106522950/ad0ffc49-d470-45f9-923d-86fc8966c7b7">
<div align=center><tt>GridShell nodes</tt></div>
 
# 👨‍🦲 JOIN

Joining GridShell is as easy as providing a 64 bytes long string, that is known only to you.

It is easiest to just generate your string with `SHA256` hashing function. 
The string you choose to identify with GridShell should not be exposed, keep it private and secure!


1. [Use GridShell PHP command line tool](https://github.com/invpe/GridShell/tree/main/Sources/PHP)

![image](https://github.com/invpe/GridShell/assets/106522950/d0dd1f6d-0362-4224-8239-d02323896726)

![image](https://github.com/invpe/GridShell/assets/106522950/599819d2-98d5-40d9-a9e4-2118e5b660d0)

   
2. Or [Use sha256 hash generator](https://tools.keycdn.com/sha256-online-generator) 

![image](https://github.com/invpe/GridShell/assets/106522950/593c9361-6f5a-4cd5-bbe4-096a041d0a3a)

# Configure

Now after you have your private user id, simply upload the release and configure the node over serial terminal.

1. [Upload release binary to ESP32](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Release.md)
2. Configure the node over serial

   ![image](https://github.com/invpe/GridShell/assets/106522950/d6da4f18-2acb-48a4-a0e6-c627a598a381)


All steps completed, your node will join the network shortly.


You can [read more](https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0070-V09Accountless.md) in the GIP about the details of accounts process,
and check [release docs](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Release.md) for how to upload the release to your ESP32.


