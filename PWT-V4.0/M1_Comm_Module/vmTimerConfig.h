#ifndef _VM_TIMER_CONFIG_H_
#define _VM_TIMER_CONFIG_H_


#include "vmM1Common.h"
#include "../Common/vmIBUConfig.h"
#include "../Common/vmModbusGlobal.h"


#define T0_DIV_PRESCALE                     T0_DIV_8 
#define TIMER_0_VALUE_LOAD_FOR_1MS          0xFD8F
#define ALL_TIMER_TEST_VAL_FOR_100_US       (0xFFFF - 0x003F + 1)
#define ALL_TIMER_TEST_VAL_FOR_70_US        (0xFFFF - 0x002C + 1)


#define FLAG_DI_STATE_CHANGED                   11
#define FLAG_DI_STATE_NOT_CHANGED               12

//#define CHECK_FOR_AT_RESPONSE                   15
//#define DO_NOT_CHECK_FOR_AT_RESPONSE            16
//#define AT_RESPONSE_CHECK_INTRVL_TIMEOUT        10 // In Minutes.

#define MILLISECOND_TIMEOUT                     1000
#define SECOND_TIMEOUT                          60

#define SHOULD_SEND_DATA                        25
#define SHOULD_NOT_SEND_DATA                    26
#define DATA_SEND_INTERVAL_TIMEOUT              40 // In Seconds.

#define  GPRS_QUEUE_EMPTY_TIME_COUNT_START      80
#define  GPRS_QUEUE_EMPTY_TIME_COUNT_STOP       81
#define  GPRS_QUEUE_EMPTY_TIMEOUT               15 // In Minutes.

#define  SHOULD_CHECK_GPRS_CONNECTIVITY         55
#define  SHOULD_NOT_CHECK_GPRS_CONNECTIVITY     56

#define  GPRS_CONNECTION_QUERY_TIMEOUT          3  // In seconds.
#define  HTTP_URL_PARAMETER_SET_TIMEOUT         2
#define  HTTP_GET_SESSION_TIMEOUT               18
#define  DATA_RESEND_INTRVL_TIMEOUT             10

#define  RESET_TIME_INTERVAL_COUNTING           20
#define  POWER_CUT_TIME_COUNTING                21
#define  NETWORK_RECONNECT_HALT_TIME_COUNTING   22
#define  MODEM_RESET_PROCESS_ON                 24
#define  MODEM_RESET_PROCESS_OFF                25
#define  PERIODIC_MODEM_RESET_TIMEOUT           30 // In minutes.

#define  TIME_STAMP_WRITE_INTERVAL_TIME         60 // In seconds.
#define  TIME_STAMP_SIGNAL_ON                   20
#define  TIME_STAMP_SIGNAL_COMPLETE             21

#define  KEEP_ALIVE_SEND_PROCESS_ON             33
#define  KEEP_ALIVE_SEND_PROCESS_OFF            44

#define  CHECK_FOR_QUERY_START                  45
#define  CHECK_FOR_QUERY_END                    46

// *****************************************************************************
// ***************** Below -> Global Variable Declarations *********************
// *****************************************************************************

unsigned int16 g_ui16StackerEmptyDelayCount           = 0;
unsigned int8  g_fDIChangeState                       = FLAG_DI_STATE_NOT_CHANGED;
unsigned int16 g_ui16MillisecondToSecondCounter       = 0;
unsigned int8  g_ui8SecondToMinuteCounter             = 0;
unsigned int16 g_ui16ShouldLookTimerCounter           = 0;
unsigned int8  g_fShouldSendData                      = SHOULD_NOT_SEND_DATA;
unsigned int16 g_ui16DataSendIntervalTime             = 0;
unsigned int8  g_ui8TSWriteIntrvlCounter              = 0;
unsigned int8  g_fTSWriteToEEPROMSignal               = TIME_STAMP_SIGNAL_COMPLETE;
unsigned int8  g_ui8KeepAliveMonitoringCounter        = 180;
unsigned int8  g_fKeepAliveSendStatus                 = KEEP_ALIVE_SEND_PROCESS_OFF;
unsigned int8  g_fKeepAliveQueryCheck                 = CHECK_FOR_QUERY_END;

