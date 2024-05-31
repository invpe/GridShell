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

Key considerations include:

- Ensuring the security of dynamically executed binaries to prevent malicious actions.
- Implementing heap management to prevent tasks from exceeding available memory.
- Maintaining existing crash handling logic.
- Implementing task timeouts to terminate tasks that exceed execution time limits.
- Enabling task access to telemetry data, possibly by reusing the GridShell library within the master sketch.
- Establishing a pipeline for task submission and execution, replacing the current script-based approach.
- Maintaining the existing protocol for communication between nodes and the master sketch.

This transition is still a work in progress, with ongoing testing and development conducted on a separate project available at https://github.com/invpe/GP-IOT.

