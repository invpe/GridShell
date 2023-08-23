# GIP 0010
- Author(s): GridShell
- Date: --/8/2023

# Synopsis
Implement additional counters (Total Executions, Total Submissions) for users

# Rationale
- Reason for this GIP, what does it solve, improve.
  
This implementation will help to track execs and submissions per user.
It solves the lack of visibility to : how many tasks a user has submitted and how many task a user has executed.

- What are the expected results.
  
Each user will have additional counters added to their profile.
Since this comes late, no previous numbers will be added, we will start from 0.

# Explanation in details
- Provide clean explanation about the new concept

To allow users to track their stats implement two counters with total values of execs/submissions to their profiles.
Render the values on the explorer, but also include them in the user JSON files.

- Clarify implementation details

1) Update core user class with the counters + persistance and loading of the numbers.
2) Update server to dump these values per user when exporting JSON 
3) Update explorer dashboard to visualize these values
  
- Is documentation affected? Which?

  No
  
- Share examples of the implementation usage

  Based on these two values it is clear to understand the ratio behind taking from/making for the ecosystem per user.
  It will be also easier to understand the dynamics of the system from the user perspective.

# Success
- How can we measure the success of this implementation?

Values `Execs` and `Submitted` will be rendered on the user explorer dashboard and visible in the JSON file.
