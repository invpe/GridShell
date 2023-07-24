# 📰 Series of blog entries for GridShell

## 20/07/2023 - Playing with a simple neural network 🛰️

Required: [telemetry](https://api.gridshell.net/fs/neuraltest), [script](https://api.gridshell.net/scripts/neuralnetwork.bas)  

Let's try learning XOR with a simple feedforward neural network that uses supervised backpropagation for training and sigmoid activation function.
When executed i expect the script to return a value of total error after the training epochs are completed so that i know, how did the training go.
Thanks to this value i can run multiple of training tasks with different network configurations and see which performed best (had lowest total error value).

First i will create new telemetry file named `neuraltest` which has a defined structure and will serve me as a template for :

- Constructing the neural network in this example and future tasks,
- Providing a training dataset for supervised learning

The structure of the file (which is required by the script) is as follows:

```
2,10,1,20,0.1, <- Inputs, Hidden layers, Outputs, Number of training epochs(cycles), Lear rate
4, <- 4 training datasets
0,0,0, <- 1st training dataset, input0 = 0, input1 = 0, expected output0 = 0
1,0,1, <- 2nd training dataset, input0 = 1, input1 = 0, expected output0 = 1
0,1,1, <- 3rd training dataset, input0 = 0, input1 = 1, expected output0 = 1
1,1,0, <- 4th training dataset, input0 = 1, input1 = 1, expected output0 = 0
```

The plan is to execute 20 epochs of training for each of the training datasets and return the last error in `OUTPUTPAYLOAD`.
This will give me enough info to understand how does the network perform with its current configuration.
Remember, that the configuration is stored in telemetry file, i can make lots of them and run plenty of tasks to find out which configuration serves best.
Changing things like: `hidden layers`, `learn rate`, `epochs` makes big differences in the overall performance of the training.

For now, we will start with running the network on the above telemetry file and check the results.
Let's fire a simple task on the grid:

```
01:07:55.862 -> Epoch 0-Total Error: 0.920739
01:07:58.716 -> Epoch 1-Total Error: 0.917742
01:08:01.537 -> Epoch 2-Total Error: 0.914533
01:08:04.292 -> Epoch 3-Total Error: 0.911089
01:08:07.081 -> Epoch 4-Total Error: 0.907386
01:08:09.935 -> Epoch 5-Total Error: 0.903395
01:08:12.655 -> Epoch 6-Total Error: 0.899088
01:08:15.443 -> Epoch 7-Total Error: 0.894426
01:08:18.164 -> Epoch 8-Total Error: 0.88937
01:08:20.985 -> Epoch 9-Total Error: 0.883873
01:08:23.707 -> Epoch 10-Total Error: 0.877882
01:08:26.427 -> Epoch 11-Total Error: 0.871338
01:08:29.149 -> Epoch 12-Total Error: 0.864171
01:08:31.937 -> Epoch 13-Total Error: 0.856304
01:08:34.658 -> Epoch 14-Total Error: 0.847647
01:08:37.444 -> Epoch 15-Total Error: 0.7381
01:08:40.231 -> Epoch 16-Total Error: 0.727549
01:08:42.983 -> Epoch 17-Total Error: 0.611232
01:08:44.377 -> Epoch 18-Total Error: 0.590076
01:08:46.269 -> Epoch 19-Total Error: 0.490076 
```


 
```
{
"ID": 83750,
"Owner": "PocNetGroupMiners00000000000000000000001",
"Script": "neuralnetwork",
"Input": "bmV1cmFsdGVzdA==", ==> neuraltest
"Reward": 0,
"Fails": 0,
"Restarts": 0,
"State": 1,
"Created": 1690066814,
"Execs": 1,
"Start": 1690066814,
"End": 1690066852,
"ExecUser": "JadeChartreuseDromiceiomimus",
"ExecPayload": "RE9ORSwwLjQ5MDA3Niw="  ===> DONE,0.490076,
}
```

The script took `1690066852-1690066814=38seconds` to complete, returning `0.490076` as the last error.
Nice, but can i tweak it so that the error is less ?
Let's check by updating the telemetry file with slightly different configuration, this time i will increase the learning rate from `0.1` to `0.21`.

```
2,2,1,20,0.21,
4,
0,0,0,
1,0,1,
0,1,1,
1,1,0,
```

This value  plays a crucial role in determining how quickly or slowly the network converges to the optimal set of parameters that minimize the loss function. 
I will write this telemetry as `neuraltest2` and start another task with this telemetry as `INPUTPAYLOAD`.

```
{
"ID": 83770,
"Owner": "PocNetGroupMiners00000000000000000000001",
"Script": "neuralnetwork",
"Input": "bmV1cmFsdGVzdDI=", ===> neuraltest2
"Reward": 1,
"Fails": 0,
"Restarts": 1,
"State": 4,
"Created": 1690068875,
"Execs": 2,
"Start": 1690068883,
"End": 1690068899,
"ExecUser": "JadeChartreuseDromiceiomimus",
"ExecPayload": "RE9ORSwwLjM1OTYyMSw=" ===> DONE,0.359621,
}
```

This time the error is `0.359621`,
as you see the configuration of the network plays critical role in the efficency of the training process.
This is why, choosing correct parameters is often a paralellized task. Easily you can now imagine, that you can run 100 of training tasks,
each with different configuration parameters to find optimal settings for your neural network.



## 18/07/2023 - Integrating GridShell with DuCo mining 👷‍♂️

Required: [script](https://api.gridshell.net/scripts/dduco.bas) 

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
