#ifndef _VM_SYSTEM_CONFIG_H_
#define _VM_SYSTEM_CONFIG_H_

//****************** Inclusion Files ***************************
#include "18F66K40.h"
#include "vmPINConfig.h"

//****************** Component Configuration ********************


//********* CPU Settings *******************************************
#pragma device HIGH_INTS=TRUE

#pragma fuses ECH,WDT,NOPROTECT,NOBROWNOUT,NOFCMEN,NOPUT 
#pragma use delay(clock=20000000,restart_wdt)

//******** Serial Interface ************
//#pragma use rs232(stream=GSM_M66, xmit=PIN_GPRS_COMM_XMIT, rcv=PIN_GPRS_COMM_RCV, baud=9600, bits=8, parity=N, stop=1, restart_wdt)

// ############### Using UART3 -> Configure for RS485 ##########################
#PIN_SELECT U3RX=PIN_E1
#PIN_SELECT U3TX=PIN_E0
#use rs232(baud=38400,UART3,stream=RS485,ERRORS)

// ############### Using UART1 -> Configure for UART 2 UART ####################
#PIN_SELECT U1RX=PIN_C7
#PIN_SELECT U1TX=PIN_C6
#use rs232(baud=19200,UART1,stream=UART,ERRORS)

//// ############### Using UART2 -> Configure for GPRS Module;EC20 ###############
//#PIN_SELECT U2RX=PIN_G2
//#PIN_SELECT U2TX=PIN_G1
//#use rs232(baud=115200,UART2,stream=GSM_M66,ERRORS)

// ################# Using UART2 -> Configure for LAN Module; USR_K3 ###########
#PIN_SELECT U5RX=PIN_E3
#PIN_SELECT U5TX=PIN_E2
#use rs232(baud=115200,UART5,stream=LAN_USR,ERRORS)

#pragma priority INT_RDA,INT_RDA5,INT_RDA3

#ifndef M1_COMM_MODULE
#define M1_COMM_MODULE  1
#endif

#endif /* _VM_SYSTEM_CONFIG_H_ */