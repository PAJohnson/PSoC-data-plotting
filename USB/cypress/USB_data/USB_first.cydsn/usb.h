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
    
//usb command set
#define NOCMD       0x00000000
#define NUMVARS     0x00000001
#define VARSIZE     0x00000002
#define VARNAME     0x00000003
#define VARMAX      0x00000004
#define VARMIN      0x00000005
#define VARSCALE    0x00000006
#define VAROFFSET   0x00000007
#define START       0x00000008
#define STOP        0x00000009
#define RESET       0x0000000A
#define NUMPARS     0x0000000B
#define PARSIZE     0x0000000C
#define PARVAL      0x0000000D
    
struct usb_cmd_ts{
    int state;   
};

struct usb_private{
    //private variables, initialization/interface state variables
};

struct usb_public{
    //data to be used elsewhere
    struct usb_cmd_ts usb_cmd_s;
    uint8 transmit_flag;
    uint8 cmd_flag;
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
    uint8 response[64];
    uint8 out_flag;
};

struct usb_ts{
    struct usb_private usb_prv;
    struct usb_public usb_pub;
    struct usb_input usb_in;
    struct usb_output usb_out;
};

    
void usb_task(struct usb_ts * usb_s);
void usb_stream(struct usb_ts * usb_s);
void usb_set_stream(struct usb_ts * usb_s, uint8 * data);
void usb_response(struct usb_ts * usb_s);
void usb_commands(struct usb_ts * usb_s);
void usb_init(struct usb_ts * usb_s);
    
    
#endif





/* [] END OF FILE */
