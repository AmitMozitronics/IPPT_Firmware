/* 
 * File:   vmAverageWeightCalculation.h
 * Author: OEM
 *
 * Created on December 20, 2018, 11:50 AM
 */

#ifndef VM_AVERAGE_WEIGHT_CALCULATION_H
#define	VM_AVERAGE_WEIGHT_CALCULATION_H

//#include "../Common/vmModbusGlobal.h"

#define SIZE_AVG_WT_ARR                 50
#define MAX_DEVIATIION_FROM_AVG_WT      1000  // We are considering +- 1000gm weight deviation
#define MAX_DEVIATION_FOR_BATCH_CHANGE  1200   // If Current weight 850gm deviated from last average weight then we consider batch must be changed.
                                              // In that case we reset the queue.

typedef struct _ST_AVG_WT_QUEUE
{
    int32 m_i32Arr[SIZE_AVG_WT_ARR];
    int8  m_i8Rear;
    int8  m_i8Length;
    int8  m_i8QueueFull;
    int32 m_i32CurAvg;
    int32 m_i32LstAvg;
} ST_AVG_WT_QUEUE;

ST_AVG_WT_QUEUE g_stAvgWtQueue;

void InitializeAvgWtQueue()
{
    unsigned int8 ui8LoopCount   = 0;
    g_stAvgWtQueue.m_i8Rear      = -1;
    g_stAvgWtQueue.m_i8Length    = SIZE_AVG_WT_ARR;
    g_stAvgWtQueue.m_i32CurAvg   = 0;
    g_stAvgWtQueue.m_i8QueueFull = 0;
    
    // Initialize array member.
    for(ui8LoopCount = 0; ui8LoopCount < SIZE_AVG_WT_ARR; ui8LoopCount++)
    {
        g_stAvgWtQueue.m_i32Arr[ui8LoopCount] = 0;
    }
    
    g_stAvgWtQueue.m_i32LstAvg = 0;
}

void ResetAvgWtQueue(int8 i8Length)
{
    if (i8Length <= SIZE_AVG_WT_ARR)
    {
        g_stAvgWtQueue.m_i8Length = i8Length;
    }
    else
    {
        g_stAvgWtQueue.m_i8Length = SIZE_AVG_WT_ARR;
    }
}

void InsertAvgWtQueue(int32 i32LastWeight)
{
    int8  i8LoopCount = 0;
    int8  i8MaxEntry  = 0;
    int32 i32Sum      = 0;
    
//    // Check the weight deviation from Selected SKU Average Weight
//    if (i32LastWeight > (g_stCurBatchInfo.m_i32AvgWeight + MAX_DEVIATIION_FROM_AVG_WT) ||
//        i32LastWeight < (g_stCurBatchInfo.m_i32AvgWeight - MAX_DEVIATIION_FROM_AVG_WT))
//    {
//        // The weight is out of range, we are not inserting this weight into Average weight queue 
//        return;
//    }
    
    // Increment Rear
    g_stAvgWtQueue.m_i8Rear++;
    
    // Check queue rear with size.
    if(g_stAvgWtQueue.m_i8Rear >= g_stAvgWtQueue.m_i8Length)
    {
        // Queue is full.
        // Set flag.
        g_stAvgWtQueue.m_i8QueueFull = 1;
        
        // Reset rear.
        g_stAvgWtQueue.m_i8Rear = 0;
    }
    
    // Check if the current weight is a new batch weight.
    if(i32LastWeight >= g_stAvgWtQueue.m_i32LstAvg + MAX_DEVIATION_FOR_BATCH_CHANGE)
    {
        g_stAvgWtQueue.m_i8Rear = 0;
    }
    
    
    // Insert into Queue
    g_stAvgWtQueue.m_i32Arr[g_stAvgWtQueue.m_i8Rear] = i32LastWeight;
    
    // Get the Max entry in queue
    if (1 == g_stAvgWtQueue.m_i8QueueFull)
    {
        // Set maximum entry will be same as queue size.
        i8MaxEntry = SIZE_AVG_WT_ARR;
        
        // Reset queue full flag.
        g_stAvgWtQueue.m_i8QueueFull = 0;
    }
    else
    {
        i8MaxEntry = g_stAvgWtQueue.m_i8Rear + 1;
    }
    
    // Get the summation of queue values   
    for (i8LoopCount = 0; i8LoopCount < i8MaxEntry; i8LoopCount++)
    {
        i32Sum += g_stAvgWtQueue.m_i32Arr[i8LoopCount];
    }
    
    g_stAvgWtQueue.m_i32CurAvg = (int32)(i32Sum / (int32)i8MaxEntry);
    
    // Store the current average in last average of all CBBs.
    g_stAvgWtQueue.m_i32LstAvg = g_stAvgWtQueue.m_i32CurAvg;
}

void IBUSendProdModeAvgData()
{
    ST_IBU_DATA_NODE stIBUDataNode;
    int8 i8LoopCount = 0;
    

    // Prepare IBU Data Node
    g_unIBUProdModeAvg.m_stAvgData.m_i32Padding  = 0;
    g_unIBUProdModeAvg.m_stAvgData.m_i32CurAvg   = g_stAvgWtQueue.m_i32CurAvg;
    
    // Stuff IBU Command
    stIBUDataNode.g_ui8ArrIBUData[0] = IBU_CMD_PROD_AVG_WT_DATA;
    for (i8LoopCount = 1; i8LoopCount < IBU_MSG_BYTE_COUNT; i8LoopCount++)
    {
        stIBUDataNode.g_ui8ArrIBUData[i8LoopCount] = g_unIBUProdModeAvg.m_ui8Arr[i8LoopCount - 1];
    }
    
    // Send to IBU Insert.
    InsertIBUMsgQueue(&g_stIBUSendQueue, &stIBUDataNode);
}
#endif	/* VM_AVERAGE_WEIGHT_CALCULATION_H */

