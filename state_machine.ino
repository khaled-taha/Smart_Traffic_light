#include "state_machine.h"
#include "traffic_interface.h"

void FSM_init(void)
{
  PT_INIT(&asynchUpdateStateMachine);
}
void handle_state_machine(uint8_t state)
{
  
    switch (state)
    {
    case STATE_STOP_ALL:
        Serial.println("I'm in STATE_STOP_ALL");
        handle_STOP_ALL();
        current_state = STATE_STOP_ALL;
        next_state = STATE_NS_SN;
        break;
    case STATE_NS_SN: // Vertical case
        Serial.println("I'm in STATE_NS_SN");
        current_state = STATE_NS_SN;
        next_state = (vertical_round == 1) ? STATE_NS_NE : STATE_SN_SW;
        handle_state(traffic_light_NS, traffic_light_SN, NORMAL_STATE_GREEN_DURATION);
        break;
    case STATE_NS_NE: // Sub 1
        Serial.println("I'm in STATE_NS_NE");
        vertical_round = 2;
        current_state = STATE_NS_NE;
        next_state = STATE_EW_WE;
        handle_state(traffic_light_NS, traffic_light_NE, SUB_STATE_GREEN_DURATION);
        break;
    case STATE_SN_SW: // Sub 2
        Serial.println("I'm in STATE_SN_SW");
        vertical_round = 1;
        current_state = STATE_SN_SW;
        next_state = STATE_EW_WE;
        handle_state(traffic_light_SN, traffic_light_SW, SUB_STATE_GREEN_DURATION);
        break;
    case STATE_EW_WE: // Horizontal case
        Serial.println("I'm in STATE_EW_WE");
        current_state = STATE_EW_WE;
        next_state = (horizontal_round == 1) ? STATE_EW_ES : STATE_WE_WN;
        handle_state(traffic_light_EW, traffic_light_WE, NORMAL_STATE_GREEN_DURATION);
        break;
    case STATE_EW_ES: // Sub 1
        Serial.println("I'm in STATE_EW_ES");
        horizontal_round = 2;
        current_state = STATE_EW_ES;
        next_state = STATE_NS_SN;
        handle_state(traffic_light_EW, traffic_light_ES, SUB_STATE_GREEN_DURATION);
        break;
    case STATE_WE_WN: // Sub 2
        Serial.println("I'm in STATE_WE_WN");
        horizontal_round = 1;
        current_state = STATE_WE_WN;
        next_state = STATE_NS_SN;
        handle_state(traffic_light_WE, traffic_light_WN, SUB_STATE_GREEN_DURATION);
        break;
    default:
        Serial.println("ERROR in handle_state_machine");
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

void handle_state(uint8_t traffic_light_1, uint8_t traffic_light_2, int green_duration)
{
    Serial.println("Currently handling "+ String(traffic_light_1)+ " " +String(traffic_light_2));

    // GREEN
    turn_traffic_light(traffic_light_1, signal_GREEN);
    turn_traffic_light(traffic_light_2, signal_GREEN);

    check_ambulance_or_density(green_duration);
    Serial.println("Green");
    // Yellow
    turn_traffic_light(traffic_light_1, signal_YELLOW);
    turn_traffic_light(traffic_light_2, signal_YELLOW);

    state_delay(YELLOW_DURATION);
    Serial.println("Yellow");

    // Red
    turn_traffic_light(traffic_light_1, signal_RED);
    turn_traffic_light(traffic_light_2, signal_RED);
}

void turn_traffic_light(uint8_t traffic_light, uint8_t signal)
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
        Serial.println("Duration Before: "+String(duration));
        state_delay(duration/3);  // CHECK EVERY duration/3
        Serial.println("Duration After:"+String(duration));
        check_density();
        check_ambulance();
        if (next_temp_state == current_state) {
          Serial.println("Duration increased : "+String(duration));
            i--; // Increase one more more duration/3
            next_temp_state = NO_NEXT_TEMP;
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
    if (TRFC_boolGetEmrgncy(road_SN) == 1)
    {
        next_temp_state = STATE_SN_SW;
        Serial.println("Emergency Change state @ 0");
    } else if (TRFC_boolGetEmrgncy(road_NS) == 1) {
        next_temp_state = STATE_NS_NE;
        Serial.println("Emergency Change state @ 1");
    } else if (TRFC_boolGetEmrgncy(road_EW) == 1) {
        next_temp_state = STATE_EW_ES;
        Serial.println("Emergency Change state @ 2");
    } else if (TRFC_boolGetEmrgncy(road_WE) == 1) {
        next_temp_state = STATE_WE_WN;
        Serial.println("Emergency Change state @ 3");
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
        next_temp_state = STATE_SN_SW;
        Serial.println("Density Change state @ 0 ");
    } else if (is_high_density_road(density_NS, density_SN, density_EW, density_WE)){
        next_temp_state = STATE_NS_NE;
        Serial.println("Density Change state @ 1 ");
    } else if (is_high_density_road(density_EW, density_NS, density_SN, density_WE)){
        next_temp_state = STATE_NS_NE;
        Serial.println("Density Change state @ 2 ");
    } else if (is_high_density_road(density_WE, density_NS, density_EW, density_SN)){
        next_temp_state = STATE_NS_NE;
        Serial.println("Density Change state @ 3 ");
    } else if ((density_NS+density_SN) > DENSITY_THRESHOLD) {
        next_temp_state = STATE_NS_SN;
        Serial.println("Density Change state @ V ");
    } else if ((density_EW+density_WE) > DENSITY_THRESHOLD) {
        Serial.println("Density Change state @ H ");
        next_temp_state = STATE_EW_WE;
    } else {
        /* DO NOTHING*/
    }
}

bool is_high_density_road   (uint8_t road1, uint8_t road2, uint8_t road3, uint8_t road4) 
{
  return (
          ((road1 - road2) > DENSITY_THRESHOLD) &&
          ((road1 - road3) > DENSITY_THRESHOLD) &&
          ((road1 - road4) > DENSITY_THRESHOLD)
          );
}

void state_delay(volatile int duration)
{
    seconds=0;
    while (duration >= seconds ){
      
    }
}

void runStateMachine  (struct pt* pt, uint32_t interval)
{
  static unsigned long timestamp = 0ul;
  PT_BEGIN(pt);
  while (1){
    // Run state machine
    handle_state_machine(next_state);
  
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval);
    timestamp = millis();  
  }
  PT_END(pt);
}