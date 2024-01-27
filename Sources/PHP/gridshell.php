<?php
$GLOBALS['lastpong'] = 0; 
$GLOBALS['grid_socket'] = 0; 
$GLOBALS['version'] = "09";
$GLOBALS['server'] = "work.gridshell.net";
$GLOBALS['port'] = 1911;
 
function SOCKET_IsOnline($socket) {  
    if (!is_resource($socket) || get_resource_type($socket) !== 'stream') {
        return false;
    }

    // Check if the end of the file has been reached
    if (feof($socket)) {        
        return false;
    }

    return true;    
}
function SOCKET_Disconnect($socket)
{
   if (!is_resource($socket) || get_resource_type($socket) !== 'stream') {
        return false;
    } 
    fclose($socket); 
    return true;
}
function GS_Login($uid, $nodeid = NULL) 
{ 
    // Autogenerate node id
    if (empty($nodeid))
        $nodeid = "PHPgnode" . mt_rand(1000, 9999);

    $context = stream_context_create(['ssl' => ['verify_peer' => false, 'verify_peer_name' => false]]);
    $GLOBALS['grid_socket'] = stream_socket_client('ssl://' . $GLOBALS['server'] . ':' . $GLOBALS['port'], $errno, $errstr, 30, STREAM_CLIENT_CONNECT, $context);

    if (!SOCKET_IsOnline($GLOBALS['grid_socket'])) {
        echo "[GS:Login] Connection failed: " . $errstr . "\n";
        return false;
    }

    // Welcome get one line (fgets)
    $a = fgets($GLOBALS['grid_socket']); 
    $welcomeString = explode(',', $a);   

    // Base64Encode
    $base64encodedUID = base64_encode($uid);
    $command = "JOB," . $base64encodedUID . "," . $GLOBALS['version'] . "," . $nodeid . "\r\n"; 
    fputs($GLOBALS['grid_socket'], $command);

    if (!SOCKET_IsOnline($GLOBALS['grid_socket'])) {
        echo "[GS:Login] Cant authorize\n";
        return false;
    }

    echo "[GS:Login] Login completed\n";
    return true;
}  
function GS_Burn($what)
{ 
    if (!SOCKET_IsOnline($GLOBALS['grid_socket'])) {        
        return false;
    } 
     fputs($GLOBALS['grid_socket'],"BURN,".$what."\r\n");
     return true;
} 
function GS_Send($amt,$receiver)
{ 
    if (!SOCKET_IsOnline($GLOBALS['grid_socket'])) {        
        return false;
    } 
     fputs($GLOBALS['grid_socket'],"SEND,".$receiver.",".$amt."\r\n");
     return true;
} 
function GS_GetTask($taskid)
{ 
    if (!SOCKET_IsOnline($GLOBALS['grid_socket'])) {        
        return false;
    } 
    fputs($GLOBALS['grid_socket'],"GETTASK,".$taskid."\r\n");
    $a = fgets($GLOBALS['grid_socket']);  
    $incoming = trim(preg_replace('/\s\s+/', ' ', $a)); 
    $returned = explode(',', $incoming);
    return $returned;
}  
function GS_Read($tname,$start,$count)
{ 
    if (!SOCKET_IsOnline($GLOBALS['grid_socket'])) {        
        return false;
    } 
    fputs($GLOBALS['grid_socket'],"READ,".$tname.",".$start.",".$count."\r\n");    
    $a = fgets($GLOBALS['grid_socket']);      
    $incoming = trim(preg_replace('/\s\s+/', ' ', $a)); 
    $returned = explode(',', $incoming);
    return $returned;
}
function GS_Persist($task,$flag,$fname)
{ 
    if (!SOCKET_IsOnline($GLOBALS['grid_socket'])) {        
        return false;
    } 
    fputs($GLOBALS['grid_socket'],"PERSIST,".$task.",".$flag.",".$fname."\r\n");    
    return NULL;
}  
function GS_Pong()
{ 
    if (!SOCKET_IsOnline($GLOBALS['grid_socket'])) {
        return false;
    } 
    if(time() - $GLOBALS['lastpong'] >= 10)
    {
        echo "[GS:Pong] Pong\r\n";
        fputs($GLOBALS['grid_socket'],"PONG\r\n");
        $GLOBALS['lastpong'] = time();

    }
    return true;
} 
function GS_AddTask($script, $payload) {
   if (!SOCKET_IsOnline($GLOBALS['grid_socket'])) {        
        return false;
    }

    fputs($GLOBALS['grid_socket'], "ADDT," . $script . "," . base64_encode($payload) . "\r\n");
    //echo "[GS:AddTask] Waiting for results:\n";

    $completeData = "";
    while (!feof($GLOBALS['grid_socket'])) {
        $incoming = fgets($GLOBALS['grid_socket']);
        if ($incoming === false) {
            break;
        }
        $completeData .= $incoming;
        // Check if the complete data contains the "ADDT" prefix
        if (strpos($completeData, "ADDT") !== false) {
            break;
        }
    }

    // Process the complete data from "ADDT" onwards
    $startIndex = strpos($completeData, "ADDT");
    if ($startIndex === false) {
        //echo "[GS:AddTask] 'ADDT' not found in response\n";
        return -1;
    }

    // Extract the relevant part of the data and process it
    $relevantData = substr($completeData, $startIndex);
    $relevantData = trim(preg_replace('/\s\s+/', ' ', $relevantData));
    $outputvalues = explode(',', $relevantData);
    //echo "[GS:AddTask] Received " . $relevantData . "\n";

    if (empty($outputvalues[1])) {
        return -1;
    }

    return $outputvalues;
}


function GS_Disconnect() {
    SOCKET_Disconnect($GLOBALS['grid_socket']);
}  
?>
