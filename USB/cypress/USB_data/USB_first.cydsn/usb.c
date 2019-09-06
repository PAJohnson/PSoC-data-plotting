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

#include "usb.h"

void usb_task(struct usb_ts * usb_s){
    static uint8 init = 0;
    //call all tasks related to usb here
    //will be closely related to the inout interface from before
    if(init == 0){
        usb_init(usb_s);
        if(USBFS_bGetConfiguration()){
            USBFS_EnableOutEP(2);
            USBFS_EnableOutEP(4);
            init = 1;
        }
    }
    else{
        usb_commands(usb_s);
        usb_response(usb_s);
        usb_stream(usb_s);
    }
}

void usb_init(struct usb_ts * usb_s){
    int i;
    //initialize usb_s 
    //public 
    usb_s->usb_pub.usb_cmd_s.state = 0;
    //private
    usb_s->usb_pub.transmit_flag = 0;
    //input
    //usb_s->usb_in.data = NULL;
    usb_s->usb_in.size = 0;
    for(i = 0; i < 64; i++){
        usb_s->usb_in.command[i] = 0;
    }
    //output
    for(i = 0; i < 64; i++){
        usb_s->usb_out.response[i] = 0;   
    }
    usb_s->usb_out.out_flag = 0;
}

void usb_stream(struct usb_ts * usb_s){
    //get/send data with the data endpoints
    if(usb_s->usb_pub.transmit_flag == 1){
        USBFS_LoadInEP(1, usb_s->usb_in.data, 64);
    }
}

void usb_response(struct usb_ts * usb_s){
    int i;
    if(usb_s->usb_out.out_flag != 0){
        USBFS_LoadInEP(3, usb_s->usb_out.response, 64);
        usb_s->usb_out.out_flag = 0;
        for(i = 0; i < 64; i++){
            usb_s->usb_out.response[i] = 0;   
        }
    }
}

void usb_set_stream(struct usb_ts * usb_s, uint8 * data){
    usb_s->usb_in.data = data;   
}

void usb_commands(struct usb_ts * usb_s){
    //this should be a state machine   
    //get/send data with the command endpoints
    
    uint32 command = 0;
    
    if(USBFS_bGetEPState(4) == USBFS_OUT_BUFFER_FULL){
        //only take action if command received?
        //some commands might take more than one cycle, might need 
        //other architecture
        usb_s->usb_pub.cmd_flag = 1;
        USBFS_ReadOutEP(4, usb_s->usb_in.command, USBFS_wGetEPCount(4));
        command = usb_s->usb_in.command[3] +0xFF * usb_s->usb_in.command[2] + \
                  0xFFFF * usb_s->usb_in.command[1] + 0xFFFFFF * usb_s->usb_in.command[0];
        switch(command){
            case NOCMD:
                ;
            break;
            
            case NUMVARS:
                ;
            break;
            
            case VARSIZE:
                ;
            break;
            
            case VARNAME:
                ;
            break;
            
            case VARMAX:
                ;
            break;
            
            case VARMIN:
                ;
            break;
            
            case VARSCALE:
                ;
            break;
            
            case VAROFFSET:
                ;
            break;
            
            case START:
                usb_s->usb_pub.transmit_flag = 1;
                usb_s->usb_pub.cmd_flag = 0;
            break;
            
            case STOP:
                usb_s->usb_pub.transmit_flag = 0;
                usb_s->usb_pub.cmd_flag = 0;
            break;
            
            case RESET:
                ;
            break;
            
            default:
                ;
            
        }
    }
    
    //parse command data for state machine info
}

/* [] END OF FILE */
