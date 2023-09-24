# GIP 0021 - User Ranks
- Author(s): GridShell
- Date 14/9/2023

# Synopsis

Implement a mechanism to give ranks to users and display them on the Explorer endpoint in the user dashboard.
Adding this for the users will allow for additional gamification in the system inluding:

- Recognition and Motivation: Users appreciate recognition for their contributions. Displaying ranks showcases the achievements of active and dedicated users, motivating them to continue participating and contributing positively to the platform.

- Competitive Spirit: Users may become more engaged in competitions or challenges if they can see their rank relative to others. This competitive aspect can lead to increased activity and participation.
  
- Establishing Expertise: Higher-ranked users may be seen as experts or leaders in the community. Newcomers or those seeking assistance are more likely to turn to higher-ranked users for guidance, leading to a more supportive environment.

# Details

In the initial version of the ranking system, each user will have a rank assigned based on a simple metric made of:

`rankValue = submitCount + totalExecutions + UPF`

The `rankValue` will be then normalized with the `min` and `max` values that are found within the users list.

`normalizedRank = 100.0 * (rankValue - minRankValue) / range;`

We're not fixing this, going forward we're opening for modifications so that ranking system can be fitted even better taking inputs from the overall ecosystem.

Following names are given based on thresholds for the ranks and are visible on the Explorer endpoint.

![image](https://github.com/invpe/GridShell/assets/106522950/c931f18a-36e8-44ac-b8fa-055a7f26b979)





