#include "main.h"
#include "usbd_cdc_if.h"
#include <string.h>
void USB_Bridge();
void UART_USB_Bridge_Task();
void USB_SendString(char *str);
void My_App();