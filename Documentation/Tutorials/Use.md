<img src="https://github.com/invpe/GridShell/assets/106522950/ad0ffc49-d470-45f9-923d-86fc8966c7b7">
<div align=center><tt>GridShell nodes</tt></div>

# 📘 Using GridShell

Indepentent of what makes you move, if you are here then you are looking for ways to utilize the GridShell network.
Maybe you are interested in contributing by simply joining your nodes to the network boosting it's performance both in storage and computation,
or you just want to send the data from your sensor and display it - getting GridShell to work for you is a very simple task.

# 👨‍🦲 Setup your account first

Start off with getting yourself account created on the network, simply use [this link](https://api.gridshell.net/getuser/) to get yourself covered. 
All things with GridShell require account, so having one is a good way to start.

- [https://api.gridshell.net/getuser/](https://api.gridshell.net/getuser/)
- [Read more](https://github.com/invpe/gridshell/blob/main/Documentation/Tutorials/Join.md#create-account-on-the-testnet)

# 🥬 No need to join a node ?

Maybe at this moment you're ready to go, not everyone has nodes connected - some of us are just sending tasks without connecting any devices.
In situations where you want to simply store telemetry data on the GridShell network without connecting any nodes, an account created in previous step is all that you need.
Of course you will need [Shells](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Shells.md) to submit work to the Grid - but don't worry,
if you don't have any, we will get you some for free for a start, let us know !

# 🔌 Maybe you want to connect nodes to contribute and get Shells for a reward?

This is the way to go, where you connect your ultra low power ESP32 [nodes](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Nodes.md) that contribute to the network, 
both benefiting users by executing their tasks and you rewarding you with [shells](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Shells.md) for [valid](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Validation.md) execution. Getting this step is as simple as getting your account created.

There is two ways of doing it:

1. Simply by uploading a release firmware to your board 👍 - definitely strongly advised for simplicity.
Hop in [here](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Join.md#setup-from-a-release-file) to set it up.

2. By compiling the sources and programming your board with [Arduino IDE](https://www.arduino.cc/en/software/).
If you like the idea, hop [here](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Join.md#setup-from-sources) to set yourself up.

This way or another you should end up with a programmed ESP32 with latest [Vanilla Release](https://github.com/invpe/GridShell/releases) - meaning, your Node contributing to the network and getting Shells for you, for executing tasks properly. 

Good job, if you just joined - don't forget to send us a welcome message 😄

# 💻 You want to join your existing device ?

Amazing, sharing the idle power with the network is the key concept in the GridShell!

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

# 🧑‍🍳 Time to store telemetry ?

That's a great idea, let's not wait with that.

Simply use GridShell library method [CGridShell::AddTask](https://github.com/invpe/GridShell/blob/4646432a7b02208b37f3177719b95c06f6a19a03/Sources/GridShell/CGridShell.cpp#L748) and you will submit a [task](https://github.com/invpe/gridshell/blob/main/Documentation/Tutorials/Task.md) which post execution & [validation](https://github.com/invpe/gridshell/blob/main/Documentation/Tutorials/Validation.md) will get your data (called [telemetry](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Telemetry.md) ) stored on the network. From that moment you can freely access it from the [API endpoint](https://github.com/invpe/GridShell/tree/main/Documentation/API) over a simple HTTPS call.

If you are trying to figure out how to use this method, don't spend a minute more - just use existing example of [Whale](https://github.com/invpe/GridShell/blob/main/Sources/Integrations/Whale/Whale.ino) - the pool temperature sensor to immediately go from guessing to doing. 

👁️‍🗨️ [See here](https://github.com/invpe/GridShell/blob/4646432a7b02208b37f3177719b95c06f6a19a03/Sources/Integrations/Whale/Whale.ino#L175) how it is implemented.


# 📱 Need to visualize the telemetry ?

Nobody likes plain text, and nobody cares about reading them - so instead of trying to spend more time on visualizing the raw data, use the [GridShell Explorer](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Explorer.md) endpoint that is made on magnificent [Grafana dashboards](https://grafana.com/). This simple dashboard covers `99%` of the telemetry needs, it is absolutely enough !

Just follow the telemetry guidelines as mentioned in the documentation above, and you're set.

The dashboard you will be looking at is called [GridShell Your Telemetry](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Explorer.md#gridshell-your-telemetry-link).

# ⛹️‍♂️ What more ?

Only fun, and keep it that way.

Build something: a sensor, a network of sensors, a device, anything that you wanted to make but always dragged due to all the backend and frontend work required.
You don't have to rent servers, configure instances, deploy services, host visualizations, keep things up to date. Just focus on your product and send whatever you need to GridShell.
Then link a dashboard and visualize your great work without spending a minute more on anything else but your product only.

Familiarize yourself with the [documentation](https://github.com/invpe/GridShell/tree/main/Documentation/Tutorials), enjoy and have lot of fun building and coding your stuff without spending a minute on the boring things that drag you from the creative part of what you enjoy the most.

🧑‍🤝‍🧑 Keep in touch, ask questions and play as much as you can by contributing to the GridShell network and using it for your needs 

# ❔ All questions are welcome !

Don't feel like you're alone, post questions if any and get answers to your concerns.


