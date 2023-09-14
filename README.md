

<p align="center">
  <img  src="https://github.com/invpe/gridshell/blob/main/Resources/gridshell_small.png">
</p>

<p align="center">
  <i>Join your arduino sketch to iot powered distributed volunteer computing network</i>
</p> 
 

<div align="center">
<a href=https://github.com/invpe/gridshell/blob/main/Documentation/Tutorials/Use.md><img src="https://badgen.net/static/>/Use GridShell/34ebd5?scale=2?"></a>  
<a href=https://github.com/invpe/GridShell/discussions><img src="https://badgen.net/static/>/Community/34ebd5?scale=2?"></a>  
  
<a href=https://github.com/invpe/GridShell/tree/main/Documentation/API><img src="https://badgen.net/static/>/API/ebcf34?scale=2?"></a>
<a href=https://github.com/invpe/GridShell/tree/main/Sources/Integrations><img src="https://badgen.net/static/>/Integrations/ebcf34?scale=2?"></a>

<a href=https://github.com/invpe/gridshell/tree/main/Documentation><img src="https://badgen.net/static/>/Documentation/ebcf34?scale=2"></a>
<a href=https://github.com/invpe/gridshell/tree/main/Sources><img src="https://badgen.net/static/>/Sources/ebcf34?scale=2?"></a>
<a href=https://gridshell.grafana.net/><img src="https://badgen.net/static/>/Explorer/ebcf34?scale=2?"></a>
<a href=https://github.com/paladin-t/my_basic><img src="https://badgen.net/static/>/MyBasic/ebcf34?scale=2?"></a><BR>
  
</div> 

--- 
# :seedling: Welcome

With billions of IOT devices connected today there is a huge potential of computing power ready for use at minimal or even zero cost. These tiny computers are capable of operating on renewable energies like solar power, giving tremendous opportunities for utilizing their computing capabilities without impacting environment. We all have WiFi enabled sensors, smart bulbs, sockets, plugs... which idle for most of their time - why not connect them together and use as one big computer to solve our problems ?
 
Welcome to GridShell, an innovative distributed computing solution powered by ESP32 nodes. Designed to address high-throughput computing challenges, GridShell seamlessly integrates data processing, analysis, and visualization for a wide array of applications.


<img src=https://github.com/invpe/gridshell/blob/main/Resources/GSEP.png>
<div align="center"><tt>GridShell cluster operating 192 nodes in ULP mode</tt></div>


# ⭐ GridShell early users
<div align="left"> 
<a href=https://looko2.pl/><img src=https://github.com/invpe/GridShell/assets/106522950/edf4e09d-5aae-49a9-825d-29a4be909509></a> 
</div>



## 🗃️ Key Features
- Distributed Computing Power: Leverage the collective computing strength of ESP32 nodes for efficient task execution.
- Distributed Storage: All telemetry data is stored on the network nodes
- Telemetry Data Processing: Store and process telemetry data from IoT devices, enabling deep analysis and insights.
- Custom Script Execution: Empower users to execute custom scripts and calculations on telemetry data.
- Real-time Visualization: Integrate with Grafana dashboards for real-time and historical data insights.
- API and Integration: Provide HTTPS API access for seamless integration with external applications.
- Renewable Energy Compatibility: Operate on renewable energy sources or at zero cost, promoting sustainability.
- Zero hosting: Use existing GridShell platform to store,read,compute and visualize data.

## 🧑‍🏭 Workload Suitability:
GridShell is best suited for high-throughput computing scenarios where the emphasis lies in achieving a high rate of job completion. It excels with workloads involving large job groups or streams, prioritizing efficient job processing over low job turnaround time. While GridShell is versatile, it may be less suitable for workloads demanding extensive memory or storage resources or those with a notably high ratio of network communication to computing.

## 🧑‍🤝‍🧑 Community-Powered Volunteer Computing:
At GridShell, we celebrate the spirit of volunteer computing. Our network is open for anyone with ESP32 or any other ULP nodes to join, creating a community of volunteers contributing computing power to collaborative projects. By participating, volunteers collectively enhance the capacity and impact of distributed computing, all while sharing the ideals of volunteerism and community-driven innovation.When you join the GridShell network with your node, you become part of a collaborative force that drives innovation. As a contributor, your node executes tasks launched by others, strengthening the collective computing power.

