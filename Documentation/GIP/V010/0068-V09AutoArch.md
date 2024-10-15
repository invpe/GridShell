# GIP 0068 - Implement automatic architecture categorization

- Author(s): gridshell
- Date: 08/02/24

# Abstract

This proposal introduces an automatic categorization system for GridShell nodes, leveraging the execution time of a benchmark task involving 20000xSHA256 hash calculations. The system aims to enhance architecture identification, optimize resource utilization, and ensure consistent task validation across the diverse range of IoT devices within the GridShell network.
 
# Introduction

The growth and diversity of devices in distributed computing networks pose challenges in architecture identification, resource utilization, and task validation. To address these challenges, propose an automatic system for categorizing nodes based on their computational performance, thereby improving the efficiency, scalability, and inclusivity of the GridShell network.

# Objectives

- Architecture Identification: Accurately identify the computing architecture of each node through performance benchmarks rather than self-reported data, enhancing network security and trust.
- Efficient Resource Utilization: Ensure that tasks are delegated to nodes with appropriate computational capabilities, optimizing network throughput.
- Consistent Task Validation: Mitigate discrepancies in task validation that arise from architecture differences, crucial for tasks sensitive to such variations.

# Implementation

## Dynamic Grouping

Nodes will be dynamically categorized into architecture groups based on their performance in the specified benchmark task. This approach allows for the accommodation of new device types and performance variations over time.


## Configurable Tolerance

A configurable offset (e.g., 10%) will be applied to execution times to account for minor performance variations among devices, establishing flexible group boundaries and minimizing over-segmentation.


## Scalability and Inclusivity

The system's design facilitates the integration of a broad spectrum of IoT architectures, from low-power devices like ESP32s to more capable computing platforms such as Raspberry Pis.


Benefits

- Broadened Task Range: The network will support a wider array of tasks, assigning complex computations to more capable devices and simpler tasks to less powerful ones.
- Enhanced Computational Efficiency: Tasks will be matched with the most suitable architectures, accelerating computation times and leveraging the full potential of faster devices.
- Expanded Network Participation: Automatic categorization enables the optimal use of various IoT devices, encouraging wider participation and contributing to network growth.

# Future Possibilities

The implementation of this categorization system positions GridShell as a more robust and versatile distributed computing network, capable of supporting an expanded range of applications and efficiently utilizing its computational resources as the network evolves.

# Conclusion

The proposed automatic categorization system addresses crucial challenges in managing a heterogeneous device environment within the GridShell network. By enhancing architecture identification, optimizing resource utilization, and ensuring consistent task validation, the system lays the groundwork for a scalable, flexible, and efficient distributed computing network. This initiative not only improves current operations but also opens the door to future expansion and innovation.


Example dataset of test groupping based on the autocat runs

```
Group Range: [20.7, 25.3] Nodes: 08b777f959da efcdaaa2e73d ab2b643ac6bf 734a368181a4 74fa34907882 788b0ddf6628 8d7bb5cc776c 3ca9a53e1ce3 8475f134ed33 fa3961917bc7 92d85cc8a70b 21e3d2b90978 4a9a8cda0d3b d0667894bae2 cea9398faf43 030fcdb20e1b fd9c6db99027 f72da63f143f 
Group Range: [18, 22] Nodes: 31b8266fddf7
```
