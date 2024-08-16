<img src="https://github.com/invpe/GridShell/assets/106522950/ad0ffc49-d470-45f9-923d-86fc8966c7b7">
<div align=center><tt>GridShell nodes</tt></div>
 
# 👨‍🦲 JOIN

Joining GridShell is as easy as providing a 64 bytes long string, that is known only to you.

It is easiest to just generate your string with `SHA256` hashing function. 
The string you choose to identify with GridShell should not be exposed, keep it private and secure!

You can simply use the [GridShell PHP command line tool](https://github.com/invpe/GridShell/tree/main/Sources/PHP) or any [web sha256 generator](https://tools.keycdn.com/sha256-online-generator) to get yourself a hash.


![gridshell_create_user](https://github.com/user-attachments/assets/b86905e1-5e37-409e-83ce-de06d8028124)



 
# Configure

Now after you have your private user id, simply upload the vanilla release firmware and configure the node over serial terminal.

To do that - use the release docs and follow the steps [here](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Release.md).

## Read more

You can [read more](https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0070-V09Accountless.md) in the GIP about the details of accounts process,
and check [release docs](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Release.md) for how to upload the release to your ESP32.


# 🤾‍♂️ USE

Your newly generated user string will undergo a [cooling period](https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0070-V09Accountless.md) for some time.
Meantime see how easy it is to get your first task executed on the network, it only takes 17 lines of code :-)

![image](https://github.com/user-attachments/assets/b5b2234d-af49-49e9-89b3-fe50e6c3734a)






