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

#define getName(var) #var

uint8 buffer[64], length;
uint16 analogRead;
int sample_count = 0;
int i;

char * name;

Log_ts log_s;
uint8 dumvar1 = 0;
uint16 dumvar2 = 0;
uint32 dumvar3 = 0;

volatile uint8 main_flag;


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    name = getName(analogRead);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    ADC_DelSig_1_Start();
    ADC_DelSig_1_StartConvert();
    USBFS_Start(0u, USBFS_5V_OPERATION);
    
    //logger initialization
    logger_init(&log_s);
    attach_variable(&dumvar1,UINT8,&log_s);
    attach_variable(&analogRead,UINT16,&log_s);
    attach_variable(&dumvar3,UINT32,&log_s);
    
    
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
            
            //---------------------------
            //usb
            //---------------------------
            if(USBFS_bGetConfiguration()){
                USBFS_EnableOutEP(2);
                /* Place your application code here. */
                /*wait for data received */
                //while(USBFS_bGetEPState(2) != USBFS_OUT_BUFFER_FULL);
            
                /* Read received bytes count */
                //length = USBFS_wGetEPCount(2);
                
                /* unload the out buffer */
                //USBFS_ReadOutEP(2, &buffer[0], length);
                //USBFS_LoadInEP(1, &buffer[0], 64);
                USBFS_LoadInEP(1, get_buffer(&log_s), 64);
                clear_log(&log_s);
            }
            if(ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT)){
                analogRead = ADC_DelSig_1_GetResult16();   
            }
            
        }
    }
}

/* [] END OF FILE */
