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
    echo "<YOURHASH> BURN <TSLOT>\n"; 
}

function PrintExit()
{
    echo "Done, if all criteria met you can see your execution in the user dashboard on explorer endpoint.\n";
    echo "Head to: https://explorer.gridshell.net:3000/d/cd4c7abe-f622-4384-8a6d-348f3bffde47/user?orgId=1\n";
    echo "To track the exeuction of the command\n";
}

system("clear");
echo "----------------------------------------\n";
echo "-- PHP GridShell command line tool 08 --\n";
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
    }
}
?>
