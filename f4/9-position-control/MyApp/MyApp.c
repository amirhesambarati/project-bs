#include "main.h"
#include "time.h"
#include "MyApp.h"
#include "Queue.h"
#include "StepperDriver.h"
#include "command_parser.h"
#include "uart_usb_bridge.h"
#include "usart-get-position.h"

extern int position ;

void My_App(){
	//test1();
	send_request_position();
	process_uart();
	check_timeout();

}

