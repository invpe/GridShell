
![image](https://github.com/invpe/GridShell/assets/106522950/45e3c90d-a9a8-40b1-b8ed-a9049cf5a222)
<p align="center">
  <i>Artistic vision of the EtherFog "sensor"</i>
</p> 

# [EtherFog](https://github.com/invpe/ETHERFOG)
WiFi Pollution Sensor @ https://github.com/invpe/ETHERFOG

EtherFog is an open-source device designed to monitor and visualize the density of WiFi traffic in a given area, referred to as "WiFi pollution." Utilizing the ESP32's promiscuous mode, EtherFog passively scans WiFi channels and tallies the number of packets, giving users a real-time indication of the WiFi activity surrounding them.

# Features:

- Channel Scanning: Hops across all WiFi channels, collecting data on WiFi traffic.
- Real-Time Data: Displays the total count of WiFi packets detected as a live graph.
- Traffic Averaging: Implements a moving average algorithm to smooth short-term fluctuations and highlight overall WiFi activity trends.
- Visual Feedback: Includes an RGB LED that changes color based on the level of WiFi traffic, offering an at-a-glance pollution indicator.

# How It Works:

EtherFog operates by continuously hopping between WiFi channels (1 to 14), staying on each channel for a predefined interval. Every packet detected is counted, and this data is processed to compute a moving average, providing a stable measure of WiFi activity over time. The result is then visually represented through an RGB LED—green for low activity, transitioning to red for high activity levels, indicating the presence of dense WiFi traffic or "pollution."


# Applications:

EtherFog is ideal for those who are sensitive to WiFi exposure or for anyone interested in the digital infrastructure of their environment. It can be used in homes, workplaces, or public spaces where understanding WiFi signal density is essential.

The data from EtherFog can potentially lead to a broader discussion on the density of wireless signals in our daily lives and might encourage users to think more critically about their personal and public digital spaces. It can also serve as an educational tool, showing how WiFi technology permeates nearly every location, and might even lead to further research into the implications of living in such a digitally saturated environment.

## Environmental Monitoring: 
EtherFog could be integrated into a network of sensors to monitor the electromagnetic environment of public spaces, contributing data for urban planning and public health.

## Public Awareness: 
Provide real-time data to citizens about WiFi signal strength in different parts of the city through an app or public displays.

## Signal-Free Zones: 
In the development of new housing areas, EtherFog could be used to identify "quiet" zones with minimal WiFi signals, which could be marketed as a selling point for certain demographics

## Health-Conscious Living: 
For individuals concerned about potential health effects, EtherFog can help them monitor and manage their exposure in homes and workplaces.

