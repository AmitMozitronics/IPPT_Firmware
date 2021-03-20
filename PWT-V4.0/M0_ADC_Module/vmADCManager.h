/* 
 * File:   vmADCManager.h
 * Author: Amit
 *
 * Created on December 12, 2018, 11:38 PM
 */

#ifndef VM_ADCMANAGER_H
#define	VM_ADCMANAGER_H

//#include "vmCalibration.h"
#include "vmM0ADCModuleCommon.h"
#include "vmAverageWeightCalculation.h"
#include "vmEEPROMConfig_M0.h"
#include "vmM0Common.h"

#define LOAD_CELL_FACTOR         818
//#define ADC_SCANNING_START     0
//#define ADC_SCANNING_RUNNING   1
//#define ADC_SCANNING_STOP      2
#define ADC_SCAN_ON              1
#define ADC_SCAN_OFF             0

#define TIMER_IGNORE_ON          1
#define TIMER_IGNORE_OFF         0

int8 g_fIgnore_TIMER = TIMER_IGNORE_OFF;


#define COLLECT_DYNAMIC_TARE_DATA           3
#define COLLECT_WEIGHING_DATA               4
#define COLLECT_NOTHING                     5

#define SIZE_DATA_QUEUE                     20
#define SIZE_ADC_DATA_QUEUE                 200      //200
#define SIZE_DATA_QUEUE_FOR_TARE            200
#define SIZE_DATA_QUEUE_FOR_WEIGHING        200
#define MAX_TARE_MULTIPLAYER                100

#define ADC_DATA_NOT_READY                  0
#define ADC_DATA_READY                      1

#define ADC_DATA_Q_VALUES_TRIMMING_PERCNTG     30

#define ADC_MEAN_CALCULATION_START    0
#define ADC_MEAN_CALCULATION_STOP     1
#define ADC_MEAN_CALCULATION_RUNNING  2
#define ADC_MEAN_CALCULATION_DONE     3

unsigned int8 g_ui8ADCMeanCalState = ADC_MEAN_CALCULATION_STOP;
unsigned int32 g_ui32ADCCurMeanValue = 0;
unsigned int32 g_ui32ADCCurSum       = 0;
unsigned int32 g_ui32TarePrev     = 0;
unsigned int32 g_ui32PipeWtPrev   = 0;
unsigned int16 g_ui16ADCSumLoopCount = 0;
int16          g_i16LoadCellFactor   = 0;
float32        g_f32LoadCellFactor   = 0;

//AMIT 02/12/2020 START
unsigned int32 g_ui32ADCArr[SIZE_ADC_DATA_QUEUE];
int16          g_i16ADCArrIdx = 0;
//AMIT 02/12/2020 END

unsigned int8  g_ui8ADCScanningStatus   = ADC_SCAN_OFF;
unsigned int32 g_ui32ADCValue           = 0;
unsigned int8  g_ui8DataCollectionState = COLLECT_NOTHING;
unsigned int8  g_fADCDataReadyStatus    = ADC_DATA_NOT_READY;
unsigned int8 g_ui8CurMaxDataCount    = 0;

unsigned int16    g_ui16NoOfADCData   = 0;
unsigned int32    g_ui32ADCAvg        = 0;

//unsigned int8       g_ui8ArrSortedTemp[SIZE_DATA_QUEUE]          = {0};
//ST_MEAN_SHIFT_VALUE g_ui16ArrMeanShiftVal[SIZE_DATA_QUEUE]       = {0};
//ST_MEAN_SHIFT_VALUE g_ui16ArrSortedMeanShiftVal[SIZE_DATA_QUEUE] = {0};

void ResetADCDataQ(int8 i8CollectionState);
unsigned int32 GetADCMeanValue();
unsigned int32 GetTareData();
int16          GetWeighingData(unsigned int32);
//void           IBUSendWeighingData();
//void           IBUSendTareData();
void           IBUSendTareData(unsigned int32);



int32 GetCalibratedWeight(unsigned int32 ui32ADCMean)
{
    int32 i32ActualWeight = 0;
    int32    i32Temp = 0;
    float32  f32Temp = 0;

    f32Temp = (float32)ui32ADCMean * g_f32LoadCellFactor;   //g_f32LoadCellFactor
    i32Temp = (int32)f32Temp;
    i32Temp /= 1000;
    i32ActualWeight = i32Temp;

    return i32ActualWeight;
}
////////int16 GetCalibratedWeight(unsigned int32 ui32ADCMean)
////////{
////////    int16 i16ActualWeight = 0;
////////    int32    i32Temp = 0;
////////    float32  f32Temp = 0;
////////    unsigned int32 ui32DiffValue = 0;
////////    
////////
////////    if(COLLECT_DYNAMIC_TARE_DATA == g_stADCDataQ.m_i8CollectionState)
////////    {
////////        ui32DiffValue = ui32ADCMean;
////////        f32Temp = (float32)ui32DiffValue * g_f32LoadCellFactor;   //g_f32LoadCellFactor
////////        i32Temp = (int32)f32Temp;
////////        i32Temp /= 1000;
////////        i16ActualWeight = (int16)i32Temp;
////////    }
////////    else if(COLLECT_WEIGHING_DATA == g_stADCDataQ.m_i8CollectionState)
////////    {
////////        ui32DiffValue = ui32ADCMean - g_ui32DynamicTareADCValue;
////////        f32Temp = (float32)ui32DiffValue * g_f32LoadCellFactor;
////////        i32Temp = (int32)f32Temp;
////////        i32Temp /= 1000;
////////        i16ActualWeight = (int16)i32Temp;
////////    }
////////    else
////////    {
////////        /* Do Nothing */
////////    }
//////// 
////////    return i16ActualWeight;
////////}

