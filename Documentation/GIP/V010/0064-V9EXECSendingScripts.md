# GIP 0064 - Streamlining EXEC Process: Direct Script Transmission to Nodes

- Author(s): gridshell
- Date: 26/01/24

 1. Abstract
    
Previously, during task execution, nodes experienced a "split-brain" issue. They had to access the API server to retrieve the execution scripts, leading to a dual-source scenario for script management:

- The server was responsible for maintaining and calculating the script's hash.
- The API hosted the script's source code for nodes to download.

To eliminate this complexity, the updated EXEC process will now directly send the script's source to the nodes. This change centralizes script management, making the gridshell server the singular, reliable source of truth for scripting.
