# GIP 0080 - Replace scripting with dynamic binary load/execution

- Author(s): gridshell
- Date: --/--/--

# Abstract

GridShell has been functioning well with tasks executed in my-basic scripting language. However, several issues have emerged, impacting its usability, adaptability, and speed:

- Continuous maintenance is required for exposing functions within the GridShell library, and there's no guarantee that these functions will align with users' needs.
- Scripts cannot fully utilize the ESP32's capabilities until exposed by the GridShell library.
- Script interpretation is slow, leading to sluggish task execution.
- My-basic is not optimized for ESP32, making it challenging to keep up with updates.
- My-basic imposes significant overhead on program size, particularly problematic for nodes with their own logic implemented.

To address these challenges, a shift towards dynamic binary loading and execution is proposed. This change offers several benefits:

- Any code can be executed at full speed, including STL and third-party libraries.
- Binaries can be compiled with optimization parameters, reducing program size.
- The concept of General Purpose IoT is introduced, enabling idle time utilization for diverse code execution.
- Improved performance in terms of memory usage, speed, and energy efficiency based on benchmarking studies.

Key considerations include:

- Ensuring the security of dynamically executed binaries to prevent malicious actions.
- Implementing heap management to prevent tasks from exceeding available memory.
- Maintaining existing crash handling logic.
- Implementing task timeouts to terminate tasks that exceed execution time limits.
- Enabling task access to telemetry data, possibly by reusing the GridShell library within the master sketch.
- Establishing a pipeline for task submission and execution, replacing the current script-based approach.
- Maintaining the existing protocol for communication between nodes and the master sketch.

# Justification

Recent benchmarking studies, such as those outlined in [this](https://haslab.github.io/SAFER/scp21.pdf) paper, have shown that interpreted languages generally perform poorly in terms of memory usage, speed, and energy efficiency compared to compiled languages. In particular, C has been identified as a top performer across all these categories. Given that GridShell prioritizes energy and eco-friendly distributed computing, transitioning to pure C for task execution is a logical step to enhance the platform's efficiency and effectiveness.

# Proposed Changes

Dynamic Binary Loading and Execution

Framework Setup:
- Design and implement a framework that allows ESP32 nodes to dynamically load and execute binaries.
- Develop memory management functions to allocate, load, and execute these binaries securely and efficiently.

Security and Stability:
- Ensure that dynamically executed binaries are thoroughly validated to prevent malicious actions.
- Implement robust heap management to prevent memory overflow and ensure tasks do not exceed available memory.
- Maintain and enhance existing crash handling logic to manage errors effectively.

Performance Optimization:
- Allow binaries to be compiled with optimization parameters, reducing program size and increasing execution speed.
- Leverage ESP32's native capabilities fully by using pure C for task execution, ensuring minimal overhead and maximal performance.

Task Management:
- Implement task timeouts to automatically terminate tasks that exceed predefined execution time limits.
- Enable tasks to access telemetry data, potentially reusing the GridShell library within the master sketch for consistency.
- Develop a pipeline for task submission and execution, transitioning from the current script-based approach to a binary-based approach.

Communication Protocol:
- Maintain the existing communication protocol between nodes and the master sketch to ensure seamless integration and operation within the GridShell network.

# Conclusion

Transitioning from an interpreted language to dynamic binary loading and execution using pure C will significantly enhance GridShell's performance, energy efficiency, and usability. By addressing current limitations and leveraging the strengths of compiled languages, GridShell can better fulfill its mission of providing a robust, eco-friendly distributed computing platform.

This proposal is currently under active development and testing, with ongoing progress documented and shared via a dedicated project repository at https://github.com/invpe/GP-IOT.
