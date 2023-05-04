#include "sr_private.h"
#include "sr_interface.h"
#include "traffic_interface.h"

extern RoadTrafficUnit road_traffic_unit[4] ;
extern TrafficUnit     turn_traffic_unit[4] ;

void SR_init(void)
{
  Serial.println("Initialize Shift register Thread, 74HC595 pins");
  pinMode(SR_DATA ,OUTPUT);
  pinMode(SR_CLK  ,OUTPUT);
  pinMode(SR_LOAD ,OUTPUT);
  pinMode(SR_CLR  ,OUTPUT);
  PT_INIT(&asynchUpdateSR);
}

/*
MSB                            LSB
 GYR GYR GYR GYR GYR GYR GYR GYR
 777 666 555 444 333 222 111 000
*/
void SR_updateTask (struct pt* pt, uint32_t interval)
{
  static uint32_t copy_state = 0ul;
  static unsigned long timestamp = 0ul;
  PT_BEGIN(pt);
  while (1)
  {
    copy_state = 0ul;
    Serial.println("copy_state: "+String(copy_state));
    SR_vReset();
    for (int i = 0 ; i < 8 ; i++)
    {
      static uint32_t state = 0;
      state = 0;
      state = TRFC_u8GetMode(i);
      copy_state = copy_state | (state<<(3*i));
    } 

    copy_state = copy_state<<8;
    SR_vShiftOut(LSBFIRST, copy_state);
    digitalWrite(SR_LOAD, LOW);
    digitalWrite(SR_LOAD, HIGH);

    Serial.println("Update Shift register by: "+ String(copy_state));
  
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval);
    timestamp = millis();
  }
  PT_END(pt);
}

static void SR_vShiftOut(uint8_t bitOrder, uint32_t val)
{
	uint8_t i;

	for (i = 0; i < 32; i++)  {
		if (bitOrder == LSBFIRST) {
			digitalWrite(SR_DATA, val & 1);
			val >>= 1;
		} else {	
      byte temp = (val & 0x80000000) != 0;
			digitalWrite(SR_DATA, temp);
			val <<= 1;
		}
		digitalWrite(SR_CLK, HIGH);
		digitalWrite(SR_CLK, LOW);		
	}
}

static void SR_vReset(void)
{
  digitalWrite(SR_CLR, LOW);
  digitalWrite(SR_CLR, HIGH);
}