int16 GetWeighingData(unsigned int32 ui32ADCMean)
{
    int16 i16CalibratedWeight = 0;
    unsigned int16 ui16TempTareWeight   = 0;
    unsigned int16 ui16TempDynErrValue  = 0;    
    
    i16CalibratedWeight = GetCalibratedWeight(ui32ADCMean);
    
    
    return i16CalibratedWeight;
}


void IBUSendWeighingData(int32 i32PipeWeight)
{
    ST_IBU_DATA_NODE stIBUDataNode;
    int8             i8LoopCount         = 0;
    int32            i32AvgWeight        = 0;
    
    // To insert current pipe weight into average weight queue.
    i32AvgWeight = i32PipeWeight;
    // Increment pipe count.
    g_ui16PipeCount++;
    
    // Now Check if the Weight is within tolerance limit or not
    if (i32PipeWeight > g_stCurBatchInfo.m_i32MaxWt)
    {
        // Set Weighing Status.
        g_ui16RejectionStatus = PIPE_OVER_WEIGHT;
        
        // Set Indication of Tower light.
        output_bit(PIN_OUT_DO_03, 0); 
        
    }
    else if (i32PipeWeight < g_stCurBatchInfo.m_i32MinWt)
    {
        // Set Weighing Status.        
        g_ui16RejectionStatus = PIPE_UNDER_WEIGHT;
        
        // Set Indication of Tower light.
        output_bit(PIN_OUT_DO_04, 0);        
    }
    else
    {
        g_ui16RejectionStatus = PIPE_ACCEPTED;
        // Set Rejection State OFF
        //g_unDIState.m_stAllDIBits.m_ui1GDICn01 = BOX_ACCEPTED;
    }


    // Prepare IBU Data Node
    g_unIBUProdModeWtStsData.m_stWeighingStatusData.m_i32PipeWeight  = i32PipeWeight;
    g_unIBUProdModeWtStsData.m_stWeighingStatusData.m_ui16PassStatus = g_ui16RejectionStatus;
    g_unIBUProdModeWtStsData.m_stWeighingStatusData.m_ui16PipeCount  = g_ui16PipeCount;
    
    // Stuff IBU Command
    stIBUDataNode.g_ui8ArrIBUData[0] = IBU_CMD_PROD_WEIGH_DATA;
    for (i8LoopCount = 1; i8LoopCount < IBU_MSG_BYTE_COUNT; i8LoopCount++)
    {
        stIBUDataNode.g_ui8ArrIBUData[i8LoopCount] = g_unIBUProdModeWtStsData.m_ui8Arr[i8LoopCount - 1];
    }
    
    // Send to IBU Insert.
    InsertIBUMsgQueue(&g_stIBUSendQueue, &stIBUDataNode);
    
    // Send last pipe weight to calculate running average weight.
    InsertAvgWtQueue(i32AvgWeight);
    
    // Send calculated average weight to HMI.
    IBUSendProdModeAvgData();
}



void IBUSendTareData(unsigned int32 ui32ADCMean)
{
    ST_IBU_DATA_NODE stIBUDataNode;
    int8 i8LoopCount = 0;

    // Prepare IBU Data Node
    g_unIBUProdModeTrData.m_stTareData.m_i32Padding  = 0;
    g_unIBUProdModeTrData.m_stTareData.m_i32TareData = ui32ADCMean;
    
    // Stuff IBU Command
    stIBUDataNode.g_ui8ArrIBUData[0] = IBU_CMD_PROD_TARE_DATA;

    for (i8LoopCount = 1; i8LoopCount < IBU_MSG_BYTE_COUNT; i8LoopCount++)
    {
        stIBUDataNode.g_ui8ArrIBUData[i8LoopCount] = g_unIBUProdModeTrData.m_ui8Arr[i8LoopCount - 1];
    }

    // Send to IBU Insert
    InsertIBUMsgQueue(&g_stIBUSendQueue, &stIBUDataNode);
}



