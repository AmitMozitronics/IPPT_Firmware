/* 
 * File:   vmGSMM66Config.h
 * Author: mrinmoy
 *
 * Created on January 18, 2019, 5:10 PM
 */

#ifndef VM_GSM_M66_CONFIG_H
#define	VM_GSM_M66_CONFIG_H

//#include "vmGSMM66_GPRSConfig.h"
//#include "vmTimerConfig.h"

#define   SIZE_SMS_RESPONSE_BUFF                60
#define   GPRS_MODULE_HEALTH_OK                 44
#define   GPRS_MODULE_HEALTH_MODULE_IS_NOT_OK   45

// *****************************************************************************
// ***************** Below -> Global Variable Declarations *********************
// *****************************************************************************
unsigned int8  g_fIsGPRSConnected                                         = GPRS_NOT_CONNECTED;
unsigned int8  g_fIsSIM900ModuleOK                                        = GPRS_MODULE_HEALTH_MODULE_IS_NOT_OK;
// *****************************************************************************
// ***************** Above -> Global Variable Declarations *********************
// *****************************************************************************


//void InitM66RxInterrupt(void)
//{
//    // Enable SIM900 interrupt.
//    clear_interrupt(INT_RDA2);
//	enable_interrupts(INT_RDA2);
//}



void Delay_Milli(unsigned int16 ui16DelayTime)
{
    unsigned int16 ui16TimeCounter = 0;
    while(ui16TimeCounter < ui16DelayTime)
    {
        delay_ms(1);
        ui16TimeCounter++;
    }
}   

//#INT_RDA2
//void GSMM66_Rx_ISR(void)
//{
//     // Read the received byte from stream and store it in Rx buffer 
//    g_ui8M66RxBuffer[g_ui16M66RxByteCount] = fgetc(GSM_M66);
//    
//    // Increase receive byte count
//    g_ui16M66RxByteCount++;
//    
//    // Check if receive byte count reached receive buffer max size
//    if(SIZE_GSMM66_RX_BUFFER == g_ui16M66RxByteCount)
//    {
//        // Make it circular buffer
//        g_ui16M66RxByteCount = 0;
// 
//    }
//    
//    // Clear interrupt.
//    clear_interrupt(INT_RDA2);
//}



//unsigned int8 SendAtCommand(void)
//{
//    unsigned int8 ui8fIsOKFound = 0;
//    unsigned int8 ui8SendCount  = 0;
//    g_fIsTimedOut = 0;
//    
//    while(ui8SendCount < 5)
//    {
//	    // Reset Rx Buffer.
//	    ResetGSMM66RxBuffer();
//	    // Send AT Command.
//	    fputs("AT\r",GSM_M66);
//	    // Set Timeout.	
//	    SetTimeout(1);
//	    // Check for 'OK' response.
//	     while(g_fIsTimedOut == 0 && ui8fIsOKFound == 0)
//	     {
//	     	for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
//	     	{
//	     		// Check for "OK".
//	             if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
//	                g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
//	                g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'K' &&
//	                g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'O')
//	                {
//                        // Set flag denoting 'OK' is found.
//                        ui8fIsOKFound = 1;
//                        ui8SendCount = 5;
//                        break;
//	                }
//	     	}
//	     
//	     }
//	        
//		ui8SendCount++;
//	}
//	
//    
//    return ui8fIsOKFound;
//}





#endif	/* VM_GSM_M66_CONFIG_H */

