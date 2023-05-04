#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <pt.h>

#include "pin_mapping.h"
#include "app_config.h"
#include "sr_interface.h"
#include "traffic_interface.h" 
#include "app_modes_interface.h"

static uint8_t seconds = 0;

struct pt asynchUpdateStatus;
struct pt asynchUpdateSR;
struct pt asynchUpdateDensity;

void sys_tick (void)
{
  TRFC_vUpdateStatus();
  seconds += 1;
}

void setup() 
{
  Serial.begin(SERIAL_BAUDRATE);
  Serial.setTimeout(SERIAL_TIMEOUT_MS);
  Serial.println("Traffic System Initalizing");
  
  TRFC_vInit();
  SR_init();
  
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(sys_tick);

  TRFC_vUpdate(0, MODE_STOP);
  TRFC_vUpdate(1, MODE_STOP);
  TRFC_vUpdate(2, MODE_STOP);
  TRFC_vUpdate(3, MODE_STOP);
}

void loop() 
{
  SR_updateTask(&asynchUpdateSR, 500);
  TRFC_vUpdateEmergencyStatus(&asynchUpdateStatus, 1000);
  TRFC_vScan(&asynchUpdateDensity, 100);
}
