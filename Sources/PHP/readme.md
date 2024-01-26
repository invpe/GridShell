# PHP experimental GS library

> [!NOTE]  
> This is not frequently maintained/updated code, serves as an experiment for several integrations.

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
