#include "traffic_private.h"
#include "traffic_interface.h"

extern RoadTrafficUnit road_traffic_unit[4] ;
extern TrafficUnit     turn_traffic_unit[4] ;
static uint8_t TUModes[8] = {MODE_INIT, MODE_INIT, MODE_INIT, MODE_INIT, MODE_INIT, MODE_INIT, MODE_INIT, MODE_INIT};

void TRFC_vInit(void)
{
  uint8_t countingPins[8] = {
    PIN_T1_COUNT_UP, PIN_T1_COUNT_DOWN,
    PIN_T2_COUNT_UP, PIN_T2_COUNT_DOWN,
    PIN_T3_COUNT_UP, PIN_T3_COUNT_DOWN,
    PIN_T4_COUNT_UP, PIN_T4_COUNT_DOWN
  };

  String UnitName[8] = { NAME_NS, NAME_EW, NAME_SN, NAME_WE, NAME_SW, NAME_NE, NAME_ES, NAME_WN };

  
  for (int i = 0 ; i < 4 ; i++)
  {
    road_traffic_unit[i].TrafficLights.TrafficName = "Road Traffic Unit "+ UnitName[i]; 
    turn_traffic_unit[i].TrafficName = "Turn Traffic Unit "+ UnitName[4+i];
    
    TRFC_vUpdate(i, MODE_INIT);
    TRFC_vUpdate(i+4, MODE_INIT);
    
    Serial.println("Unit: "+String(i)+", Mode Init: "+TRFC_u8GetMode(i));
    Serial.println("Unit: "+String(i+4)+", Mode Init: "+TRFC_u8GetMode(i+4));

    road_traffic_unit[i].CountUpPin   = countingPins[(2*i)+0];
    road_traffic_unit[i].CountDownPin = countingPins[(2*i)+1];

    pinMode(road_traffic_unit[i].CountUpPin  , INPUT_PULLUP);
    pinMode(road_traffic_unit[i].CountDownPin, INPUT_PULLUP); 

    Serial.println("Initialized Unit: "+ String(i));

    Serial.println("CountUpPin: "  + String(road_traffic_unit[i].CountUpPin  ) );
    Serial.println("CountDownPin: "+ String(road_traffic_unit[i].CountDownPin) );
  }
  
  PT_INIT(&asynchUpdateStatus);
  PT_INIT(&asynchUpdateDensity);
}

void TRFC_vUpdate (uint8_t unit, uint8_t mode)
{
  if (((unit <= ID_WN )) && ((mode <= MODE_PASS)))
  {
    Serial.print("Update Unit"+String(unit)+", By Mode ");
    switch (mode){
      case MODE_STOP    : 
      {
        Serial.print("STOP: ");  
        TUModes[unit] = PATTERN_STOP;
        if (unit <= ID_WE)
        {road_traffic_unit[unit].TrafficLights.mode.pattern = PATTERN_STOP; } 
        else if (unit <= ID_WN)
        {turn_traffic_unit[unit].mode.pattern = PATTERN_STOP;  }
      }
      break; 

      case MODE_WAIT    : 
      {
        Serial.print("WAIT: ");
        TUModes[unit] = PATTERN_WAIT;
        if (unit <= ID_WE)
          {road_traffic_unit[unit].TrafficLights.mode.pattern = PATTERN_WAIT;  }
        else if (unit <= ID_WN)
          {turn_traffic_unit[unit].mode.pattern = PATTERN_WAIT;  }
      }
      break; 
      
      case MODE_PASS    : 
      {
        Serial.print("PASS: ");
        TUModes[unit] = PATTERN_PASS;  
        if (unit <= ID_WE)
          {road_traffic_unit[unit].TrafficLights.mode.pattern = PATTERN_PASS;  }
        else if (unit <= ID_WN)
          {turn_traffic_unit[unit].mode.pattern = PATTERN_PASS; }
      }
      break; 

      case MODE_INIT    :
      {
        Serial.print("INIT: ");
        TUModes[unit] = PATTERN_INIT;    
        if (unit <= ID_WE)
          {road_traffic_unit[unit].TrafficLights.mode.pattern = PATTERN_INIT;  }
        else if (unit <= ID_WN)
          {turn_traffic_unit[unit].mode.pattern = PATTERN_INIT; }
      }
      break; 
    }     
    Serial.println(TUModes[unit]);
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

void TRFC_vUpdateEmergencyStatus (struct pt* pt, uint32_t interval)
{ 
  static unsigned long timestamp = 0;
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
          case ID_NS : road_traffic_unit[ ID_NS ].EmergencyStatus = (status)?(EMERGENCY_ON): (EMERGENCY_OFF);  Serial.println("NS."); break; 
          case ID_EW : road_traffic_unit[ ID_EW ].EmergencyStatus = (status)?(EMERGENCY_ON): (EMERGENCY_OFF);  Serial.println("EW."); break;
          case ID_SN : road_traffic_unit[ ID_SN ].EmergencyStatus = (status)?(EMERGENCY_ON): (EMERGENCY_OFF);  Serial.println("SN."); break;
          case ID_WE : road_traffic_unit[ ID_WE ].EmergencyStatus = (status)?(EMERGENCY_ON): (EMERGENCY_OFF);  Serial.println("WE."); break;
          default: Serial.println("Not Valid input "+ readingInput); break;
        }
      }
      else 
      {
        TRFC_vGetCurrentSystemState();
      }
    }
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval);
    timestamp = millis();
  }  
  PT_END(pt);
}

