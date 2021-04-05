/* 
 * File:   vmM1Common.h
 * Author: mrinmoy
 *
 * Created on July 8, 2019, 3:52 PM
 */

#ifndef VM_M1_COMMON_H
#define	VM_M1_COMMON_H

#include "time.h"
// ################ RS485 Related Inclusion Start ##############################

#define RS485_TX_BUFF_SIZE                      20
#define RS485_RX_BUFF_SIZE                      50

#define FLAG_RS485_DATA_READY                   1
#define FLAG_RS485_DATA_NOT_READY               0

#define FLAG_RS485_DATA_SEND_ON                 1
#define FLAG_RS485_DATA_SEND_DONE               2
#define FLAG_RS485_DATA_SEND_OFF                3

#define RS485_9600_1_BYTE_RCV_TIME              1040
#define RS485_MULTIPLE_REG_BASIC_RCV_BYTE_COUNT 9

#define DELAY_TIME_RTS_BEFORE                   1
#define DELAY_TIME_RTS_AFTER                    2

#define ISR_TOTAL_BYTE_RECEIVE_TIME             9 //5
#define ISR_RECEIVED_ALL_BYTE                   21
#define ISR_PROCESSED_ALL_BYTE                  22
#define ISR_IS_READY                            40
#define ISR_IS_NOT_READY                        41
#define ISR_TOTAL_8_BYTE_RECEIVE_TIME           3 //9
#define ISR_TOTAL_19_BYTE_RECEIVE_TIME          7//21

#define MODBUS_IS_VALID_SLAVE_ID                12
#define MODBUS_IS_NOT_VALID_SLAVE_ID            13
#define ISR_RECEIVED_ALL_BYTES                  21
#define ISR_PROCESSED_ALL_BYTES                 22

#define SINGLE_VARIETY_PRODUCTION               10
#define MULTI_VARIETY_PRODUCTION                11

// ################ RS485 Related Inclusion End ################################

// ################ GPRS Related Inclusion Start ###############################

#define GPRS_CONNECTED                          1
#define GPRS_NOT_CONNECTED                      0

#define  GPRS_CONNECTION_FAILS                  30
#define  GPRS_HTTP_ENABLE_FAILS                 31
#define  GPRS_SET_BEARER_ID_FAILS               32
#define  GPRS_SET_WEB_SERVICE_URL_FAILS         33
#define  GPRS_START_HTTP_GET_SESSION_FAILS      34

// ############### GPRS Related Inclusion End ##################################


// *****************************************************************************
// ****************** Global Variable Declarations *****************************
// *****************************************************************************

// ################### RS485 Related Stuffs -> Starts ##########################

unsigned int8  g_fIsReadyToReceiveByte                = ISR_IS_NOT_READY;
unsigned int8  g_ui8ISRReceiveByteTimeCounter         = 0;
unsigned int8  g_fISRAllByteReceived                  = ISR_PROCESSED_ALL_BYTE;

unsigned int8 g_RS485TxBuffer[RS485_TX_BUFF_SIZE] = {0};
unsigned int8 g_RS485RxBuffer[RS485_RX_BUFF_SIZE] = {0};

unsigned int8  g_fRS485DataReady                  = FLAG_RS485_DATA_NOT_READY;
unsigned int8  g_ui8ByteToSend                    = 0;
unsigned int8  g_ui8fRS485DataSend                = FLAG_RS485_DATA_SEND_OFF;

unsigned int8  g_fIsValidSlaveID                      = MODBUS_IS_VALID_SLAVE_ID;
unsigned int16 g_ui16ISRAllBytesExpectedRcvTime        = 250;
unsigned int8  g_fISRReceivedByteStatus               = ISR_PROCESSED_ALL_BYTES;
unsigned int8  g_ui8UnexpectedSlaveIDIgnoreCounter    = 0;


// ################### RS485 Related Stuffs -> Ends ############################

// ################### Timer Related Stuffs -> Starts ##########################

unsigned int8  g_fIsTimedOut                          = 0;

unsigned int16 g_ui16TimeOutMilliseconds              = 0;
unsigned int8  g_fShouldLookForTimeOut                = 0;

void SetTimeout(unsigned int8 ui8TimedOutSeconds)
{
    // Convert timeout seconds into milliseconds
    g_ui16TimeOutMilliseconds = ui8TimedOutSeconds * 1000;
    
    // Set flag denoting now timer should look for timeout value
    g_fShouldLookForTimeOut = 1;
}

#define MIN_DELAY               100
unsigned int8 g_ui8DelayLoopIncr  = 0;
unsigned int8 g_ui8DelayLoopCount  = 0;
void mz_delay_ms(unsigned int16 ui16Time)
{
	g_ui8DelayLoopCount = ui16Time / MIN_DELAY;
    
    for (g_ui8DelayLoopIncr = 0; g_ui8DelayLoopIncr < g_ui8DelayLoopCount; g_ui8DelayLoopIncr++)
    {
        delay_ms(MIN_DELAY);
        restart_wdt();
    }
}

// ################### Timer Related Stuffs -> Ends ############################


unsigned int32 g_ui32HookedUpTimeStampPerMinute = 0;

typedef struct _ST_LAN_DATA_HEADER
{
    unsigned int8    m_ui8MachineID;
    unsigned int16   m_ui16BasicMaterialStandard;
    unsigned int16   m_ui16StandardSpecification;
    unsigned int8    m_ui8PressureRating;
    unsigned int16   m_ui16PipeOD;
    unsigned int8    m_ui8PipeLength;
    unsigned int32   m_ui32MaxWeight;
    unsigned int32   m_ui32MinWeight;
    unsigned int16   m_ui16Unused1;         
    unsigned int16   m_ui16Unused2;
    unsigned int16   m_ui16Unused3;         // Last three parameters are unused for now; may use in future. 
    
} ST_LAN_DATA_HEADER; 

ST_LAN_DATA_HEADER       g_stLANDataHeader;

unsigned int8 g_fIsLastElementInGPRSQueue = 0;

struct_tm g_stHMITime;
struct_tm g_stHMITimeTemp;
struct_tm* g_pstHMITimeTemp = &g_stHMITimeTemp;
time_t g_ui32UnixTimeInSec = 0;

#endif	/* VM_M1_COMMON_H */

