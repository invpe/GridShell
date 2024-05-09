# GIP 9999 - A reference to important numbers

- Author(s): gridshell
- Date: 12/10/2023

# Synopsis
This GIP serves as a foundation for accessing the operational costs and rewards, denominated in GridShell tokens, which encompass both execution and usage within the network. 
Please note that, given the evolving nature of the project and its current phase, these numbers are subject to change. However, this document will be consistently updated to reflect the latest information.

# Rationale
To centralize numerical data for easier user reference.

# Housekeeping

Accounts are being removed if not seen for longer than `259200` seconds `3 days`.

# Costs

| Type  | Shell tokens | Description |
| ------------- | ------------- | ------------- |
| Add Task  | 1  | Submitting a task to the network  |
| Send  | 1  | Transferring shell tokens  |
| Burn Telemetry Slot  | 1000  | Redeeming shells for additional telemetry slot |
| Burn Telemetry Size  | 1000  | Redeeming shells for additional telemetry size (+1000b) |

# Rewards

| Type  | Shell tokens | Description |
| ------------- | ------------- | ------------- |
| Valid task  | 1  | For valid task execution  |
| Airdrop  | 1  | For single airdrop  |
| autocat  | 1 | For valid execution from V09 |

# Void execution 

| Type  | Shell tokens | Description |
| ------------- | ------------- | ------------- | 
| replicatedfs  | 0  | Replication of a chunk |
| deletedfs  | 0 | Removal of a chunk |
| md5dfs  | 0 | Chunk checksum check |
| send  | 0 | Void task |
| burn  | 0 | Void task |

# UPF 

| Type | Percentage |
| ---- | ---------- |
| UPFRequiredTaskSubmission | >51% |
| UPFRequiredTransferTokens | >51% |
| UPFRequiredBurn | >51% |
| UPFRequiredOthers | >51% |

# Throttling & Rate limiting

The system operates according to the configuration parameter `ExcessiveCommandTime`, which permits users to send messages to the server at a rate of `one message per second`. Should this threshold be exceeded, the system enforces a temporary throttling measure by disconnecting and blocking the sender for a randomized duration of time.

# Telemetry

Core telemetry files

| Telemetry | Description |
| --------- | ----------- |
| GRIDSHELLCoreValidators1CURRENT | Uber JSON with latest snapshot including server stats, volunteers, tasks and nodes |
| GRIDSHELLCoreValidators1TASKS | JSON with the recent tasks |
| GRIDSHELLCoreValidators1STAT202459 | CSV file for easy graph creation, note the date at the end |