void TRFC_vUpdateDensityStatus (void)
{
  for (int i = 0 ; i <= ID_WE ; i++)
  {
    if (road_traffic_unit[i].VehicleCounter >= DENSITY_THRESHOLD )
    {
      road_traffic_unit[i].DensityStatus = DENSITY_ON;
    }
    else 
    {
      road_traffic_unit[i].DensityStatus = DENSITY_OFF;
    }
  }
}

uint8_t TRFC_u32GetDensity (uint8_t unit, uint32_t * density)
{

  static uint8_t Error = OK;

  if ((unit <= ID_WE) && (density != NULL))
  {
   *density = road_traffic_unit[unit].VehicleCounter;
  }
  else 
  {
    Error = NOK;
    Serial.println("Error trafic unit ID or NULL pointer");
    Serial.println("ID:"+String(unit));
  }
  return Error;
}

uint8_t TRFC_u8GetMode(uint8_t unit)
{
  uint8_t mode = 0;
  uint8_t modeStruct = 0;
  
  if (unit <= ID_WN)
  {

    if (unit <= ID_WE)
    {modeStruct = road_traffic_unit[unit].TrafficLights.mode.pattern;}
    else if  (unit <= ID_WN)
    {modeStruct = turn_traffic_unit[unit].mode.pattern;}
    mode = TUModes[unit];
  }
  return modeStruct;
}

String TRFC_u8GetName(uint8_t unit)
{
  String name;
  if (unit <= ID_WN)
  {
    if (unit <= ID_WE)
    name = road_traffic_unit[unit].TrafficLights.TrafficName;
    else if  (unit <= ID_WN)
    name = turn_traffic_unit[unit].TrafficName;
  }
  return name;

}

static void TRFC_vGetCurrentSystemState (void)
{
  for (int i = 0 ; i <= ID_WN ; i++)
  {
    uint8_t currnetMode = TRFC_u8GetMode(i);
    String name = TRFC_u8GetName(i);
    Serial.println("---------------------------------");
    Serial.println("Unit: "+ String(i));
    Serial.println("Unit Name: "+ name);
    Serial.println("Unit Mode: "+ currnetMode);

    if (i <= ID_WE)
    {
      Serial.println("Count Up Pin: "+ String(road_traffic_unit[i].CountUpPin));
      Serial.println("Count Down Pin: "+ String(road_traffic_unit[i].CountDownPin));
      Serial.println("Idle State: "+ String(road_traffic_unit[i].IdleStatus));
      Serial.println("Emergency State: "+ String(road_traffic_unit[i].EmergencyStatus));
      Serial.println("Dense State: "+ String(road_traffic_unit[i].DensityStatus));
      Serial.println("Total Vehicles: "+ String(road_traffic_unit[i].VehicleCounter));
    }
    
    Serial.print("Unit Current State: "); 
    Serial.println((currnetMode == (uint8_t)PATTERN_STOP)?("RED"):( (currnetMode == (uint8_t)PATTERN_WAIT )?("YELLOW"):((currnetMode == (uint8_t)PATTERN_PASS )?("GREEN"):("INIT"))));
    Serial.println("---------------------------------");
  }
}

static void TRFC_vScan (struct pt* pt, uint32_t interval)
{
  static unsigned long timestamp = 0;
  static uint8_t oldUpPinState[4] = {0};
  static uint8_t oldDownPinState[4] = {0};
  static uint8_t currentUpPinState[4] = {0};
  static uint8_t currentDownPinState[4] = {0};
  
  PT_BEGIN(pt);
  while(1)
  {
    for (int i = 0 ; i <= ID_WE ; i++)
    {
      currentUpPinState[i]   = digitalRead(road_traffic_unit[i].CountUpPin);
      currentDownPinState[i] = digitalRead(road_traffic_unit[i].CountDownPin);
    }
    
    for (int i = 0 ; i <= ID_WE ; i++)
    {
      if ((oldUpPinState[i] == HIGH) && (currentUpPinState[i] == LOW)) 
      {
        Serial.println("Count Up Unit: "+ road_traffic_unit[i].TrafficLights.TrafficName);
        TRFC_vCountUp(i);
        Serial.println(road_traffic_unit[i].VehicleCounter);
      }
    
      if ((oldDownPinState[i] == HIGH) && (currentDownPinState[i] == LOW)) 
      {
        Serial.println("Count Down Unit: "+ road_traffic_unit[i].TrafficLights.TrafficName);
        TRFC_vCountDown(i);
        Serial.println(road_traffic_unit[i].VehicleCounter);
      }
    
    }

    for (int i = 0 ; i < 4 ; i++)
    {
      oldUpPinState[i] = currentUpPinState[i];
      oldDownPinState[i] = currentDownPinState[i];
    }
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval);
    timestamp = millis();
  }
  PT_END(pt);

}

static void TRFC_vCountUp (uint8_t unit)
{
  static uint32_t temp = 0;
  temp = road_traffic_unit[unit].VehicleCounter;
  Serial.println("Temp: "+ String(temp));

  if (++temp != 0){
    road_traffic_unit[unit].VehicleCounter = temp;
  }
  else {
    Serial.println("Error Count Up");
  }
}

static void TRFC_vCountDown (uint8_t unit)
{
  static uint32_t temp = 0;
  temp = road_traffic_unit[unit].VehicleCounter;
  Serial.println("Temp: "+ String(temp));

  if (road_traffic_unit[unit].VehicleCounter > 0){
    road_traffic_unit[unit].VehicleCounter -= 1;
  }
  else {
    Serial.println("Error Count Down");
  }
}
