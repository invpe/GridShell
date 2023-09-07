<?php
/*
    Experimental Gridshell PHP API (c) GRIDSHELL.NET 2022-2023
    https://github.com/invpe/GridShell
*/

$GLOBALS['lastpong'] = 0; 
$GLOBALS['grid_socket'] = 0; 
$GLOBALS['version']="06";

function xor_this($string,$key) {
 
    // Our plaintext/ciphertext
    $text = $string;

    // Our output text
    $outText = '';

    // Iterate through each character
    for($i=0; $i<strlen($text); )
        for($j=0; ($j<strlen($key) && $i<strlen($text)); $j++,$i++)
            $outText .= $text[$i] ^ $key[$j];
    return $outText;
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
function GS_Login($uid,$nodeid=NULL)
{

    $server="work.gridshell.net";
    $port=1911;

    // Autogenerate node id
    if(empty($nodeid))
        $nodeid="PHPgnode".mt_rand(1000,9999);

    echo "[GS:Login] Login start ".$server.":".$port." as ".$nodeid."\n";
    $GLOBALS['grid_socket'] = fsockopen($server,$port);
 
    if (feof($GLOBALS['grid_socket']) === true) 
    {
        echo "[GS] Login socket closed\n";
        return false;
    }

    // Welcome get one line (fgets)
    $a = fgets($GLOBALS['grid_socket']); 
    $welcomeString =  str_replace("\n","",$a);  

    // Diffie Hellman line by line
    $b = fgets($GLOBALS['grid_socket']);   
    $serverPublicKey =  str_replace("\n","",$b); 

    // These are temporary values for TestNet
    $serverG = "2";
    $serverP = "9840485683654561415963922255243388377177431468711912621027980528684674331318089597310841532159423071472940950709936601452503154610443618922381114939628259";

    // Calculate our private key
    $privatekey = rand()%PHP_INT_MAX; 

    // Calculate our public key (bcmath required)
    $publickey  = bcpowmod($serverG, $privatekey, $serverP); 

    // Compute symmetric (secret) key
    $key = bcpowmod($serverPublicKey, $privatekey, $serverP);  

    // SHA1
    $sha1hashkey = sha1($key); 

    // XOR
    $cipher = xor_this($uid, $sha1hashkey);

    // Base64Encode
    echo "[GS:Login] Sending command\n";
    $base64encodedUID = base64_encode($cipher);
    $command = "JOB,".$publickey.",".$base64encodedUID.",".$GLOBALS['version'].",".$nodeid.",LINUX64,0\r\n"; 
    fputs($GLOBALS['grid_socket'],$command);


    if (feof($GLOBALS['grid_socket']) === true)
    {
        echo "[GS:Login] Login socket closed, terminated\n";
        return false;
    }

    echo "[GS:Login] Login completed\n";
    return true;
} 
function GS_GetStateString($state)
{
    if($state == 0)return "New";
    if($state == 1)return "Running";
    if($state == 2)return "Failed";
    if($state == 3)return "Completed";
    if($state == 4)return "Validated";
}
function GS_AddTask($script,$payload)
{
    echo "[GS:AddTask] ".$script." ".$payload."\n";

    if (feof($GLOBALS['grid_socket']) === true)
    {
        echo "[GS:AddTask] AddTask failed on socket\n";
        return false;
    }
    fputs($GLOBALS['grid_socket'],"ADDT,".$script.",".base64_encode($payload)."\r\n");
    $incoming = "";
    $outputvalues = "";

    echo "[GS:AddTask] Waiting for results:\n"; 
    $incoming = fgets($GLOBALS['grid_socket']);   
    $incoming = trim(preg_replace('/\s\s+/', ' ', $incoming));
    $outputvalues = explode(',', $incoming);  
    echo "[GS:AddTask] Received ".$incoming."\n";
    return $outputvalues[1];
} 
function GS_IsOnline()
{
    if(feof($GLOBALS['grid_socket']))
    {
    echo "[GS:IsOnline] Socket is offline\n";
     return false;
    }
    echo "[GS:IsOnline] Socket is online\n";
    return true;
}
function GS_Disconnect()
{
    echo "[GS:Disconnect] Disconnected\n";
    fclose($GLOBALS['grid_socket']);
}  
?>
