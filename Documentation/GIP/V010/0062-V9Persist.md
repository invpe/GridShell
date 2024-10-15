# GIP 0062 - Implement OUTPUTPAYLOAD Persistence post task execution

- Author(s): gridshell
- Date: 26/01/24

1. Abstract
   
Currently, users must manually retrieve and store task execution payloads on the network as telemetry using a writedfs task. 
To streamline this, we propose an automatic Persistence feature that allows users to pre-select tasks for OUTPUTPAYLOAD storage as telemetry immediately after validation. 
This automatic process eliminates the need for additional network interactions post-task submission.


2. Introduction
   
With the increasing reliance on telemetry over computation in our network, the process of storing execution payloads can be more efficient. 
Instead of waiting for a task's completion, retrieving its payload, and submitting a writedfs task, we propose a simpler method. By marking a task for automatic persistence upon submission, 
we can significantly reduce user interactions and streamline the telemetry storage process.

3. Proposed Methodology
   
Following the implementation of the PERSIST command in the protocol, users can now easily store task payloads by submitting the following commands:

```
SUBMIT <task> <payload>
PERSIST <task> <append?> <filename>
```

4. Limitations

~~Currently, all tasks set for persistence will store their telemetry under the filename `USERtask_id`.~~ 
~~Future GIPs may introduce options to customize the telemetry file name and decide whether to append the payload (flag 1/0).~~ 
Note that tasks can only be marked for persistence `if their state is neither TASK_STATE_FAILED nor TASK_STATE_VALIDATED`.


5. Impact

This enhancement significantly simplifies the process of storing task payloads in telemetry files. 
It reduces the steps involved and allows for the automatic generation of telemetry upon task validation.


