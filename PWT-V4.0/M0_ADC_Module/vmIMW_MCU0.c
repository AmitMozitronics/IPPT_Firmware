
#include "vmSystemConfig.h"
#include "../Common/vmIBUConfig.h"
#include "../Common/vmIBUGlobal.h"
#include "../Common/vmADS1232.h"
#include "../Common/vmEEPROMConfig.h"
#include "../Common/vmStateMachine.h"

#include "vmTimerConfig.h"
#include "mzPWTStateMachine.h"
#include "vmIBCProcessM0.h"
#include "vmAverageWeightCalculation.h"
#include "vmEEPROMConfig_M0.h"


void InitGlobalVariables(void)
{
////    g_ui16DIStatus = 0x1A;
////    g_ui16DOStatus = 0x1A;
////    g_ui16ErrorStatus = 0x1A;
////    g_ui8IncrementIndex = 0;
    
    // Set all pins digital.
    setup_adc_ports(NO_ANALOGS);
    
    // Initialize IBU Send Message Queue
    //InitializeDataSendQueue();
    InitializeDataQueue(&g_stIBURcvQueue);
    InitializeDataQueue(&g_stIBUSendQueue);
    // Initialize Average Weight Queue
    InitializeAvgWtQueue();
    
    // Read EEPROM data and store in respective registers.
    g_ui16PipePresenceDelayTime = ReadPipePresenceDelayFromEEPROM();
    g_ui16PipeStabilizationDelayTime = ReadPipeStabilizationDelayFromEEPROM();
    g_ui16PipeTransferDelayTime = ReadPipeTransferDelayTime();
    g_ui16WeighingCylinderUpActivationDelay = ReadWeighingCylinderActivationDelayfromEEPROM();
    
    // Wakeup ADS1232
    output_high(PIN_ADC_PDWN);
    output_high(PIN_ADC_SCLK);
    
    // Set Programmable Gain of ADS1232.
    // Settled GAIN is 64.
    output_high(PIN_ADC_GAIN_1);
    output_low(PIN_ADC_GAIN_0);
    
    // Clearing the latch buffer of ADS1232.
    int8 i8Count = 0;
    for(i8Count = 0; i8Count < 100; i8Count++)
    {
        delay_us(1);
        output_high(PIN_ADC_SCLK);
        delay_us(1);
        output_low(PIN_ADC_SCLK);
    }
    
                                      
    //Initialize state occurrence.
    g_ui8ExpectedNextState[0] = STATE_HOME;
    g_ui8ExpectedNextState[1] = STATE_HOME;
    
    // Initialize Batch Info 
    g_stCurBatchInfo.m_i32AvgWeight = 1000;
    g_stCurBatchInfo.m_i32MaxWt = 50;
    g_stCurBatchInfo.m_i32MinWt = 50;
    
    // Get the stored Load Cell Factor(LCF) from EEPROM.
    g_i16LoadCellFactor = ReadLCFFromEEPROM();

    // Convert decimal LCF into fraction.
    g_f32LoadCellFactor = (float32)g_i16LoadCellFactor / (float32)100;

    // Get Stored Max and Min weight of pipe from EEPROM.
    g_stCurBatchInfo.m_i32MaxWt = ReadMaxWeightFromEEPROM();
    g_stCurBatchInfo.m_i32MinWt = ReadMinWeightFromEEPROM();    
}

void InitSystemInterrupts(void)
{
    
     // Enable 1MS main timer.
    clear_interrupt(INT_TIMER0);
    enable_interrupts(INT_TIMER0);
    
    // Initialize UART-><- UART (IBU) receive Interrupt.
    InitIBURxInterrupt();

    // Initialize camera communication receive Interrupt.
    InitCamRxInterrupt();
        
}

void InitializeStateMachines()
{
    g_ui8NumberOfActiveMachines = NO_OF_ACTIVE_MACHINE;
    
    g_ui32AllDOsCurrentState            = INITAIL_DO_STATE_ON_PWR_ON;
    g_ui32AllDOsPreviousState           = INITAIL_DO_STATE_ON_PWR_ON;
    g_ui32AllCountersCurrentState       = 0;
    g_ui32AllCountersPreviousState      = 0;
    
    //Initialize_StateMachine_0();
    //Initialize_StateMachine_1();

}    


