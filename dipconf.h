#ifndef DIPCONF_H
#define DIPCONF_H

int dipState;
int dip2state;
int dip1state;

void dipSet()
{
    // Read the states of dip switches
    dip1state = digitalRead(dip1);
    dip2state = digitalRead(dip2);

    dipState = (dip2state << 1) | dip1state;
    Serial.println("=============================================");
    Serial.print("DIP 1: ");
    Serial.println(dip1state);
    Serial.print("DIP 2: ");
    Serial.println(dip2state);
    Serial.println(dipState);
    Serial.println("=============================================");
}

#endif // DIPCONF_H

//                      DIP switch configuration

//                      DIP 1 is for switching between old and new sensors
//                      DIP 2 is for switching between test mode and operation mode

//                      +-------+-------+------------------------------------+
//                      | DIP 1 | DIP 2 |              PROGRAM               |
//                      +-------+-------+------------------------------------+
//                      |   0   |   0   |    Testing mode with old sensors   | 
//                      +-------+-------+------------------------------------+
//                      |   0   |   1   |   Operation mod with old sensors   | 
//                      +-------+-------+------------------------------------+
//                      |   1   |   0   |    Testing mode with new sensors   | 
//                      +-------+-------+------------------------------------+
//                      |   1   |   1   |   Operation mode with new sensors  | 
//                      +-------+-------+------------------------------------+
