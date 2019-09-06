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
#ifndef USB_H
#define USB_H
    
#include "stdio.h"
#include "project.h"
    
struct usb_cmd_ts{
    int state;   
};

struct usb_private{
    //private variables, initialization/interface state variables
};

struct usb_public{
    //data to be used elsewhere
    struct usb_cmd_ts usb_cmd_s;
};

struct usb_input{
    //inputs to the usb module
    //data to stream
    uint8 * data;
    int size;
    
    //command data
    uint8 command[64];
};

struct usb_output{
    //outputs from the usb module
};

struct usb_ts{
    struct usb_private usb_prv;
    struct usb_public usb_pub;
    struct usb_input usb_in;
    struct usb_output usb_out;
};

    
void usb_task(struct usb_ts * usb_s);
void usb_stream(struct usb_ts * usb_s);
void usb_commands(struct usb_ts * usb_s);
    
    
#endif





/* [] END OF FILE */
