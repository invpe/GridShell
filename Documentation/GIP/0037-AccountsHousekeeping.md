# GIP 0037- Update to accounts purging logic

- Author(s): gridshell
- Date: --/--/24


1. Abstract
   
Accounts purging is a cleanup process taking place in the GridShell, so far it was only relying on accounts creation time triggered by a check based on a value defined
in the configuration file as `UserAutoRemovalTime`. This is not enough and an improvement is required for accounts defined as `not used`. 
A `not used` account is defined as an account which did not have nodes joining in last `UserAutoRemovalTime`. This means that no node was seen for an account within this time,
the account is permanently removed. 

2. Introduction

This improvement seeks to ensure that only active accounts are persisted and the focus of the network is shifted towards volunteers actively participating in the system.

3. Proposed methodology

A check already existing for never used accounts within `UserAutoRemovalTime` timeframes is updated with a logic to see when an account was used last time.
If it hits the configured value, the account is marked for deletion and all it's resources are permanently removed from the system. 

