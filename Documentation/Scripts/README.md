# 📔 Writing scripts(tasks) for GridShell
One important thing at the very start: This is a WIP document, needs more time to work on it.

Even though writing tasks for GridShell is like writing MyBasic scripts, there are small additions that you need to be aware of.
Before going anywhere further, ensure to note down these resources - key to the concept of writing tasks for the network.

1. MyBasic https://github.com/paladin-t/my_basic
2. MyBasic Web Playground https://my-basic.github.io/playground/output/index.html
3. GridShell Command Line Playground https://github.com/invpe/GridShell/tree/main/Sources/ESPPlayground

Ok, now you can continue ;-)


# :inbox_tray: Input (optional)

When submitting a task to the Grid, you can provide an argument which can be used by that task later on.
We call it `INPUTPAYLOAD` and it is stored as a string which you can later work on i.e tokenize.

Example: `INPUTPAYLOAD="A,B,C,D,"`


# :outbox_tray: Output (mandatory)

All tasks require to return output in form of a string, store all output data in `OUTPUTPAYLOAD` string.
For example if your script calculates some values, store them in this variable.

If this variable is left empty, the network will consider the task failed, so ensure to not return empty results.

Example: 
`OUTPUTPAYLOAD="Job done,12345,"`


# :crystal_ball: Additional helper functions

GridShell exposes few more internal functions that you can use in your scripting.

`BIT_AND` - Bitwise AND

`BIT_OR` - Bitwise OR

`BIT_NOT` - Bitwise NOT

`BIT_XOR` - Bitwise XOR

`BIT_LSHIFT` - Bitwise Left Shift

`BIT_RSHIFT` - Bitwise Right Shift

`DOWNLOAD` - Download telemetry file to local filesystem and save as GNODE_TELEMETRY_FILENAME

- Input: Telemetry file to download (full name) i.e `PocNetGroupMiners00000000000000000000001DNB4C11AEF6ECF02023218`
- Output: Bytes written, Output filename is always `/TELEMETRY`

- Example: `RET=DOWNLOAD("PocNetGroupMiners00000000000000000000001DNB4C11AEF6ECF02023218")

`TSIZE` - Return size of the `/TELEMETRY` file in bytes

- Input: Nothing
- Output: Bytes or 0 if failed/file not exists
- Example: `RET=TSIZE()`



`READ` - Read a GNODE_TELEMETRY_FILENAME file from internal SPIFFS filesystem

- Input: start byte, count
- Output: String or empty if failed
- Example: `STRING=READ(1,30)`

`WRITE` - Write a telemetry data to a gridshell network

`SHA1` - Generate SHA1
