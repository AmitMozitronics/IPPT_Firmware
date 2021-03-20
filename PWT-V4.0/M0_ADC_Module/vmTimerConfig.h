
#ifndef _VM_TIMER_CONFIG_H_
#define _VM_TIMER_CONFIG_H_

#include "vmSystemConfig.h"
#include "vmStateMachineForIMW_M0.h"
//#include "vmDigitalIOConfig.h"
#include "vmADCManager.h"
#include "../Common/vmIBUConfig.h"


// PWT Start
#define PIPE_PRESENT      0
#define PIPE_NOT_PRESENT  1
#define DELAY_STABILIZE_PIPE_PRESENSE  500   // INPUT From HMI
#define DELAY_PIPE_SENSE_DI_STATE      500   // INPUT From HMI
#define DELAY_PIPE_STABILIZATION       1500  // INPUT From HMI
#define DELAY_PIPE_TRANSFER            3000  // INPUT From HMI

#define FLAG_PIPE_PRESENCE_DELAY_ON              1
#define FLAG_PIPE_PRESENCE_DELAY_OFF             0
#define FLAG_PIPE_STABILIZATION_DELAY_ON         1
#define FLAG_PIPE_STABILIZATION_DELAY_OFF        0

unsigned int16 g_ui16DOToggleTikCnt         = 0;
//unsigned int16 g_ui16DlyPipeDIStateTikCnt   = 0;
unsigned int16 g_ui16DlyStabilizationTikCnt  = 0;
unsigned int16 g_ui16DlyPipePresenceTikCnt   = 0; // Pipe Presence Tick Count

unsigned int8  g_ui16DlyPipePresenceState  = FLAG_PIPE_PRESENCE_DELAY_OFF;
unsigned int8  g_ui8DlyStabilizationState  = FLAG_PIPE_STABILIZATION_DELAY_OFF;

unsigned int8  g_ui8PipePresence = PIPE_NOT_PRESENT;

unsigned int16 g_ui16DelayStabilizePipePresense = DELAY_STABILIZE_PIPE_PRESENSE;

unsigned int16 g_ui16DelayFlexi = g_ui16PipeStabilizationDelayTime;
// PWT End

#define T0_DIV_PRESCALE                 RTCC_DIV_8
#define TIMER_0_VALUE_LOAD_FOR_1MS      (0xFFFF - 0x0271 + 1)
#define ALL_TIMER_TEST_VAL_FOR_100_US   (0xFFFF - 0x003F + 1)
#define ALL_TIMER_TEST_VAL_FOR_70_US    (0xFFFF - 0x002C + 1)


#define FLAG_DI_STATE_NOT_CHANGED    1
#define FLAG_DI_STATE_CHANGED        0
#define ADC_READ_ON                  10
#define ADC_READ_OFF                 11

//#define DELAY_PUSHER               400
//#define DELAY_CAN_STPR_1           2000
//#define DELAY_CAN_PUSHER_1_TIMER_2 500
//#define DELAY_START_CAN_PLACER     20
#define DELAY_INTRMDAT_DATA_SEND     300
#define DELAY_REJECT_PUSHER_BACK     1000
#define PUSHER_DELAY_START_ON          0
#define PUSHER_DELAY_START_OFF         1

#define BOX_LEFT_DELAY_START           0
#define BOX_LEFT_DELAY_END             1
#define AFTER_BOX_LEFT_DELAY_TIME      200
#define LOCK_DATA_SENDING              10
#define UNLOCK_DATA_SENDING            11

#define WAITING_STOP                   9
#define WAITING_START                  10
#define WAITING_TIME                   2000 // In milliseconds.
#define WEIGHING_ON                    12
#define WEIGHING_OFF                   13
#define WEIGHING_CALC                  14
#define HOME_MODE_WAITING              3000

#define AIR_PRESSURE_LEVEL_BELOW_RATED  10
#define AIR_PRESSURE_LEVEL_ABOVE_RATED  11

// TESTING Start
#define DELAY_GET_ADC_DATA           500
unsigned int16 g_uiCountSendADCData = 0;
// TESTING End


unsigned int8  g_fDIChangeState =  FLAG_DI_STATE_NOT_CHANGED;
unsigned int8  g_fADCCurrState  =  ADC_READ_OFF;
unsigned int16 g_ui16IntmidDataSendTikCount = 0;
unsigned int16 g_uiPusherDelayTikCount      = 0;
int8 g_fPusherDelayStatus     = PUSHER_DELAY_START_OFF;
int8 g_fRejectPusherPrevState = 1;
//unsigned int8  g_fBeforeHomeStateDelayStatus = BOX_LEFT_DELAY_END;
unsigned int16  g_ui16AfterBoxLeftDelayTimeCounter = 0;
unsigned int8   g_fBoxLeftDelayState = BOX_LEFT_DELAY_END;
unsigned int8   g_fExitTimeDataSendingStatus = UNLOCK_DATA_SENDING;
unsigned int8   g_fCurrentAirPressureLevel   = AIR_PRESSURE_LEVEL_ABOVE_RATED;


