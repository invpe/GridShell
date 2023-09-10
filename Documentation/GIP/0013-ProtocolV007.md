# GIP 0012 - Protocol V007 Version

- Author(s): gridshell
- Date: --/-/2023

# Synopsis
Protocol update to version 007, revoking logic for ARCH and EXEC parameters but also
simplify parsing welcome message by passing DH details in one line instead of sending it after `\n`

Revoke [GIP0008](https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0008-ProtocolV005)
Revoke [GIP0002](https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0002-ARCH.md)


# Rationale
Since ARCH is not required as part of the Node identification (will be replaced with a task), this needs to be removed.
Exec parameter is no longer used, also it is planned to be removed.



