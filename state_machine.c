// GLOBAL VARS and ENUMS

unsigned char traffic_lights[8];
unsigned char road_density[4];
char ambulance_exist[4];
unsigned char vertical_round, horizontal_round;
int GREEN_DURATION = 30;
int YELLOW_DURATION = 5;
int RED_DURATION = 30;
int roundi = 1;
int roundx = 1;

typedef enum
{
    GREEN,
    YELLOW,
    RED,
    MAX_SIGNAL,
} Signal;

typedef enum
{
    ROAD_SN,
    ROAD_NS,
    ROAD_EW,
    ROAD_WE,
    MAX_ROAD,
} Road;

typedef enum
{
    LIGHTS_SN,
    LIGHTS_SW,
    LIGHTS_NS,
    LIGHTS_NE,
    LIGHTS_EW,
    LIGHTS_ES,
    LIGHTS_WE,
    LIGHTS_WN,
    MAX_LIGHTS,
} Traffic_lights;

typedef enum
{
    STATE_STOP_ALL,
    STATE_NS_SN,
    STATE_NS_NE,
    STATE_EW_WE,
    STATE_EW_ES,
    STATE_SN_SW,
    STATE_WE_WN,
    MAX_STATE,
} State;


// PROTOTYPES
void handle_state_machine(State *current_state);
void handle_STOP_ALL(void);
void handle_NS_SN(void);
void handle_NS_NE(void);
void handle_SN_SW(void);
void handle_EW_WE(void);
void handle_EW_ES(void);
void handle_WE_WN(void);

// Define the state machine handler function
void handle_state_machine(State *current_state)
{
    switch (*current_state)
    {
    case STATE_STOP_ALL:
        // handle stop state
        handle_STOP_ALL();
        // transition to next state
        *current_state = STATE_NS_SN;
        break;
    case STATE_NS_SN:
        // handle NS_SN state
        handle_NS_SN();
        // transition to next state
        *current_state = STATE_NS_NE;
        break;
    case STATE_NS_NE:
        // handle NS_NE state
        handle_NS_NE();
        // transition to next state
        *current_state = STATE_EW_WE;
        break;
    case STATE_EW_WE:
        // handle EW_WE state
        handle_EW_WE();
        // transition to next state
        *current_state = STATE_EW_ES;
        break;
    case STATE_EW_ES:
        // handle EW_ES state
        handle_EW_ES();
        // transition to next state
        *current_state = STATE_NS_SN;
        break;
    case STATE_SN_SW:
        // handle SN_SW state
        handle_SN_SW();
        // transition to next state
        *current_state = STATE_EW_WE;
        break;
    case STATE_WE_WN:
        // handle WE_WN state
        handle_WE_WN();
        // transition to next state
        *current_state = STATE_NS_SN;
        break;
    default:
        // handle error
        break;
    }
}

State nextState;

void handle_STOP_ALL(void)
{
    // update traffic light states
    traffic_lights[LIGHTS_SN] = RED;
    traffic_lights[LIGHTS_SW] = RED;
    traffic_lights[LIGHTS_NS] = RED;
    traffic_lights[LIGHTS_NE] = RED;
    traffic_lights[LIGHTS_EW] = RED;
    traffic_lights[LIGHTS_ES] = RED;
    traffic_lights[LIGHTS_WE] = RED;
    traffic_lights[LIGHTS_WN] = RED;
}

void handle_NS_SN(void)
{
    if (roundi % 2 == 1)
    {
        nextState = STATE_NS_NE;
    }
    else
    {
        nextState = STATE_SN_SW;
    }
    // update traffic light states
    // GREEN
    traffic_lights[LIGHTS_SN] = GREEN;
    traffic_lights[LIGHTS_NS] = GREEN;

    // handle ambulance presence and road density
    nextState = check_ambulance_or_density(GREEN_DURATION); // -> delay | state
    // Yellow
    traffic_lights[LIGHTS_SN] = YELLOW;
    traffic_lights[LIGHTS_NS] = YELLOW;

    // handle ambulance presence and road density
    nextState = check_ambulance_or_density(YELLOW_DURATION); // -> delay | state
    // Red
    traffic_lights[LIGHTS_SN] = RED;
    traffic_lights[LIGHTS_NS] = RED;

    // handle ambulance presence and road density
    nextState = check_ambulance_or_density(RED_DURATION);
}

