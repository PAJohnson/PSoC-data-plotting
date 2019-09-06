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
    //call all tasks related to usb here
    //will be closely related to the inout interface from before
    usb_commands(usb_s);
    usb_stream(usb_s);
}

void usb_stream(struct usb_ts * usb_s){
    //get/send data with the data endpoints
    USBFS_LoadInEP(1, usb_s->usb_in.data, 64);
}

void usb_commands(struct usb_ts * usb_s){
    //this should be a state machine   
    //get/send data with the command endpoints
    if(USBFS_bGetEPState(4) != USBFS_OUT_BUFFER_FULL){
        USBFS_ReadOutEP(4, usb_s->usb_in.command, USBFS_wGetEPCount(2));
    }
    
    //parse command data for state machine info
}

/* [] END OF FILE */
