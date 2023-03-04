<p align=center>
  <img src=https://gridshl.files.wordpress.com/2022/06/img_3490.png>
</p>

-------------------

# User Performance Factor
The UPF "User Performance Factor" is a value that tells the Grid system how well nodes for a user are performing their job.
As we know, the main job of a node is to execute a given task and return proper output.

To calcualte the performance of every user, the GridShell is looking at two very important numbers:

- Number of tasks a node returned that are valid
- Number of tasks a node returned that are invalid

This is part of the Validation process that is crucial to the success of task execution.

Every user on the grid has UPF value calculated, this is a score of the user and it is rendering its overall performance factor for the system.
UPF is used in every major logic: assigning tasks, rewarding for task execution, validation and throttling for task execution.

With the current version, users with low UPF will:

- have their tasks validated once their nodes complete the work
- have less chance to be picked for task execution 
- have less chances to be rewarded for a valid execution  

It is good to check out `GIP` for `UPF` as things can and will definitely change in this area.

Why is it so important to keep this value high?

The GridShell performance is tightly coupled with the users UPF values, 
the higher the UPF's the faster the Grid as the number of validation reduces.

There will be more logic introduced for low UPF users within upcoming [Grid Improvement Proposals](https://github.com/invpe/gridshell/tree/main/GIP).

