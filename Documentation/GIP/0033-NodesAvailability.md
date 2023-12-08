# GIP 0033 - Nodes Availability calculation and tracking

- Author(s): gridshell
- Date: 8/12/2023

# Objective:
Improve the reliability and efficiency of our distributed file system by implementing a node availability tracking system. 
This system will allow us to intelligently replicate data chunks, prioritizing nodes with higher availability. 
Additionally, a periodic review of node performance to potentially adjust user performance factor based on availability metrics.

## Implementation Details:

- Node Availability Tracking:
  
Connection and Disconnection Logging: Each node's connect and disconnect events are logged with precise timestamps.
These events are stored in a container, keyed by node IDs.
  
Rolling 24-Hour Window: The system maintains a rolling 24-hour window of node activity, ensuring current and relevant data is always used for calculations.

- Availability Calculation:

A function calculates the availability of each node based on the stored timestamps. It considers the durations of uptime and downtime within the last 24 hours to compute the availability percentage.

- Map Rotation Mechanism:
  
Implemented within the server's ::Tick method, this mechanism purges data older than 24 hours. It ensures the map always reflects the latest 24-hour window of node activity.

- Utilization for Data Replication:

The availability metrics are used to guide the replication strategy. Chunks are preferentially replicated on nodes demonstrating higher availability, enhancing the overall reliability of the system.

- Periodic User Performance Review:

At midnight each day, the system additionaly evaluates the User Performance Factor (UPF) for each user based on their nodes' availability.

Users whose nodes consistently fall below a certain availability threshold may have their UPF adjusted, impacting their participation level or incentives in the system.

## Expected Outcomes:

Increased Reliability: By favoring nodes with higher availability for data replication, the system's resilience to node failures is significantly enhanced.

Optimized Resource Utilization: Intelligent replication based on real-time availability data ensures efficient use of network resources.

User Accountability: Regular performance reviews promote user accountability, encouraging participants to maintain high availability of their nodes.

Adaptive System: The ability to dynamically adjust replication strategies and user participation based on node performance makes the system more adaptable to changing conditions.

## Next Steps:

* Implement nodes availability metrics rendering on the explorer endpoint.
* Finalize and define the threshold for the UPF adjustment.
   








