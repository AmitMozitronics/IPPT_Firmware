/* 
 * File:   mzPWTStateMachine.h
 * Author: Amit
 *
 * Created on December 29, 2019, 8:12 PM
 */

#ifndef MZ_PWT_STATEMACHINE_H
#define	MZ_PWT_STATEMACHINE_H

#include "vmM0Common.h"
#include "vmTimerConfig.h"

int i = 0;
void PWTStateMachine()
{
    switch (g_ui8CurrentState)
    {
        case STATE_CALIB_STEP_1:
        {
            if (STATE_CALIB_STEP_WAITING == g_ui8StateWaiting)
            {
                break;
            }
            // Check if the Pipe Present or not
            if (PIPE_PRESENT == g_ui8PipePresence)
            {
                // Wait for a while to stabilize the pipe
                g_ui8DlyStabilizationState = FLAG_PIPE_STABILIZATION_DELAY_ON;
                g_ui16DelayFlexi = g_ui16PipeStabilizationDelayTime;
                g_ui8StateWaiting = STATE_CALIB_STEP_WAITING;
            }
        }
        break;
        case STATE_CALIB_STEP_2:
        {
            if (STATE_CALIB_STEP_WAITING == g_ui8StateWaiting)
            {
                break;
            }
            else if (STATE_CALIB_STEP_2 == g_ui8CurrentState)
            {
                // Start Filling up the Tare Array of 200 samples
                // Step 1: Start Reading the ADC Data for Tare
                g_i16ADCArrIdx = -1;

                // Step 2: Start ADC Scanning
                g_ui8ADCScanningStatus = ADC_SCAN_ON;

                // Wait for ADC Array to fill completely
                // It will be triggered from ADC Filling
                g_ui8StateWaiting = STATE_CALIB_STEP_WAITING;
            }
            else
            {
                // Do Nothing
            }
        }
        break;
        case STATE_CALIB_STEP_3:
        {
            if (STATE_CALIB_STEP_WAITING == g_ui8StateWaiting)
            {
                break;
            }
            else if (STATE_CALIB_STEP_3 == g_ui8CurrentState)
            {
                // Calculate the Average
                g_ui32ADCCurSum = 0;
                for(g_ui16ADCSumLoopCount = 0; g_ui16ADCSumLoopCount < SIZE_ADC_DATA_QUEUE; g_ui16ADCSumLoopCount++)
                {
                    g_ui32ADCCurSum += g_ui32ADCArr[g_ui16ADCSumLoopCount];
                    g_ui32ADCArr[g_ui16ADCSumLoopCount] = 0;
                }
                if (!g_ui32ADCCurSum )
                {
                    // Sum is 0, so update with the previous tare value
                    g_ui32ADCCurMeanValue = g_ui32TarePrev;
                }
                else
                {
                    g_ui32ADCCurMeanValue = g_ui32ADCCurSum / SIZE_ADC_DATA_QUEUE;
                    g_ui32TarePrev = g_ui32ADCCurMeanValue;
                }
                g_ui8CurrentState++;
                g_ui8StateWaiting = STATE_NOTHING;
            }
            else
            {
                // Do Nothing
            }

        }
        break;
        case STATE_CALIB_STEP_4:
        {
            if (STATE_CALIB_STEP_WAITING == g_ui8StateWaiting)
            {
                break;
            }
            else if (STATE_CALIB_STEP_4 == g_ui8CurrentState)
            {
                // Tare Calculation Done
                // Step 1 : Store the ADV Mean Value to Tare Value
                g_ui32DynamicTareADCValue = g_ui32ADCCurMeanValue;
                // Step 2 : Calculate the Calibrated Tare Weight
                g_ui32DynamicTareValue = GetCalibratedWeight(g_ui32DynamicTareADCValue);
                // Step 3: Send the Tare Value to IBU
                IBUSendTareData(g_ui32DynamicTareValue);

                output_bit(PIN_OUT_DO_00, 0);
                // Wait for a while to stabilize the pipe
                //g_ui8DlyStabilizationState = FLAG_PIPE_STABILIZATION_DELAY_ON;
                //g_ui16DelayFlexi = g_ui16PipeStabilizationDelayTime;
                g_ui8StateWaiting = STATE_CALIB_STEP_WAITING;
            }
            else
            {
                // Do Nothing
            }
        }
        break;
        case STATE_CALIB_STEP_5:
        {
            if (STATE_CALIB_STEP_WAITING == g_ui8StateWaiting)
            {
                break;
            }
            else if (STATE_CALIB_STEP_5 == g_ui8CurrentState)
            {
                // Pipe got stabilized now start Filling ADC Data Array
                // Step 1: Start Reading the ADC Data for Pipe Weight
                ////ResetADCDataQ(COLLECT_WEIGHING_DATA);
                g_i16ADCArrIdx = -1;
                //memset(g_ui32ADCArr, 0, SIZE_ADC_DATA_QUEUE);
                // Step 2: Start ADC Scanning
                g_ui8ADCScanningStatus = ADC_SCAN_ON;
                // Step 3: Wait until the step completes
                g_ui8StateWaiting = STATE_CALIB_STEP_WAITING;
            }
            else
            {
                // Do Nothing
            }
        }
        break;
        case STATE_CALIB_STEP_6:
        {
            if (STATE_CALIB_STEP_WAITING == g_ui8StateWaiting)
            {
                break;
            }
            else if (STATE_CALIB_STEP_6 == g_ui8CurrentState)
            {
                // Calculate the Average
                g_ui32ADCCurSum = 0;
                for(g_ui16ADCSumLoopCount = 0; g_ui16ADCSumLoopCount < SIZE_ADC_DATA_QUEUE; g_ui16ADCSumLoopCount++)
                {
                    g_ui32ADCCurSum += g_ui32ADCArr[g_ui16ADCSumLoopCount];
                    g_ui32ADCArr[g_ui16ADCSumLoopCount] = 0;
                }
                if (!g_ui32ADCCurSum)
                {
                    // Sum is 0, so update with the previous pipe weight value
                    g_ui32ADCCurMeanValue = g_ui32PipeWtPrev;
                }
                else
                {
                    g_ui32ADCCurMeanValue = g_ui32ADCCurSum / SIZE_ADC_DATA_QUEUE;
                    g_ui32PipeWtPrev = g_ui32ADCCurMeanValue;
                }
                //g_ui32ADCCurMeanValue = g_ui32ADCCurSum / SIZE_ADC_DATA_QUEUE;
                g_ui8CurrentState++;
                g_ui8StateWaiting = STATE_NOTHING;
            }
            else
            {
                // Do Nothing
            }
        }
        break;
        case STATE_CALIB_STEP_7:
        {
            if (STATE_CALIB_STEP_WAITING == g_ui8StateWaiting)
            {
                break;
            }
            else if (STATE_CALIB_STEP_7 == g_ui8CurrentState)
            {
                if (SYS_MODE_CALIB == g_ui8SystemMode)
                {
                    // IN Calibration Mode We are sending Two Values to HMI
                    // Dynamic Tare ADC Value and Actual Weight ADC Values
                    IBUSendCalibResetData(g_ui32ADCCurMeanValue);
                }
                else if (SYS_MODE_PROD == g_ui8SystemMode)
                {
                    // Weighing Calculation Done
                    // Step 1: Calculate the Pipe Weight ADC Value.
                    // Check if current weight is a valid weight of any proper pipe.
                    if(g_ui32ADCCurMeanValue > g_ui32DynamicTareADCValue)
                    {
                        // It is a valid weight of pipe.
                        // Calculate weight.
                        g_ui32CurPipeADCValue = g_ui32ADCCurMeanValue - g_ui32DynamicTareADCValue;
                        // Step 2: Calculate the Actual Weight.
                        g_ui32CurPipeWeight = GetCalibratedWeight(g_ui32CurPipeADCValue);
                    }
                    else  // Erroneous ADC value arrived. weight will be zero to indicate.
                    {
                        // set zero weight to indicate erroneous operation.
                        g_ui32CurPipeWeight = 0;
                    }
                    
                    // Step 3: Send the Actual Weight to HMI
                    IBUSendWeighingData(g_ui32CurPipeWeight);
                }
                else
                {
                    /* Do Nothing */
                }
                     
                g_ui8CurrentState++;
                g_ui8StateWaiting = STATE_NOTHING;                
                                
            }
            else
            {
                // Do Nothing
            }
        }
        break;
        case STATE_CALIB_STEP_8:
        {
            if (STATE_CALIB_STEP_WAITING == g_ui8StateWaiting)
            {
                break;
            }
            else if (STATE_CALIB_STEP_8 == g_ui8CurrentState)
            {
                // Step 1: Pull Down the Weighing Pusher
                output_bit(PIN_OUT_DO_00, 1);
                // Step 2: Wait for a While
                g_ui8DlyStabilizationState = FLAG_PIPE_STABILIZATION_DELAY_ON;
                g_ui16DelayFlexi = g_ui16PipeStabilizationDelayTime;
                // Wait until the step completes
                g_ui8StateWaiting = STATE_CALIB_STEP_WAITING;
            }
            else
            {
                // Do Nothing
            }
        }
        break;
        case STATE_CALIB_STEP_9:
        {
            if (STATE_CALIB_STEP_WAITING == g_ui8StateWaiting)
            {
                break;
            }
            else if (STATE_CALIB_STEP_9 == g_ui8CurrentState)
            {
                // Step 1: Pull up the Pipe Transfer Pusher.
                output_bit(PIN_OUT_DO_01, 0);
                // Reset weighing cylinder up activation delay counter.
                g_ui16WeighingCylinderActuationDelayCnt = 0;                
                // Step 2: Wait for a While
                g_ui8DlyStabilizationState = FLAG_PIPE_STABILIZATION_DELAY_ON;
                g_ui16DelayFlexi = g_ui16PipeTransferDelayTime;
                // Wait until the step completes
                g_ui8StateWaiting = STATE_CALIB_STEP_WAITING;
                
                // Reset Indicating Tower light(If any)
                output_bit(PIN_OUT_DO_04, 1);
                output_bit(PIN_OUT_DO_03, 1);                
            }
            else
            {
                // Do Nothing
            }
        }
        break;
        case STATE_CALIB_STEP_10:
        {
            if (STATE_CALIB_STEP_WAITING == g_ui8StateWaiting)
            {
                break;
            }
            else if (STATE_CALIB_STEP_10 == g_ui8CurrentState)
            {
                // Step 1: Pull down the Pipe Transfer Pusher
                //output_high(PIN_OUT_DO_01);
//                for (i = 0; i < 10; i++)
//                {
//                    output_bit(PIN_OUT_DO_01, 1);
//                    delay_ms(1);
//                }
                output_bit(PIN_OUT_DO_01, 1);
                // Step 2: Loop back to Step 1
                g_ui8CurrentState = STATE_CALIB_STEP_1;
                //g_ui8StateWaiting = STATE_CALIB_STEP_WAITING;
                g_ui8PipePresence = PIPE_NOT_PRESENT;
            }
            else
            {
                // Do Nothing
            }

        }
        break;
        default:
        {
            // Nothing to do
        }
        break;
    }
}

#endif	/* MZ_PWT_STATEMACHINE_H */

