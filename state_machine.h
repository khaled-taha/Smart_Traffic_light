#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

// Defines
#define NORMAL_STATE_GREEN_DURATION 6 // MUST NOT BE LESS THAN 3
#define SUB_STATE_GREEN_DURATION 3 // MUST NOT BE LESS THAN 3
#define YELLOW_DURATION 2
#define DENSITY_THRESHOLD 5
#define NO_NEXT_TEMP -1

// GLOBAL VARS and ENUMS

enum Signal
{
    signal_GREEN,
    signal_YELLOW,
    signal_RED,
    MAX_SIGNAL
} ;

enum Road
{
    road_SN,
    road_NS,
    road_EW,
    road_WE,
    MAX_ROAD
} ;

enum Traffic_light
{

  traffic_light_NS,
  traffic_light_EW,
  traffic_light_SN,
  traffic_light_WE,
  traffic_light_SW,
  traffic_light_NE,
  traffic_light_ES,
  traffic_light_WN,
  MAX_TRAFFIC_LIGHT
} ;

enum State
{
    STATE_STOP_ALL,
    STATE_NS_SN, //2,0
    STATE_NS_NE, //2,3
    STATE_EW_WE, //4,6
    STATE_EW_ES, //4,5
    STATE_SN_SW, //0,1
    STATE_WE_WN, //6,7
    MAX_STATE
} ;

static unsigned char traffic_lights[8];
static unsigned char road_density[4];
static char ambulance_exist[4];
static unsigned char vertical_round, horizontal_round;
static uint8_t current_state;
static uint8_t next_state = 0;
static int  next_temp_state = -1;

// PROTOTYPES
void FSM_init               (void);
void handle_state_machine   (uint8_t state);
void handle_STOP_ALL        (void);
void handle_state           (uint8_t traffic_light_1, uint8_t traffic_light_2, int green_duration);
void turn_traffic_light     (uint8_t traffic_light, uint8_t signal);
void state_delay            (int duration);
void check_ambulance_or_density(int duartion);
void check_ambulance        (void);
void check_density          (void);
bool is_high_density_road   (uint8_t road1, uint8_t road2, uint8_t road3, uint8_t road4);
void runStateMachine        (void);

#endif