## 🆕 Innovative Reward Mechanism:
GridShell's innovative token-based system rewards users for their contributions. When your node successfully executes a task launched by another user, you are granted a valuable shell token. These tokens are a testament to your commitment to the GridShell community.

## 💝 Token Economy:
Your Shell tokens are your key to accessing GridShell's full potential. Submit tasks by exchanging shell tokens. When your submitted task is successfully validated, you contribute back to the community and allow them to earn more tokens. This cycle of participation and reward fuels the GridShell ecosystem.

## 🧮 Data-Driven Computations:
GridShell introduces a new dimension to computing through the capability for tasks to read/write telemetry files. Tasks can utilize telemetry data as input, enhancing the network's ability to perform complex computations that draw insights from real-world data.

## 🧑‍🤝‍🧑 Distributed Telemetry Storage (ShellSync)
Users can write and store telemetry data in a collaborative, distributed manner. Telemetry entries, referred to as "chunks," are seamlessly stored on available nodes. In cases where nodes disconnect, the server handles replication, ensuring the availability of telemetry data. The server also maintains a local copy as a fallback option.

## 👁️‍🗨️ Empowering Insights:
By allowing tasks to access telemetry files, GridShell bridges the gap between raw data and powerful insights. Users can perform computations that leverage real-world telemetry data, enhancing the precision and relevance of their analyses.

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

## Use Case: Environmental Monitoring and Analysis 🥬

## Scenario:
Imagine you're an environmental researcher working on a project to monitor air quality in a metropolitan area. To achieve accurate results, you need to process a significant amount of data collected from various sensors across the city.

## GridShell Solution:
GridShell provides an ideal solution for this task, completely redefining how data processing and analysis are accomplished. You write a custom script in MY BASIC to process the sensor data, removing noise and identifying pollution patterns. This script utilizes telemetry entries containing sensor readings and their timestamps.

## Task Submission and Telemetry:
You submit the script as a task to the GridShell network. To initiate the task, you spend a shell token as a transaction cost. Volunteer nodes, distributed across the city or even your nodes (sensors) if you connect them to GridShell; pick up the task and execute your script using real-time telemetry data collected.

## Validation and Rewards:
Upon execution completion, the validation mechanism compares the results from the volunteer nodes with those from a trusted validator node. When results match, the volunteer nodes' owners are rewarded with shell tokens as recognition for their accurate execution. 

## Analysis and Decision-Making:
With validated results, you access the processed data through GridShell's comprehensive visualization tools, like Grafana dashboards. The data is stored on GridShell itself, eliminating the need for external hosting. GridShell's integrated environment streamlines your analysis of air quality trends, pollution hotspots, and potential correlations. You make informed recommendations for urban planning and pollution control, all within the GridShell ecosystem.

## Collaborative Impact and Host-Free Environment:
In this use case, GridShell not only accelerates data processing and analysis but also eliminates the need for external hosting. Volunteer nodes from individuals across the city or globally contribute to the accuracy of your analysis. 


# :boom: Message from the author

I am excited to work on this project and have you visiting and joining the GridShell. 
As we stride forward in reshaping the landscape of ultra low power distributed computing, I want to acknowledge that this project is still in its development phase.
While we continue to innovate and refine our systems, it's only natural that you might encounter occasional hiccups or challenges along the way and i recognize that unforeseen issues might arise as we work to enhance the platform's capabilities.

During this development phase, all feedback is invaluable to me. 
If you encounter any issues, have suggestions, or experience any uncertainties feel free to reach out to me - because, as the old adage goes, there's nothing quite like the synergy that arises when two minds come together to innovate and solve challenges.

GridShell is being developed ~~few months~~ a year now by single me, ironing out issues, optimizing the platform, creating a seamless experience, building the logic behind.
With your support and understanding, I am confident that this hobby project will continue to evolve into something much bigger than i initially thought.

# :running: Where do i go next

- **Connect** your first node [link](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Join.md)
- **Use** GridShell [link](https://github.com/invpe/GridShell/tree/main/Documentation/Tutorials/Examples) 
- Checkout the **sources** [link](https://github.com/invpe/GridShell/tree/main/Sources)
- Checkout the **explorer** [link](https://github.com/invpe/GridShell/blob/main/Documentation/Tutorials/Explorer.md)



# 🗃️ License
```
MIT License

Copyright (c) 2023 GridShell.net

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
 

