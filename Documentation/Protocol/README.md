# 🔡 GridShell work protocol (v07)



## Generic assumptions
- The work protocol is plain text in the current version of GridShell 
- All nodes connect to a central server
- All nodes have to heartbeat every **30**s 
- Any misuse of the protocol, executing or incorrect behaviour will get you temporarily blocked or permanently banned

## Keep your nodes safe!
Since in it's current shape the protocol is plain text for simplicity, i suggest keeping your nodes running in a safe network.

## :electric_plug: Get connected and identify yourself

During the connection, server will provide few important numbers based on which you can tune your node logic.
Most important are numbers representing workload needing processing, see below for deatils.

----

1. Connect to `work.gridshell.net` port `1911`

The server will send `01,108,110,739,148722804`

`01` is the version

Ensure your node runs the same version 

`108` number of nodes online 

`110` is the number of tasks waiting to execute.

`739` is the number of tasks waiting for validation.

`148722804` Server `public key` value

----
 


2. Calculate node `Public`, `Private key` and `symmetric key`
3. `SHA1(symmetric_key)` - to generate a XOR key
4. `XOR(user_hash with SHA1(symmetric_key))` - to generate cipher
5. `BASE64ENCODE(XOR(user_hash, SHA1(symmetric_key)))` - to encode the cipher with base64
6. Send `JOB,node_public_key,base64_cipher,VERSION,IDENT` - to identify with GridShell server
 
Where `IDENT` is a 12 character identifier of your node.

When Number of tasks to execute is equal to `0`, you can put your device to sleep.
There is absolutely no need to keep them up and running if there is nothing to process. Wake them up sporadically
and keep running when there is really work to be done.

Same as above but validators only, do not waste energy - come back in a moment.

----

7. Heartbeat by sending `PONG` to the server, every `10` seconds
8. Wait for a task to be provided in a format of
`EXEC,BASE64(https://api.gridshell.net/task/name.bas),BASE64(payload),TIMEOUT` 
9. Execute a task with MYBASIC interpreter, timeout when `TIMEOUT` reached during execution
10. Once completed return task payload with RESULTS 
`RESULTS,RETCODE,BASE64(results)` 
11. Go to point 9

 
