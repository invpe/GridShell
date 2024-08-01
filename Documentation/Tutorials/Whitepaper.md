<img src="https://github.com/invpe/GridShell/assets/106522950/f8ecc335-7358-4b62-9c80-6a5f18ebbac6">
<div align=center><tt>GridShell nodes</tt></div>


# GridShell Whitepaper
## Abstract
GridShell is an innovative distributed computing system that leverages the idle processing power of underutilized IoT devices. This project taps into a market valued at 2.6 billion USD, with projections of reaching 1 trillion IoT devices by 2035. Despite the market's immense potential, many of the 15 billion connected devices today remain idle most of the time, excelling only at their primary functions and missing out on additional benefits like passive revenue and multipurpose use.

Imagine your smart plugs at home, which only turn on and off, yet house a 240MHz CPU. Wouldn't it be beneficial if they could also generate passive income instead of just idling?

GridShell aims to connect these devices, sharing their processing power during idle periods. By enabling tokenomics, this system targets to allow everyone involved to earn passive income, unlocking the untapped computing power currently idling in billions of devices installed in homes worldwide.

## Problem statement
Currently, smart homes and IoT devices are underutilized and largely disconnected, failing to form a cohesive ecosystem. This results in limited or no opportunities for owners to generate passive income or leverage the full computational potential of their devices. If just 1% of these devices—amounting to 10 million—were connected, their combined processing power could be harnessed during idle times. This would enable profitable tokenomics, allowing volunteers/owners to earn passive income for sharing their IOT idle time and allow everyone to utilize the untapped computing power currently lying dormant in billions of devices.

GridShell aims to solve this problem by repurposing idle devices for distributed computing tasks, creating a collaborative network that maximizes their collective power for mutual benefit.

# Technical Overview

## Important
The current GridShell technical implementation serves as a Proof of Concept (POC). It demonstrates the feasibility of engaging in the concept of IoT volunteer computing but is not a production-ready solution. Developing a market-ready version would require a larger team, significant effort, and extensive technical expertise.

## Architecture
GridShell's architecture is built on a network of IoT devices, including ESP32 sensors, and a central server. Nodes in the network share computational tasks, validate results, and store telemetry data.

## Protocols
Secure data transfer is ensured through the implementation of SSL/TLS protocols, guaranteeing the integrity and confidentiality of data exchanged between nodes and the server.

# Key Features
GridShell offers several key features:

- Distributed Computing: Users can submit computational tasks that are executed across the network, harnessing the combined power of IoT devices.

- GridShell supports distributing parts of the code natively within the source code, enabling developers to seamlessly integrate distributed computing tasks using provided methods for task submission and retrieval.

- Telemetry Storage: Telemetry data, referred to as "chunks," is stored and replicated across nodes, providing collaborative storage capabilities.

- Validation Mechanism: A validation process ensures the accuracy and reliability of task execution, with rewards in the form of Shell Tokens for successful validations.

- Shell Tokens: GridShell's native reward, Shell Tokens, are used within the ecosystem for various purposes, including accessing distributed computing power, redeeming telemetry storage slots, and incentivizing participants.
  - Earning Shell Tokens: Users can earn Shell Tokens by successfully executing tasks for others within the GridShell network. Each validated task completion rewards the user with Shell Tokens.

  - Redeeming Telemetry Storage: Shell Tokens can be redeemed to unlock telemetry storage slots. As users accumulate tokens, they gain the ability to store and manage more telemetry data within the GridShell ecosystem.

  - Incentivizing Participation: Shell Tokens play a pivotal role in incentivizing active participation within GridShell. They encourage users to contribute their processing power, making the network more robust and dynamic.
    
  - Burning Tokens: Every time a user submits a task for execution within the GridShell network, they "burn" a Shell Token. This mechanism ensures that tokens are used as intended, promoting fair and active participation in the ecosystem.
  
- Serverless Device and Application Building: GridShell enables the creation of serverless devices and applications, reducing the need for external hosting services.
  
- Easy integration: Simply use a ready-to-use PHP script or the Library to allow for quick integration with other applications. The GridShell library can contribute to and use the GridShell network at the same time

- Green Computing: By repurposing idle IoT devices, GridShell promotes environmental sustainability and reduces electronic waste. Utilizing ultra-low-power devices, such as ESP32, GridShell operates efficiently and can be powered by renewable energy sources like solar power.

# Hybrid Edge Computing and Centralized Orchestration

GridShell employs a hybrid approach that combines edge computing principles with centralized task orchestration. Here's how it works:

- Edge Data Collection: IoT devices, such as Node A, collect data at the network edge, where it is generated. This data can include sensor readings, telemetry, or other relevant information.

- Edge Task Submission: Users or applications submit computational tasks to GridShell directly from the edge nodes. These tasks can be triggered based on specific events, thresholds, or schedules, allowing for real-time analysis and decision-making at the edge.

