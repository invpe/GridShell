# PHP experimental GS library and command line client

# Create identification string (account on gridshell)
`php use.php GET IDENT NOW` - and follow the prompts

```
----------------------------------------
-- PHP GridShell command line tool 09 --
----------------------------------------
   https://github.com/invpe/GridShell   
----------------------------------------

Generating your private identification string...

Your PRIVATE identification string is: xxxxxxxxxxxxxxxxxxxxxxxxxxxxx

Few things before we take off:

1. The identification string - it is private do not share!
   Your nodes will use it to connect to GridShell, you will have to provide it during configuration.
2. Store it somewhere safe - there is no way to restore it!
3. The server will reply with a JSON response, containing your public username
   This username is visible on explorer page and can be shared

Ready when you are, hit ENTER to continue generating your account...
```

# Submit
`php use.php your_hash SUBMIT task payload` - To submit a task with payload

# Burn
`php use.php your_hash BURN TSLOT` - To burn for 1 Telemetry slot

`php use.php your_hash BURN TSIZE` - To burn for 1kb Telemetry size

# Send
`php use.php your_hash SEND 500 GRIDSHELLCoreValidators1`

# Submit and Persist
`php use.php your_hash SUBMITPERSIST task payload` - To submit and immediately flag for peristance

# Read 
`php use.php your_hash READ telemetry_filename start_bytes count_bytes` - To read telemetry file

# Persist
`php use.php your_hash PERSIST task_id` - To flag task for persistance

# Get task payload
`php use.php your_hash GETTASK task_id` - To retrieve last exec task payload
