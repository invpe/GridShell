# GIP 0030 - Conducting Integrity Checks in Gridshell telemetry

- Author(s): gridshell
- Date: 10/11/2023

# Synopsis
This GIP outlines a systematic approach for conducting integrity checks within the Gridshell system. 
It proposes a method to verify the consistency and accuracy of data stored across distributed nodes, ensuring high data reliability and trustworthiness.

# Motivation
As Gridshell evolves into a robust distributed file system, ensuring the integrity of data becomes paramount. 
Inconsistencies or errors in stored data can lead to trust issues and system inefficiencies. 
The motivation behind this proposal is to establish a reliable and scalable method to routinely check and verify the integrity of stored data.

# Approach for Conducting Integrity Checks

## Maintain a Chunks-to-Nodes Mapping

Keep an updated record of which chunks are stored on which nodes. This mapping should include both regular nodes and validators.
Update this mapping whenever a chunk is added, replicated, or removed from a node.

## Initiate Integrity Checks Based on Availability

Before starting an integrity check for a particular chunk, verify that both the primary node storing the chunk and its corresponding validator are available and have the chunk in question.
Proceed with the integrity check only if both parties are available and have the chunk.

## Task-Based MD5 Validation

Dispatch a task to both the node and its validator to compute the MD5 hash of the chunk.
Once both parties return their computed hashes, compare them to ensure integrity.

## Handling Node Unavailability or Chunk Loss

If either the node or the validator is unavailable, or if either has lost the chunk due to disconnection or other reasons, postpone the integrity check for that chunk.
In the meantime, trigger a replication process to restore the chunk on another node or validator as needed.

## Continous

The integrity check process within Gridshell is designed to be continuous and unending. This ensures that data integrity is maintained over time, and any tampering or data degradation is promptly identified and addressed.

Even after a chunk has been checked and validated, it will be subject to future checks. This approach accounts for potential changes or issues that may arise after the initial verification.

## Randomized Selection

To avoid predictability in the checking process, the selection of data chunks for integrity checks will be randomized. This ensures a fair and comprehensive coverage over the entire dataset.

## Periodic Comprehensive Checks

Schedule regular, comprehensive integrity checks across the network to ensure overall data consistency and reliability.

## Sucessfull checks [GIP0032](https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0032-GFS_IntegrityCheckSuccess.md)

## Automated Response to Integrity Failures [GIP0032](https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0031-GFS_Integrity_Discrepancy_Handling_and_Replication.md)

If an integrity check fails (i.e., mismatched MD5 hashes), automatically initiate procedures to resolve the discrepancy. This could involve re-uploading the chunk from a trusted source or redistributing it to other nodes. 

# Conclusion

This approach leverages the existing knowledge about chunk distribution across the network, making the integrity checks more targeted and efficient. By focusing on available nodes and validators that currently hold the relevant chunks, the process becomes more streamlined and effective, thus ensuring the reliability and integrity of the distributed file system. Continuous and repetitive integrity checks are crucial for a distributed storage system like Gridshell. They provide an ongoing assurance of data reliability and play a vital role in detecting and preventing data tampering or corruption.
