# GridShell Explorer

Easy visualization is one of the key concepts of using GridShell, that's why we've got Grafana to get you covered.

[https://gridshell.grafana.net/](https://gridshell.grafana.net/)

# List of dashboards to use

## GridShell Core [link](https://gridshell.grafana.net/d/e66e2a62-ef5d-482b-aeee-3dc8a3bb6e2b/core)
This one is a core stats of the server

## GridShell Whale [link](https://gridshell.grafana.net/d/ca74c27d-03f9-4f18-8017-001d013b93ce/whale)
This one is a dashboard created for Whale Pool Temperature sensors integration.

## GridShell User [link](https://gridshell.grafana.net/d/cd4c7abe-f622-4384-8a6d-348f3bffde47/user)
This one is to help getting user details

## GridShell Your Telemetry [link](https://gridshell.grafana.net/d/cf9d5f97-9f02-488a-b49a-28409bdc1593/your-telemetry)
This is a generic dashboard you can use to visualize the data, but ensure to keep your telemetry data aligned to the following format:

|unix epoch|valueA|valueB|valueC|valueD|
|----------|------|------|------|------|
|1688087943|1|2|3|4|
|1688087963|11|32|32|41|
|...|...|...|...|...|

