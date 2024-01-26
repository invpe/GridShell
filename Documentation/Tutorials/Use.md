<img src="https://github.com/invpe/GridShell/assets/106522950/ad0ffc49-d470-45f9-923d-86fc8966c7b7">
<div align=center><tt>GridShell nodes</tt></div>

# 📘 Using GridShell

Indepentent of what makes you move, if you are here then you are looking for ways to utilize the GridShell network.
Maybe you are interested in contributing by simply joining your nodes to the network boosting it's performance both in storage and computation,
or you just want to send the data from your sensor and display it - getting GridShell to work for you is a very simple task.

# 👨‍🦲 Setup your account first

Start off with getting yourself account created on the network, simply use [this link](https://api.gridshell.net/getuser/) to get yourself covered. 
All things with GridShell require account, so having one is a good way to start.

- [Create account](https://api.gridshell.net/getuser/)
- [Read more](https://github.com/invpe/gridshell/blob/main/Documentation/Tutorials/Join.md#create-account-on-the-testnet)

# 🥬 Use straight away ?

Maybe at this moment you're ready to go, not everyone has nodes connected - some of us are just sending tasks without connecting any devices.
In situations where you want to simply store telemetry data or run tasks on the GridShell network without connecting any nodes, an account created in previous step is all that you need.
Of course you will need Shells to submit work to the Grid - but don't worry,
if you don't have any, we will get you some for free for a start, let us know !

- [Just use it with PHP](https://github.com/invpe/GridShell/tree/main/Sources/PHP)
- [Just use it with ESP32](https://github.com/invpe/GridShell/tree/main/Sources/Integrations)

# 🔌 Connect with pure Vanilla nodes 🍨

Vanilla nodes are plain ESP32 devices with the latest GridShell firmware installed, no custom code running - simply 
nodes that contribute and earn rewards, zero coding required. This is the simplest way to start, you will be sharing ESP32 processing power
and earning rewards for executing tasks submitted by other users.

- [Upload](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Join.md#setup-from-a-release-file) the release firmware to your board and you're ready is the preffered way.


- [Building](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Join.md#setup-from-sources) from the source code is more complex but also possible if you are interested.


This way or another you should end up with a programmed ESP32 with latest firmware meaning, your Node contributing to the network and getting Shells for you, for executing tasks properly. 
Good job, if you just joined - don't forget to say hello in the discussions panel.

# 💻 You want to join your existing device ?

Amazing, sharing the idle power with the network is the key concept in the GridShell!
Here you update your existing arduino sketch and incorporate GridShell to it, so that any idle processing power is shared with the GridShell network.
This requires some coding, but surely you're aware of that.

Simply [download](https://github.com/invpe/GridShell/tree/main/Sources/GridShell) the library and store it in your sketch folder. 

Grab only these files, from the repo:

```
CGridShell.cpp
CGridShell.h
my_basic.cpp
my_basic.hpp
```

Next initiate the GridShell with a call to [::Init](https://github.com/invpe/GridShell/blob/17ae6bf044d357150c0bae0ab921022d1807206e/Sources/Integrations/Whale/Whale.ino#L99) setting the "Auto update flag to FALSE" and providing your User Hash from [Step 1](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Use.md#-setup-your-account-first)

In your main loop just [::Tick](https://github.com/invpe/GridShell/blob/17ae6bf044d357150c0bae0ab921022d1807206e/Sources/Integrations/Whale/Whale.ino#L120) the network to start contributing and executing the network tasks, that's how simple it is!

Your sensor device will now be part of the network executing tasks and earning rewards!

Additionaly you can see how [Whale](https://github.com/invpe/GridShell/tree/main/Sources/Integrations/Whale) is integrated, it is exactly a device sensor that contributes to the network.

# Once you are in
Now that you are connected and part of the network, there are tons of things you can do. 
Let's list the basic ones to get you started.

## 🧑‍🍳 Time to store telemetry ?

That's a great idea, at any point in time if you want to store telemetry from within your arduino sketch simply use GridShell library method [CGridShell::AddTask](https://github.com/invpe/GridShell/blob/4646432a7b02208b37f3177719b95c06f6a19a03/Sources/GridShell/CGridShell.cpp#L748) to submit a `writedfs` task which post execution and validation will get your data (called [telemetry](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Telemetry.md)) stored on the network. 
 
👁️‍🗨️ [See here](https://github.com/invpe/GridShell/blob/4646432a7b02208b37f3177719b95c06f6a19a03/Sources/Integrations/Whale/Whale.ino#L175) how it is implemented.


## 📜 Need to execute tasks ?

Absolutely, simply use the same `CGridShell::AddTask` [method](https://github.com/invpe/GridShell/blob/main/Sources/GridShell/CGridShell.h#L94) from the library to submit a task to the network.
This time instead of submitting `writedfs` you submit whatever task you want to run on the network.

If you see there is a functionality missing or you want to write your own script - do not hesitate to build it and post [here](https://github.com/invpe/GridShell/discussions/3) so together we will convert this to a task that everyone can use. 

## 📱 Need to visualize the telemetry ?

Nobody likes plain text, and nobody cares about reading them - so instead of trying to spend more time on visualizing the raw data, use the [GridShell Explorer](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Explorer.md) endpoint that is made on magnificent [Grafana dashboards](https://grafana.com/). This simple dashboard covers `99%` of the telemetry needs, it is absolutely enough !

Just follow the telemetry guidelines as mentioned in the documentation above, and you're set.

The dashboard you will be looking at is called [GridShell Your Telemetry](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Explorer.md#gridshell-your-telemetry-link).

## 🤾‍♂️ Want to play with the scripting ?
Absolutely good idea, download the Playground sketch and change your ESP32 to become a scripting environment,
dont forget to share you4 creations !

Maybe even turn them into tasks if you are ready !

- [Playground](https://github.com/invpe/GridShell/tree/main/Sources/Playground)

## 🧮 Monitor your tasks and user

The explorer endpoint allows to monitor all aspects of the network using Grafana.
Familiarize yourself with the metrics and get most out of it for you.

- [GridShell Explorer](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Explorer.md) documentation

## ⚗️ Run your own GridShell instance

You can get your own server up and running in few seconds, maybe there's things you want to test, fuzz, play or simply adjust to fit your needs.

## ❇️ Share ideas, contribute

This is an open source project, feel free to contribute share ideas and provide feedback.
There are many areas which can be improved in the current version of the GridShell project, lots of tools that could be built and even more integrations that could be done.
Feel free to contribute at any point in time!

# ⛹️‍♂️ What more ?

Only fun, and keep it that way.

Build something: a sensor, a network of sensors, a device, anything that you wanted to make but always dragged due to all the backend and frontend work required.
You don't have to rent servers, configure instances, deploy services, host visualizations, keep things up to date, register anywhere. Just focus on your product and send whatever you need to GridShell.
Then link a dashboard and visualize your great work without spending a minute more on anything else but your product only.

Familiarize yourself with the [documentation](https://github.com/invpe/GridShell/tree/main/Documentation/Tutorials), enjoy and have lot of fun building and coding your stuff without spending a minute on the boring things that drag you from the creative part of what you enjoy the most.

🧑‍🤝‍🧑 Keep in touch, ask questions and play as much as you can by contributing to the GridShell network and using it for your needs 

# ❔ All questions are welcome !

Don't feel like you're alone, post questions if any and get answers to your concerns.