unsigned int8   g_fWaitingStatus             = WAITING_STOP;
unsigned int8   g_fWeighingStatus            = WEIGHING_OFF;
unsigned int8   g_fHomeWaitingStatus         = WAITING_STOP;
unsigned int8   g_fTareStartStatus           = WAITING_STOP;
unsigned int16  g_ui16TimingCounter          = 0;
unsigned int16  g_ui16TickCounter             = 0;
unsigned int8   g_ui8AirPressureAlarmSendCount = 0;
unsigned int16  g_ui16WeighingCylinderActuationDelayCnt = 0;

//*****************************************************************************
// Below -> 1MS Timer Initialization Functions ********************************
//*****************************************************************************
// Init Polling Timer.
void InitDigitalIOPollingTimer(void)
{
    // Setup Timer0 at 1 ms Time.
    setup_timer_0( RTCC_INTERNAL | T0_DIV_PRESCALE );
    set_timer0( TIMER_0_VALUE_LOAD_FOR_1MS );  // TODO => Need to change the value load in timer
    // Enable Timer0.
    clear_interrupt( INT_TIMER0 );
    enable_interrupts( INT_TIMER0 );
}

//*****************************************************************************
// Above -> 1MS Timer Initialization Functions ********************************
//*****************************************************************************


     


//*****************************************************************************
// Below -> 1MS Timer Implementation ******************************************
//*****************************************************************************
unsigned int8 g_ui8PipeSensorState              = 1;
unsigned int8 g_ui8LeftSensorState              = 1;
unsigned int8 g_ui8RightSensorState             = 1;
unsigned int8 g_ui8AirPressureSensorState       = 1;
unsigned int8 g_ui8PipeSensorPrvState           = 1;
unsigned int8 g_ui8LeftWeighingCylinderState    = 1;
unsigned int8 g_ui8RightWeighingCylinderState   = 1;
unsigned int8 g_ui8WeighingCylinderState        = 1;

