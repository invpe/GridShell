<p align=center>
  <img src=https://camo.githubusercontent.com/67f3d5b189fd0c66ac455e9b421a7c9df2811b31716926e76ec00547165c8438/68747470733a2f2f6772696473686c2e66696c65732e776f726470726573732e636f6d2f323032322f30362f696d675f333438362e706e67>
  </p>
  

# Tasks
For a distrigbuted computing to actually work, we need to provide it's computational power some real work to do.
This work is called Tasks on the GridShell.
  
Tasks are exactly what is being executed(interpreted) and computed on all GridShell nodes and are written in an [BASIC](https://en.wikipedia.org/wiki/BASIC)-like language, leveraging a great open source interpreter called [mybasic](https://github.com/paladin-t/my_basic).
There is a great amount of tutorials and playgrounds for BASIC language, and since it was made to be _Begginers all purpose_ it was adopted for GridShell  to minimise the effort needed to build projects that can be run on the grid.

The speed of the project execution on the Grid is defined by the number of tasks, their computational complexity and available nodes online.
Tasks can't run forever therefore a timeout limit is applied to task execution for every node. 
After this limit is exceeded task is considered failed and restarted.

Task is considered completed once done and results are returned in `OUTPUTPAYLOAD`, 
however on top of the completion there is a process called validation - 
that ensures that all completed tasks have returned valid outputs from their execution.

Checkout some tasks that are running on test net today by heading to the GTN status page and clicking through last executions.
https://api.gridshell.net/status/



# Who is creating the tasks ?
In the current version of GridShell, tasks are created by the core team.
Moving forward, when the grid network matures enough submitting tasks to the grid will be possible for everyone.

Submitting tasks to the grid, requires `Shell Tokens` - credits that are given to nodes for executing the tasks correctly.

# How can i create a script (task)
So you want to write a task ? Perfect, head straight to the tutorial.


# Why are tasks created ?
There are various reasons for why you would like to distribute your computational workload to a grid system.
The most common one the GridShell was approached with are:
 
- Repeatable computations

Generating values for dynamic graphs always requires some time to compute.
Instead of executing this repeatable task on the web server, it can be offloaded to a grid system that will compute and provide results within minutes.

- Offloading  

Moving computation from devices to a grid system to allow more time and space for additional logic.

- Headless setups

Using GridShell as your all-in-one service can also work fine for small devices like sensors.
Sending telemetry chunks, requesting calculations from the network and visualizing data without hosting any in-between servers is perfectly doable!

 
# Example tasks

All tasks that are executed on the grid are placed in the https://github.com/invpe/gridshell/tree/main/Documentation/Tasks folder. 
Feel free to browse and review, as more and more tasks are added this folder will grow becoming quite a nice library of scripts you can run / build based on existing projects.
