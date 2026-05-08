
#include "MyApp.h"
#include "usbd_cdc_if.h"
#include "string.h"

uint8_t BUFFER_CDC[40] ;

void USB_SendString(char *str)
{
    CDC_Transmit_FS((uint8_t*)str, strlen(str));
}

void My_App(){

//USB_SendString("this is crc test ! /n");

}