#INT_TIMER0
void DigitalIOPolling_isr(void)
{

    if (g_ui16DOToggleTikCnt++ > 500)
    {
        g_ui16DOToggleTikCnt = 0;
    }
    
    // Digital Input (DI) Polling Start for Pipe Sensor
    g_ui8LeftSensorState            = input(PIN_IN_DI_00);
    g_ui8RightSensorState           = input(PIN_IN_DI_01);
    g_ui8AirPressureSensorState     = input(PIN_IN_DI_02);
    g_ui8LeftWeighingCylinderState  = input(PIN_IN_DI_03);
    g_ui8RightWeighingCylinderState = input(PIN_IN_DI_04);
    
    // Merging both sensor state.
    g_ui8PipeSensorState = (g_ui8LeftSensorState & g_ui8RightSensorState);
    
    // Check If there is any State Change
    if (g_ui8PipeSensorPrvState != g_ui8PipeSensorState)
    {
        // Pipe Sensor State Changed
        g_ui16DlyPipePresenceTikCnt  = 0;
        g_ui16DlyPipePresenceState   = FLAG_PIPE_PRESENCE_DELAY_ON;
        
        // Check How long it will lasts
        // Update the Previous State
        g_ui8PipeSensorPrvState = g_ui8PipeSensorState;
    }
    
    // Merging both side weighing cylinder state.
    g_ui8WeighingCylinderState = (g_ui8LeftWeighingCylinderState | g_ui8RightWeighingCylinderState);
    
    // Check if both side weighing cylinder stretched full stroke.
    if(!g_ui8WeighingCylinderState)
    {
        // Check if the state continues for certain time to 
        // ensure it is a false trigger or not.
        if(++g_ui16WeighingCylinderActuationDelayCnt == g_ui16WeighingCylinderUpActivationDelay)                           // g_ui16WeighingCylinderUpActivationDelay
        {
            // Reset counter.
            //g_ui16WeighingCylinderActuationDelayCnt = 0;
            
            // Weighing cylinder stretched full stroke.
            // Trigger Production toNext stage.
            if (SYS_MODE_CALIB == g_ui8SystemMode)
            {
                // Go to the Next State
                g_ui8CurrentState++;
                g_ui8StateWaiting = STATE_NOTHING;
            }
            else if (SYS_MODE_PROD == g_ui8SystemMode)
            {
                // Go to the Next State
                g_ui8CurrentState++;
                g_ui8StateWaiting = STATE_NOTHING;
            }
            else
            {
                /* Do Nothing */
            }
        }
        else
        {
            /* Do nothing */
        }
        
    }
    else
    {
        // Reset counter.
        g_ui16WeighingCylinderActuationDelayCnt = 0;
    }
    
    // Check if DI from pressure switch goes low.
    if(1 == g_ui8AirPressureSensorState)
    {
        // Check if it continues for One second.
        // If yes; then it is a valid indication.
        // Else ignore this.
        if(++g_ui16TickCounter >= 1000)
        {
            // Check DI state again.
            if(1 == g_ui8AirPressureSensorState)
            {
                // Set flag to denote, pressure switch 
                // indicating air-pressure goes below 
                // from rated level.
                g_fCurrentAirPressureLevel = AIR_PRESSURE_LEVEL_BELOW_RATED;
                
                // Reset counter.
                g_ui16TickCounter = 0;
            }
        }
    }
    else // DI state goes up.
    {
        // Was not a valid indication.
        // False trigger.
        // Reset tick counter.
        g_ui16TickCounter = 0;
        
        // Unmark the set flag for air-pressure 
        // goes low from rated level.
        // This will work after corrective action will be
        // taken for rated air-pressure achievement.
        g_fCurrentAirPressureLevel = AIR_PRESSURE_LEVEL_ABOVE_RATED;
    }

       
    // Step 3: Check if the ADC is ready with data or not
    // Check if ADC is ready
    if (ADC_SCAN_ON    == g_ui8ADCScanningStatus &&
        ADC_DATA_READY == g_fADCDataReadyStatus)
    {
        // Increment the Counter
        g_i16ADCArrIdx++;
        //g_stADCDataQ.m_i16Rear++;
        if (g_i16ADCArrIdx >= SIZE_ADC_DATA_QUEUE)
        {
            // Stop ADC Scanning
            g_ui8ADCScanningStatus = ADC_SCAN_OFF;
            // ADC Data Queue overflowed
            if (SYS_MODE_CALIB == g_ui8SystemMode)
            {
                // Trigger Calibration Next stage
                g_ui8CurrentState++;
                g_ui8StateWaiting = STATE_NOTHING;
            }
            if (SYS_MODE_PROD == g_ui8SystemMode)
            {
                // Trigger Production Next stage
                g_ui8CurrentState++;
                g_ui8StateWaiting = STATE_NOTHING;
            }
        }
        else
        {
            //g_stADCDataQ.m_ui32Arr[g_stADCDataQ.m_i16Rear] = g_ui32ADCValue;
            g_ui32ADCArr[g_i16ADCArrIdx] = g_ui32ADCValue;
        }

        // Reset Temp ADC Variable
        g_ui32ADCValue = 0;
        // Reset Data Ready Status
        g_fADCDataReadyStatus = ADC_DATA_NOT_READY;
    }
    
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
    //----------------------------------------------------
    // Start : Check for Pipe Sense Time
    //----------------------------------------------------
    if (FLAG_PIPE_PRESENCE_DELAY_ON == g_ui16DlyPipePresenceState)
    {
        g_ui16DlyPipePresenceTikCnt++;
        if (g_ui16DlyPipePresenceTikCnt >= g_ui16PipePresenceDelayTime)
        {
            g_ui16DlyPipePresenceTikCnt = 0;
            
            // Reset flag denoting IBU is free now.
            //g_fIBUSendBusy = IBU_SENDING_FREE;
            if (0 == g_ui8PipeSensorState)
            {
                g_ui8PipePresence = PIPE_PRESENT;
            }
            else if (1 == g_ui8PipeSensorState)
            {
                g_ui8PipePresence = PIPE_NOT_PRESENT;
            }
            else
            {
                // Do Nothing
            }
            
            g_ui16DlyPipePresenceState = FLAG_PIPE_PRESENCE_DELAY_OFF;
        }
    }
    //----------------------------------------------------
    // End : Check for Pipe Sense Time
    //----------------------------------------------------    
    //----------------------------------------------------
    // Start : Check for Pipe Stabilization Delay
    //----------------------------------------------------
    if (FLAG_PIPE_STABILIZATION_DELAY_ON == g_ui8DlyStabilizationState)
    {
        g_ui16DlyStabilizationTikCnt++;
        if (g_ui16DlyStabilizationTikCnt >= g_ui16DelayFlexi)
        {
            g_ui16DlyStabilizationTikCnt = 0;
            g_ui8DlyStabilizationState = FLAG_PIPE_STABILIZATION_DELAY_OFF;
            
            if (SYS_MODE_CALIB == g_ui8SystemMode)
            {
                // Go to the Next State
                g_ui8CurrentState++;
                g_ui8StateWaiting = STATE_NOTHING;
            }
            else if (SYS_MODE_PROD == g_ui8SystemMode)
            {
                // Go to the Next State
                g_ui8CurrentState++;
                g_ui8StateWaiting = STATE_NOTHING;
            }
        }
    }
    //----------------------------------------------------
    // End : Check for Pipe Stabilization Delay
    //----------------------------------------------------

    
    // Reset Timer0.
    set_timer0( TIMER_0_VALUE_LOAD_FOR_1MS );  // TODO => 
    clear_interrupt( INT_TIMER0 );
}


//*****************************************************************************
// Above -> 1MS Timer Implementation ******************************************
//*****************************************************************************

#endif /* _VM_TIMER_CONFIG_H_ */

