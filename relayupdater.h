#ifndef RELAYUPDATER_H
#define RELAYUPDATER_H
#include "config.h"
FirebaseData fbdo;

int rel2Status;
int rel1Status;

void relayStatus()
{
  if (Firebase.RTDB.getInt(&fbdo, "LiveData/relaySW/rel1"))
  {
    if (fbdo.dataType() == "int")
    {
      rel1Status = fbdo.intData();
      digitalWrite(relaySW1, !rel1Status);  
    }
    else{
      Serial.println("FAILED: " + fbdo.errorReason());
    }
  }
  delay(100);
  if (Firebase.RTDB.getInt(&fbdo, "LiveData/relaySW/rel2"))
  {
    if (fbdo.dataType() == "int")
    {
      rel2Status = fbdo.intData();
      digitalWrite(relaySW2, !rel2Status);  
    }
    else{
      Serial.println("FAILED: " + fbdo.errorReason());
    }
  }
}

#endif // RELAYUPDATER_H
