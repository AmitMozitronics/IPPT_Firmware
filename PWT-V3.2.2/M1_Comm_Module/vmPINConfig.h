#ifndef _VM_PIN_CONFIG_H_
#define _VM_PIN_CONFIG_H_

//#include "18F46K80.h"

// RS485 Communication Pin Mapping.
#define PIN_RS485_XMIT              PIN_E0
#define PIN_RS485_RCV               PIN_E1
#define PIN_RS485_RTS               PIN_G0			

// GPSR Communication PIN Mapping; SIM900/SIM800/Quectel.
#define PIN_GPRS_COMM_XMIT          PIN_G1
#define PIN_GPRS_COMM_RCV           PIN_G2

// UART -><- UART(IBU) Communication PIN Mapping.
#define PIN_IBU_RCV                 PIN_C7
#define PIN_IBU_XMIT                PIN_C6

// Extra UART PIN Mapping.(For Spare Purpose)
#define PIN_EXT_UART_XMIT           PIN_C0
#define PIN_EXT_UART_RCV            PIN_C1

// Digital Input(DI) PIN Mapping.
#define PIN_INPUT_DI_00             PIN_C2
#define PIN_INPUT_DI_01             PIN_C3

// Digital Output PIN Mapping.
#define PIN_OUT_DO_00               PIN_E5
#define PIN_OUT_DO_01               PIN_E4

// Spare Serial Clock and Serial Data PIN Mapping.
#define PIN_MCU1_SCLK               PIN_D6
#define PIN_MCU1_SDA                PIN_D5
  

#endif /* _VM_PIN_CONFIG_H_ */