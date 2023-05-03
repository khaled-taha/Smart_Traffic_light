
#include "sr_interface.h"

extern TrafficUnit traffic_unit[4] ;

void SR_init(void)
{
  Serial.println("Initialize Shift register 74HC595 pins");
  pinMode(SR_DATA ,OUTPUT);
  pinMode(SR_CLK  ,OUTPUT);
  pinMode(SR_LOAD ,OUTPUT);
}

void SR_updateTask (void)
{
  static uint16_t copy_state = 0;
  
  copy_state = (uint16_t)(traffic_unit[3].mode.pattern<<9)|(traffic_unit[2].mode.pattern<<6)|(traffic_unit[1].mode.pattern<<3)|(traffic_unit[0].mode.pattern);
  
  digitalWrite(SR_LOAD, LOW);
  shiftOut(SR_DATA, SR_CLK, MSBFIRST, copy_state);
  digitalWrite(SR_LOAD, HIGH);
  Serial.println("Update Shift register by: "+ String(copy_state));
}