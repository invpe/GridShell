# GIP 0067 - Reduce API hosting and Explorer update

- Author(s): gridshell
- Date: 06/02/24


1. Abstract

In light of recent enhancements to our server/lib that enable direct user access to data, the necessity for an intermediary API server has been obviated. 
Considering the minimal user engagement with this system and the associated maintenance and operational costs, I've decided to decommission the API server. 
This move will streamline our infrastructure and reduce expenses without impacting data accessibility for our users. 
However, we will continue to maintain and update the explorer page (Grafana), adapting it to utilize the newly introduced commands 
for fetching data directly from the work server. We plan to reassess the necessity of this component later in the year to determine its ongoing 
relevance and utility.

Following are the Gridshell improvement proposals that went live, and an example use of the newly introduced commands.

- https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0063-V9CommandREAD.md
- https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0066-V9CommandGetTask.md
- https://github.com/invpe/GridShell/tree/main/Sources/PHP/Examples  


2. Impact

- User's creation is temporarily impossible - i am anyway planning to address this with https://github.com/invpe/GridShell/blob/main/Documentation/GIP/0070-TODOAccountless.md.
