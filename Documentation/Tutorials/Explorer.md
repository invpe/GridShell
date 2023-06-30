# GridShell Explorer

Easy visualization is one of the key concepts of using GridShell, that's why we've got Grafana to get you covered.

[Use this endpoint](https://explorer.gridshell.net:3000/) 

`login: explorer
password: gridshellexplorer `

to move from JSON data to graphs and colors.

# List of dashboards to use

## GridShell Core   [🌎](https://explorer.gridshell.net:3000/d/e66e2a62-ef5d-482b-aeee-3dc8a3bb6e2b/gridshell-core)
This one is a core stats of the server

## GridShell Pool [🌎](https://explorer.gridshell.net:3000/d/ca74c27d-03f9-4f18-8017-001d013b93ce/gridshell-pool)
This one is a dashboard created for Whale Pool Temperature sensors integration.

## GridShell User [🌎](https://explorer.gridshell.net:3000/d/c67e1c66-8ac3-480e-860b-7d1ba05aa8f8/gridshell-user)
This one is to help getting user details

## GridShell Generic Graph [🌎](https://explorer.gridshell.net:3000/d/cf9d5f97-9f02-488a-b49a-28409bdc1593/gridshell-your-telemetry)

This is a generic dashboard you can use to visualize the data, but ensure to keep your data aligned to the following format:

|unix epoch|valueA|valueB|valueC|valueD|
|----------|------|------|------|------|
|1688087943|1|2|3|4|

