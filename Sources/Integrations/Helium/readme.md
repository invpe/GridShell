![image](https://github.com/invpe/GridShell/assets/106522950/522e9be4-492e-47dc-8da2-0849773ba24f)

# Integrating with [Helium network](https://www.helium.com/)
"_Helium Network, often referred to as simply "Helium," is a decentralized wireless network designed for the Internet of Things (IoT) and low-power device connectivity. It is based on a unique blockchain-based incentive mechanism that allows individuals and businesses to deploy and operate low-power, long-range wireless hotspots called Helium Hotspots. These hotspots provide coverage for IoT devices in a given area and earn cryptocurrency rewards in the form of HNT (Helium Network Token) for participating in the network._"



# Sending via LoRA to Helium and pushing to GridShell

We will store the sensors telemetry on the GridShell network by sending them to Helium and using integrating (over HTTP) with the GridShell network.
Here are some basic assumptions:

1. For this example we will use [Lora Environmental sensor](https://www.rakwireless.com/en-us/products/wisblock)
2. We will have access to [Helium Console](https://console.helium.com/)
3. We will have a web server running that will host [PHP](https://github.com/invpe/GridShell/tree/main/Sources/PHP) experimental library
4. We will have [account created and some shells for use on GridShell](https://github.com/invpe/gridshell/blob/main/Documentation/Tutorials/Use.md)

> :warning: **This is an example**: does not provide any security related checks

# Host a web server

Spin up the web sever with the latest PHP GridShell experimental script.
You will need two files, [gridshell.php](https://github.com/invpe/GridShell/blob/main/Sources/PHP/gridshell.php) and `writedfs.php` hosted.

`writedfs` will be used by Helium to provide your web server with arguments which will be passed to the GridShell.
Here is the simple version of this script, feel free to modify it to suit your needs.

```
<?php
header('Content-Type: text/plain');
include "gridshell.php";
$guid=$_GET['guid'];
$append=$_GET['append'];
$fname=$_GET['file'];
$text=$_GET['text'];

$taskpayload = $fname.",".$append.",".base64_encode($text).",";

 if( GS_Login($guid) )
 {     
    $taskid = GS_AddTask("writedfs",  $taskpayload);  
    $taskArray[]=$taskid;
 }
 GS_Disconnect();

?>
```



# Configure Helium Console

## Decoder

```
function Decoder(bytes, port) {
var decoded = {};
decoded.temperature = (bytes[1] << 8 | (bytes[2])) / 100
decoded.humidity = (bytes[3] << 8 | (bytes[4])) / 100
decoded.pressure = (bytes[8] | (bytes[7] << 8) | (bytes[6] << 16) | (bytes[5] << 24)) /100
decoded.gas = bytes[12] | (bytes[11] << 8) | (bytes[10] << 16) | (bytes[9] << 24)
var decodedPayload=decoded.temperature+","+decoded.humidity+","+decoded.pressure+","+decoded.gas+"\n"
return decodedPayload
}
```

## HTTP

Now add a http GET connection for your integration in helium console.

![image](https://github.com/invpe/GridShell/assets/106522950/f2d3c590-95ec-4837-9b3e-f78899161777)

