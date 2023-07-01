# GIP 0009
- Author(s): GridShell
- Date: 1/7/2023

# Synopsis
Introduce MAX Open files within Retention window

# Rationale
- Reason for this GIP, what does it solve, improve. 

This implementation is required to disallow creating hundreths of files without any limits.

- What are the expected results.

Max Open Files flag is applied per Grid User, with `default=10` meaning 10 open files within `retention window`.

- What if it is not done?

It will be hard to progress towards distributed filesystem but also keep current GFS clean

- Why is it considered the best option?

There needs to be a mechanism to limit number of files opened on GFS (future: DFS), it is not existing today.

# Explanation in details
- Provide clean explanation about the new concept

Each user will have `10` files to open within `retention` window.
When `WRITE` command is used it will return `FILE,BAD_MAX_FILES` error when limit reached.

- Clarify implementation details

A simple check is made:

`If user is opening new file (not appending) and currently has >= MAX_OPEN_FILES_PER_USER then do not continue write operation`

- Is documentation affected? Which?

None, this GIP serves this purpose

- Share examples of the implementation usage

When opening multiple files i as a user am limited to 10 files within my 7 days retention window.
Once the file is removed (after reaching retention time) i can open another file keeping up to 10 files open only.

Appending is not impacted by this GIP, only creating new files.



