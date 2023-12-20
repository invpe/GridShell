# GIP 0035 - GFS Improved Chunk Management
- Author(s): gridshell
- Date: 20/12/23

# Objective:

To enhance the reliability and efficiency of the hybrid distributed file system in GridShell, a chunk splitting logic will be implemented. This logic involves maintaining a fixed chunk size (5120 bytes) and generating new chunks when the size limit is reached. Completed chunks, those that reach the fixed size, will be marked for replication across the network nodes.

## Implementation Details:

### Fixed Chunk Size:

Introduce a fixed chunk size: DFS_CHUNK_SIZE = 5120 bytes.

### Chunk Generation:

- Every write (append) to a telemetry file will contribute to the total bytes for the current chunk.
- When the size of the current chunk reaches DFS_CHUNK_SIZE, a new chunk is created for subsequent writes.
- This process repeats until the user reaches their Telemetry Size limit.
- A single telemetry file can comprise multiple chunks, each of DFS_CHUNK_SIZE bytes.

### Chunk Replication:

- When a telemetry chunk reaches DFS_CHUNK_SIZE, it is considered completed and is no longer available for writing (as a new chunk is created).
- Completed chunks are marked for replication.
- A core replicatedfs task is submitted to any available node in the network to store the chunk on its SPIFFS file system.
- **Optimized Replication**: 
The `replicatedfs` task has been significantly optimized, running approximately 10 times faster due to fixed indexing to the payload.
With a fixed chunk name size (30 bytes), the task now runs super fast (~4seconds total), facilitating even faster chunks replication across the network.

# Expected Outcomes:

This chunk management logic is expected to:

- Reduce the total number of chunks on the network.
- Facilitate further development of hybrid telemetry hosting within the network, where completed chunks are known to be replicated and can be served directly from nodes' local SPIFFS file systems.
