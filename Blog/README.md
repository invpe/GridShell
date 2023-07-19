# 📰 Series of blog entries for GridShell


## 18/07/2023 - Integrating GridShell with DuCo mining

I find [DuinoCoin](https://duinocoin.com/) very fun, enabling low power devices in the *mining* world? 
Why not, i like ideas that are fresh and *green* - especially if i can connect them with GridShell.

One of my friends asked if he can use GridShell to distribute SHA1 mining on Duinocoin without dedicating any ESP node/hardware to it.
His idea was to keep his sensors up and contribute to the network generating Shells for execution but also using them to mine few Duco Tokens here and there.
Well in this scenario, everyone is happy.

- He is giving away Shells to volunteers for executing his mining script
- He mines some tokens in exchange

I've spent few minutes rolling out a distributed DuinoCoin miner, since this is SHA1 it works with ESP32 out of the box.
Of course, this will never be efficent to run on GridShell due to the characteristics of distributed computing, but this wasn't the case here from the start.
Assuming he was to run that adhoc during idling, we knew this is ideal plan for workloads of large groups or streams of jobs where the goal is high rate of job completion rather than low job turnaround time.

So we went into it.

Their (Duinocoin) protocol is simple and allows for a quick integration so the work took few pizza slices to complete.
Few minutes into the game and we had a script ready and shells burned.

![image](https://github.com/invpe/GridShell/assets/106522950/64b0d2af-092f-4ad2-8204-fa98cf27f4c7)


We left it working for some time, just to finish our 🍕 and it turned out to work exactly as planned.
We called it a fair exchange, where he exchanged his Shells for DuCo tokens by simply executing task on the grid.


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

Sure, this as mentioned earlier is not optimal - but it shows how easy it is to integrate GridShell with other projects around.
He earned few DUCOs, which we observed on his wallet so we consider this idea to be a 100% success. 
Checkout the wallet dumps below for details. Since i am not a mining specialist, i don't know the value - but as mentioned earlier,
the value i was looking for was hidden in the intregration itself not the tokens generated. 

```
5621.810133 DUCO
5621.82252972 DUCO
5621.8245512 DUCO
5621.83147469 DUCO
```


If you're interested into the task and integration itself, checkout one of the submitted tasks below or see the [distributed duco](https://api.gridshell.net/scripts/dduco.bas) script itself.
Feel free to play with it, the input payload is simple and you should not have any issues to run it. 

### [Task](https://api.gridshell.net/task/81806.json)

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
