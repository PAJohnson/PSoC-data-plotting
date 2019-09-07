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

#ifndef PARAMETERS_H
#define PARAMETERS_H
    
#include "stdio.h"
#include "project.h"
#include "shared_types.h"
#include "datalogger.h"
    
//parameter header file.
//it would be nice if this used ideas from datalogger.c
//shouldn't be too hard to implement
//should have two arrays
//one array of "struct vars" like the logger
    //each "parameter" variable gets added to this object
//one array for the byte array
    //this one gets accessed according to the first array
    //can calculate position of a parameter in memory based on preceeding parameter data
    
#define PARAM_SIZE 256
typedef struct var_parameter_ts{
    void * var_ptr;
    enum types_e type;
}var_parameter_ts;

typedef struct parameter_ts{
    var_parameter_ts var_s[PARAM_SIZE];
    int num_vars;
    uint8 param[PARAM_SIZE];
}parameter_ts;

void init_parameters(parameter_ts * parameter_s);
void set_parameter(uint8 par_num, void * value, parameter_ts * parameter_s);
void get_parameter(uint8 par_num, void * value, parameter_ts * parameter_s);
void _set_parameter(uint32 address, void * data, uint32 size,parameter_ts * parameter_s);
void _get_parameter(uint32 address, void * data, uint32 size,parameter_ts * parameter_s);
void _save_parameters(parameter_ts * parameter_s);
void _read_parameters(parameter_ts * parameter_s);
void add_parameter(void * var, enum types_e type, parameter_ts * parameter_s);
void get_parameter_command(parameter_ts * parameter_s, struct usb_ts * usb_s);
int get_parameter_offset(uint8 par_num, parameter_ts * parameter_s);
    
#endif

/* [] END OF FILE */
