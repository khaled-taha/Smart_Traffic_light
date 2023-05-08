#include <stdio.h>

// Defines
#define NORMAL_STATE_GREEN_DURATION 5
#define SUB_STATE_GREEN_DURATION 3
#define YELLOW_DURATION 2

// GLOBAL VARS and ENUMS

static unsigned char traffic_lights[8];
static unsigned char road_density[4];
static char ambulance_exist[4];
static unsigned char vertical_round, horizontal_round;

typedef enum
{
    signal_GREEN,
    signal_YELLOW,
    signal_RED,
    MAX_SIGNAL,
} Signal;

typedef enum
{
    road_SN,
    road_NS,
    road_EW,
    road_WE,
    MAX_ROAD,
} Road;

typedef enum
{
    traffic_light_SN,
    traffic_light_SW,
    traffic_light_NS,
    traffic_light_NE,
    traffic_light_EW,
    traffic_light_ES,
    traffic_light_WE,
    traffic_light_WN,
    MAX_TRAFFIC_LIGHT
} Traffic_light;

typedef enum
{
    STATE_STOP_ALL,
    STATE_NS_SN, //2,0
    STATE_NS_NE, //2,3
    STATE_EW_WE, //4,6
    STATE_EW_ES, //4,5
    STATE_SN_SW, //0,1
    STATE_WE_WN, //6,7
    MAX_STATE,
} State;


static State *current_state, *next_state;

// PROTOTYPES
void handle_state_machine(State *state);
void handle_STOP_ALL(void);
void handle_state(Traffic_light traffic_light_1, Traffic_light traffic_light_2, int green_duration);
void turn_traffic_light(Traffic_light traffic_light, Signal signal);
void delay(int duartion);
void check_ambulance_or_density(int duartion);
