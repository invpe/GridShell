# GIP 0091

- Author(s): gridshell
- Date: --/--/--
- Status:

# Synopsis
Nodes will connect to the GridShell Git repository to fetch the latest version of tasks to execute. This approach eliminates the need for the server to host tasks, reducing duplicate efforts.

# Rationale
Using Git as the central tool for hosting tasks enhances the open-source nature of the project. It enables the community to submit new tasks, have them reviewed and approved, and make them available directly from the repository. This shift clarifies the process of task submission, deployment, and distribution, leveraging community contributions and ensuring transparency and efficiency.

# Storage

Following repo directories are used to store the binaries and tasks.
Binaries are base64 encoded for simplicty of sending over the network to the nodes.

- [source files](https://github.com/invpe/GridShell/tree/main/Tasks/Source)
  
- [binary files](https://github.com/invpe/GridShell/tree/main/Tasks)

