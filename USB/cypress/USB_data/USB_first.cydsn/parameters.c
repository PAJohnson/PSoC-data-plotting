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

#include "parameters.h"

void init_parameters(parameter_ts * parameter_s){
    int i;
    for(i = 0; i < PARAM_SIZE; i++){
        parameter_s->var_s[i].type = 0;
        parameter_s->var_s[i].var_ptr = 0;
    }
    _read_parameters(parameter_s);
    parameter_s->num_vars = 0;
}

int get_parameter_offset(uint8 par_num, parameter_ts * parameter_s){
    int i = 0;
    int offset = 0;
    for(i = 0; i < par_num; i++){
        offset += var_size(parameter_s->var_s[i].type);   
    }
    return offset;
}

void set_parameter(uint8 par_num, void * value, parameter_ts * parameter_s){
    int offset= 0;
    int i;
    
    offset = get_parameter_offset(par_num, parameter_s);
    _set_parameter(offset,value,var_size(parameter_s->var_s[par_num].type),parameter_s);
}

void get_parameter(uint8 par_num, void * value, parameter_ts * parameter_s){
    int offset = 0;
    int i;
    
    offset = get_parameter_offset(par_num,parameter_s);
    _get_parameter(offset,value,var_size(parameter_s->var_s[par_num].type),parameter_s);
}

void _set_parameter(uint32 address, void * data, uint32 size, parameter_ts * parameter_s){
    uint8 * data_ptr;
    uint32 i;
    data_ptr = (uint8 *)data;
    if(size + address < PARAM_SIZE){
        for(i = 0; i < size; i++){
            parameter_s->param[address+i] = data_ptr[i]; 
        }
    }
}

void _get_parameter(uint32 address, void * data, uint32 size, parameter_ts * parameter_s){
    uint8 * data_ptr;
    uint32 i;
    data_ptr = (uint8 *) data;
    if(size + address < PARAM_SIZE){
        for(i = 0; i < size; i++){
            data_ptr[i] = parameter_s->param[address+i];   
        }
    }
}

void _save_parameters(parameter_ts * parameter_s){
    cy_en_em_eeprom_status_t eepromReturnValue;
    eepromReturnValue = Em_EEPROM_Write(0,parameter_s->param,PARAM_SIZE); 
    return;
}

void _read_parameters(parameter_ts * parameter_s){
    Em_EEPROM_Read(0,parameter_s->param,PARAM_SIZE);   
}

void add_parameter(void * var, enum types_e type, parameter_ts * parameter_s){
    var_parameter_ts temp_var;
    temp_var.type = type;
    temp_var.var_ptr = var;
    if(parameter_s->num_vars < PARAM_SIZE){
        parameter_s->var_s[parameter_s->num_vars] = temp_var;
        parameter_s->num_vars++;
    }
    return;
}

void get_parameter_command(parameter_ts * parameter_s, struct usb_ts * usb_s){
    uint32 command;
    uint8 argument;
    uint32 argument_2 = 0;
    int i;
    if(usb_s->usb_pub.cmd_flag != 0){
        command = usb_s->usb_in.command[3] +0xFF * usb_s->usb_in.command[2] + \
                  0xFFFF * usb_s->usb_in.command[1] + 0xFFFFFF * usb_s->usb_in.command[0];
                
        switch(command){
            case NOCMD:
                ;
            break;
            
            case NUMPARS:
                //how many parameters are there?
                //reply with number of parameters
                usb_s->usb_out.response[0] = parameter_s->num_vars;
                usb_s->usb_out.out_flag = 1;
                usb_s->usb_pub.cmd_flag = 0;
            break;
            
            case PARSIZE:
                //size of parameter in bits?
                //parameter number is the arg, argument is first byte after command
                argument = usb_s->usb_in.command[4];
                if(argument <= parameter_s->num_vars){
                    usb_s->usb_out.response[0] = 8*var_size(parameter_s->var_s[argument].type);
                }
                usb_s->usb_out.out_flag = 1;
                usb_s->usb_pub.cmd_flag = 0;
            break;
            
            case PARGET:
                //get parameter value
                //parameter number is the arg, first byte after command
                argument = usb_s->usb_in.command[4];
                if(argument <= parameter_s->num_vars){
                    memcpy(usb_s->usb_out.response,&parameter_s->param[get_parameter_offset(argument,parameter_s)],var_size(parameter_s->var_s[argument].type));
                }
                usb_s->usb_out.out_flag = 1;
                usb_s->usb_pub.cmd_flag = 0;
            break;
                
            case PARSET:
                //first arg is par num, second arg is value
                //size computed automatically
                argument = usb_s->usb_in.command[4];
                for(i = 0; i < var_size(parameter_s->var_s[argument].type); i++){
                    //loop though bytes according to parameter size
                    argument_2 += usb_s->usb_in.command[5+i] * 1<<(i*8);
                }
                set_parameter(argument,&argument_2,parameter_s);
                //set command flag to 0 to indicate the command has been acted upon.
                usb_s->usb_pub.cmd_flag = 0;
                
            case PARSAVE:
                //save parameters to flash
                _save_parameters(parameter_s);
                usb_s->usb_pub.cmd_flag = 0;
            
            default:
                ;
            
        }
    }
}

/* [] END OF FILE */
