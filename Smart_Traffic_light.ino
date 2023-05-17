#include <TimerOne.h>
#include <pt.h>

#include "pin_mapping.h"
#include "app_config.h"
#include "sr_interface.h"
#include "traffic_interface.h" 
#include "state_machine.h"

volatile uint8_t seconds = 0;
extern RoadTrafficUnit road_traffic_unit[4] ;
extern TrafficUnit     turn_traffic_unit[4] ;

struct pt asynchUpdateStatus;
struct pt asynchUpdateDensity;
struct pt asynchUpdateStateMachine;
struct pt asynchDelay;

void sys_tick ( void );

void setup() 
{
  Serial.begin(SERIAL_BAUDRATE);
  Serial.setTimeout(SERIAL_TIMEOUT_MS);
  Serial.println("Traffic System Initalizing");
  
  SR_init();
  TRFC_vInit();
  
  PT_INIT(&asynchUpdateStatus);
  PT_INIT(&asynchUpdateDensity);
  PT_INIT(&asynchUpdateStateMachine);
  PT_INIT(&asynchDelay);

}

void loop() 
{
  TRFC_vUpdateStatus(&asynchUpdateStatus, 200);
  TRFC_vScan(&asynchUpdateDensity, 100);
  runStateMachine(&asynchUpdateStateMachine, 1000);
}