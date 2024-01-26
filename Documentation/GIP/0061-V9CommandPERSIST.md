# GIP 0061 - Implement PERSIST

- Author(s): gridshell
- Date: 26/01/24

 1. Abstract

Part of the simplification of OUTPUTPAYLOAD automatic telemetry storage [GIP](https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0062-V9Persist.md)

Implement `PERSIST` command to the protocol.

This command will flag a task to submit a 'writedfs' job after it is validated to store task outputpayload as telemetry.

It reduces a need to wait for the task completion and submit a writedfs manually.
