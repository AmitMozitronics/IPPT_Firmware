/* 
 * File:   vmIBUProcessM0.h
 * Author: Amit
 *
 * Created on December 9, 2018, 6:17 PM
 */

#ifndef VM_IBU_PROCESS_M0_H
#define	VM_IBU_PROCESS_M0_H

#include "../Common/vmIBUConfig.h"
#include "vmM0ADCModuleCommon.h"
#include "vmAverageWeightCalculation.h"
#include "vmEEPROMConfig_M0.h"



void ProcessIBUMsg(void)
{
    int8 i8LoopCount = 0;
    ST_IBU_DATA_NODE stIBURcvNode;
    
    // Get Node from IBU Receive Message Queue
    DeleteIBUMsgQueue(&g_stIBURcvQueue, &stIBURcvNode);
    // There is some valid message in IBU Receive Buffer
    // Try to process that message
    switch (stIBURcvNode.g_ui8ArrIBUData[0])
    {
        case IBU_CMD_MODE_HOME:
            g_ui8SystemMode = SYS_MODE_HOME;
            break;
            
        case IBU_CMD_MODE_CALIB:
            g_ui8SystemMode = SYS_MODE_CALIB;
            g_ui8CurrentState = STATE_CALIB_STEP_1;
            g_ui8StateWaiting = STATE_NOTHING;
            break;
            
        case IBU_CMD_MODE_PRE_PROD:
            g_ui8SystemMode = SYS_MODE_PRE_PROD;
            break;  
        
        case IBU_CMD_MODE_PROD:
            g_ui8SystemMode = SYS_MODE_PROD;
            g_ui8CurrentState = STATE_CALIB_STEP_1;
            g_ui8StateWaiting = STATE_NOTHING;
            break;
            
        case IBU_CMD_MODE_POST_PROD:
            g_ui8SystemMode = SYS_MODE_POST_PROD;
            break;
            
        case IBU_CMD_MODE_CONFIG:
            g_ui8SystemMode = SYS_MODE_CONFIG;
            break;            
    
        case IBU_CMD_SET_LOAD_CELL_FACTOR:
           
            // Receive current load cell factor.
            for (i8LoopCount = 1; i8LoopCount < IBU_MSG_BYTE_COUNT; i8LoopCount++)
            {
                g_unIBUCurrLoadCellFactor.m_ui8Arr[i8LoopCount - 1] = stIBURcvNode.g_ui8ArrIBUData[i8LoopCount];
            }
            
            //g_stCurBatchInfo.m_i16AvgWeight = g_unIBUAvgNode.m_stIBUAvgNode.m_i16AvgWt;
            // Store current LCF(Load Cell Factor) in RAM.
            g_i16LoadCellFactor = (int16)g_unIBUCurrLoadCellFactor.m_stLoadCellFactor.m_ui16CurrLoadCellFactor;
            
            // Store the LCF in EEPROM to retrieve data on system restart.
            WriteLCFToEEPROM(g_i16LoadCellFactor);
            
            // Convert decimal load cell factor into fraction.
            g_f32LoadCellFactor = (float32)g_i16LoadCellFactor / (float32)100;
            
            break;

        case IBU_CMD_START_CALIBRATION:
            // Set flag to notify state_machine_0 that calibration mode is ON.
            ////////g_fResetCalibStatus = IBU_RESET_CALIB_ON;
            g_ui8SystemMode = SYS_MODE_CALIB;
            g_ui8CurrentState = STATE_CALIB_STEP_1;
            g_ui8StateWaiting = STATE_NOTHING;
            break;
        case IBU_CMD_PRE_PROD_SCRAP_MODE:
            // Set flag to notify state_machine_0 that calibration mode is ON.
            g_ui8SystemMode = SYS_MODE_PRE_PROD_SCRAP;
            break;
        case IBU_CMD_TARE_REQUEST:
            //ResetCalibTable(g_ui16DynamicTareValue);
            break;
        case IBU_CMD_PRE_PROD_TOLERANCE:
            // Copy the Message for Parsing
            for (i8LoopCount = 1; i8LoopCount < IBU_MSG_BYTE_COUNT; i8LoopCount++)
            {
                g_unIBUTolNode.m_ui8Arr[i8LoopCount - 1] = stIBURcvNode.g_ui8ArrIBUData[i8LoopCount];
            }
            
            // Store the received data in respective register.
            g_stCurBatchInfo.m_i32MaxWt = g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt;
            g_stCurBatchInfo.m_i32MinWt = g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt;
            
            // Send acknowledgement to HMI.
            // a. prepare IBU data node.
            g_stIBUSendData.g_ui8ArrIBUData[0] = IBU_CMD_PRE_PROD_TOL_STORE_ACK;
            
            // b. Stuff data; here padding.
            for (i8LoopCount = 1; i8LoopCount < IBU_MSG_BYTE_COUNT; i8LoopCount++)
            {
                g_unIBUTolNode.m_ui8Arr[i8LoopCount - 1] = 0;
            }

            // c. Insert data in IBU sent message queue.
            InsertIBUMsgQueue(&g_stIBUSendQueue, &g_stIBUSendData);
            
            // Store data in EEPROM.
            WriteMaxWeightToEEPROM(g_stCurBatchInfo.m_i32MaxWt);
            WriteMinWeightToEEPROM(g_stCurBatchInfo.m_i32MinWt);
            break;
        case IBU_CMD_START_PROD:
            break;
        case IBU_CMD_STORE_PROD_DELAY_TIMERS:
            // Copy the data.
            for (i8LoopCount = 1; i8LoopCount < IBU_MSG_BYTE_COUNT; i8LoopCount++)
            {
                g_unProdDelayTimers.m_ui8Arr[i8LoopCount - 1] = stIBURcvNode.g_ui8ArrIBUData[i8LoopCount];
            }

            // Check the message header.     1. Pipe presence delay 2. Pipe stabilization delay 3. Pipe transfer delay.
            if(PIPE_PRESENCE_DELAY == g_unProdDelayTimers.m_stProdDelayTimers.m_ui8MessageHeader)
            {
                // Parse the data.
                g_ui16PipePresenceDelayTime = g_unProdDelayTimers.m_stProdDelayTimers.m_ui16DelayTimer;
                
                // Store the data in EEPROM.
                WritePipePresenceDelayToEEPROM(g_ui16PipePresenceDelayTime);
            }
            else if(PIPE_STABILIZATION_DELAY == g_unProdDelayTimers.m_stProdDelayTimers.m_ui8MessageHeader)
            {
                // Parse the data.
                g_ui16PipeStabilizationDelayTime = g_unProdDelayTimers.m_stProdDelayTimers.m_ui16DelayTimer;
                
                // Store the data in EEPROM.
                WritePipeStabilizationDelayToEEPROM(g_ui16PipeStabilizationDelayTime);                
            }
            else if(PIPE_TRANSFER_DELAY == g_unProdDelayTimers.m_stProdDelayTimers.m_ui8MessageHeader)
            {
                // Parse the data.
                g_ui16PipeTransferDelayTime = g_unProdDelayTimers.m_stProdDelayTimers.m_ui16DelayTimer;
                
                // Store the data in EEPROM.
                WritePipeTransferDelayToEEPROM(g_ui16PipeTransferDelayTime);                
            }
            else
            {
                /* Do Nothing */
            }
            break;
        case IBU_CMD_WEGHNG_CYLDR_UP_ACT_DELAY:
            // Copy the data.
            for (i8LoopCount = 1; i8LoopCount < IBU_MSG_BYTE_COUNT; i8LoopCount++)
            {
                g_unWeighingCylinderUpActivationDelay.m_ui8Arr[i8LoopCount - 1] = stIBURcvNode.g_ui8ArrIBUData[i8LoopCount];
            }

            // Store the data in the respective register.
            g_ui16WeighingCylinderUpActivationDelay = g_unWeighingCylinderUpActivationDelay.m_stWeighingCylinderUpActivationDelay.m_ui16DelayTime;
            
            // Store the data in EEPROM.
            WriteWeighingCylinderActivationDelayToEEPROM(g_ui16WeighingCylinderUpActivationDelay);
            break;
        default:
            break;
    }
}

#endif	/* VM_IBU_PROCESS_M0_H */

