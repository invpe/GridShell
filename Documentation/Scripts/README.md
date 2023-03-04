# 📔 Writing scripts(tasks) for GridShell

Even though writing tasks for GridShell is like writing MyBasic scripts, there are small additions that you need to be aware of.

# Input (optional)

When submitting a task to the Grid, you can provide an argument which can be used by that task later on.
We call it `INPUTPAYLOAD` and it is stored as a string which you can later work on i.e tokenize.

Example: `INPUTPAYLOAD="A,B,C,D,"`


# Output (mandatory)

All tasks require to return output in form of a string, store all output data in `OUTPUTPAYLOAD` string.
For example if your script calculates some values, store them in this variable.

If this variable is left empty, the network will consider the task failed, so ensure to not return empty results.

Example: 
`OUTPUTPAYLOAD="Job done,12345,"`


# Additional helper functions

GridShell exposes few more internal functions that you can use in your scripting.

`BIT_AND` - Bitwise AND

`BIT_OR` - Bitwise OR

`BIT_NOT` - Bitwise NOT

`BIT_XOR` - Bitwise XOR

`BIT_LSHIFT` - Bitwise Left Shift

`BIT_RSHIFT` - Bitwise Right Shift

`DOWNLOAD` - Download telemetry file to local filesystem 

- Input: Telemetry file to download (full name) i.e `PocNetGroupMiners00000000000000000000001DNB4C11AEF6ECF02023218`
- Output: Bytes written, Output filename is always `TELEMETRY`

- Example: `RET=DOWNLOAD("PocNetGroupMiners00000000000000000000001DNB4C11AEF6ECF02023218")`
- Shells: 1



`READ` - Read a file from internal SPIFFS filesystem

- Input: Filename, start byte, size
- Output: String or empty if failed
- Example: `STRING=READ("TELEMETRY",1,30)`
- Shells: 1

`WRITE` - Write a telemetry data to a gridshell network

`SHA1` - Generate SHA1
