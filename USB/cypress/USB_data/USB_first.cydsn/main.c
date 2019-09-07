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

//implement type tagging for the info header
//first, send a message indicating the order and type of variables
//from then on, send variables csv style
//might be a good idea to make use of control endpoints
//for example, do nothing until a "start" ctrl message is received
//send this message from python on a keypress
//implement an event loop in python for this

//send variable names individually, end with a termination message
//unlikely to hit name limit problems this way
//state machine for usb data, even in interrupt
//state 0 init -> state 1 on RX start
//state 1 run -> state 0 on RESET

//logger file next
//similar to the DI4 datalogger
#include "project.h"
#include "stdio.h"
#include "datalogger.h"
#include "parameters.h"

#define getName(var) #var

uint8 buffer[64], length;
uint16 analogRead;
int sample_count = 0;
int i;

char * name;

Log_ts log_s;
struct usb_ts usb_s;
parameter_ts parameter_s;
uint8 dumvar1 = 0;
uint16 dumvar2 = 0;
uint32 dumvar3 = 0;

volatile uint8 main_flag;

const uint8 emEeprom[Em_EEPROM_PHYSICAL_SIZE]
        __ALIGNED(CY_FLASH_SIZEOF_ROW) = {0u};
        
uint32 test_param = 0xAABBCCDD;
uint32 test_param_2 = 0;
uint32 test_param_3;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    Em_EEPROM_Init((uint32)&emEeprom[0]);
    
    name = getName(analogRead);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    ADC_DelSig_1_Start();
    ADC_DelSig_1_StartConvert();
    USBFS_Start(0u, USBFS_5V_OPERATION);
    
    //logger initialization
    logger_init(&log_s);
    attach_variable(&dumvar1,UINT8,"dumvar1",&log_s);
    attach_variable(&analogRead,UINT16,"analogRead",&log_s);
    attach_variable(&dumvar3,UINT32,"dumvar3",&log_s);
    
    //usb initialization
    usb_set_stream(&usb_s,get_buffer(&log_s));
    
    //parameter initialization
    init_parameters(&parameter_s);
    add_parameter(&test_param,UINT32,&parameter_s);
    add_parameter(&test_param_2,UINT32,&parameter_s);
    add_parameter(&test_param_3,UINT32,&parameter_s);
    
    /* enumeration is done, enable out endpoint for receive data from Host */
    Loop_Interrupt_Start();
    sample_count = 0;
    for(;;)
    {
        if(main_flag == 1){
            //main interrupt driven loop
            main_flag = 0;
            //insert code to run here
            dumvar1 += 1;
            dumvar2 += 10;
            dumvar3 += 100;
            //---------------------------
            //logger
            //---------------------------
            logger(&log_s);
            get_command_usb(&log_s, &usb_s);
            
            //---------------------------
            //usb
            //---------------------------
            usb_task(&usb_s);
            clear_log(&log_s);
            
            if(ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT)){
                analogRead = ADC_DelSig_1_GetResult16();   
            }
            
            //----------------------------
            //parameter
            //----------------------------
            get_parameter_command(&parameter_s, &usb_s);
            usb_response(&usb_s);
        }
    }
}

/* [] END OF FILE */