void IBUSendCalibResetData(unsigned int32 ui32ADCMean)
{
    ST_IBU_DATA_NODE stIBUDataNode;
    int8 i8LoopCount = 0;
    
    //CHANGE_RUNNING_AVG START
    
    // 1. Send Dynamic Tare ADC Value : g_ui32DynamicTareADCValue
    // 2. Send Current Dynamic Weight : ui32ADCMean
    
    // Set dynamic tare ADC value and weight ADC value in variables.
    g_unIBUResetCalibData.m_stIBUResetCalibData.m_ui32CalibADCForTare   = g_ui32DynamicTareADCValue;
    g_unIBUResetCalibData.m_stIBUResetCalibData.m_ui32CalibADCForWeight = ui32ADCMean;

    
    // Stuff IBU Command.
    // 1. Dynamic Tare ADC value.
    stIBUDataNode.g_ui8ArrIBUData[0] = IBU_CMD_RESET_CALIB_DATA;
    for (i8LoopCount = 1; i8LoopCount < IBU_MSG_BYTE_COUNT; i8LoopCount++)
    {
        stIBUDataNode.g_ui8ArrIBUData[i8LoopCount] = g_unIBUResetCalibData.m_ui8Arr[i8LoopCount - 1];
    }
    
    // Insert in IBU queue.
    InsertIBUMsgQueue(&g_stIBUSendQueue, &stIBUDataNode);

}


void AfterADCMeanCalculationDone()
{

    // Final Weight Calculation
    if (ADC_MEAN_DATA_FINAL_WEIGHT == g_ui8ADCMeanDataState)
    {
        // ADC Mean Calculation Done

        // Step 6: Reset ADC Data Queue
        ResetADCDataQ(COLLECT_NOTHING);
        
        // Calibration mode
        if (SYS_MODE_CALIB == g_ui8SystemMode)
        {
            // The Final Tare Calculation Done so move to Next Step
            g_ui8CurrentState++;
            g_ui8StateWaiting = STATE_NOTHING;
        }
        if (SYS_MODE_PROD == g_ui8SystemMode)
        {
            g_ui8CurrentState++;
            g_ui8StateWaiting = STATE_NOTHING;
        }

        //g_fWtCalculationState = FLAG_WT_CALCULATION_DONE;
        
        //g_fWeighingStatus = WEIGHING_OFF;
    }

    // Final Tare Value Set
    if (ADC_MEAN_DATA_FINAL_TARE  == g_ui8ADCMeanDataState)
    {
        // Calibration mode
        if (SYS_MODE_CALIB == g_ui8SystemMode)
        {
            // The Final Tare Calculation Done so move to Next Step
            g_ui8CurrentState++;
            g_ui8StateWaiting = STATE_NOTHING;
        }
        if (SYS_MODE_PROD == g_ui8SystemMode)
        {
            g_ui8CurrentState++;
            g_ui8StateWaiting = STATE_NOTHING;
        }
        
        // ADC Mean Calculation Done
        //IBUSendTareData(g_ui32ADCCurMeanValue);
        ResetADCDataQ(COLLECT_NOTHING);
    }
    
    if (ADC_MEAN_DATA_CALIB_WEIGHT  == g_ui8ADCMeanDataState)
    {
        // ADC Mean Calculation Done
        IBUSendCalibResetData(g_ui32ADCCurMeanValue);
        ResetADCDataQ(COLLECT_NOTHING);
    }
    
    // Reset Flags
    g_ui8ADCMeanCalState  = ADC_MEAN_CALCULATION_STOP;
    g_ui8ADCMeanDataState = ADC_MEAN_DATA_NOTHING;
}



void SendAlarmtoAnotherMCU(void)
{
    ST_IBU_DATA_NODE stIBUDataNode;
    int8 i8LoopCount = 0;

    // Stuff IBU Command.
    stIBUDataNode.g_ui8ArrIBUData[0] = IBU_CMD_AIR_PRESSURE_ALARM;
    for (i8LoopCount = 1; i8LoopCount < IBU_MSG_BYTE_COUNT; i8LoopCount++)
    {
        stIBUDataNode.g_ui8ArrIBUData[i8LoopCount] = 0;
    }
    
    // Insert in IBU queue.
    InsertIBUMsgQueue(&g_stIBUSendQueue, &stIBUDataNode);    
}

void SendResetAlarmtoAnotherMCU(void)
{
    ST_IBU_DATA_NODE stIBUDataNode;
    int8 i8LoopCount = 0;

    // Stuff IBU Command.
    stIBUDataNode.g_ui8ArrIBUData[0] = IBU_CMD_RESET_AIR_PRESSURE_ALARM;
    for (i8LoopCount = 1; i8LoopCount < IBU_MSG_BYTE_COUNT; i8LoopCount++)
    {
        stIBUDataNode.g_ui8ArrIBUData[i8LoopCount] = 0;
    }
    
    // Insert in IBU queue.
    InsertIBUMsgQueue(&g_stIBUSendQueue, &stIBUDataNode);    
}

#endif	/* VM_ADCMANAGER_H */

