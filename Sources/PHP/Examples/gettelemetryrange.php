<?php
/*
  This script will return a single data from multiple telemetry files as given by date From-To.
  Good for using in Grafana, when given from/to arguments: i.e: https:/server/gettelemetryrange.php?file=${User}&from=${__from:date:YYYYMMDD}&to=${__to:date:YYYYMMDD}
*/
include "gridshell.php";

header('Content-Type: text/plain');
//
define("CHUNK_SIZE", 1024); 

$output = "";

// Assuming these values come from $_GET
$file = $_GET['file']; // The file prefix
$from = $_GET['from']; // Start date in YYYYMMD format
$to = $_GET['to']; // End date in YYYYMMD format 


// Convert $from and $to into DateTime objects
$fromDate = DateTime::createFromFormat('Ymd', $from);
$toDate = DateTime::createFromFormat('Ymd', $to);

if (!$fromDate || !$toDate) {
    die("Invalid dates provided. Ensure format is YYYYMMDD.");
}

// Create a DateInterval of one day
$interval = new DateInterval('P1D');

// Create a DatePeriod to iterate from $from to $to, inclusive
$period = new DatePeriod($fromDate, $interval, $toDate->modify('+1 day'));

if(GS_Login("xxxxxxxxxxxxx"))
{
	foreach ($period as $date) 
	{
	    // Format the date as YYYYMD for file operations
	    $newFileName = $file . $date->format('Ynj');
	  
	    $offset = 0;  
	    $continueReading = true;  

	    while ($continueReading) { 
	        $output = base64_decode(GS_Read($newFileName, $offset, CHUNK_SIZE)[1]);

	        $bytesRead = strlen( $output ) ;

	        if ($bytesRead == 0) { 
	            $continueReading = false;
	        } else { 
	            echo $output; 
	            $offset += $bytesRead;
	        }
	    }  
	}
}
GS_Disconnect();


?>