// *****************************************************************************
// ***************** Above -> Global Variable Declarations *********************
// *****************************************************************************


//******************************************************************************
//**************** Below -> Timer Initialization Function **********************
//******************************************************************************

//Initialize 1MS Timer.
void Init1MSTimer(void)
{
	//setup timer0 for 1MS time.
	setup_timer_0( T0_INTERNAL | T0_DIV_PRESCALE );
	set_timer0(TIMER_0_VALUE_LOAD_FOR_1MS);

	//Enable timer0
	clear_interrupt( INT_TIMER0 );
	enable_interrupts( INT_TIMER0 );
}

#INT_TIMER0
void timer0_isr(void)
{   
    
	// Increase millisecond counter at each millisecond.
    g_ui16MillisecondToSecondCounter++;  
    
    
     // Check if millisecond counter overflows
    if(g_ui16MillisecondToSecondCounter >= MILLISECOND_TIMEOUT)  
    {
        // Increment counter to Sync with HMI Time to get proper timestamp.
        g_ui32UnixTimeInSec++;

        // Reset counter.
        g_ui16MillisecondToSecondCounter = 0;
        
        // 1000MS i.e. 1second completed.. put stuff here that repeats after every second.
        
        // Increase each second counter
        g_ui8SecondToMinuteCounter++;
        
        // If data should not send through GPRS then count data send interval time
        if(SHOULD_NOT_SEND_DATA == g_fShouldSendData)
        {
            // Increase data send interval time.
            g_ui16DataSendIntervalTime++;
        }
        
                
        // Check for interval timeout to write time-stamp into EEPROM.
        if(++g_ui8TSWriteIntrvlCounter >= TIME_STAMP_WRITE_INTERVAL_TIME)
        {
            // Reset counter.
            g_ui8TSWriteIntrvlCounter = 0;
            
            // Hook current unix time.
            g_ui32HookedUpTimeStampPerMinute = g_ui32UnixTimeInSec;
            
            // Set flag to save current unix time to EEPROM.
            g_fTSWriteToEEPROMSignal = TIME_STAMP_SIGNAL_ON;
        }
        
        // Decrease keep alive monitoring counter at each second.
        // and check if it meets zero level i.e. 180 seconds passed and no data in queue.
        // Time to send Keep Alive to Server.
        if(--g_ui8KeepAliveMonitoringCounter == 0)
        {
            // set flag to denote 180 seconds over.
            // Time to send keep alive.
            g_fKeepAliveSendStatus = KEEP_ALIVE_SEND_PROCESS_ON;
        }
    }   
    
    
    // Checking ISR received byte flag.
    if(ISR_IS_READY == g_fIsReadyToReceiveByte) 
    {
        // Increase the counter.
        g_ui8ISRReceiveByteTimeCounter++;
    }
    else if(MODBUS_IS_NOT_VALID_SLAVE_ID == g_fIsValidSlaveID)
    {
        // Increase counter.
        g_ui8UnexpectedSlaveIDIgnoreCounter++;
    }
    else
    {
        /* Do Nothing */
    }
     
    if(g_ui8UnexpectedSlaveIDIgnoreCounter >= ISR_TOTAL_8_BYTE_RECEIVE_TIME)
    {
        // Reset counter.
        g_ui8UnexpectedSlaveIDIgnoreCounter = 0;
        
        // Update flag status.
        g_fIsValidSlaveID = MODBUS_IS_VALID_SLAVE_ID;
        // Update Is received byte flag. 
        g_fIsReadyToReceiveByte =  ISR_IS_NOT_READY;
        
    } 
     
    if(g_ui8ISRReceiveByteTimeCounter > g_ui16ISRAllBytesExpectedRcvTime) 
    {
        // Update all received byte flag.
        g_fISRReceivedByteStatus = ISR_RECEIVED_ALL_BYTES;
        
        // Update Is received byte flag. 
        g_fIsReadyToReceiveByte =  ISR_IS_NOT_READY;
        
        // Reset Counter
        g_ui8ISRReceiveByteTimeCounter = 0;
    }    
    
    // ############################### Non Critical ###############################
    //----------------------------------------------------
    // IBU MSG ACK Receive Delay Start
    //----------------------------------------------------
    if (IBU_WAIT_FOR_ACK_TIMEOUT_ON == g_fIBUMsgAckDelayStatus)
    {
        g_ui16IBUWaitForACKTikCount++;
        if (g_ui16IBUWaitForACKTikCount >= DELAY_IBU_MSG_ACK_NOT_RCV)
        {
            g_ui16IBUWaitForACKTikCount = 0;
            
            // Reset flag denoting IBU is free now.
            g_fIBUSendBusy = IBU_SENDING_FREE;
            
            g_fIBUMsgAckDelayStatus = IBU_WAIT_FOR_ACK_TIMEOUT_OFF;
        }
    }
    //----------------------------------------------------
    // IBU MSG ACK Receive Delay End
    //----------------------------------------------------
    
    // Check if second counter overflows
    if(g_ui8SecondToMinuteCounter >= SECOND_TIMEOUT)
    {
        // Reset counter
        g_ui8SecondToMinuteCounter = 0;
        
        // 60Seconds i.e. 1 Minute completed.. put stuff here that repeats after every minute.
        
    }
    
    
    // Check if timer should look for timeout
    if(g_fShouldLookForTimeOut)
    {
        // Increase timer counter
        g_ui16ShouldLookTimerCounter++;
        
        // Check if timer counter reached timeout value
        if(g_ui16ShouldLookTimerCounter >= g_ui16TimeOutMilliseconds)
        {
            // Set flag denoting that timer declares set time is timed out
            g_fIsTimedOut = 1;
            
            // Reset variables
            g_fShouldLookForTimeOut      = 0;
            g_ui16ShouldLookTimerCounter = 0;
            g_ui16TimeOutMilliseconds    = 0;
        }
    }
        
    // Check if data send interval time reached timeout
    if(g_ui16DataSendIntervalTime >= g_ui8DataSendingIntervalTime)
    {
        // Update should send data flag
        g_fShouldSendData = SHOULD_SEND_DATA;
        
        // Reset variable
        g_ui16DataSendIntervalTime = 0;
    }
        
	// Reset Timer0.
    set_timer0( TIMER_0_VALUE_LOAD_FOR_1MS );
    clear_interrupt( INT_TIMER0 );	
}



