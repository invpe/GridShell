# Your first sketch

Simply run a task on the network every 60 seconds.

![image](https://github.com/user-attachments/assets/79a203dd-8524-47db-b9e6-a42883ed6575)

```
#include <WiFi.h>
#include "CGridShell.h"
uint64_t uiLastTaskSubmitted = 0;
uint32_t uiLastTaskID = 0;
void setup() {
  CGridShell::GetInstance().Init("Your Grid Hash Goes here", false);
  WiFi.begin("YOUR_INTERNET", "YOUR_PASSWORD");
  while (WiFi.status() != WL_CONNECTED) { delay(1000); }
}

void loop() {
  CGridShell::GetInstance().Tick();
  if (millis() - uiLastTaskSubmitted >= 60000) {
    uiLastTaskID = CGridShell::GetInstance().AddTask("OUTPUTPAYLOAD=\"Hello From \"+INPUTPAYLOAD", "GridShell");
    uiLastTaskSubmitted = millis();
  }
}
```

# Your second sketch

Run and obtain the output from the task

![image](https://github.com/user-attachments/assets/b24f1b07-b244-4670-b2fc-fbf4ba678e78)

```
#include <WiFi.h>
#include "CGridShell.h"
uint64_t uiLastTaskSubmitted = 0;
uint32_t uiLastTaskID = 0;
void setup() {
  CGridShell::GetInstance().Init("Your Grid Hash Goes here", false);
  WiFi.begin("YOUR_INTERNET", "YOUR_PASSWORD");
  while (WiFi.status() != WL_CONNECTED) { delay(1000); }
}

void loop() {
  CGridShell::GetInstance().Tick();
  if (millis() - uiLastTaskSubmitted >= 60000) {

    uiLastTaskID = CGridShell::GetInstance().AddTask("OUTPUTPAYLOAD=\"Hello From \"+INPUTPAYLOAD", "GridShell");

    if (uiLastTaskID > 0) {
      String strPayload = CGridShell::GetInstance().GetTask(uiLastTaskID);
    }

    uiLastTaskSubmitted = millis();
  }
}
```
