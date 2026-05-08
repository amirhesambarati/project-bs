
#include "MyApp.h"
#include "STM32PrintfUART.h"


void test1(){

		//HAL_UART_Transmit(&huart1,(uint8_t*)"Test SERIAL TX 1",16,100);
}

void test2(){

	Log("TEST Serial TX2!\r\n");

}

void test3(){

	printf("TEST Serial TX3!\r\n");

}



void My_App(){
	test1();
	HAL_Delay(1000);
	test2() ;
	HAL_Delay(1000);
	test3() ;
	HAL_Delay(1000);
	printf("System Started!\r\n");
  UART_TX_Task();
	HAL_Delay(1);
	
}