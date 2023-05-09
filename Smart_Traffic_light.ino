#include <Arduino_FreeRTOS.h>

#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <pt.h>

#include "pin_mapping.h"
#include "app_config.h"
#include "sr_interface.h"
#include "traffic_interface.h" 
#include "state_machine.h"

#pragma pack(1)
extern RoadTrafficUnit road_traffic_unit[4] ;
extern TrafficUnit     turn_traffic_unit[4] ;

volatile uint8_t seconds = 0;

struct pt asynchUpdateStatus;
struct pt asynchUpdateSR;
struct pt asynchUpdateDensity;
struct pt asynchUpdateStateMachine;

xTaskCreate(
    TaskAnalogRead
    ,  "AnalogRead" // A name just for humans
    ,  128  // Stack size
    ,  NULL //Parameters for the task
    ,  2  // Priority
    ,  NULL
  ); //Task Handle

void sys_tick (void)
{
  seconds++;
  TRFC_vUpdate((seconds+0)%8, MODE_WAIT);
  TRFC_vUpdate((seconds+1)%8, MODE_PASS);
  TRFC_vUpdate((seconds+2)%8, MODE_STOP);
  TRFC_vUpdate((seconds+3)%8, 0);
  TRFC_vUpdate((seconds+4)%8, MODE_WAIT);
  TRFC_vUpdate((seconds+5)%8, MODE_PASS);
  TRFC_vUpdate((seconds+6)%8, MODE_STOP);
  TRFC_vUpdate((seconds+7)%8, 0);
  TRFC_vUpdateStatus();
  Serial.println("SEC:"+ String(seconds));
}

void setup() 
{
  Serial.begin(SERIAL_BAUDRATE);
  Serial.setTimeout(SERIAL_TIMEOUT_MS);
  Serial.println("Traffic System Initalizing");
  
  TRFC_vInit();
  SR_init();
  FSM_init();
  
  for (int i = 0; i < 8 ; i ++)
  {
    TRFC_u8GetMode(i);
  }
  for (int i = 0; i < 4 ; i ++)
  {
    Serial.println("Emergency: "+String(road_traffic_unit[i].EmergencyStatus));
  }

  Timer1.initialize(1000000);
  Timer1.attachInterrupt(sys_tick);
 
}

void loop() 
{
  SR_updateTask(&asynchUpdateSR, 300);
  TRFC_vUpdateEmergencyStatus(&asynchUpdateStatus, 200);
  TRFC_vScan(&asynchUpdateDensity, 100);
  runStateMachine(&asynchUpdateStateMachine, 1000);
}
