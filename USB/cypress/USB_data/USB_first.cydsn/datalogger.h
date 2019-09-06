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
    
#define LOG_SIZE 32
#define LOG_BUFF_SIZE 64
    
//do a better implementation of type handling.
    
enum types_e{
    UINT8,
    INT8,
    UINT16,
    INT16,
    UINT32,
    INT32,
    UINT64,
    INT64,
    FLOAT,
    DOUBLE,
    CHAR
};
    
// data structures

typedef struct var{
    void * var_ptr;
    enum types_e type;
}Var_ts;

typedef struct circbuff{
    uint8 data[LOG_BUFF_SIZE];
    int data_index;
    int data_size;
}CircBuff_ts;
    
typedef struct log{
    Var_ts vars[LOG_SIZE];
    CircBuff_ts CircBuff;
    int num_vars;
} Log_ts;

void logger_init(Log_ts * log);
void attach_variable(void * var, enum types_e type, Log_ts * log);
void logger(Log_ts * log);
uint8 * get_buffer(Log_ts * log);
int var_size(enum types_e type);
void circbuff_add(void * data, int size, CircBuff_ts * circbuff);
void circbuff_add_byte(uint8 data, CircBuff_ts * circbuff);
int get_buffer_size(Log_ts * log);
void _clear_circbuff(CircBuff_ts * circbuff);
void clear_log(Log_ts * log);

#endif



/* [] END OF FILE */
