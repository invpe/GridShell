# Frequently Asked Questions (FAQ)

The FAQ is split between non technical and technical questions i have received since the project has started.
The idea is to keep it separate for clarity.

<details>
<summary>Generic</summary>
   
1. What is GridShell?

   GridShell is a collaborative distributed computing system (POC/MVP) designed to utilize the idle processing power of underutilized IoT devices.
   It creates a network where these devices can contribute to computational tasks, storage, and serverless capabilities.

2. How does GridShell work?

   GridShell works by connecting IoT devices, such as ESP32 sensors, into a network. Users submit computational tasks to the network,
   which are then executed across these devices, harnessing their combined processing power. Telemetry data is stored and replicated across nodes,
   and a validation mechanism ensures task accuracy and reliability.

3. What are Shell Tokens?

   Shell Tokens are GridShell's native reward system. Users earn Shell Tokens by successfully executing tasks for others within the GridShell network.
   These tokens can be redeemed for accessing distributed computing power, redeeming telemetry storage slots, and incentivizing participation within the ecosystem.


4. How can I earn Shell Tokens?
   
   You can earn Shell Tokens by successfully completing tasks within the GridShell network. Each validated task completion rewards you with Shell Tokens.

5. Can i swap tokens?

   Currently, there is no way to swap the GridShell utility token. If you are interested, you can take the initiative to list the token on free swapping services.

7. What are the applications of GridShell?

   GridShell has diverse applications including educational projects, IoT sensor networks, collaborative data analysis, visualization, offloading tasks,
   custom processing, and much more.

8. Can GridShell work with other IoT platforms?

   Yes, GridShell is designed with extensibility in mind and can seamlessly integrate with other platforms like Raspberry Pi.
   The project is open source and it can be adapted to various hardware environments, opening up a broader range of use cases and possibilities.

9. How does GridShell contribute to a sustainable future?

   GridShell reduces waste by utilizing the idle processing power of underutilized IoT devices, contributing to a greener,
   more interconnected future. It also empowers users to make the most of their IoT investments and reduce infrastructure expenses.

10. What makes GridShell unique?

   GridShell is unique in its innovative approach to harnessing the idle processing power of IoT devices for distributed computing.
   Unlike traditional systems, GridShell repurposes underutilized devices, such as ESP32 sensors, creating a collaborative network that maximizes their potential.

11. How does GridShell contribute to the future of IoT?
   
   GridShell is positioned at the forefront of the rapidly expanding IoT ecosystem, tapping into the billions of online devices,
   with even more predicted to go online in the coming years. This innovative platform introduces the concept of IoT devices earning real value
   for their owners instead of idling. By unlocking the latent processing power of these devices and creating a marketplace where their
   contributions are rewarded with Shell Tokens, GridShell idea introduces the potential to spark a new market dynamic,
   where every connected device becomes a valuable asset in the distributed computing landscape.

11. Why is this project a POC / MVP ?
    As the sole developer of this project, advancing GridShell beyond its current Proof of Concept (POC) or Minimum Viable Product (MVP) stage demands
    significant time, resources, and networking. Focusing solo, my efforts are concentrated on demonstrating the project's potential through its MVP/POC iteration.
    This allows for a clear showcase of its capabilities while acknowledging that further development into a fully-fledged product requires additional investment and collaboration.

12. What are GIP(s) ?

    These are documents i use to store the improvement ideas for GridShell. They help out to find ourselves in the vast space of ideas and implementations done so far.

13. What is TV ?

    TV stand for `Theoretical Value of a Shell`, it is dynamically calculated based on several key factors that reflect the current state and performance of the network.
    Here's a summary of how each component contributes to this calculation: [GIP060](https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0060-TV.md)

</details>


<details>
<summary>Technical</summary>

1. How do i start computing?
   
   `task_id = submit("script","input payload")`
   
   `results = gettask(task_id)`
   
   It's that simple and follows the distributed tasks execution pattern.

   - You can start simply with the easy-to-use php script: [here](https://github.com/invpe/GridShell/tree/main/Sources/PHP), check [this GIP](https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0081-V09-ADDTASKCustomScript.md) for examples.

   - Or by using the GridShell C++ library [here](https://github.com/invpe/GridShell/tree/main/Sources/GridShell), check [basic integrations](https://github.com/invpe/GridShell/tree/main/Sources/Integrations) for lots of examples.
   
      

3. What are the supported devices for use?

    Pretty much any ESP32 can become a GridShell node, as long as you can compile the sources or upload the latest release, you're ready to go. Check [here](https://github.com/invpe/GridShell/edit/main/Documentation/Hardware/README.md) for the list of devices tested.

4. Why use a scripting language like MyBasic?

    To prioritize simplicity in implementation and ease of demonstration, I've opted for a scripting language for tasks executed on GridShell. MyBasic's  straightforward syntax ensures accessibility for all users, fostering a welcoming environment for experimentation and testing within the GridShell ecosystem.

5. Why a centralized server instead of decentralization?

    In pursuit of simplicity and expeditious development, GridShell adopts a centralized server architecture rather than a decentralized peer-to-peer approach. This design choice facilitates rapid development and streamlines implementation processes, aligning with our priority of creating a user-friendly environment for efficient deployment and utilization. Simply put - because it was easier.


6. When using `use.php` or other way of interacting with the server i get an error:

```
PHP Warning:  stream_socket_client(): Failed to enable crypto in gridshell.php on line 35
PHP Warning:  stream_socket_client(): Unable to connect to ssl://work.gridshell.net:1911 (Unknown error) in gridshell.php on line 35
[GSHELL] Cant connect
```

In most cases this means you have been temporarily blocked, either by the exceed of the rate limits or incorrect behaviour.
Wait untill your address gets auto unblocked.

5. How do i submit a task, send shells, burn shells ?

    Use the simplest form of interacting with GridShell by using [php](https://github.com/invpe/GridShell/tree/main/Sources/PHP) client. But you can also work with GS via the library. See example integrations.


6. Are scripts fast?
    
    The current implementation employs an interpreted language (MyBasic), which may not deliver optimal speed performance, especially on Ultra-Low Power (ULP) devices like ESP32. While this aspect is subject to improvement in the future, the primary focus at present is on stability and simplicity. As GridShell evolves, efforts to enhance script execution speed on such devices will be considered.

7. How can I integrate GridShell into my IoT device's idle time?

    For IoT devices that typically follow a cycle of polling sensor data, waiting, and then polling again in an endless loop, you can replace any 'wait' period with a single call to `GridShell::Tick()`. This will allow your IoT device to participate in the GridShell network during its idle time.

8. How can I integrate GridShell into my IoT device's idle time if it goes into deep sleep?

    If your device goes into deep sleep after polling, you can use the `EVENT_NO_TASKS_TO_EXECUTE` callback to determine if there are any tasks to perform. If there are no tasks, the device can continue sleeping. However, if there are tasks, the device can work on them, even if it is at random intervals. You decide!

9. Can I execute tasks only if there is something to do?

    Absolutely. The core principle of GridShell is minimal power usage, aiming for green computing. If you don't want to keep your node online, simply use the `EVENT_NO_TASKS_TO_EXECUTE` callback to check if there are any tasks to perform. When this value is `zero`, you can disconnect and go to sleep.

All vanilla nodes starting from version 0.10 (V10) will have this functionality built into the setup screen [GIP0075](https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0075-TODO_V10_GoToSleepFlagVanilla.md)
</details>


