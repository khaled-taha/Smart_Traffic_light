#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <pt.h>

struct pt asynchUpdateStatus;

#include "pin_mapping.h"
#include "app_config.h"
#include "sr_interface.h"
#include "traffic_interface.h" 
#include "app_modes_interface.h"

static uint8_t seconds = 0;

void sys_tick (void)
{
  static uint8_t joins = 0;
  joins += 1;
  SR_updateTask();
  TRFC_vUpdateStatus();
  
  if (joins == 4){
    Serial.println("SEC:"+String(seconds));
    seconds += 1;
    joins = 0;
  }
}

void setup() 
{
  Serial.begin(SERIAL_BAUDRATE);
  Serial.setTimeout(SERIAL_TIMEOUT_MS);
  Serial.println("Traffic System Initalizing");
  
  TRFC_vInit();
  SR_init();
  
  Timer1.initialize(250000);
  Timer1.attachInterrupt(sys_tick);

  TRFC_vUpdate(0, MODE_STOP);
  TRFC_vUpdate(1, MODE_STOP);
  TRFC_vUpdate(2, MODE_STOP);
  TRFC_vUpdate(3, MODE_STOP);
}

void loop() 
{
  Serial.println("Supre Loop");
  TRFC_vUpdateEmergencyStatus(&asynchUpdateStatus);
  delay(1500);
}
