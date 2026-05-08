#include "command_parser.h"
#include "Queue.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "uart_usb_bridge.h"


void CommandParser_Process(char *cmd)
{
    char axis;
    uint32_t steps;
    uint32_t interval;
    uint8_t dir;

    if(sscanf(cmd,"%c %u %u %hhu",&axis,&steps,&interval,&dir) == 4)
    {
        MoveBlock move;

        move.axis = axis;
        move.steps = steps;
        move.step_interval = interval;
        move.direction = dir;

        if(!Queue_Push(&move))
            Log("Queue Full\r\n");
        else
            Log("OK\r\n");
    }
    else
    {
        Log("Bad Command\r\n");
    }
}

