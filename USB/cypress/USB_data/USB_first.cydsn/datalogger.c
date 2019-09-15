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

#include "datalogger.h"

void logger_init(Log_ts * log){
    int i;
    for(i = 0; i < LOG_BUFF_SIZE; i++){
        log->CircBuff.data[i] = 0;   
    }
    log->num_vars = 0;
    log->CircBuff.data_index = 0;
    log->CircBuff.data_size = LOG_BUFF_SIZE;
}

void attach_variable(void * var, enum types_e type, const char * name, Log_ts * log){
    Var_ts temp_var;
    temp_var.type = type;
    temp_var.var_ptr = var;
    if(log->num_vars < LOG_SIZE){
        log->vars[log->num_vars] = temp_var;
        memcpy(log->vars[log->num_vars].name,name,strlen(name)+1);
        log->num_vars++;
    }
    return;
}

void logger(Log_ts * log){
    //fetch most recent values of all vars, store in data array
    //separate by a delimiter
    
    //important to do this with a circular buffer 
    int i;
    int max_size;

    for(i = 0; i < log->num_vars; i++){
        //add each logged var to the data array
        
        circbuff_add((log->vars[i].var_ptr),var_size(log->vars[i].type),&log->CircBuff);
    }
    
    return;
}

void circbuff_add(void * data, int size, CircBuff_ts * circbuff){
    int i;
    for(i = 0; i < size; i++){
        switch(size){
            case 1:
                circbuff_add_byte(*((uint8 *)data),circbuff);
            break;
            
            case 2:
                circbuff_add_byte((uint8)(*((uint16 *)data)>>i*8),circbuff);
            break;
            
            case 4:
                circbuff_add_byte((uint8)(*((uint32 *)data)>>i*8),circbuff);
            break;
            
            case 8:
                circbuff_add_byte((uint8)(*((uint64 *)data)>>i*8),circbuff);
            break;
                
            default:
                ;
                
        }
    }
}

void circbuff_add_byte(uint8 data, CircBuff_ts * circbuff){
    
    //will store data until the buffer is full, then do nothing
    if(circbuff->data_index < circbuff->data_size){
        circbuff->data[circbuff->data_index] = data;
        circbuff->data_index = circbuff->data_index + 1;
    }
}

uint8 * get_buffer(Log_ts * log){
    return (log->CircBuff.data);
}

int get_buffer_size(Log_ts * log){
    return (int) log->CircBuff.data_index;   
}

int var_size(enum types_e type){
    
    int size;
    switch (type){
        case UINT8:
            size = 1;
        break;
        
        case INT8:
            size = 1;
        break;
        
        case UINT16:
            size = 2;
        break;
        
        case INT16:
            size = 2;
        break;
        
        case UINT32:
            size = 4;
        break;
        
        case INT32:
            size = 4;
        break;
        
        case UINT64:
            size = 8;
        break;
        
        case INT64:
            size = 8;
        break;
        
        case CHAR:
            size = 1;
        break;
        
        case FLOAT:
            size = 4;
        break;
        
        case DOUBLE:
            size = 8;
        break;
    
        default:
            size = 0;
      // default statements
    }
    return size;
}

void _clear_circbuff(CircBuff_ts * circbuff){
    int i;
    for(i = 0; i < circbuff->data_size; i++){
        circbuff->data[i] = 0;   
    }
    circbuff->data_index = 0;
}

void clear_log(Log_ts * log){
    _clear_circbuff(&log->CircBuff);
}

void get_command_usb(Log_ts * log, struct usb_ts * usb_s){
    uint32 command;
    uint8 argument;
    if(usb_s->usb_pub.cmd_flag != 0){
        command = usb_s->usb_in.command[3] +0xFF * usb_s->usb_in.command[2] + \
                  0xFFFF * usb_s->usb_in.command[1] + 0xFFFFFF * usb_s->usb_in.command[0];
                
        switch(command){
            case NOCMD:
                ;
            break;
            
            case NUMVARS:
                //how many variables are being logged?
                //reply with number of logged variables
                usb_s->usb_out.response[0] = log->num_vars;
                usb_s->usb_out.out_flag = 1;
                usb_s->usb_pub.cmd_flag = 0;
            break;
            
            case VARSIZE:
                //size of variable in bytes
                //variable number is the arg, argument is first byte after command
                argument = usb_s->usb_in.command[4];
                if(argument <= log->num_vars){
                    usb_s->usb_out.response[0] = var_size(log->vars[argument].type);
                }
                usb_s->usb_out.out_flag = 1;
                usb_s->usb_pub.cmd_flag = 0;
            break;
            
            case VARNAME:
                //variable name? (string)
                //variable number is the arg, first byte after command
                argument = usb_s->usb_in.command[4];
                if(argument <= log->num_vars){
                    memcpy(usb_s->usb_out.response,log->vars[argument].name,strlen(log->vars[argument].name));
                }
                usb_s->usb_out.out_flag = 1;
                usb_s->usb_pub.cmd_flag = 0;
            break;
            
            case VARMAX:
                //max value for variable?
                //variable number is the arg, first byte after command
                argument = usb_s->usb_in.command[4];
                if(argument <= log->num_vars){
                    ;
                }
                usb_s->usb_pub.cmd_flag = 0;
            break;
            
            case VARMIN:
                //min value for variable?
                //variable number is the arg, first byte after command
                argument = usb_s->usb_in.command[4];
                if(argument <= log->num_vars){
                    ;
                }
                usb_s->usb_pub.cmd_flag = 0;
            break;
            
            case VARSCALE:
                //multiplier for variable?
                //variable number is the arg, first byte after command
                argument = usb_s->usb_in.command[4];
                if(argument <= log->num_vars){
                    ;
                }
                usb_s->usb_pub.cmd_flag = 0;
            break;
            
            case VAROFFSET:
                //offset for variable?
                //variable number is the arg, first byte after command
                argument = usb_s->usb_in.command[4];
                if(argument <= log->num_vars){
                    ;
                }
                usb_s->usb_pub.cmd_flag = 0;
            break;
            
            case START:
                
            break;
            
            case STOP:
            
            break;
            
            case RESET:
            
            break;
            
            default:
                ;
            
        }
    }
}

/* [] END OF FILE */
