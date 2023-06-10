# :loudspeaker: Gridshell - Noiseblaster

A Gridshell enabled, headless noise sensor based on ESP32 and SKU_SEN0232 analog sensor.

_Sound level meters are widely used in environmental noise detection, such as highway noise monitoring station, room noise monitoring and so on. 
It’s time for you to DIY a sound level detector to protect your hearing._

![image](https://github.com/invpe/GridShell/assets/106522950/c11a6805-4e6a-4681-aaf9-5ca4ecb0d961)



# :mag_right: What is it ?

Noise Blaster is a first real "demo" integration with the GridShell environment.
It is using DFRobot's sound meter to measure the sound levels and stores the output on the network for visualisations.
This device is using solely Grid Shell to store, calculate and display data while still contributing with execution of tasks in the background.

It has a simple to [3D Print case](https://github.com/invpe/GridShell/blob/main/Sources/Integrations/NoiseBlaster/NoiseBlaster.stl) (non waterproof!) for you to pack it up and start using.

Noiseblaster is using GridShell explorer frontend made in Grafana for visualisations, keeping your setup completely headless.
You don't need to host anything to be able to use the sensor and the network from the moment you connect.
Just visit the NoiseBlaster dashboard page and provide your's Noiseblaster device id.

![image](https://github.com/invpe/GridShell/assets/106522950/4e065f89-2d81-4450-92b1-eadd964ef72b)
![image](https://github.com/invpe/GridShell/assets/106522950/3dc06ae8-b28f-44f4-9bfc-2c2ec2ab4950)



# :art: What is it made of ?

Noiseblaster is using two devices connected together with three wires.

This is all you need to measure noise levels in your surroundings.

1. ESP32 Mini

![image](https://github.com/invpe/GridShell/assets/106522950/c331718f-63d7-4223-bdf3-f12eba790483)

2. [DFRobot SKU SEN0232](https://wiki.dfrobot.com/Gravity__Analog_Sound_Level_Meter_SKU_SEN0232) 

![image](https://github.com/invpe/GridShell/assets/106522950/9fe0d047-b72c-452d-800d-63c8739f100b)

# :pizza: How do i make one ?

This is really simple, let's build one together.

1. Get ESP32 Mini and the DFRobot SEN0232 sensor.
2. Print out [the PLA case](https://github.com/invpe/GridShell/blob/main/Sources/Integrations/NoiseBlaster/NoiseBlaster.stl)
3. [Pull out the ESP32 Mini pinout](https://nettigo.eu/products/wifi-bluetooth-module-esp32-d1-mini)
4. Connect wires 

- Sensor GND to ESP GND
- Sensor Analog to ESP SVP
- Sensor VOLTAGE to ESP VCC

![image](https://github.com/invpe/GridShell/assets/106522950/1e885f54-ffdc-4c6b-921e-f35ddbd68ed0)


5. Create new sketch in arduino, name it **NoiseBlaster**
6. [Download GS Lib files](https://github.com/invpe/GridShell/tree/main/Sources/Node/ClientESP32) and place them in your new sketch folder
7. [Download NoiseBlaster.ino](https://github.com/invpe/GridShell/blob/main/Sources/Integrations/NoiseBlaster/NoiseBlaster.ino) and place in your new sketch folder

Your sketch folder files should look like this now:


![image](https://github.com/invpe/GridShell/assets/106522950/407767b4-6e76-4c08-bb92-0ba4a2cfef6e)




8. Connect ESP32 and upload the sketch
9. From now on, your Noiseblaster :

- Is part of the GridShell network
- Measures noise levels
- Contributes to the network executing scripts(tasks) and earning shells
- Is available to view over GridShell explorer grafana dashboards.

# :beer: Grab a beer, you're all set !
