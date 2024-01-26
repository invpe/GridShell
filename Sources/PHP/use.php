<?php
/*
PHP GridShell command line tool
Important: this tool has no checks, and serves as an dirty example.
Ensure to provide proper values to not lock yourself out (throttle)
*/

include "gridshell.php";

function PrintHelp()
{
    echo "Use with following options:\n";
    echo "<YOURHASH> SEND <AMOUNT> <RECEIPENT>\n";
    echo "<YOURHASH> BURN <TSLOT/TSIZE>\n"; 
    echo "<YOURHASH> SUBMIT <task> <payload>\n";       
    echo "<YOURHASH> GETTASK <task>\n";       
    echo "<YOURHASH> READ <telemetryname> <start> <count>\n";   
    echo "<YOURHASH> PERSIST <task> <append?>\n";
    echo "<YOURHASH> SUBMITPERSIST <task> <payload> <append?>\n";
}

function PrintExit()
{
    echo "Done\n";
}

system("clear");
echo "----------------------------------------\n";
echo "-- PHP GridShell command line tool ".$GLOBALS['version']." --\n";
echo "----------------------------------------\n";
echo "   https://github.com/invpe/GridShell   \n";
echo "----------------------------------------\n";

if($argc < 3)
{
    PrintHelp();
    exit(0);  
}


$grid_owner = $argv[1];
$command = $argv[2];
 
if($command=="SEND")
{
    if($argc != 5)
    {
        PrintHelp();
        exit(0);
    }
    else
    {
        $amount = $argv[3];
        $receipent = $argv[4];

        echo "Transfer ".$amount." to ".$receipent."\n";
       
        if(GS_Login($grid_owner))
        { 
            GS_Send($amount, $receipent); 
            GS_Disconnect();
            PrintExit();
        } else echo "Something went wrong, throttled ?\n";
    }
}
else if($command=="BURN")
{
    if($argc != 4)
    {
        PrintHelp();
        exit(0);
    }
    else
    {
        $argument = $argv[3];
        if($argument=="TSLOT")
        {
            echo "Burn requested for 1 Telemetry Slot\n";
            if(GS_Login($grid_owner))
            { 
                GS_Burn("TSLOT"); 
                GS_Disconnect();
                PrintExit();
            } else echo "Something went wrong, throttled ?\n";
        }
        else if($argument=="TSIZE")
        {
            echo "Burn requested for 1k Telemetry Size\n";
            if(GS_Login($grid_owner))
            { 
                GS_Burn("TSIZE"); 
                GS_Disconnect();
                PrintExit();
            } else echo "Something went wrong, throttled ?\n";
        }
    }
}
else if($command=="SUBMIT")
{
    if($argc != 5)
    {
        PrintHelp();
        exit(0);
    }

    echo "Task submission\n";
    if(GS_Login($grid_owner))
    { 
        print_r(GS_AddTask($argv[3],$argv[4]));
        
        GS_Disconnect();
        PrintExit();
    } else echo "Something went wrong, throttled ?\n";  
    
}else if($command=="READ")
{
    if($argc != 6)
    {
        PrintHelp();
        exit(0);
    }
    
    echo "Reading ".$argv[3]." starting at ".$argv[4]." to ".$argv[5]." bytes\n";
    if(GS_Login($grid_owner))
    { 
        $output=GS_Read($argv[3],$argv[4],$argv[5]);
        print_r($output);
        GS_Disconnect();
        PrintExit();
    } else echo "Something went wrong, throttled ?\n";  

} 
// Data comes base64encoded
else if($command=="GETTASK")
{
    if($argc != 4)
    {
        PrintHelp();
        exit(0);
    }
    
    echo "Getting Task ".$argv[3]."\n";    
    
    if(GS_Login($grid_owner))
    { 
        $output=GS_GetTask($argv[3]);
        print_r($output);
        GS_Disconnect();
        PrintExit();
    } else echo "Something went wrong, throttled ?\n";  

}else if($command=="PERSIST")
{
    if($argc != 5)
    {
        PrintHelp();
        exit(0);
    }
    else
    {
        $t = $argv[3];
        $receipent = $argv[4];

        echo "Set persist flag ".$argv[4]." on ".$argv[3]."\n";
       
        if(GS_Login($grid_owner))
        { 
            GS_Persist($argv[3], $argv[4]);
            GS_Disconnect();
            PrintExit();
        } else echo "Something went wrong, throttled ?\n";
    }
}
else if($command=="SUBMITPERSIST")
{
    if($argc != 6)
    {
        PrintHelp();
        exit(0);
    }
    else
    { 
        if(GS_Login($grid_owner))
        { 
            $ooo = GS_AddTask($argv[3],$argv[4]);
            GS_Persist($ooo['1'],$argv[5]);
 
            GS_Disconnect();
            PrintExit();
        } else echo "Something went wrong, throttled ?\n";
    }
}
GS_Disconnect();
?>
