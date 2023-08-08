<p align="center">
  <img  src="https://github.com/invpe/gridshell/blob/main/Resources/gridshell_small.png">
</p>

<p align="center">
  <i>Join your arduino sketch to iot powered distributed volunteer computing network</i>
</p>
 

- - - - - - - - - - - -

 
<div align="center">
 
[ [Documentation](https://github.com/invpe/gridshell/tree/main/Documentation) ] [ [TestNet](https://api.gridshell.net/status/) ] [ [Sources](https://github.com/invpe/gridshell/tree/main/Sources) ] [ [Explorer](https://explorer.gridshell.net:3000) ] [ [MyBasic](https://github.com/paladin-t/my_basic) ] [ [API](https://github.com/invpe/GridShell/tree/main/Documentation/API) ] [ [ Examples ](https://github.com/invpe/GridShell/tree/main/Sources/Integrations) ] [ [Blog](https://github.com/invpe/GridShell/tree/main/Blog) ]
  
</div> 

<img src=https://github.com/invpe/gridshell/blob/main/Resources/GSEP.png>
<div align="center"><tt>GridShell cluster operating 192 nodes in ULP mode</tt></div>
   
-----   

# :seedling: Welcome

With billions of IOT devices connected today there is a huge potential of computing power ready for use at minimal or even zero cost. These tiny computers are capable of operating on renewable energies like solar power, giving tremendous opportunities for utilizing their computing capabilities without impacting environment. We all have WiFi enabled sensors, smart bulbs, sockets, plugs... which idle for most of their time - why not connect them together and use as one big computer to solve our problems ?
 
Welcome to GridShell, an innovative distributed computing solution powered by ESP32 nodes. Designed to address high-throughput computing challenges, GridShell seamlessly integrates data processing, analysis, and visualization for a wide array of applications.

## 🗃️ Key Features
- Distributed Computing Power: Leverage the collective computing strength of ESP32 nodes for efficient task execution.
- Telemetry Data Processing: Store and process telemetry data from IoT devices, enabling deep analysis and insights.
- Custom Script Execution: Empower users to execute custom scripts and calculations on telemetry data.
- Real-time Visualization: Integrate with Grafana dashboards for real-time and historical data insights.
- API and Integration: Provide HTTPS API access for seamless integration with external applications.
- Renewable Energy Compatibility: Operate on renewable energy sources or at zero cost, promoting sustainability.

## 🧑‍🏭 Workload Suitability:
GridShell is best suited for high-throughput computing scenarios where the emphasis lies in achieving a high rate of job completion. It excels with workloads involving large job groups or streams, prioritizing efficient job processing over low job turnaround time. While GridShell is versatile, it may be less suitable for workloads demanding extensive memory or storage resources or those with a notably high ratio of network communication to computing.

## 🧑‍🤝‍🧑 Community-Powered Volunteer Computing:
At GridShell, we celebrate the spirit of volunteer computing. Our network is open for anyone with ESP32 or any other ULP nodes to join, creating a community of volunteers contributing computing power to collaborative projects. By participating, volunteers collectively enhance the capacity and impact of distributed computing, all while sharing the ideals of volunteerism and community-driven innovation.When you join the GridShell network with your node, you become part of a collaborative force that drives innovation. As a contributor, your node executes tasks launched by others, strengthening the collective computing power.

## 🆕 Innovative Reward Mechanism:
GridShell's innovative token-based system rewards users for their contributions. When your node successfully executes a task launched by another user, you are granted a valuable shell token. These tokens are a testament to your commitment to the GridShell community.

## 💝 Token Economy:
Your Shell tokens are your key to accessing GridShell's full potential. Submit tasks by exchanging shell tokens. When your submitted task is successfully validated, you contribute back to the community and allow them to earn more tokens. This cycle of participation and reward fuels the GridShell ecosystem.

## ⭐ Value Proposition:
GridShell empowers through innovation, sustainability, and community collaboration. High-throughput computing and renewable energy compatibility drive optimized processes and decisions.

# :sparkles: How does it work 

Please see documentation for more detailed explanation. In simple words, it works as follows:

- People - You - join the network with their ESP32 devices (nodes)
- Network executes tasks (written in [MyBasic](https://github.com/paladin-t/my_basic)) on volunteer nodes
- Each volunteer is granted a "Shell" (after validation) for a proper execution
- Shells are used to submit a tasks and use the network

The more nodes connected, the more computing power the network has.
Since version `0.05` everyone can submit a task and start using available resources :+1:

All tasks are written with MyBasic, this allows to simplify 
integration / adoption even more.

# :briefcase: What are some use cases ?

There are dozens of DIY projects requiring a simple way to visualize data, 
from simple humidity sensors to advanced solutions like air quality devices that
require calibrations/tunning and more sophisticated hardware layer. 

However they all have one thing in common - at some stage you defocus from the product
to all-things-around when all you need is just to display the data and process it.

You've made a great device, and now want to show off the results.
You made 10 of them, even more and you start thinking how do you put the
puzzles together so that it takes little or no effort in the end.
This should be simple, absolutely effortless and never drag you from 
making the product better and focus on the code and h/w right ?

The last thing you want to do is to rent a server, deploy services and maintain
it to display your data. This is boring, waste of time and forces you to focus on
activities that are dragging you from your DIY work. Keeping all these things right,
paying for hosting, getting certs deplyed and maintaining it is really not fun when
all you really want to do is focus on the product and eventual sale of it.

Very often, you end up copy/pasting lots of commands to spin up a web server on
a paid vps that you just dont need. It should be simple and quick to go 
from a working sensor to presenting data. We all have been there and dont want to come back
to read manuals, and maintain servers for few sensors we have deployed here and there.

There is another option, you can pay for some integration - which will keep you happy as
long as have not breached the 'few bucks' price for couple of devices. This is ok,
but how fast do you integrate ?

A single line of code will store your `telemetry` on the GridShell system, this is all you need :

`CGridShell::GetInstance().AddTask("write", strPayload);`

From the moment it is executed, a file is written and you can:

- visualize it
- process it
- download it
- share it
- append it

No hosting, no services, no maintenance - the single line that you put in your Arduino sketch
will keep you focused on the product. Once you call it:

- Nodes that are online will take care to store your file
- API endpoint will ensure you can access it
- EXPLORER will visualize it.

Add one more line of code : 

`CGridShell::GetInstance().Tick();`

And you earn a Shell token when there is someone executing a task on the network and your device
will run it. As simple as that.




# :boom: Important

This is a very dynamic project where i often do changes, ensure to keep up with the latest and greatest versions of the sources.
The project has nothing to do with mining, crypto or things like that so don't approach it that way. As of today it is in early shapes
being developed ~~few months~~ a year now by single me, so don't expect things to work like a charm everytime here ;-)
Also,
I try to document all major changes in `Gridshell Improvement Proposal` documents that are stored in the [Documentation](https://github.com/invpe/gridshell/tree/main/Documentation) directory.

# :running: Where do i go next

- **Connect** your first node [link](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Join.md)
- **Use** GridShell [link](https://github.com/invpe/GridShell/tree/main/Documentation/Tutorials/Examples) 
- Checkout the **sources** [link](https://github.com/invpe/GridShell/tree/main/Sources)
- Checkout the **explorer** [link](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Explorer.md)


 

 
  
    

  
