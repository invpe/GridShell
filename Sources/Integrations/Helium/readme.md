![image](https://github.com/invpe/GridShell/assets/106522950/522e9be4-492e-47dc-8da2-0849773ba24f)

# Integrating with [Helium network](https://www.helium.com/)
"_Helium Network, often referred to as simply "Helium," is a decentralized wireless network designed for the Internet of Things (IoT) and low-power device connectivity. It is based on a unique blockchain-based incentive mechanism that allows individuals and businesses to deploy and operate low-power, long-range wireless hotspots called Helium Hotspots. These hotspots provide coverage for IoT devices in a given area and earn cryptocurrency rewards in the form of HNT (Helium Network Token) for participating in the network._"


# Sending via LoRA to Helium and pushing to GridShell

We will store the sensors telemetry on the GridShell network by sending them to Helium and using integrating (over HTTP) with the GridShell network.
Here are some basic assumptions:

1. For this example we will use [Lora Environmental sensor](https://www.hackster.io/astagiulio/lora-environmental-monitoring-lorem-41675c)
2. We will have access to [Helium Console](https://console.helium.com/)
3. We will have a web server running that will host [PHP](https://github.com/invpe/GridShell/tree/main/Sources/PHP) experimental library
4. We will have [account created and some shells for use on GridShell](https://github.com/invpe/gridshell/blob/main/Documentation/Tutorials/Use.md)




