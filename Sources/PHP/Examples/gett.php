<?php
/*
  This script will obtain task details ($task) from the server and return json
  ex. http://yourserver/gett.php?task=12345

  No errors checking!
*/
include "gridshell.php";
$task=$_GET['task'];
$output = "";

    if(GS_Login("xxxxxxxxxxxxxxxxxxxxxxxx"))
    {
        $output=GS_GetTask($task); 
    }

GS_Disconnect();
print_r(base64_decode($output[1]));
?>
