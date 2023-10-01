# 🔡 GridShell work protocol (v08)



## Generic assumptions

- All communication is done over TLS/SSL
- The work protocol is plain text in the current version of GridShell 
- All nodes connect to a central server
- All nodes have to heartbeat every **10**s 
- Any misuse of the protocol, executing or incorrect behaviour will get you temporarily blocked or permanently banned
 
## :electric_plug: Get connected and identify yourself

During the connection, server will provide few important numbers based on which you can tune your node logic.
Most important are numbers representing workload needing processing, see below for deatils.

----

1. Connect to `work.gridshell.net` port `1911`

The server will send `01,108,110,739`

`01` is the version

Ensure your node runs the same version 

`108` number of nodes online 

`110` is the number of tasks waiting to execute.

`739` is the number of tasks waiting for validation.


----
2. Send `JOB,base64(grid user hash),VERSION,IDENT` - to identify with GridShell server
 
Where `IDENT` is a 12 character identifier of your node.

When Number of tasks to execute is equal to `0`, you can put your device to sleep.
There is absolutely no need to keep them up and running if there is nothing to process. Wake them up sporadically
and keep running when there is really work to be done.

Same as above but validators only, do not waste energy - come back in a moment.

----

3. Heartbeat by sending `PONG` to the server, every `30` seconds
4. Wait for a task to be provided in a format of
`EXEC,BASE64(https://api.gridshell.net/task/name.bas),BASE64(payload),TIMEOUT` 
5. Execute a task with MYBASIC interpreter, timeout when `TIMEOUT` reached during execution
6. Once completed return task payload with RESULTS 
`RESULTS,RETCODE,BASE64(results)` 
7. Go to point 3

 
