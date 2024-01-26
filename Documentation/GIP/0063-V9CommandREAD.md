# GIP 0062 - Implement READ

- Author(s): gridshell
- Date: 26/01/24

1. Abstract

Nodes can now retrieve telemetry directly from the GridShell server, bypassing the need for external source downloads. This change simplifies the deployment process and consolidates hosting responsibilities with the server. It enhances processing efficiency and control, while also simplifying the deployment structure by ensuring data transfer occurs solely between the server and nodes, eliminating the need for extra instances.   
This improvement also removes the dependency on external telemetry hosting (currently done over API web server) within the nodes code.

