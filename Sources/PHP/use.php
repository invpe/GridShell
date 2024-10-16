<?php
/*
PHP GridShell command line tool
Important: this tool has no checks, and serves as an dirty example.
Ensure to provide proper values to not lock yourself out (throttle)
But hey! You can improve it if you wish :-)
*/

include "gridshell.php";

function PrintHelp()
{
    echo "Use with following options:\n\n";
    echo "------------[NEW SETUP]------------\n";    
    echo "GET IDENT NOW - create yourself an account\n\n";

    echo "------------[ALREADY IN]-----------\n";
    echo "<YOURHASH> SEND <AMOUNT> <RECEIPENT> - send shells\n";
    echo "<YOURHASH> BURN <TSLOT/TSIZE> - burn shells\n"; 
    echo "<YOURHASH> SUBMIT <task> <payload> - submit task for execution\n";       
    echo "<YOURHASH> GETTASK <task> - get task status and payload\n";       
    echo "<YOURHASH> READ <telemetryname> <start> <count> - read telemetry\n";   
    echo "<YOURHASH> UPLOAD <filename> <telemetryname>\n";
    echo "\n";
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
echo "----------------------------------------\n\n";

if($argc < 3)
{
    PrintHelp();
    exit(0);  
}


$grid_owner = $argv[1]; 
$command = $argv[2];

if($command=="IDENT")
{
    echo "Generating your private identification string...\n\n";

    // Generate random bytes
    $randomBytes = openssl_random_pseudo_bytes(5024);  

    // Generate SHA256 hash
    $sha256String = hash('sha256', $randomBytes);
    
    //
    echo "Your PRIVATE identification string is: ".$sha256String."\n";
    echo "\n";
 
    // Prompt the user
    echo "Few things before we take off:\n\n";
    echo "1. The identification string - it is private do not share!\n";
    echo "   Your nodes will use it to connect to GridShell, you will have to provide it during configuration.\n";
    echo "2. Store it somewhere safe - there is no way to restore it!\n";    
    echo "3. The server will reply with a JSON response, containing your public username\n";    
    echo "   This username is visible on explorer page and can be shared\n";
    echo "\n";
    echo "Ready when you are, hit ENTER to continue generating your account...";
    

    // Read input from the user
    fgets(STDIN);

    GS_Login($sha256String);

    
}
else if($command=="SEND")
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
            $pd=$receipent.",".$amount;
            print_r(GS_AddTask("send",$pd));     
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
        echo "Burn shells for ".$argument."\n";
        if(GS_Login($grid_owner))
        { 
            $pd=$argument;
            print_r(GS_AddTask("burn",$pd));     
            GS_Disconnect();
            PrintExit();
        } else echo "Something went wrong, throttled ?\n";
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

}
else if($command=="UPLOAD")
{
    if($argc != 5)
    {
        PrintHelp();
        exit(0);
    }
    else
    { 
        echo "Submitting: ".$argv[3]."\n";
        echo "Will store as: ".$argv[4]."\n";
        
        if(GS_Login($grid_owner))
        { 
            $contents = file_get_contents($argv[3]);
            $chunkSize = 512;
            $chunks = str_split($contents, $chunkSize);

            echo "Sending first part.\n";
            $output=GS_AddTask("writedfs",$argv[4].",0,".base64_encode($chunks[0]).",");

            for($i = 1; $i < sizeof($chunks); $i++)
            {
                echo "Sending ".$i." out of ".sizeof($chunks)."\n";
                GS_AddTask("writedfs",$argv[4].",1,".base64_encode($chunks[$i]).",");
                sleep(1);
            }

        }else echo "Something went wrong, throttled ?\n";
    }
}
GS_Disconnect();
?>
