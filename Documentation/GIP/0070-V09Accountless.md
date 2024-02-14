# GIP 0070 - Implement accountless contribution to the network

- Author(s): gridshell
- Date: 13/02/24

# Abstract

This GIP outlines improvements to the user onboarding process to simplify volunteering within GridShell. 
The proposed enhancements aim to strike a balance between simplicity for users and protection against potential abuses such as flooding and other malicious activities.
This GIP is an entry point implementation for further improvements in the process.

# Motivation

The conventional "create an account" method seems impractical for a project that encourages volunteer participation through a streamlined and eco-friendly joining process. The act of creating accounts has consistently been viewed as an unnecessary step within the GridShell network. In our efforts to minimize this additional hurdle and simplify the process, we are opting to streamline it further. Volunteers can now effortlessly choose their User ID, consisting of a straightforward 64-byte string, eliminating the need for traditional account creation.



# Changes Proposed

0. User-provided Unique IDs

Starting at this stage, volunteers furnish their IDs as an integral part of the "JOB" command. If the ID is already known, volunteers effortlessly associate it with their nodes, signifying a straightforward account connection. In cases where the ID is unfamiliar, the designated ID is generated. After a designated 'cooldown' period, volunteers' nodes seamlessly adopt this ID as their account.

`JOB,64_bytes_long_user_selected_ID,...`

1. Cool down period

In instances where the ID is unknown (indicating the creation of a new one on the network), the node's address that sends the "JOB" command undergoes temporary throttling for a cooldown period. This measure is in place to prevent the rapid and mass creation of accounts. During this period, users only need to leave their nodes as the cooldown will conclude after a random duration. Subsequently, nodes will seamlessly rejoin, actively contributing to the network.

2. Trust Score or Reputation System

Empowering User Performance Ratio (UPF) as a trust score for nodes within the network. New accounts start with a probationary UPF of 1%, governing tasks execution, token sending, and commands usage. Gradually increase UPF as users contribute positively to the network through task completion, validated by a trusted validator pool.

3. Gradual Trust Building

The UPF serves as an implicit verification step, restricting the impact of new accounts on the network until they've demonstrated value. This incentivizes positive contributions and mitigates the risk of dummy activity.

4. Account Deletion

This [GIP](https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0037-AccountsHousekeeping.md) ensures accounts are used and not left for stagnation.

# Impact Assessment

These proposed changes aim to enhance the usability of GridShell by maintaining a user-friendly accountless contribution process. 
The trust-building mechanism encourages positive user engagement, contributing to the overall reliability and integrity of the distributed computing network.

# Conclusion

This GIP delineates fundamental enhancements to GridShell's user onboarding procedure. The suggested modifications are geared towards establishing a harmonious equilibrium between user simplicity and safeguarding against potential misuse. It's crucial to emphasize that this marks a foundational stage expected to serve as a cornerstone for the project's future advancements.
