# GridShell Explorer

![image](https://github.com/invpe/GridShell/assets/106522950/d6a24352-07ed-4f94-9059-23dfb5fbc743)


Easy visualization is one of the key concepts of using GridShell, that's why we've got Grafana to get you covered.

[https://explorer.gridshell.net:3000/](https://explorer.gridshell.net)

Login with: `explorer/gridshellexplorer`

# List of dashboards to use

![image](https://github.com/invpe/GridShell/assets/106522950/b9156906-e3a7-45ee-ba3f-d5e51b0ab4fc)

## GridShell Community Leaderboard [link](https://explorer.gridshell.net:3000/d/b032899f-fffa-47fc-b38e-4a58be40197b/community-leaderboard?orgId=1)

![image](https://github.com/invpe/GridShell/assets/106522950/542e3f87-e780-4bf2-8d0b-067b84d02680)


## GridShell Core [link](https://explorer.gridshell.net:3000/d/e66e2a62-ef5d-482b-aeee-3dc8a3bb6e2b/core)
This one is a core stats of the server

![image](https://github.com/invpe/GridShell/assets/106522950/c9432390-e87f-4a50-ad4c-9469444df72b)


## GridShell Whale [link](https://explorer.gridshell.net:3000/d/ca74c27d-03f9-4f18-8017-001d013b93ce/whale)
This one is a dashboard created for Whale Pool Temperature sensors integration.

## GridShell User [link](https://explorer.gridshell.net:3000/d/cd4c7abe-f622-4384-8a6d-348f3bffde47/user)
This one is to help getting user details

![image](https://github.com/invpe/GridShell/assets/106522950/a8e827ea-c2a8-4322-90fa-4704b444a341)


## GridShell Your Telemetry [link](https://explorer.gridshell.net:3000/d/cf9d5f97-9f02-488a-b49a-28409bdc1593/your-telemetry)

![image](https://github.com/invpe/GridShell/assets/106522950/e840abb4-af37-45f1-93af-9514c3cc080e)


This is a generic dashboard you can use to visualize the data, but ensure to keep your telemetry data aligned to the following CSV format:

|unix epoch|valueA|valueB|valueC|valueD|
|----------|------|------|------|------|
|1688087943|1|2|3|4|
|1688087963|11|32|32|41|
|...|...|...|...|...|

![image](https://github.com/invpe/GridShell/assets/106522950/4a0d32ff-8329-4e2d-b819-faa8aa69b4a3)

This is not the end, as this dashboard also supports JSON data, so just unfold the category for the telemetry format you are storing and you're good.

## Ideas ?
New dashboards, custom dashboards, your own Grafana instance ? 
Feel free to share!

