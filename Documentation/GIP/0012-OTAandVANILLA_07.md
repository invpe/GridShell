# GIP 0012 - Introducing OTA Functionality and Emphasizing Vanilla Nodes
- Author(s): GridShell
- Date: -/9/2023

# Synopsis

In light of the evolving nature of our GridShell network project, I propose a crucial update to our system to ensure its robustness and scalability. 
Currently, our network lacks remote update capabilities, which poses significant risks as the project continues to grow and undergo dynamic development. 
Part of the network is currently running the GridShell library alongside user code, which can lead to instability in nodes. Without the benefit of OTA updates, 
combined with frequent project improvements, feature additions, and the absence of an update process like OTA for pushing the latest firmware and patches, 
there is a risk of losing numerous network nodes—a scenario we are determined to avoid at all costs.

# The Solution: Implementing OTA Functionality

To address this concern and ensure the overall health of our network, I propose the implementation of Over-The-Air (OTA) functionality, starting from version 07. 
OTA updates will empower us to remotely and securely update nodes in our GridShell network. This enhancement will serve as a cornerstone in our network's stability and adaptability, 
allowing us to deliver improvements seamlessly without the need for physical intervention.

# Emphasizing Vanilla Nodes from Version 07 Onward

With version 07, we will shift our focus towards maintaining Vanilla Nodes within our network. 
Vanilla Nodes will be defined as nodes running the release binary directly sourced from our GitHub repository, devoid of any additional 'private' code. 
This strategic move serves several important purposes:

- Isolated SPIFFS Allocation: Each node will have exclusive access to its SPIFFS (file space), eliminating the possibility of conflicts and ensuring the integrity of node-specific data.
- Enhanced Command Exposure: We will be able to expose additional commands, such as "REBOOT," for Vanilla Nodes that may encounter issues. Given the absence of user code on these nodes, there will be no risk of inadvertently affecting critical user logic.
- OTA process
- Tested and Supported Binaries: The binaries used for Vanilla Nodes will be thoroughly tested and officially supported by our network, ensuring a reliable and stable environment.

# Advantages of the Vanilla Nodes Approach

This approach offers several advantages:

- Data Integrity: By minimizing user code on nodes, we mitigate the risk of data loss during OTA updates.
- Resource Allocation: Nodes will utilize their SPIFFS space efficiently, preventing shared space-related issues.
- Enhanced Control: We gain the ability to execute support commands like "REBOOT" to resolve Vanilla Node problems swiftly and efficiently.
- Reliable Binaries: Vanilla Nodes will utilize tested and officially supported binaries, guaranteeing a dependable network environment.

# User-Hosted Software Disclaimer

> **Warning**
It's crucial to mention that the OTA (Over-The-Air) update process comes with the important caveat that it will erase all data on the node during the update.

It's worth emphasizing that users do have the option to host their own software alongside the GridShell library. However, it's important to note that in such instances, the network cannot assume responsibility for any potential losses, interruptions, or issues that may arise as a result of this choice. We strongly advise users to exercise caution and verify compatibility when integrating their software with Vanilla nodes.


In summary, the introduction of OTA functionality and the transition to Vanilla Nodes starting from version 07 of our GridShell network project 
represent a pivotal step forward. This approach ensures the network's robustness, scalability, and ease of maintenance.




# References

[Over The Air](https://en.wikipedia.org/wiki/Over-the-air_update)






  
