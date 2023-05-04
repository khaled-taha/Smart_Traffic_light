#ifndef TRAFFIC_INTERFACE_H_
#define TRAFFIC_INTERFACE_H_

#define CHECK_SYSTEM  "-1"
#define MODE_STOP   0 
#define MODE_WAIT   1
#define MODE_PASS   2

#define PATTERN_STOP 0b001
#define PATTERN_WAIT 0b010
#define PATTERN_PASS 0b100

#define TRAFFIC_STATUS_IDLE       0
#define TRAFFIC_STATUS_EMERGENCY  1
#define TRAFFIC_STATUS_DENSITY    2

struct UL {
    uint8_t red:1;
    uint8_t yellow:1;
    uint8_t green:1;
};
typedef struct UL UnitLeds;


union TUP{
    uint8_t pattern:3;
    UnitLeds TrafficUnitLed;
};
typedef union TUP TrafficUnitPattern;


struct TU{
  String TrafficName;
  TrafficUnitPattern mode;
  uint8_t EmergencyStatus : 1 ;
  uint8_t DensityStatus : 1 ;
  uint8_t IdleStatus : 1 ;
  uint8_t CountUpPin: 6;
  uint8_t CountDownPin: 6;
  uint32_t VehicleCounter;
};
typedef struct TU TrafficUnit;

void TRFC_vInit                   (void);
void TRFC_vUpdate                 (uint8_t unit, uint8_t mode);
void TRFC_vUpdateStatus           (void);
void TRFC_vUpdateEmergencyStatus  (struct pt* pt, uint32_t interval);
void TRFC_vUpdateDensityStatus    (void);
void TRFC_vScan                   (struct pt* pt, uint32_t interval);
void TRFC_vCountUp                (uint8_t unit);
void TRFC_vCountDown              (uint8_t unit);

#endif