# GIP 0027 - Introducing GridShell Award System

- Author(s): gridshell
- Date: --/--/2023

# Synopsis
GridShell is taking a step forward in recognizing and rewarding the contributions of our valued users. With the proposed GridShell Award System, we aim to acknowledge and incentivize users who actively participate in the GridShell ecosystem.

# Rationale
Award System will track user activity, including task submissions, successful executions, length of involvement, telemetry desynchs, and more. 
Based on predefined criteria, users will receive Shell Tokens as awards, allowing them to unlock additional benefits within the GridShell network.

The GridShell Award System not only promotes active user engagement but also fosters a sense of community and collaboration. By rewarding contributions,
we empower our users to play a central role in the evolution of the GridShell project.

# Categories and Milestones

In this revision awards in the shape of a medal will be visible on the Grafana dashboard and generated for the following categories:

    Submissions:
        Bronze: Awarded for the first task submitted.
        Silver: Awarded for the first 100 tasks submitted.
        Gold: Awarded for the first 1,000 tasks submitted.

    Execution:
        Bronze: Awarded for the first task executed.
        Silver: Awarded for the first 100 tasks executed.
        Gold: Awarded for the first 1,000 tasks executed.

    Nodes:
        Bronze: Awarded for the first node connected.
        Silver: Awarded for the first 10 nodes connected.
        Gold: Awarded for the first 50 nodes connected. 

    Shells:
        Bronze: Awarded for accumulating 100 shells.
        Silver: Awarded for accumulating 1,000 shells.
        Gold: Awarded for accumulating 10,000 shells. 

As we progress forward, this can be updated with new categories but also values to adopt to the volumes within the ecosystem.

# User Progression

The award system operates seamlessly within the GridShell server logic. As users progress, their medals will adapt to their current status. This approach motivates users to continue their contributions to the network.

The awards system is designed to celebrate the achievements of our GridShell community and inspire further engagement within the network. 

# Availability

All categories and values in the respective numbers 0 None - 3 Gold will be available on the Explorer endpoint but also in the users JSON file hosted on API endpoint,
making it easy to read and integrate within users solutions.
