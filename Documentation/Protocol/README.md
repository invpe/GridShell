# 🔡 GridShell work protocol (v05)



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

The server will send `01,108,110,739,`

`01` is the version

Ensure your node runs the same version 

`108` number of nodes online 

`110` is the number of tasks waiting to execute.

`739` is the number of tasks waiting for validation.


----

2. Obtain Server `public key` value from the welcome message
  
`148722804 - Server Public Key`


3. Calculate node `Public`, `Private key` and `symmetric key`
4. `SHA1(symmetric_key)` - to generate a XOR key
5. `XOR(user_hash with SHA1(symmetric_key))` - to generate cipher
6. `BASE64ENCODE(XOR(user_hash, SHA1(symmetric_key)))` - to encode the cipher with base64
7. Send `JOB,node_public_key,base64_cipher,VERSION,MAC,ARCH,1` - to identify with GridShell server

Following `ARCH` types are defined for now:
- ESP32
- ARMV7L
- LINUX64
- LINUX32
- WIN32
- WIN64

See rationale here: https://github.com/invpe/gridshell/blob/main/GIP/0002-ARCH.md



When Number of tasks to execute is equal to `0`, you can put your device to sleep.
There is absolutely no need to keep them up and running if there is nothing to process. Wake them up sporadically
and keep running when there is really work to be done.

Same as above but validators only, do not waste energy - come back in a moment.

----

8. Heartbeat by sending `PONG` to the server, every `10` seconds
9. Wait for a task to be provided in a format of
`EXEC,BASE64(https://api.gridshell.net/task/name.bas),BASE64(payload),TIMEOUT` 
10. Execute a task with MYBASIC interpreter, timeout when `TIMEOUT` reached during execution
11. Once completed return task payload with RESULTS 
`RESULTS,RETCODE,BASE64(results)` 
12. Go to point 9

 