void main()
{
	// Set Watch Dog Timer ON.
    setup_wdt(WDT_4S | WDT_WINDOW_100_PERCENT);
    
    // Initialization Section Start
    //int8 i8StateMachineIndex = 0;
    
    // Initialize all kind of system requirements.
    InitGlobalVariables();
    InitSystemInterrupts();
    //InitializeStateMachines(); //TODO Must go to Pre Production Mode
    //InitilizeStates();
    delay_ms(10);
    
    // Initialize 1MS main timer i.e.=>TIMER0.
    InitDigitalIOPollingTimer();
    
    // Initialize Receive Queue
    InitializeRcvQ();
      
    // Enable Global Interrupts.
    enable_interrupts(GLOBAL);   
    
    
    // TESTING Start.
    ST_IBU_DATA_NODE stIBUData;

    // Set system mode at start up.
    g_ui8SystemMode = SYS_MODE_WAKE_UP;
  
    // Set Rejection State ON.
    g_unDIState.m_stAllDIBits.m_ui1GDICn01  = PIPE_REJECTED;
    g_unDIState.m_stAllDIBits.m_ui1GDICn02  = 1;
    g_unDOState.m_stAllDOBits.m_ui1HWDOCn01 = 1;
    
   // Reset WDT.
	restart_wdt();
    
    for(;;)
    {
        
        // Step 1: Check if there is any message in IBU message queue to send through
        //         UART.
        if((IBU_SENDING_FREE == g_fIBUSendBusy) &&
            IsMSGInIBUQueue(&g_stIBUSendQueue))
        {
            // Set flag to denote IBU is busy in sending data.
            g_fIBUSendBusy = IBU_SENDING_BUSY;
            
            // Send data from queue.
            SendIBUMSGFromQ();
        }
        

        // step 2: Check if there is any message received in IBU through UART.
        //if ((g_ui8RCVQRear - g_ui8RCVQFront) >= SIZE_IBU_COMPLETE_SEND_MSG - 1)
        g_ui8ElementInQ = ElementsInQ();
        if (g_ui8ElementInQ >= SIZE_IBU_COMPLETE_SEND_MSG)
        {
            // Hopefully a Complete Message Received
            CheckIBUReceiveQ();
        }        

        // step 3: Check if there is any message in IBC Receive Queue
        if (IsMSGInIBUQueue(&g_stIBURcvQueue))
        {
            // Process IBC 
            ProcessIBUMsg();
        }
        
        // Step 4: Check if ADC is ready to read data.
        if (0 == input(PIN_ADC_DOUT_DRDY)) //&& IBU_SEND_UNLOCK == g_fIBUSendLockStatus)
        {
            g_ui32ADCValue = read();

            if (g_ui32ADCValue)
            {
                g_fADCDataReadyStatus = ADC_DATA_READY;
            }
        }
        
        // step 5: Check flag state of current air-pressure level.
        if(AIR_PRESSURE_LEVEL_BELOW_RATED == g_fCurrentAirPressureLevel &&
           g_ui8AirPressureAlarmSendCount == 0)
        {
            // Send alarm signal to MCU-1 through IBU.
            SendAlarmtoAnotherMCU();
            
            // Set send count.
            g_ui8AirPressureAlarmSendCount = 1;
        }
        else if(AIR_PRESSURE_LEVEL_ABOVE_RATED == g_fCurrentAirPressureLevel &&
                g_ui8AirPressureAlarmSendCount == 1)
        {
            // Send reset alarm signal to MCU-1 through IBU.
            SendResetAlarmtoAnotherMCU();
            
            // Reset alarm send count.
            g_ui8AirPressureAlarmSendCount = 0;
        }
        else
        {
            /* Do Nothing */
        }
        
        
        if (g_ui8CurrentState > STATE_CALIB_STEP_10)
        {
            g_ui8CurrentState = STATE_CALIB_STEP_1;
        }
        // Step 6: Mode Scanner
        switch(g_ui8SystemMode) //g_ui8AllBoardCurrentMode
        {
            // Enter once at wake up stage or after reset.
            case SYS_MODE_WAKE_UP:
            {
                // Set system mode at home state.
                g_ui8SystemMode = SYS_MODE_HOME;
            }
            break;
            case SYS_MODE_HOME:
            {

            }
            break;
            case SYS_MODE_PRE_PROD:
            {

            }
            break;
            case SYS_MODE_PRE_PROD_SCRAP:
            {
                // Nothing to do
                g_ui8SystemMode = g_ui8SystemMode;
            }
            break;
            case SYS_MODE_CALIB:  // Intentional Fall through
            case SYS_MODE_PROD:
            {
                //PWTStateMachine();
                // Check if air-pressure is above rated 
                // level.
                if(AIR_PRESSURE_LEVEL_ABOVE_RATED == g_fCurrentAirPressureLevel)
                {
                    PWTStateMachine();                
                }
                else  // Air-pressure is below rated level.
                {
                    /* State Machine will not run.*/
                }
            }
            break;
        }
        // Reset WDT.
        restart_wdt();
    }
}