//******************************************************************************
//**************** Above -> Timer Initialization Function **********************
//******************************************************************************




void ExecuteALLDOs(void)
{
//    delay_us(10);
//    output_bit(PIN_OUT_DO_00, g_unDOState.m_stAllDOBits.m_ui1HWDOCn00); // Solenoid opeartion.
//    //output_bit(PIN_OUT_DO_01, g_unDOState.m_stAllDOBits.m_ui1HWDOCn01); // Shipper rejection RED LED.
//    //output_bit(PIN_OUT_DO_02, g_unDOState.m_stAllDOBits.m_ui1HWDOCn02);
//    output_bit(PIN_OUT_DO_03, g_unDOState.m_stAllDOBits.m_ui1HWDOCn03); // Ready To Feed.
//    //output_bit(PIN_OUT_DO_04, g_unDOState.m_stAllDOBits.m_ui1HWDOCn04);
//    //output_bit(PIN_OUT_DO_05, g_unDOState.m_stAllDOBits.m_ui1HWDOCn05);
//    //output_bit(PIN_OUT_DO_06, g_unDOState.m_stAllDOBits.m_ui1HWDOCn06);
//    //output_bit(PIN_OUT_DO_07, g_unDOState.m_stAllDOBits.m_ui1HWDOCn07);   
}

#endif /* _VM_TIMER_CONFIG_H_ */