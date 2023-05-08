void handle_state_machine(State *state)
{
    switch (*state)
    {
    case STATE_STOP_ALL:
        handle_STOP_ALL();
        break;
    case STATE_NS_SN: // Vertical case
        next_state = (vertical_round == 1) ? STATE_NS_NE : STATE_SN_SW;
        handle_state(traffic_light_NS, traffic_light_SN, NORMAL_STATE_GREEN_DURATION);
        break;
    case STATE_NS_NE: // Sub 1
        vertical_round = 2;
        next_state = STATE_EW_WE;
        handle_state(traffic_light_NS, traffic_light_NE, SUB_STATE_GREEN_DURATION);
        break;
    case STATE_SN_SW: // Sub 2
        vertical_round = 1;
        next_state = STATE_EW_WE;
        handle_state(traffic_light_SN, traffic_light_SW, SUB_STATE_GREEN_DURATION);
        break;
    case STATE_EW_WE: // Horizontal case
        next_state = (horizontal_round == 1) ? STATE_EW_ES : STATE_WE_WN;
        handle_state(traffic_light_EW, traffic_light_WE, NORMAL_STATE_GREEN_DURATION);
        break;
    case STATE_EW_ES: // Sub 1
        horizontal_round = 2;
        next_state = STATE_NS_SN;
        handle_state(traffic_light_EW, traffic_light_ES, SUB_STATE_GREEN_DURATION);
        break;
    case STATE_WE_WN: // Sub 2
        horizontal_round = 1;
        next_state = STATE_NS_SN;
        handle_state(traffic_light_WE, traffic_light_WN);
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

void handle_state(Traffic_light traffic_light_1, traffic_light_2, int green_duration)
{
    printf("Currently handling\n");

    // GREEN
    turn_traffic_light(traffic_light_1, signal_GREEN);
    turn_traffic_light(traffic_light_2, signal_GREEN);

    check_ambulance_or_density(green_duartion);

    // Yellow
    turn_traffic_light(traffic_light_1, signal_YELLOW);
    turn_traffic_light(traffic_light_2, signal_YELLOW);

    delay(YELLOW_DURATION);

    // Red
    turn_traffic_light(traffic_light_1, signal_YELLOW);
    turn_traffic_light(traffic_light_2, signal_YELLOW);
}

void turn_traffic_light(Traffic_lights traffic_light, Signal signal)
{
    // UPDATE ARRAY
    traffic_lights[traffic_light] = signal;
    // SIGNAL LOWRE INTERFACE
}

void check_ambulance_or_density(int duration)
{
    delay(duartion);
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
