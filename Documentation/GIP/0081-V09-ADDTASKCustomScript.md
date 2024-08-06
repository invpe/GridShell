GIP 0072 - Allow for custom scripts execution

    Author(s): gridshell
    Date: 06/08/2024


# Synopsis
Allow custom scripts to be executed on the network by simply providing the script source and payload.

# Rationale
This update enables scripts to be executed effortlessly, allowing users to submit their scripts and payloads directly. This ensures absolute simplicity and ease of use within the network.

# Explanation in details

Simply use the php client to submit your work: 

`php use.php YOUR_ID SUBMIT BASE64(SCRIPT_SOURCE_CODE) PLAIN_TEXT_INPUT_PAYLOAD`

# Examples

Example script:

```
OUTPUTPAYLOAD=INPUTPAYLOAD
```

Example input payload:

```
THIS_IS_JUST_A_TEST
```

Example distributed execution with GridShell:

```
$ php use.php HASH SUBMIT T1VUUFVUUEFZTE9BRD1JTlBVVFBBWUxPQUQ THIS_IS_JUST_A_TEST

----------------------------------------
-- PHP GridShell command line tool 09 --
----------------------------------------
   https://github.com/invpe/GridShell   
----------------------------------------

Task submission
[GSHELL] Timeout: No confirmation received within 1 second.
Array
(
    [0] => ADDT
    [1] => 3573
)
Done
```

Continue the usual way to obtain the ouptut

```
----------------------------------------
-- PHP GridShell command line tool 09 --
----------------------------------------
   https://github.com/invpe/GridShell   
----------------------------------------

Getting Task 3573
[GSHELL] Timeout: No confirmation received within 1 second.
Array
(
    [0] => GETTASK
    [1] => ewoiSUQiOiAzNTczLAoiT3duZXIiOiAiSmFkZUNoYXJ0cmV1c2VEcm9taWNlaW9taW11cyIsCiJTY3JpcHQiOiAiNWRiMzQ5ZWY1MjEwM2Y2MTMzNDRmZGU2YzU2MmExZmYzNjBkZGNkM2FiNzFkNzhjOGMyYzExMDIyZjNhMThiZCIsCiJJbnB1dCI6ICJWRWhKVTE5SlUxOUtWVk5VWDBGZlZFVlRWQT09IiwKIkZhaWxzIjogMCwKIlJld2FyZCI6IDEsCiJQZXJzaXN0IjogIiIsCiJSZXN0YXJ0cyI6IDAsCiJTdGF0ZSI6IDQsCiJDcmVhdGVkIjogMTcyMjk0MDI2NiwKIkV4ZWNzIjogMiwKIlN0YXJ0IjogMTcyMjk0MDI2NiwKIkVuZCI6IDE3MjI5NDAyNjYsCiJFeGVjVXNlciI6ICJKYWRlQ2hhcnRyZXVzZURyb21pY2Vpb21pbXVzIiwKIkV4ZWNQYXlsb2FkIjogIlZFaEpVMTlKVTE5S1ZWTlVYMEZmVkVWVFZBPT0iCn0=
)
Done
```

Now simply b64decode the output of the `GETTASK` 

```
{
"ID": 3573,
"Owner": "JadeChartreuseDromiceiomimus",
"Script": "5db349ef52103f613344fde6c562a1ff360ddcd3ab71d78c8c2c11022f3a18bd",
"Input": "VEhJU19JU19KVVNUX0FfVEVTVA==",
"Fails": 0,
"Reward": 1,
"Persist": "",
"Restarts": 0,
"State": 4,
"Created": 1722940266,
"Execs": 2,
"Start": 1722940266,
"End": 1722940266,
"ExecUser": "JadeChartreuseDromiceiomimus",
"ExecPayload": "VEhJU19JU19KVVNUX0FfVEVTVA=="
}

```

