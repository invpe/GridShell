<?php
$GLOBALS['lastpong'] = 0; 
$GLOBALS['grid_socket'] = 0; 
$GLOBALS['version'] = "08";
$GLOBALS['server'] = "work.gridshell.net";
$GLOBALS['port'] = 1911;
 
function GS_Login($uid, $nodeid = NULL) { 
    // Autogenerate node id
    if (empty($nodeid))
        $nodeid = "PHPgnode" . mt_rand(1000, 9999);

    echo "[GS:Login] Login start " . $GLOBALS['server'] . ":" . $GLOBALS['port'] . " as " . $nodeid . "\n";

    $context = stream_context_create(['ssl' => ['verify_peer' => false, 'verify_peer_name' => false]]);
    $GLOBALS['grid_socket'] = stream_socket_client('ssl://' . $GLOBALS['server'] . ':' . $GLOBALS['port'], $errno, $errstr, 30, STREAM_CLIENT_CONNECT, $context);

    if (!$GLOBALS['grid_socket']) {
        echo "[GS] Connection failed: " . $errstr . "\n";
        return false;
    }

    // Welcome get one line (fgets)
    $a = fgets($GLOBALS['grid_socket']); 
    $welcomeString = explode(',', $a);
    

    // Base64Encode
    echo "[GS:Login] Sending command\n";
    $base64encodedUID = base64_encode($uid);
    $command = "JOB," . $base64encodedUID . "," . $GLOBALS['version'] . "," . $nodeid . "\r\n"; 
    fputs($GLOBALS['grid_socket'], $command);

    if (feof($GLOBALS['grid_socket'])) {
        echo "[GS:Login] Login socket closed, terminated\n";
        return false;
    }

    echo "[GS:Login] Login completed\n";
    return true;
}  
function GS_Burn($what)
{ 
     fputs($GLOBALS['grid_socket'],"BURN,".$what."\r\n");
} 
function GS_Send($amt,$receiver)
{ 
     fputs($GLOBALS['grid_socket'],"SEND,".$receiver.",".$amt."\r\n");
} 
function GS_Pong()
{ 
    echo "[GS:Pong]\n";
    if (feof($GLOBALS['grid_socket']) === true) 
    {
        echo "[GS:Pong] Pong failed on socket\n";
        return false;
    }
    if(time() - $GLOBALS['lastpong'] > 30)
    {
        echo "[GS:Pong] Pong\r\n";
        fputs($GLOBALS['grid_socket'],"PONG\r\n");
        $GLOBALS['lastpong'] = time();
    }
} 
function GS_GetStateString($state) {
    if ($state == 0) return "New";
    if ($state == 1) return "Running";
    if ($state == 2) return "Failed";
    if ($state == 3) return "Completed";
    if ($state == 4) return "Validated";
}

function GS_AddTask($script, $payload) {
    echo "[GS:AddTask] " . $script . " " . $payload . "\n";

    if (feof($GLOBALS['grid_socket'])) {
        echo "[GS:AddTask] AddTask failed on socket\n";
        return false;
    }

    fputs($GLOBALS['grid_socket'], "ADDT," . $script . "," . base64_encode($payload) . "\r\n");
    $incoming = "";
    $outputvalues = "";
    echo "[GS:AddTask] Waiting for results:\n"; 
    $incoming = fgets($GLOBALS['grid_socket']);   
    $incoming = trim(preg_replace('/\s\s+/', ' ', $incoming));
    $outputvalues = explode(',', $incoming);  
    echo "[GS:AddTask] Received " . $incoming . "\n";
    
    if (empty($outputvalues[1]))
        return -1;

    return $outputvalues[1];
} 

function GS_Disconnect() {
    echo "[GS:Disconnect] Disconnected\n";
    fclose($GLOBALS['grid_socket']);
}  
?>