void handle_NS_NE(void)
{
    roundx = 2;
    // update traffic light states
    nextState = STATE_EW_WE;
    // GREEN
    traffic_lights[LIGHTS_NS] = GREEN;
    traffic_lights[LIGHTS_NE] = GREEN;
    // handle ambulance presence and road density
    nextState = check_ambulance_or_density(GREEN_DURATION); // -> delay | state
    // Yellow
    traffic_lights[LIGHTS_NS] = YELLOW;
    traffic_lights[LIGHTS_NE] = YELLOW;
    // handle ambulance presence and road density
    nextState = check_ambulance_or_density(YELLOW_DURATION); // -> delay | state
    // Red
    traffic_lights[LIGHTS_NS] = RED;
    traffic_lights[LIGHTS_NE] = RED;
    // handle ambulance presence and road density
    nextState = check_ambulance_or_density(RED_DURATION); // -> delay | state
}

void handle_SN_SW(void)
{
    roundi = 1;
        // update traffic light states
    nextState = STATE_NS_SN;
    // GREEN
    traffic_lights[LIGHTS_SN] = GREEN;
    traffic_lights[LIGHTS_SW] = GREEN;
    // handle ambulance presence and road density
    nextState = check_ambulance_or_density(GREEN_DURATION); // -> delay | state
    // Yellow
    traffic_lights[LIGHTS_SN] = YELLOW;
    traffic_lights[LIGHTS_SW] = YELLOW;
    // handle ambulance presence and road density
    nextState = check_ambulance_or_density(YELLOW_DURATION); // -> delay | state
    // Red
    traffic_lights[LIGHTS_SN] = RED;
    traffic_lights[LIGHTS_SW] = RED;
    // handle ambulance presence and road density
    nextState = check_ambulance_or_density(RED_DURATION); // -> delay | state
}

void handle_EW_WE(void)
{
    if (roundx % 2 == 1)
    {
        nextState = STATE_EW_ES;
    }
    else
    {
        nextState = STATE_WE_WN;
    }
    // update traffic light states
    // GREEN
    traffic_lights[LIGHTS_EW] = GREEN;
    traffic_lights[LIGHTS_WE] = GREEN;
    // handle ambulance presence and road density
    nextState = check_ambulance_or_density(GREEN_DURATION); // -> delay | state
    // Yellow
    traffic_lights[LIGHTS_EW] = YELLOW;
    traffic_lights[LIGHTS_WE] = YELLOW;
    // handle ambulance presence and road density
    nextState = check_ambulance_or_density(YELLOW_DURATION); // -> delay | state
    // Red
    traffic_lights[LIGHTS_EW] = RED;
    traffic_lights[LIGHTS_WE] = RED;
    // handle ambulance presence and road density
    nextState = check_ambulance_or_density(RED_DURATION); // -> delay | state

    // printf(nextState);
}

void handle_EW_ES(void)
{
    roundx = 2;
    // update traffic light states
    nextState = STATE_NS_SN;
    // GREEN
    traffic_lights[LIGHTS_EW] = GREEN;
    traffic_lights[LIGHTS_ES] = GREEN;
    // handle ambulance presence and road density
    nextState = check_ambulance_or_density(GREEN_DURATION); // -> delay | state
    // Yellow
    traffic_lights[LIGHTS_EW] = YELLOW;
    traffic_lights[LIGHTS_ES] = YELLOW;
    // handle ambulance presence and road density
    nextState = check_ambulance_or_density(YELLOW_DURATION); // -> delay | state
    // Red
    traffic_lights[LIGHTS_EW] = RED;
    traffic_lights[LIGHTS_ES] = RED;
    // handle ambulance presence and road density
    nextState = check_ambulance_or_density(RED_DURATION); // -> delay | state
}

void handle_WE_WN(void)
{
    State nextState;
    roundx = 1;
    // update traffic light states
    nextState = STATE_NS_SN;
    // GREEN
    traffic_lights[LIGHTS_WE] = GREEN;
    traffic_lights[LIGHTS_WN] = GREEN;
    // handle ambulance presence and road density
    nextState = check_ambulance_or_density(GREEN_DURATION); // -> delay | state
    // Yellow
    traffic_lights[LIGHTS_WE] = YELLOW;
    traffic_lights[LIGHTS_WN] = YELLOW;
    // handle ambulance presence and road density
    nextState = check_ambulance_or_density(YELLOW_DURATION); // -> delay | state
    // Red
    traffic_lights[LIGHTS_WE] = RED;
    traffic_lights[LIGHTS_WN] = RED;
    // handle ambulance presence and road density
    nextState = check_ambulance_or_density(RED_DURATION); // -> delay | state
}

State check_ambulance_or_density(int duration)
{
    delay(duration);
    return STATE_STOP_ALL;
}

int main()
{
    // Initialize state machine
    State current_state = STATE_STOP_ALL;

    // Run state machine
    while (1)
    {
        handle_state_machine(&current_state);
    }
}
