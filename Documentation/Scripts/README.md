
# 📔 Writing scripts(tasks) for GridShell
One important thing at the very start: This is a WIP document, needs more time to work on it.

Even though writing tasks for GridShell is like writing MyBasic scripts, there are small additions that you need to be aware of.
Before going anywhere further, ensure to note down these resources - key to the concept of writing tasks for the network.

1. [MyBasic](https://github.com/paladin-t/my_basic) 
2. [MyBasic Web Playground](https://my-basic.github.io/playground/output/index.html) 
3. [GridShell Web Playground](https://github.com/invpe/GridShell/tree/main/Sources/Playground) 
 
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
The best way to find them is to [look](https://github.com/invpe/GridShell/blob/bf7d8c47b384a4d4cc06de98340da7cf90961900/Sources/GridShell/CGridShell.cpp#L313) in the library sources.

 
