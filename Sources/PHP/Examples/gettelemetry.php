<?php
/*
  This script will return a given telemetry filename from the GridShell server
  simply http://yourserver/gettelemetry.php?file=abcdefghijk

  It will read the telemetry file and return it decoded from b64
*/
include "gridshell.php";
$file=$_GET['file'];

$output = "";
define("CHUNK_SIZE", 1024); 
header('Content-Type: text/plain');


    if(GS_Login("xxxxxxxx"))
    {
          	$offset = 0;  
		    $continueReading = true;  

		    while ($continueReading) { 
		        $output = base64_decode(GS_Read($file, $offset, CHUNK_SIZE)[1]);

		        $bytesRead = strlen( $output ) ;

		        if ($bytesRead == 0) { 
		            $continueReading = false;
		        } else { 
		            echo $output; 
		            $offset += $bytesRead;
		        }
		    }
    }
GS_Disconnect(); 
?>
