#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <pt.h>

#include "pin_mapping.h"
#include "app_config.h"
#include "sr_interface.h"
#include "traffic_interface.h" 
#include "app_modes_interface.h"

extern RoadTrafficUnit road_traffic_unit[4] ;
extern TrafficUnit     turn_traffic_unit[4] ;

static uint8_t seconds = 0;

struct pt asynchUpdateStatus;
struct pt asynchUpdateSR;
struct pt asynchUpdateDensity;

void sys_tick (void)
{
  TRFC_vUpdateStatus();

  seconds += 1;
  Serial.println("SEC: "+String(seconds));
}

void setup() 
{
  Serial.begin(SERIAL_BAUDRATE);
  Serial.setTimeout(SERIAL_TIMEOUT_MS);
  Serial.println("Traffic System Initalizing");
  
  TRFC_vInit();
  SR_init();
  
  delay(1000);
  
  for (int i = 0; i < 8 ; i ++)
  {
    TRFC_u8GetMode(i);
  }

  Timer1.initialize(1000000);
  Timer1.attachInterrupt(sys_tick);
  
}

void loop() 
{
  SR_updateTask(&asynchUpdateSR, 400);
  TRFC_vUpdateEmergencyStatus(&asynchUpdateStatus, 1000);
  TRFC_vScan(&asynchUpdateDensity, 100);

}
