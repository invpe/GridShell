# 📰 Series of blog entries for GridShell
## 🗒️ 7/9/2023 - V06 Deployment

We are moving to `06` today, and will upload latest sources EOD, please ensure to update your nodes as
we're changing things, see latest  [GIP](https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0011-ShellSync1.md).

## ➡️ 4/9/2023 - ShellSync and Explorer migration

The ShellSync work is in progres, see newest [GIP](https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0011-ShellSync1.md).
Meantime i am also moving away from hosting explorer to Grafana in Cloud, links have been updated.

## 📊 24/8/2023 - Moving from API status to Explorer

Moved out from using API/status endpoint displaying all core details now in the Explorer dashboard.
See [dashboard](https://explorer.gridshell.net:3000/d/e66e2a62-ef5d-482b-aeee-3dc8a3bb6e2b/gridshell-core?orgId=1) instead 👍



## 💹 10/8/2023 - Updates to explorer

This time adding total number of tasks executed that were validated on the user dashboard and user json file.

![image](https://github.com/invpe/GridShell/assets/106522950/20dc8684-3e61-45f2-840a-cf433cd24643)

This is not taking into consideration previous executions, however all tasks forward that will get executed and confirmed valid,
will increase the counter for the user.

## 🧮 8/8/2023 - Updates to explorer

Following last week comments, there were few updates made to the explorer and node.
Explorer now displays rewards per time for a specific grid user

![image](https://github.com/invpe/GridShell/assets/106522950/a6c8f4a2-452f-48b4-a844-5e6eaac1a9a1)

This helps to visualize the execution for your account 👍

Nodes have also been updated to store their ID in lower case when writing telemetry [here](https://github.com/invpe/GridShell/commit/4e89c7c6924d6a802584ca1e468a50f642a3e2b2),
to allow linkage from User dashboard to Node telemetry from the Online List table.

![image](https://github.com/invpe/GridShell/assets/106522950/be34402f-9e6c-43b1-ae86-b1066f201ea5)

Now with a single click you can move from User dashboard to your GridShell node.


## 🍀 27/07/2023 - Building solar powered node 

Keeping in mind how important for this project is to use renewable energy sources, i targetted for this month to run a single node absolutely on sun energy,
using cheap components available in any electronic store. There will be two approaches to this task:

- Continous run, where no deep sleep is introduced, to see how long it will stay up and what is the charge ratio to uptime.
- Idle time, where node will wake up (from the deep sleep) check if any work is requiring execution and proceed, finishing with entering a deep sleep again.

I am pretty sure that the second approach will be easy to achieve, so i'll try first with the first one.
One very important point is that i want to be able to measure the battery level via A0 (Analog pin) and report as part of the telemetry.
This will give me a clear view on which of the two scenarios i will be testing gives best results. 

So to start, let's first check what hardware i will be using.

### 💪 Hardware 

I am focusing only on already existing electronics in my garage.
If any updates will be required, i'll do so but first let's reuse whatever i have to kick start.

1. ESP 32 Mini [specs](https://nettigo.eu/products/wifi-bluetooth-module-esp32-d1-mini?_gl=1*bp3yf*_ga*NjUwOTgzNjU5LjE2OTA0NTgzMDQ.*_ga_2MCQHEHG4M*MTY5MDQ1ODMwMy4xLjAuMTY5MDQ1ODMwMy4wLjAuMA..)
2. A TP4056 charging board [specs](https://botland.store/charger-modules-for-li-po-batteries/6944-lipol-charger-tp4056-1s-37v-microusb-with-5904422375355.html)
3. A battery 18650 Li-Ion Sony US18650VTC5 2600mAh [specs](https://botland.store/li-ion-batteries/4271-18650-li-ion-cell-sony-us18650vtc5-2600mah-5900000000411.html)
4. A solar panel [specs](https://botland.store/low-power-solar-panels/3695-solar-cell-07w55v-95x95x3mm-5904422302856.html)

Simply put - a solar panel will be connected to the charger. Charger gets connect to battery (to charge it) - from now, we have a circuit which charges the battery that will power up the node from the sun.
Next, we connect ESP to the outout of the charger and we have a continous flow of voltage, powering up our node. 
Since i want to measure the battery voltage, i'll also run a wire between 3.3 and A0 (SVP) on the board.

### Simple schematics

![image](https://github.com/invpe/GridShell/assets/106522950/b2b61b2e-73ff-41be-9439-d86877b59432)

```
SOLAR+ to TP4056 +
SOLAR- to TP4056 -

TP4056B+ to BATTERY +
TP4056B- to BATTERY -

TP4056OUT+ TO ESP 3.3
TP4056OUT- TO ESP GND

ESP 3.3 TO A0
```

This setup will:

- Load the battery
- Power up the ESP
- Allow ESP to read A0 values (0,4095) - we're maxing a bit, since it requires 3.3 there, but i have nothing to drop it down before A0 to 3.3, so for now i'll keep it that way.

### ⛹️‍♂️ Software 

I will use the usual [node sketch](https://github.com/invpe/GridShell/tree/main/Sources/Node/ClientESP32) for ESP32.
The only update i will use is enable `ENABLE_TELEMETRY` which will write a telemetry file that consists of few diagnostic items but also readings from A0 mapped to `<0,100>` in percents.

`float fBatteryLevel = map(analogRead(ANALOG_PIN), 0.0f, 4095.0f, 0, 100);`

This is absolutely enough to cover the requirements.
The node will operate as usual, but send diagnostic telemetry including readings from analog pin.
I can track it later with the Explorer using [Gridshell Node Telemetry dashboard](https://explorer.gridshell.net:3000/d/d46107a0-d8c2-49a0-8924-de7299152567/gridshell-node-telemetry?orgId=1).
       
Let's fire off, and monitor the progress over [here](https://explorer.gridshell.net:3000/d/d46107a0-d8c2-49a0-8924-de7299152567/gridshell-node-telemetry?orgId=1&var-NodeID=7C9EBD793FA0&var-GUser=PocNetGroupMiners00000000000000000000001)

🤞

![image](https://github.com/invpe/GridShell/assets/106522950/d3b78b27-7f69-4dc9-88ab-2f5f461a7636)

📆 Update with Deep Sleep (10m)

Added deep sleep to the [Whale](https://github.com/invpe/GridShell/tree/main/Sources/Integrations/Whale) sensor plugged into the solar, seems very promising.
The sleep time is only `10 minutes` which for a pool sensor is too often, 
[Explorer](https://explorer.gridshell.net:3000/d/ca74c27d-03f9-4f18-8017-001d013b93ce/gridshell-pool?orgId=1).


📆 Update

Keeping ESP32 up and running without deep sleep is a **no go**, the solution dies after running 2hrs on fully charged battery. 
I'll be migrating to an option where i'll deep sleep, check if any tasks are to be executed and deep sleep if not.
Only keep up, when there is really something to execute. Will come back with updates after i get this done 👍



## 🛰️ 20/07/2023 - Playing with a simple neural network 

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
![image](https://github.com/invpe/GridShell/assets/106522950/43be2566-f82c-4a89-bc2f-51faa7552dbf)


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
