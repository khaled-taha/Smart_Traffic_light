#include "traffic_private.h"
#include "traffic_interface.h"

extern TrafficUnit traffic_unit[4] ;

void TRFC_vInit(void)
{
  PT_INIT(&asynchUpdateStatus);
  uint8_t countingPins[8] = {
    PIN_T1_COUNT_UP, PIN_T1_COUNT_DOWN,
    PIN_T2_COUNT_UP, PIN_T2_COUNT_DOWN,
    PIN_T3_COUNT_UP, PIN_T3_COUNT_DOWN,
    PIN_T4_COUNT_UP, PIN_T4_COUNT_DOWN
  };

  for (int i = 0 ; i < 4 ; i++)
  {
    String UnitName [4] = {NAME_NORTH, NAME_EAST, NAME_SOUTH, NAME_WEST };
    traffic_unit[i].TrafficName = "Traffic Unit "+ UnitName[i];
    traffic_unit[i].CountUpPin   = countingPins[2*i+0];
    traffic_unit[i].CountDownPin = countingPins[2*i+1];

    pinMode(traffic_unit[i].CountUpPin  , INPUT_PULLUP);
    pinMode(traffic_unit[i].CountDownPin, INPUT_PULLUP); 

    Serial.print("Initialized Unit: "+ String(i));

    Serial.println("CountUpPin: "  + String(traffic_unit[i].CountUpPin  ) );
    Serial.println("CountDownPin: "+ String(traffic_unit[i].CountDownPin) );
  }
  
}

void TRFC_vUpdate (uint8_t unit, uint8_t mode)
{
  if (((unit <= ID_WEST)) && ((mode <= MODE_PASS)))
  {
    Serial.println("Update Unit"+String(unit)+", By Mode");
    switch (mode){
      case MODE_STOP    : Serial.println(" STOP");  traffic_unit[unit].mode.pattern = PATTERN_STOP;  break;      
      case MODE_WAIT    : Serial.println(" WAIT");  traffic_unit[unit].mode.pattern = PATTERN_WAIT;  break;      
      case MODE_PASS    : Serial.println(" PASS");  traffic_unit[unit].mode.pattern = PATTERN_PASS;  break;      
    }
  }
  else 
  {
    Serial.println("Error trafic unit ID or MODE");
    Serial.print("MODE:"+String(mode)); Serial.print(" ID:"+String(unit));
  }
}

void TRFC_vUpdateStatus (void)
{
  TRFC_vUpdateDensityStatus();
}

void TRFC_vUpdateEmergencyStatus (struct pt* pt)
{
  PT_BEGIN(pt);
  while (1){
    while ( Serial.available() != 0 )
    {
      String readingInput = Serial.readString();
      readingInput.trim();
      uint8_t target = readingInput.charAt(0) - '0' ;
      uint8_t status = readingInput.charAt(1) - '0' ;
      if (readingInput != CHECK_SYSTEM)
      {
        Serial.println("Emergency for: "+readingInput);
        Serial.println("Target: "+String(target)+"\tStatus: "+String(status));
        
        switch (target)
        {
          case ID_NORTH : traffic_unit[ID_NORTH].EmergencyStatus = (status)?(EMERGENCY_ON): (EMERGENCY_OFF);  Serial.println("E0"); break; 
          case ID_EAST  : traffic_unit[ID_EAST ].EmergencyStatus = (status)?(EMERGENCY_ON): (EMERGENCY_OFF);  Serial.println("E1"); break;
          case ID_SOUTH : traffic_unit[ID_SOUTH].EmergencyStatus = (status)?(EMERGENCY_ON): (EMERGENCY_OFF);  Serial.println("E2"); break;
          case ID_WEST  : traffic_unit[ID_WEST ].EmergencyStatus = (status)?(EMERGENCY_ON): (EMERGENCY_OFF);  Serial.println("E3"); break;
          default: Serial.println("Not Valid input "+ readingInput); break;
        }
      }
      else 
      {
        TRFC_vGetCurrentSystemState();
      }
    }

  }  
  PT_END(pt);
}

void TRFC_vUpdateDensityStatus (void)
{
  for (int i = 0 ; i < 4 ; i++)
  {
    if (traffic_unit[i].VehicleCounter >= DENSITY_THRESHOLD )
    {
      traffic_unit[i].DensityStatus = DENSITY_ON;
    }
    else 
    {
      traffic_unit[i].DensityStatus = DENSITY_OFF;
    }
  }

}

void TRFC_vGetCurrentSystemState (void)
{
  for (int i = 0 ; i < 4 ; i++)
  {
    uint8_t currnetMode = traffic_unit[i].mode.pattern;
    Serial.println("---------------------------------");
    Serial.println("Unit: "+ String(i));
    Serial.println("Unit Name: "+ traffic_unit[i].TrafficName);
    Serial.println("Count Up Pin: "+ String(traffic_unit[i].CountUpPin));
    Serial.println("Count Down Pin: "+ String(traffic_unit[i].CountDownPin));
    Serial.println("Idle State: "+ String(traffic_unit[i].IdleStatus));
    Serial.println("Emergency State: "+ String(traffic_unit[i].EmergencyStatus));
    Serial.println("Dense State: "+ String(traffic_unit[i].DensityStatus));
    Serial.println("Total Vehicles: "+ String(traffic_unit[i].VehicleCounter));
    Serial.println("Unit Current State: "); 
    Serial.println((currnetMode == PATTERN_STOP)?("RED"):( (currnetMode == PATTERN_WAIT )?("YELLOW"):("GREEN") ));
    Serial.println("---------------------------------");
  }
}