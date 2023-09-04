# GIP 0011 - Distributing telemetry storage
- Author(s): GridShell
- Date: --/-/----

# Synopsis
Implement ShellSync - a distributed filesystem for telemetry storage.
Leverage Nodes to store `CHUNKS` of telemetry data, `REPLICATE` chunks across the network when Nodes become unavailable, persist on local server storage for eventual fallback scenarios. 
This is the first out of few improvements that segway GridShell to distributed storage - closing the gap in it's core functionality.

# Reasoning
It is observed that the GridShell is predominantly utilized (constituting 90% of the monthly workload) for telemetry storage, with only 10% allocated for computation tasks. Consequently, we have decided to allocate resources towards enhancing the codebase and transitioning to a distributed file storage solution, a crucial step in aligning with best practices for distributed systems.
This marks the inaugural iteration of the implementation, wherein we will introduce storage and replication mechanisms. As a result, there will be a heightened emphasis on ensuring that nodes remain consistently online to provide telemetry data with optimal availability. 

With the growing emphasis on decentralization in the world of technology, and considering that we have thus far underutilized the storage capacity inherent to the nodes, it is now an opportune moment to take a significant step towards incorporating distributed storage into our project. This move addresses a crucial gap in our project's evolution, as it has been primarily focused on distributed computing without the inclusion of distributed storage. This transition aligns our system with the broader trend towards distributed and decentralized architectures

# What storage ?
ESP32 devices come equipped with few MB of available SPIFFS (flash) storage. 
Some ESP32 modules offer even more storage, with options such as 4MB, 8MB, or 16MB, as detailed in Espressif Products Ordering Information.

```
ESP32-WROOM-32 By default the module has 4MB flash and may be also ordered with custom flash size of 8 or 16MB, see Espressif Products Ordering Information.
```

When these devices are programmed from the Arduino environment, the storage undergoes partitioning. For example, in the case of the M5Stack AtomU based on the [spec](http://docs.m5stack.com/en/core/atom_lite), there is `4MB` of flash for use.

The responsibility for optimizing available storage for telemetry data chunks lies in the hands of our volunteers. Each successful write operation contributes to the improvement of the User Performance Factor (UPF) and is rewarded with Shell tokens. This encourages volunteers to maximize their storage allocation, thereby enhancing the system's overall performance.
 
# How will this work ?

The process of writing TELEMETRY data from the end user's perspective remains unchanged. Users will continue to submit tasks with their desired text as the payload. However, a fundamental shift occurs in how this data is managed. Instead of storing the data solely on the server, an internal mechanism within the server orchestrates the distribution of the data to available nodes for storage.

As long as there are nodes available to execute tasks, there exists a repository to store a portion of the telemetry data. In instances where no nodes are immediately available, the server will pause the distribution of data until at least one node becomes active. This process is referred to as replication, where data is replicated from the local server storage to the online nodes.

The replication mechanism will also diligently work to guarantee the seamless migration of data from a node that experiences a chunk drop to other available nodes. In such instances, a standard task submission process will ensue, where the node responsible for recovering the lost chunk after a disconnection will receive a shell reward, just as with any other validated task within the ecosystem.

Whenever a node goes offline, its file system undergoes a clearing process, which guarantees that upon reconnection, the node possesses sufficient storage capacity to undertake the task of writing new data chunks. This deliberate approach places an increased workload on the replication mechanism, as it continually strives to migrate lost chunks to available nodes whenever others experience temporary drops. This aligns with the observed, common lifecycle in the GridShell system, where nodes frequently disconnect and reconnect multiple times daily.

In cases where a telemetry chunk cannot be written, typically due to insufficient storage space, the process will adhere to the standard task execution procedure. It will make up to three attempts on different nodes to locate a suitable one with sufficient available space to store the chunk in its filesystem. A failure in this scenario, akin to any other task execution failure, results in a reduction of the User Performance Factor (UPF). This encourages volunteers to maintain their nodes in an available state for telemetry writes at all times.

# What is new ?
- We will now use different DH numbers, so an update to your node is mandatory
- Commands `FMD5` `B64D` `B64E` `DEL` exposed to MyBasic
- Tasks `deldfs.bas` (core), `writedfs.bas` (all), `replicatedfs.bas` (core), `md5dfs.bas` (core) introduced
- All `Write tasks` will end up making `CHUNK`s of data on your node(s)
- SPIFFS on your ESP32 nodes will be heavily utilized and adequately on other arch's their filesystem too
- `CHUNKS` get removed after disconnection to prepare SPIFFS for next session with the server
- `CHUNKS` will be replicated if required
- `Explorer` will render the `TELEMETRY` with it's `CHUNK`s allocaton for visualization/debugging purposes


 # What will stay fro the old telemetry ?
- Submitting write tasks

- Server will still serve data from local storage for computation, visualization and API purposes while keeping Shell Sync in the processing pipeline.
However i am targeting to move to serving data from Shell Sync in the next releases of Shell Sync.
   


 





  
