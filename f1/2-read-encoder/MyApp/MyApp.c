
#include "MyApp.h"
#include "STM32PrintfUART.h"
#include "ReadENC.h"
#include "time.h"
	static uint32_t last_print = 0;	
void My_App(){
	
	  Encoder_Update();	
		int32_t Count = Encoder_GetCount();
		int32_t Speed = Encoder_GetSpeed();
		int32_t rpm = Encoder_GetRPM();
		uint32_t now = millis();
    if (now - last_print >= 200)
    {
        last_print = now;

        printf("count=%ld | speed=%ld pps | RPM=%ld | time = %d\r\n",
               (long)Count,
               (long)Speed,
               (long)rpm,
								last_print
							 );
    }

}