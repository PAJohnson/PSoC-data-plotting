/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef DATALOGGER_H
#define DATALOGGER_H
#include "stdio.h"
#include "CyLib.h"
#include "usb.h"
#include "shared_types.h"
    
#define LOG_SIZE 32
#define LOG_BUFF_SIZE 64
    
//do a better implementation of type handling.
    
// data structures



//this type and the functions that go with it don't make sense!
//not great to use a circular buffer here, plus this isn't even
//a circular buffer, as implemented.
//a deque would be interesting. Could make one out of a real
//circular buffer fairly easily.
typedef struct circbuff{
    uint8 data[LOG_BUFF_SIZE];
    int data_index;
    int data_size;
}CircBuff_ts;

typedef struct var{
    void * var_ptr;
    enum types_e type;
    char name[64];
}Var_ts;

typedef struct log{
    Var_ts vars[LOG_SIZE];
    CircBuff_ts CircBuff;
    int num_vars;
} Log_ts;

void logger_init(Log_ts * log);
void attach_variable(void * var, enum types_e type, const char * name, Log_ts * log);
void logger(Log_ts * log);
uint8 * get_buffer(Log_ts * log);
int var_size(enum types_e type);
void circbuff_add(void * data, int size, CircBuff_ts * circbuff);
void circbuff_add_byte(uint8 data, CircBuff_ts * circbuff);
int get_buffer_size(Log_ts * log);
void _clear_circbuff(CircBuff_ts * circbuff);
void clear_log(Log_ts * log);
void get_command_usb(Log_ts * log, struct usb_ts * usb_s);

#endif



/* [] END OF FILE */
