# 😄 Lets make some fun

## Mining DuinoCoin on the GridShell 
I find [DuinoCoin](https://duinocoin.com/) very fun, enabling low power devices in the *mining* world? 
Why not, i like ideas that are fresh and *green* - especially if i can connect them with GridShell.
Once i spend few minutes rolling out a distributed DuinoCoin miner, since this is SHA1 it works with ESP32 out of the box.
Their protocol is simple and allows for a quick integration. Of course, this will never be efficent to run on GridShell 
due to the characteristics of distributed computing, but executing a mining task on the network is a good fun and exercise ✨

This isn't anything special, but demonstrates how quickly i could integrate a network of nodes to a specific task.


![image](https://github.com/invpe/GridShell/assets/106522950/64b0d2af-092f-4ad2-8204-fa98cf27f4c7)


### Idea

I kept the idea very simple:

- Connect to DUCO pool and obtain the JOB
  
`[DUCO] 14fdda11b02fc91a3a85fec35d7150cc0d24aa5a,d657a8b3018db68a32b270c14b23f0fcdc56e710,8`

- Submit a task to the GRIDSHELL looking for a value that generates a SHA1 matching DUCO
  
`[GS:AddTask] dduco 998ba1adb4c0d632df8cb73bb5c13796fb55bfa8,9478d5ff0122708a90fa3bc06c1f069086933ed3,0,1000,`
    
- Wait for job completion polling the API (task status 3 or 4)
  
`[GSHELL] 81807 State 4 : 324`
  
- Once done, return results to DUCO

```
[DUCO] Sending resulst: 324,100,SolarMiner,GridShell
[DUCO] GOOD
```

I left it working for some time, had free Shells to spend and kept it going.

```
...
[DUCO] Reconnecting
[DUCO] Pool : 45.13.225.135:3767
[DUCO] Connecting to 45.13.225.135:3767
[DUCO] Version 3.0
[GSHELL] Logging
[GS:Login] Login start work.gridshell.net:1911
[GS:Login] Sending command
[GS:Login] Login completed
[DUCO] Asking new job
[DUCO] c1dcbd93311af3d76072ee35f9d9d1d23721f7c4,2afbd85022b5124e2835081b8fe4a2632cf2a0e9,8
[DUCO] Difficulty 800
[GSHELL] Adding tasks
[GS:AddTask] dduco c1dcbd93311af3d76072ee35f9d9d1d23721f7c4,2afbd85022b5124e2835081b8fe4a2632cf2a0e9,0,1000,
[GS:AddTask] Waiting for results:
[GS:AddTask] Received ADDT,81807
[GS:Pong] Pong
[GS:Disconnect] Disconnected
[GSHELL] Waiting for grid execution
[GSHELL] 81807 State 4 : 324
[GSHELL] Found 324
[DUCO] Sending resulst: 324,100,SolarMiner,GridShell
[DUCO] GOOD
Restarting
...
```

### Was it really working ?

Yes, see some balance dumps here from the wallet page.

```
5621.810133 DUCO
5621.82252972 DUCO
5621.8245512 DUCO
5621.83147469 DUCO
```


### Example task results
 
[Task](https://api.gridshell.net/task/81806.json)

```	
	
ID	81807
Owner	"PocNetGroupMiners00000000000000000000001"
Script	"dduco"
Input	"YzFkY2JkOTMzMTFhZjNkNzYwNzJlZTM1ZjlkOWQxZDIzNzIxZjdjNCwyYWZiZDg1MDIyYjUxMjRlMjgzNTA4MWI4ZmU0YTI2MzJjZjJhMGU5LDAsMTAwMCw="
Reward	1
Fails	0
Restarts	0
State	4
Created	1689686072
Execs	2
Start	1689686072
End	1689686079
ExecUser	"JadeChartreuseDromiceiomimus"
ExecPayload	"MzI0"

```

