# GIP 0075

- Author(s): gridshell
- Date: 08/08/2024


# Synopsis
Implement a configuration parameter asking wether a vanilla node should go to sleep for defined time if there is no tasks to pickup, this will use `EVENT_NO_TASKS_TO_EXECUTE` callback from the GridShell core.

# Rationale
The core principle of GridShell is to leverage low-power devices to distribute task execution across online devices rather than relying on power-hungry servers. Our goal is to enable simple tasks and logic execution with minimal ecological impact. Many of these devices run on solar energy, where every second of efficiency counts, or are connected to the grid, where we aim to reduce their already minimal footprint to nearly zero. We achieve this by allowing users to configure their nodes to wake up sporadically, check for tasks, execute them if necessary, and then return to sleep, thus conserving power.

# Explanation in details
During the setup, ask user: `Go to sleep if no work(YES/NO):` 

* A positive answer here `YES`, will use existing callback :

```
  // Nothing to execute
        if (strTasksToExecute.toInt() == 0)
          if (m_pCallback != NULL) m_pCallback(CGridShell::eEvent::EVENT_NO_TASKS_TO_EXECUTE);
```

And perform a `DeepSleep` for `5 minutes`.

* A negative answer here `NO`, will use existing logic where the node stays online.

> [!WARNING]  
> ESP8266 Needs GPIO16 and RST to be soldered to wake up from the deep sleep
> Ensure it is done, or your ESP8266 wont wake up after entering the sleep.



# Documentation

Update the [GIP9999-HandyNumbers](https://github.com/invpe/GridShell/blob/main/Documentation/GIP/9999-HandyNumbers.md) with the default time for a deep sleep.
 


