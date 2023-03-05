# GIP 0005 - UPF based Tasks assignment
- Author(s): gridshell
- Date: 26/01/23

# Synopsis
When nodes fail task executions - their UPF is reduced.

The ratio of assignment of tasks for each node is determined by the UPF of the user.

- The higher the UPF the more chance a node is given a task for execution.
- The lower the UPF the less chance a node is given a task for execution.

Avoid assigning tasks to nodes whos users have poor UPF factor.

# Rationale
- Reason for this GIP, what does it solve, improve. 
- What are the expected results.
- What if it is not done?
- Why is it considered the best option?

# Explanation in details
- Provide clean explanation about the new concept
- Clarify implementation details
- Is documentation affected? Which?
- Share examples of the implementation usage

# Success
- How can we measure the success of this implementation?
