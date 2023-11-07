# 🔠 Library of tasks

This is a library of tasks created for executing on the network.

With community contributions, we hope to expand this list of versatile tasks that can be easily utilized by anyone, eventually creating a comprehensive resource for a wide range of purposes.

# 👱‍♀️ 👷‍♂️ Contribute 

GridShell encourages contributions in all areas, but scripts are the essential components that drive its functionality. You are encouraged to submit your solutions here, and they will be thoroughly tested. Once your contributions have been successfully validated, they will be published, and you will receive due credit for your efforts.

Use [Playground](https://github.com/invpe/GridShell/tree/main/Sources/Playground) a simple web application to write and test scripts.

----

## mcpi - Calculate Pi with Monte Carlo simulation
Demonstrates the effectiveness of using GridShell for distributed computing tasks and simulations.

Input: `num_points,seed,`

Example: `10000,10,` Output: `count of points inside,total count`

Example: `10000,20,` Output: `count of points inside,total count`

Example: `10000,30,` Output: `count of points inside,total count`

Example: `10000,40,` Output: `count of points inside,total count`



Demo:

```
Task 217228 returned: 7801,10000
Task 217229 returned: 7814,10000
Task 217230 returned: 7799,10000
Task 217231 returned: 7821,10000
Task 217232 returned: 7790,10000
Task 217233 returned: 7783,10000
Task 217234 returned: 7822,10000
Task 217235 returned: 7797,10000
Task 217236 returned: 7781,10000
Task 217237 returned: 7781,10000
Task 217238 returned: 7837,10000
Task 217239 returned: 7767,10000
Task 217240 returned: 7787,10000
Task 217241 returned: 7831,10000
Task 217242 returned: 7807,10000
Task 217243 returned: 7783,10000
Task 217244 returned: 7752,10000
Task 217245 returned: 7753,10000
Task 217246 returned: 7783,10000
Task 217247 returned: 7760,10000
Task 217248 returned: 7801,10000
Task 217249 returned: 7773,10000
Task 217250 returned: 7805,10000
Task 217251 returned: 7784,10000
Task 217252 returned: 7758,10000
Task 217253 returned: 7829,10000
Task 217254 returned: 7839,10000
Task 217255 returned: 7772,10000
Task 217256 returned: 7832,10000
Task 217257 returned: 7794,10000
Task 217258 returned: 7813,10000
Task 217259 returned: 7799,10000
Task 217260 returned: 7837,10000
Task 217261 returned: 7878,10000
Task 217262 returned: 7789,10000
Task 217263 returned: 7887,10000
Task 217264 returned: 7837,10000
Task 217265 returned: 7771,10000
Task 217266 returned: 7880,10000
Task 217267 returned: 7760,10000
Task 217268 returned: 7821,10000
Task 217269 returned: 7857,10000
Task 217270 returned: 7761,10000
Task 217271 returned: 7789,10000
Task 217272 returned: 7806,10000
Task 217273 returned: 7818,10000
Task 217274 returned: 7808,10000
Task 217275 returned: 7784,10000
Task 217276 returned: 7784,10000
Task 217277 returned: 7829,10000
PI: 3.121152


```

## writedfs - Submit a telemetry to the network.

Input: `FILENAME,APPEND?,BASE64(TEXT_TO_WRITE),`

Example: `MyTelemetry,0,BASE64("Ovewrites the file\n"),`

Example: `MyTelemetry,1,BASE64("Appends the file\n"),`

## avgc - Average column

Input: `FILENAME,IDX,`

Example: `UsernameTelemetryfile,6,`

## kalmanc - Apply Kalman filter on column

Input: `FILENAME,IDX,`

Example: `UsernameTelemetryFile,6,`

## sha1 - Generate SHA1 from input

Input: `THIS_IS_A_TEXT`

Example: `Generate SHA1 from this message` 

## l2daily - Calculate IJP (Air Quality Index) for the last hour

Input: `TELEMETRY FILE`

Example: `JadeChartreuseDromiceiomimusLOOKO24c7525a25d82023109`

## widgeridoo - Geneate a simple JSON for use with [Widgeridoo](https://apps.apple.com/us/app/widgeridoo/id1531359008)

Input: `TITLE,VALUE,`

Example: `My Sensor, 123,`