- Centralized Task Orchestration: Upon receiving a task request, the GridShell server orchestrates its execution by identifying an available node within the network to perform the computation. This central coordination ensures efficient resource allocation and load balancing across the GridShell ecosystem.

By combining edge data collection and task submission with centralized task orchestration, GridShell optimizes the performance and scalability of distributed computing while leveraging the proximity and efficiency benefits of edge computing.

This hybrid model enables GridShell to handle a wide range of use cases.

# Use Cases
GridShell's applications are diverse:

- Educational Projects: Use the network to teach concepts of distributed computing, sensor networks, and data collection.

- IoT Sensor Networks: IoT devices can contribute to distributed computing tasks while performing their primary sensing functions.
  Imagine a vast air quality operation where IoT sensors continuously collect data on atmospheric pollutants levels. GridShell allows these sensors to not only perform their primary
  functions but also contribute their idle processing power to predictive analytics, reporting and visualization of the data.

- Logic execution through GridShell enables the execution of scripts for device logic management, eliminating the need for hardcoding directly onto IoT sensors. This approach allows for the centralized management of hundreds of sensors using single or multiple scripts, significantly simplifying the process of updating and customizing device logic across a vast network.
  
- Reducing costs: Solution can be used to zero out the cost at the entry stage of your product. No hosting needed, deploy & test with GridShell.
  Startups and entrepreneurs can eliminate hosting costs during the initial stages of product development by leveraging GridShell's serverless capabilities.
  Deploy and test your product without the burden of infrastructure expenses.
  
- Collaborative Data Analysis: Users can pool their computational resources for collaborative data analysis and processing.
  In the field of research, multiple institutions can collaboratively analyze datasets for studies.
  GridShell enables distributed data processing, fostering innovation and breakthroughs through collective computational power.
    
- Offloading: Reduce load and move it to the network
  Offload tasks from your local device to the GridShell network. This reduces device strain and ensures efficient, uninterrupted operation.
  
- Custom processing: Write scripts and execute them on the network
  Tailor computing tasks to your specific needs using a simple BASIC-like scripting language.

- Simplify IOT data storage: Submit telemetry to GridShell from all of your nodes.
  Use built feature to store your telemetry and access it from your scripts.

- Personalized Instances: Tailor GridShell to your needs by creating customized instances. Build and manage your own simple distributed computing solutions on IoT devices.

- Ultra Low Power Cryptocurrency Mining: GridShell has demonstrated its capabilities in participating in cryptocurrency mining activities. Users have successfully harnessed the power of distributed IoT devices for tasks like Bitcoin Lottery and other Coins mining. While GridShell operates at an ultra-low power scale, it showcases its adaptability to various computational purposes, even within the cryptocurrency realm.
  
  
## Extending GridShell to Other Platforms
GridShell is designed with extensibility in mind. While its current focus is on ESP32 microcontrollers to showcase the concept, it can seamlessly integrate with other platforms like Raspberry Pi. The flexibility of GridShell means it can be adapted to various hardware environments, opening up a broader range of use cases and possibilities.

Incorporating GridShell into platforms like Raspberry Pi unlocks even more potential, making it suitable for a wider array of applications and projects. As GridShell evolves, so does its ability to work with different hardware, creating a versatile and adaptable ecosystem for distributed computing and IoT.

# Roadmap
GridShell's development roadmap includes:

- Enhanced Security: Ongoing security improvements to protect the environment

- Expanded Use Cases: Continuously exploring new applications and integrations, expanding GridShell's utility across industries.

- Opening for new IOT platforms: 
  Introduce a streamlined process for adding new IoT platforms, such as Raspberry Pi and ESP8266, to the GridShell network. Community input and collaboration will be central to this effort.
  
- Transition to Real-Value Rewards: GridShell aims to empower its volunteers to transition Shell Tokens into real rewards. This strategic shift encourages volunteers to actively participate in establishing real value for Shell Tokens, fostering a mutually beneficial ecosystem.

- Transition to Binary Code Execution:
In order to boost performance and leverage low-level functionalities, there's plans to transition from scripting languages to executing binary code. 
  
# Conclusion

In conclusion, GridShell represents more than just a project; it embodies a vision of a smarter, more sustainable future. 
Envision a world where your IoT-connected smart home appliances not only enhance convenience but also generate income. 
As the value of GridShell Tokens continues to rise, this vision hopefully transforms into reality.

GridShell breathes life into your idle devices, converting them into valuable assets that contribute to a greener, more interconnected future. 

Inspired by the history of BOINC and its ambitious pursuit of volunteer computing, GridShell aims to carry forward the spirit of distributed volunteer computing into the realm of IoT devices. (Reference: [BOINC History](https://continuum-hypothesis.com/boinc_history.php))
