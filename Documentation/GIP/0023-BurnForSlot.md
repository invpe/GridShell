# GIP 0023 - Create a chain and task for increasing telemetry slots
- Author(s): gridshell
- Date: --/9/23

# Synopsis
Allow users to spend Shells for Telemetry Slots submitting a dedicated task to the network.
Create a dedicated core chain and task for submission to execute the [GIP0015](https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0015-Shell2Telemetry.md).

# Details
Users can now submit a task `task-name` in the network, that once validated will increase their Telemetry Slot size reducing their Shells in exchange.
Only one instance of the task is allowed to run, another can be submitted when the first executes. 
Valid execution will adjust users Slot size and reduce their Shell tokens accordingly.



