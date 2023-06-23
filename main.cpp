#include <string>

#include "mbed.h"
#include "platform.h"
#include "I2S.h"
#include "SineTable.h"

Ticker sampletick;

//I2S i2s(PB_5, PB_13, PB_12, PB_14, PC_6);
I2S i2s(PB_5, PB_13, PB_12, NC, PC_6);


void i2s_callback(int narg)
 {
 }

int main(void)
{
	printf("HERE YOU ARE...");
#if false	

	i2s.mode(MASTER_TX, true);
	int err = i2s.transfer<uint16_t>(SINE_TABLE, sizeof(SINE_TABLE),    // tx buffer, tx size
	                                 NULL, 0,                           // rx buffer, rx size
	                                 &i2s_callback, I2S_EVENT_ALL);     // callback, callback events
	printf("i2s transfer error: %d\n", err);

	while (true)
    {
        i2s.i2s_bh_queue.dispatch(100);
    }
#endif	

	return 0;
}

