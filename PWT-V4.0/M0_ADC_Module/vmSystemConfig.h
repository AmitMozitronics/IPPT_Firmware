
#ifndef _VI_SYSTEM_CONFIG_H_
#define _VI_SYSTEM_CONFIG_H_

//****************** Inclusion Files ***************************
//#include "18F46K80.h"
#include "18F66k40.h"
#include "vmPINConfig.h"

//****************** Component Configuration ********************

//optimization level.
#pragma opt 8
 
//********* CPU Settings *******************************************

#pragma device HIGH_INTS=TRUE

#pragma fuses ECH,WDT,NOPROTECT,NOBROWNOUT,NOFCMEN,NOPUT 
#pragma use delay(clock=20000000,restart_wdt)

//******** Serial Interface ************
////#pragma use rs232(stream=RS485, xmit=PIN_RS485_XMIT, rcv=PIN_RS485_RCV, baud=9600, bits=8, parity=N, stop=1, restart_wdt)
//#pragma use rs232(stream=ARD_RS232, xmit=PIN_LAN_COMM_XMIT, rcv=PIN_LAN_COMM_RCV, baud=9600, bits=8, parity=N, stop=1, restart_wdt)

// ********** Serial Interface **************************************

// ################ Using UART2 -> Configure for Camera ########################
#PIN_SELECT U4RX=PIN_C1
#PIN_SELECT U4TX=PIN_C0
#use rs232(baud=9600,UART4,stream=CAMERA,ERRORS)

// ############### Using UART1 -> Configure for UART 2 UART ####################
#PIN_SELECT U1RX=PIN_C7
#PIN_SELECT U1TX=PIN_C6
#use rs232(baud=19200,UART1,stream=UART,ERRORS)


//#define unsigned int8 BYTE
#ifndef M0_ADC_MODULE
#define M0_ADC_MODULE 1
#endif

#endif /* _VI_SYSTEM_CONFIG_H_ */