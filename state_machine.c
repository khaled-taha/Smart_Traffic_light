// GLOBAL VARS and ENUMS

unsigned char traffic_lights[8];
unsigned char road_density[4];
bool ambulance_exist[4];
unsigned char vertical_round, horizontal_round;
int GREEN_DUARATION = 30;

typedef enum {
    GREEN,
    YELLOW,
    RED,
    MAX_SIGNAL,
} Signal;

typedef enum {
    ROAD_SN, 
    ROAD_NS, 
    ROAD_EW, 
    ROAD_WE,
    MAX_ROAD,
} Road;

typedef enum {
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

typedef enum {
    STATE_STOP_ALL,
    STATE_NS_SN,
    STATE_NS_NE,
    STATE_SN_SW,
    STATE_EW_WE,
    STATE_EW_ES,
    STATE_WE_WN,
    MAX_STATE,
} State;

// PROTOTYPES
void handle_state_machine(State* current_state);
void handle_STOP_ALL(void);
void handle_NS_SN(void);
void handle_NS_NE(void);
void handle_SN_SW(void);
void handle_EW_WE(void);
void handle_EW_ES(void);
void handle_WE_WN(void);

// Define the state machine handler function
void handle_state_machine(State* current_state) {
    switch(*current_state) {
        case STATE_NS_SN:
            // handle idle state
			handle_NS_SN(30);
            // transition to next state
            *current_state = STATE_RUNNING;
            break;
        case STATE_RUNNING:
            // handle running state
            // transition to next state
            *current_state = STATE_COMPLETE;
            break;
        case STATE_COMPLETE:
            // handle complete state
            // reset to idle state
            *current_state = STATE_IDLE;
            break;
        default:
            // handle error
            break;
    }
}

void handle_STOP_ALL(void) {
    // update traffic light states
	  traffic_lights[Traffic_lights.SN] = LIGHT.RED;
	  traffic_lights[Traffic_lights.SW] = LIGHT.RED;
	  traffic_lights[Traffic_lights.NS] = LIGHT.RED;
	  traffic_lights[Traffic_lights.NE] = LIGHT.RED;
	  traffic_lights[Traffic_lights.EW] = LIGHT.RED;
	  traffic_lights[Traffic_lights.ES] = LIGHT.RED;
	  traffic_lights[Traffic_lights.WE] = LIGHT.RED;
	  traffic_lights[Traffic_lights.WN] = LIGHT.RED;
}

void handle_NS_SN(void) { 
	if (round % 2 == 1) {
		State nextState = State.STATE_NS_NE;
	} else {
		State nextState = State.STATE_SN_SW;
	}
    // update traffic light states
	// GREEN
	traffic_lights[Traffic_lights.SN] = LIGHT.GREEN;
	traffic_lights[Traffic_lights.NS] = LIGHT.GREEN;
	
	
	
    // handle ambulance presence and road density
	nextState = check_ambulance_or_density(GREEN_DUARATION); -> delay | state
	// Yellow
	
	// Red
	
}

void handle_NS_NE(void) { 
	round = 2;
    // update traffic light states

    // handle ambulance presence and road density
	
}

void handle_SN_SW(void) {
	round = 1;
    // update traffic light states

    // handle ambulance presence and road density
}

void handle_EW_WE(void) {
    // update traffic light states
	
    // handle ambulance presence and road density
}

void handle_EW_ES(void) {
    // update traffic light states

    // handle ambulance presence and road density
}

void handle_WE_WN(void) {
    // update traffic light states

    // handle ambulance presence and road density
}

void check_ambulance_or_density(int duration) {
	delay(duration);
}

int main() {
    // Initialize state machine
    State current_state = STATE_STOP_ALL;
	
    // Run state machine
	 while(1) {
        handle_state_machine(&current_state);
    }
}
