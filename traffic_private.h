#ifndef TRAFFIC_PRIVATE_H_
#define TRAFFIC_PRIVATE_H_

#define CHECK_SYSTEM  "-1"

#define NAME_NS  "NORTH_SOUTH"  
#define NAME_EW  "EAST_WEST"
#define NAME_SN  "SOUTH_NORTH"
#define NAME_WE  "WEST_EAST"
#define NAME_SW  "SOUTH_WEST"
#define NAME_NE  "NORTH_EAST"
#define NAME_ES  "EAST_SOUTH"
#define NAME_WN  "WEAST_NORTH"

#define MODE_INIT   0

#define PATTERN_INIT 0b111
#define PATTERN_STOP 0b001
#define PATTERN_WAIT 0b010
#define PATTERN_PASS 0b100

#define DENSITY_ON  1
#define DENSITY_OFF 0

#define EMERGENCY_ON  1
#define EMERGENCY_OFF 0

#define OK    0
#define NOK   1

static void TRFC_vGetCurrentSystemState  (void);
static void TRFC_vCountUp                (uint8_t unit);
static void TRFC_vCountDown              (uint8_t unit);
static void TRFC_vUpdateDensityStatus    (void);

#endif