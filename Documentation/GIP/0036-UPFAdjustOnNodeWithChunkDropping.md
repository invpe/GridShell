# GIP 0036 - Integrating UPF Penalties for Node Drops in Telemetry Chunk Management

- Author(s): gridshell
- Date: --/--/2023
 
# Objective:

Enhance the reliability and integrity of the hybrid distributed file system, particularly for sensitive telemetry data, by implementing a penalty system for node drops. This system aims to adjust the User 
Performance Factor (UPF) of users whose nodes drop while handling telemetry chunks, thereby incentivizing higher standards of node reliability.

Details of the Penalty System:

## Trigger for UPF Penalty:
- The penalty is triggered when a node drops while having one or more telemetry chunks.

## Assessment of Telemetry Chunk Count:
- Upon a node drop, the system assesses the number of telemetry chunks the node was handling at that time.

## Calculation of UPF Reduction:
- The UPF reduction is based on the number of telemetry chunks on the node, with a maximum reduction cap of 10.
  
- The reduction formula is as follows:
  
-- If the node has 10 or more chunks, the reduction is 10.
  
-- If the node has fewer than 10 chunks, the reduction equals the number of chunks.

-- Example: A node with 3 chunks will have a UPF reduction of 3.

## Maximum Penalty Cap:
- The UPF reduction does not exceed 10, regardless of the number of telemetry chunks the node was handling.

# Implementation Strategy:
- Integrate the penalty calculation into the existing UPF management system.
- Ensure accurate tracking and timely application of penalties upon node drops.

# Expected Outcomes:

- Increased Network Reliability: By penalizing node drops, especially those handling critical telemetry data, the network's overall reliability is expected to improve.
- Encouragement of Responsible Node Management: The penalty system incentivizes users to maintain stable nodes, particularly when handling sensitive data.
- Fair and Proportional Penalties: The tiered penalty system based on the number of chunks ensures fairness, with more significant penalties for larger responsibilities.
  
