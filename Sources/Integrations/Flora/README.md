![image](https://github.com/invpe/GridShell/assets/106522950/6d8a0068-5217-43d9-abf7-b738601f7114)
`Soil moisture sensor for 30y old Malus Haliana (Crab Apple) in late Autumn`
  
## Bonsai
In the world of gardening and horticulture, few practices offer the same exquisite blend of artistry and nature as the ancient art of Bonsai. Originating in China over a thousand years ago, Bonsai represents the skillful sculpting of miniature trees, shaping them to emulate the grandeur and magnificence of their full-sized counterparts. Each Bonsai tree tells a story, revealing the passage of time and the hands of dedicated enthusiasts who lovingly tend to their tiny creations.

But here's the secret known to all Bonsai enthusiasts: Bonsai trees are living works of art, and their care is a subtle dance that only the most devoted and experienced gardeners can master. Among the many challenges faced by Bonsai novices, one task often proves daunting—watering.

![image](https://github.com/invpe/GridShell/assets/106522950/caa6c56b-1316-4b16-bf51-aac09050f460)
`Fagus sylvatica with Soil Moisture sensor`


Bonsai trees, especially the venerable ones that have been nurtured for decades, require precision when it comes to watering. Too much, and they suffer from root rot; too little, and they wither away. For beginners, striking the perfect balance is like navigating a tightrope.

# FLORA

![image](https://github.com/invpe/GridShell/assets/106522950/a39b7181-307c-403b-8160-81144e4b9147)

That's where our journey begins. In our example project, we will introduce you to the world of Bonsai, where the wisdom of nature's ancient guardians meets the innovation of modern technology. We will be monitoring the soil moisture levels of these venerable Bonsai trees, ensuring that their care aligns perfectly with their unique needs.

As we embrace the beauty of these ageless trees, we'll leverage IoT sensors, GridShell's decentralized network, and the power of community collaboration to make the care of Bonsai accessible to enthusiasts of all levels. By monitoring soil moisture in real-time and sharing this data on the GridShell network, we will provide the knowledge and resources needed for everyone to nurture these captivating living sculptures.

![image](https://github.com/invpe/GridShell/assets/106522950/0d6448be-7eb4-43be-b4e7-9ebc7476a0eb)

## Data
By providing real-time data on soil moisture levels through the power of IoT sensors and GridShell's decentralized network, we are opening up a treasure trove of knowledge. Enthusiasts can now access invaluable insights to understand when and how much to water their Bonsai trees. As a community, we not only nurture the art of Bonsai but also contribute to a collective pool of data, ripe for analysis and research.

This data has the potential to reveal patterns, best practices, and perhaps even unforeseen secrets about nurturing Bonsai trees. Researchers and nature enthusiasts alike can delve into this data, enhancing our understanding of these timeless living sculptures.

## Parts
Following hardware is used to enable this project:

- [Cytron Soil Moisture sensor](https://botland.store/humidity-sensors/21207-maker-soil-moisture-capacitive-soil-moisture-sensor-5904422383442.html)
- [M5Stack Atom U](https://botland.store/atom-series/20859-atomu-esp32-iot-development-module-with-usb-a-connector-m5stack-k117-6972934173416.html)


## Visualization
A dedicated Explorer dashboard called [Flora](https://explorer.gridshell.net:3000/d/ef6c31a6-961c-4dac-89f8-c166c4d8691d/flora?orgId=1) has been created.

![image](https://github.com/invpe/GridShell/assets/106522950/5a4161fb-6bcc-4df1-b531-c5adad69a8b0)

## Storage
All plants report data to a single CSV formatted file

![image](https://github.com/invpe/GridShell/assets/106522950/b23ec046-90b6-48d1-9527-23a2be469ef4)

## Customizations
Name each of the plan in the code, by updating [Line](https://github.com/invpe/GridShell/blob/eba05847577363e3436357e1c8fcb2e89ab9236a/Sources/Integrations/Flora/Soil.ino#L14) in the sketch.


## Measurements

![image](https://github.com/invpe/GridShell/assets/106522950/1818e861-6471-43e8-af7b-8c55c4f031a3)
`Malus Haliana with crab apples connected to GridShell`

The moisture is measured every 30 mintues and reported as a telemetry (append) to GridShell.

## Contribution

This sensor will also contribute to the GridShell network, to make use of it's idle time in between readings.
You can update the code to put the device to sleep, however with its logic if woken up and given task it will still contribute before going to sleep 👍
