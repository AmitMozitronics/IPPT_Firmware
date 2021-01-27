/* 
 * File:   vmIBUProcessM1.h
 * Author: Mrinmoy
 *
 * Created on December 11, 2018, 3:11 PM
 */

#ifndef VM_IBU_PROCESS_M1_H
#define	VM_IBU_PROCESS_M1_H

#include "../Common/vmIBUConfig.h"
#include "../Common/vmModbusGlobal.h"
#include "vmM1Common.h"
#include "vmGSMM66_GPRSConfig.h"
#include "mzLANCommConfig.h"

unsigned int16 g_ui16PrevItemCount = 0;
unsigned int16 g_ui16ItemCount     = 0;

 // TESTING 
    unsigned int8 ui8Counter = 0;

// IBU Received message process proc
void ProcessIBUMsg()
{   
    ST_IBU_DATA_NODE stIBURcvNode;
    unsigned int8  ui8LoopCount = 0;

    // Get Node from IBU Receive Message Queue
    DeleteIBUMsgQueue(&g_stIBURcvQueue, &stIBURcvNode);
    
    // Valid message present in IBU receive buffer
    // So go for process the data
    switch(stIBURcvNode.g_ui8ArrIBUData[0])
    {
        case IBU_CMD_RESET_CALIB_DATA:
            // Store receive data.
            for(ui8LoopCount = 1; ui8LoopCount < IBU_MSG_BYTE_COUNT; ui8LoopCount++)
            {
                g_unIBUResetCalibData.m_ui8Arr[ui8LoopCount - 1] = stIBURcvNode.g_ui8ArrIBUData[ui8LoopCount];
            }
            
            // Retrieve and store the dynamic ADC tare value.
            g_ui32TareADCValueForCalibration    = g_unIBUResetCalibData.m_stIBUResetCalibData.m_ui32CalibADCForTare;
            g_ui32gWeightADCValueForCalibration = g_unIBUResetCalibData.m_stIBUResetCalibData.m_ui32CalibADCForWeight;
            break;
        case IBU_CMD_START_CALIBRATION:
            break;
        case IBU_CMD_RESET_CALIB_POINT:            
            break;
        case IBU_CMD_PROD_TARE_DATA:
        {
            for(ui8LoopCount = 1; ui8LoopCount < IBU_MSG_BYTE_COUNT; ui8LoopCount++)
            {
                g_unIBUProdModeTrData.m_ui8Arr[ui8LoopCount-1] = stIBURcvNode.g_ui8ArrIBUData[ui8LoopCount];
            }
            g_ui32RunningTareValue = (unsigned int16)(g_unIBUProdModeTrData.m_stTareData.m_i32TareData);
        }
            
            break;
        case IBU_CMD_PRE_PROD_AVG_WEIGHT:
            break;
        case IBU_CMD_PRE_PROD_TOLERANCE:
            break;
        case IBU_CMD_START_PROD:
            break;
        case IBU_CMD_PROD_AVG_WT_DATA:
            {
                for(ui8LoopCount = 1; ui8LoopCount < IBU_MSG_BYTE_COUNT; ui8LoopCount++)
                {
                    g_unIBUProdModeAvg.m_ui8Arr[ui8LoopCount-1] = stIBURcvNode.g_ui8ArrIBUData[ui8LoopCount];
                }
                
                g_ui32RunningAvgWt = g_unIBUProdModeAvg.m_stAvgData.m_i32CurAvg;
            }
            break;            
        case IBU_CMD_PROD_WEIGH_DATA:
            {
                
                // TESTING.
                //output_toggle(PIN_OUT_DO_00);

                // Copy individual Box data.
                for(ui8LoopCount = 1; ui8LoopCount < IBU_MSG_BYTE_COUNT; ui8LoopCount++)
                {
                    g_unIBUProdModeWtStsData.m_ui8Arr[ui8LoopCount - 1] = stIBURcvNode.g_ui8ArrIBUData[ui8LoopCount];
                }
                
                // Update current box weight in register.
                g_stProdModeCurrBoxData.m_i32PipeWeight = g_unIBUProdModeWtStsData.m_stWeighingStatusData.m_i32PipeWeight;
                // Insert current box data to variables for Display in HMI
                g_ui32CurrentWt = (unsigned int32)g_stProdModeCurrBoxData.m_i32PipeWeight;
                
                // Update Current box count in register.
                //g_ui16PipeCount = g_unIBUProdModeBoxCount.m_stBoxCount.m_ui16BoxCount;
                g_ui16PipeCount = g_unIBUProdModeWtStsData.m_stWeighingStatusData.m_ui16PipeCount;
                g_ui16ItemCount = g_ui16PipeCount;
                
                // Update last box pass status.
                g_ui16CurrentPassStatus = g_unIBUProdModeWtStsData.m_stWeighingStatusData.m_ui16PassStatus;
                
                // Update HMI Data Trigger Status.
                //g_ui8HMITriggerStatus = 1;

                if(g_ui16ItemCount > 0 && g_ui16PrevItemCount != g_ui16ItemCount)
                {
                    // Update production mode current box data.
                    g_stProdModeCurrBoxData.m_ui16SequenceNo = g_ui16PipeCount;
                    g_stProdModeCurrBoxData.m_i32PipeWeight = g_ui32CurrentWt;
                    g_stProdModeCurrBoxData.m_ui16RejectionStatus = g_ui16CurrentPassStatus;  // TODO
                    g_stProdModeCurrBoxData.m_ui32TimeStamp = g_ui32UnixTimeInSec;
                    // Insert current box data in GPRS send Message queue.
                    InsertInPipeDataQueue();

                    // update previous item count for next box reference.
                    g_ui16PrevItemCount = g_ui16ItemCount;
                }
            }
            break;            
        case IBU_CMD_PRE_PROD_TOL_STORE_ACK:
            // No need to parse data.
            // This is acknowledgement message. Transfer to HMI.
            g_ui8ToleranceStoreAck = 1;
            break;
        case IBU_CMD_AIR_PRESSURE_ALARM:
            // Remaining all bytes are padding.
            // No need to parse that.
            
            // This command denotes air-pressure below rated alarm occurred.
            // Set flag to transfer the message to HMI.
            g_ui8AirPressureAlarm = 1;
            break;
        case IBU_CMD_RESET_AIR_PRESSURE_ALARM:
            // Remaining all bytes are padding.
            // No need to parse that.
            
            // This command denotes air-pressure recovers.
            // Now it is above rated.
            // Reset flag to transfer the message to HMI.
            g_ui8AirPressureAlarm = 0;
            break;
        default :
            break;
    }
}   



#endif	/* VM_IBU_PROCESS_M1_H */

