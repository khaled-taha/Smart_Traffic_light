#include "state_machine.h"

void handle_state_machine(State *state)
{
    switch (*state)
    {
    case STATE_STOP_ALL:
        handle_STOP_ALL();
        current_state = STATE_STOP_ALL;
        next_state = STATE_NS_SN;
        break;
    case STATE_NS_SN: // Vertical case
        current_state = STATE_NS_SN;
        next_state = (vertical_round == 1) ? STATE_NS_NE : STATE_SN_SW;
        handle_state(traffic_light_NS, traffic_light_SN, NORMAL_STATE_GREEN_DURATION);
        break;
    case STATE_NS_NE: // Sub 1
        vertical_round = 2;
        current_state = STATE_NS_NE;
        next_state = STATE_EW_WE;
        handle_state(traffic_light_NS, traffic_light_NE, SUB_STATE_GREEN_DURATION);
        break;
    case STATE_SN_SW: // Sub 2
        vertical_round = 1;
        current_state = STATE_SN_SW;
        next_state = STATE_EW_WE;
        handle_state(traffic_light_SN, traffic_light_SW, SUB_STATE_GREEN_DURATION);
        break;
    case STATE_EW_WE: // Horizontal case
        current_state = STATE_EW_WE;
        next_state = (horizontal_round == 1) ? STATE_EW_ES : STATE_WE_WN;
        handle_state(traffic_light_EW, traffic_light_WE, NORMAL_STATE_GREEN_DURATION);
        break;
    case STATE_EW_ES: // Sub 1
        horizontal_round = 2;
        current_state = STATE_EW_ES;
        next_state = STATE_NS_SN;
        handle_state(traffic_light_EW, traffic_light_ES, SUB_STATE_GREEN_DURATION);
        break;
    case STATE_WE_WN: // Sub 2
        horizontal_round = 1;
        current_state = STATE_WE_WN;
        next_state = STATE_NS_SN;
        handle_state(traffic_light_WE, traffic_light_WN, SUB_STATE_GREEN_DURATION);
        break;
    default:
        // handle error
        break;
    }
}

void handle_STOP_ALL(void) // INITILIZATION
{
    vertical_round = 0;
    horizontal_round = 0;
    turn_traffic_light(traffic_light_NS, signal_RED);
    turn_traffic_light(traffic_light_SW, signal_RED);
    turn_traffic_light(traffic_light_NE, signal_RED);
    turn_traffic_light(traffic_light_EW, signal_RED);
    turn_traffic_light(traffic_light_ES, signal_RED);
    turn_traffic_light(traffic_light_WE, signal_RED);
    turn_traffic_light(traffic_light_WN, signal_RED);
    turn_traffic_light(traffic_light_SN, signal_RED);
}

void handle_state(Traffic_light traffic_light_1, Traffic_light traffic_light_2, int green_duration)
{
    printf("Currently handling %d %d\n", traffic_light_1, traffic_light_2);

    // GREEN
    turn_traffic_light(traffic_light_1, signal_GREEN);
    turn_traffic_light(traffic_light_2, signal_GREEN);

    check_ambulance_or_density(green_duration);

    // Yellow
    turn_traffic_light(traffic_light_1, signal_YELLOW);
    turn_traffic_light(traffic_light_2, signal_YELLOW);

    delay(YELLOW_DURATION);

    // Red
    turn_traffic_light(traffic_light_1, signal_YELLOW);
    turn_traffic_light(traffic_light_2, signal_YELLOW);
}

void turn_traffic_light(Traffic_light traffic_light, Signal signal)
{
    // UPDATE ARRAY
    traffic_lights[traffic_light] = signal;
    // SIGNAL LOWRE INTERFACE
    TRFC_vUpdate(traffic_light, signal);
}

void check_ambulance_or_density(int duration)
{
    int i = 0;
    do {
        check_density();
        check_ambulance();
        delay(duration/3);  // CHECK EVERY duration/3
        if (next_temp_state == next_state) {
            i--; // Increase one more more duration/3
            *next_temp_state = NO_NEXT_TEMP;
        } else if (next_temp_state != NO_NEXT_TEMP) { // next state diff
            break;
        } else {
            /* DO NOTHING */
        }
        i++;
    } while (i < 3);
}

void check_ambulance(void)
{
    if (TRFC_u8GetMode(road_SN) == 1)
    {
        *next_temp_state = STATE_SN_SW;
    } else if (TRFC_u8GetMode(road_NS) == 1) {
        *next_temp_state = STATE_NS_NE;
    } else if (TRFC_u8GetMode(road_EW) == 1) {
        *next_temp_state = STATE_EW_ES;
    } else if (TRFC_u8GetMode(road_WE) == 1) {
        *next_temp_state = STATE_WE_WN;
    } else {
        /* DO NOTHING*/
    }
}

void check_density(void) {
    uint32_t density_SN = TRFC_u32GetDensity(road_SN);
    uint32_t density_NS = TRFC_u32GetDensity(road_NS);
    uint32_t density_EW = TRFC_u32GetDensity(road_EW);
    uint32_t density_WE = TRFC_u32GetDensity(road_WE);

    if (is_high_density_road(density_SN, density_NS, density_EW, density_WE))
    {
        *next_temp_state = STATE_SN_SW;
    } else if (is_high_density_road(density_NS, density_SN, density_EW, density_WE)){
        *next_temp_state = STATE_NS_NE;
    } else if (is_high_density_road(density_EW, density_NS, density_SN, density_WE)){
        *next_temp_state = STATE_NS_NE;
    } else if (is_high_density_road(density_WE, density_NS, density_EW, density_SN)){
        *next_temp_state = STATE_NS_NE;
    } else if ((density_NS+density_SN) > DENSITY_THRESHOLD) {
        *next_temp_state = STATE_NS_SN;
    } else if ((density_EW+density_WE) > DENSITY_THRESHOLD) {
        *next_temp_state = STATE_EW_WE;
    } else {
        /* DO NOTHING*/
    }
}

void is_high_density_road(road1, road2, road3, road4) {
    return (
            (road1 - road2) > DENSITY_THRESHOLD) &&
            (road1 - road3) > DENSITY_THRESHOLD) &&
            (road1 - road4) > DENSITY_THRESHOLD)
           )
}

void delay(int duration)
{
     for(int i = duration; i >=0; i--) {
        printf("%d\n",i);
        sleep(1);
     }
}

int main()
{
    // Initialize state machine
    State current_state = STATE_STOP_ALL;

    // Run state machine
    while (1)
    {
        handle_state_machine(&next_state);
    }
}
