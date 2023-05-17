
#ifndef TRAFFIC_INTERFACE_H_
#define TRAFFIC_INTERFACE_H_

#define ID_NS 0
#define ID_EW 1
#define ID_SN 2
#define ID_WE 3
#define ID_SW 4
#define ID_NE 5
#define ID_ES 6
#define ID_WN 7

#define MODE_PASS   0
#define MODE_WAIT   1
#define MODE_STOP   2

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
};
typedef struct TU TrafficUnit;

struct RTU{
  TrafficUnit TrafficLights;
  uint8_t EmergencyStatus : 1 ;
  uint8_t DensityStatus : 1 ;
  uint8_t IdleStatus : 1 ;
  uint8_t CountUpPin: 6;
  uint8_t CountDownPin: 6;
  uint8_t EmergPin: 6;
  uint32_t VehicleCounter;
};
typedef struct RTU RoadTrafficUnit;


void TRFC_vInit                   (void);
void TRFC_vUpdate                 (uint8_t unit, uint8_t mode);
void TRFC_vUpdateStatus           (struct pt* pt, uint32_t interval);
void TRFC_vUpdateEmergencyStatus  ( void );
void TRFC_vScan                   ( struct pt* pt, uint32_t interval );
bool TRFC_boolGetEmrgncy          (uint8_t unit);
uint8_t TRFC_u32GetDensity        (uint8_t unit);
uint8_t TRFC_u8GetMode            (uint8_t unit);
String TRFC_u8GetName             (uint8_t unit);

#endif