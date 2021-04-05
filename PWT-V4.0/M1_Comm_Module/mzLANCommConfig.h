/* 
 * File:   mzLANCommConfig.h
 * Author: Andrian
 *
 * Created on 26 March 2020, 9:56 PM
 */

#ifndef MZ_LAN_COMM_CONFIG_H
#define	MZ_LAN_COMM_CONFIG_H

#include "vmM1Common.h"


#define LAN_DATA_SENDING_SEQUENCE_1         1
#define LAN_DATA_SENDING_SEQUENCE_2         2
#define LAN_DATA_SENDING_SEQUENCE_3         3
#define LAN_DATA_SENDING_SEQUENCE_4         4
#define LAN_DATA_SENDING_SEQUENCE_5         5

#define SERVER_RESPONSE_RECEIVE_TIMEOUT     3
#define DATA_RESEND_INTERVAL_TIMEOUT        10

#define SIZE_LAN_COMM_RCV_BUFFER            20

typedef struct _ST_PIPE_DATA_NODE
{
    unsigned int32 m_ui32TimeStamp;
    unsigned int16 m_ui16SequenceNo;
    unsigned int32 m_ui32ItemWeight;
    unsigned int8  m_ui8RejectionStatus;
} ST_PIPE_DATA_NODE;

typedef struct _ST_PIPE_DATA_QUEUE
{
    ST_PIPE_DATA_NODE m_stArrGPRSDataNode[SIZE_GPRS_DATA_NODE_QUEUE];
    int8              m_i8Front;
    int8              m_i8Rear;
} ST_PIPE_DATA_QUEUE;


// *****************************************************************************
// ***************** Below -> Global Variable Declarations *********************
// *****************************************************************************
unsigned int8  g_fDataSendingSequence                           = 0;
unsigned int8  g_ui8LANCommRxBuffer[SIZE_LAN_COMM_RCV_BUFFER]   = {0};
unsigned int8  g_ui8LANCommRxByteCount                          = 0;



ST_PIPE_DATA_QUEUE   g_stPipeDataSendQueue;
// *****************************************************************************
// ***************** Above -> Global Variable Declarations *********************
// *****************************************************************************


void InitLANCommRxInterrupt(void)
{
    clear_interrupt(INT_RDA5);
    enable_interrupts(INT_RDA5);
}

void ResetLANCommRxBuffer(void)
{
    // Clear buffer.
    memset(g_ui8LANCommRxBuffer, 0, SIZE_LAN_COMM_RCV_BUFFER);
    
    // Reset byte count.
    g_ui8LANCommRxByteCount = 0;
    
}

void InitializeLANMsgHeader(void)
{
    unsigned int8 ui8MachineID = 0;
    
    // Restore Machine ID. 
    ui8MachineID = ReadMachineIDFromEEPROM();
    g_stLANDataHeader.m_ui8MachineID = ui8MachineID;
    
    // Remaining all header data will be send from HMI before start 
    // the production.
}


#INT_RDA5
void LANData_rcv_isr(void)
{
    // Read the received byte.
    g_ui8LANCommRxBuffer[g_ui8LANCommRxByteCount] = fgetc(LAN_USR);
    
    // Increment receive byte count.
    g_ui8LANCommRxByteCount++;
    
    // Check the buffer size.
    if(g_ui8LANCommRxByteCount >= SIZE_LAN_COMM_RCV_BUFFER)
    {
        // If reach to maximum; circulate the buffer.
        g_ui8LANCommRxByteCount = 0;
    }
    
    // Clear the interrupt.
    clear_interrupt(INT_RDA5);
}


void InitializePipeDataQueue(void)
{
    // Initialize front and rear of the queue
    g_stPipeDataSendQueue.m_i8Front = -1;
    g_stPipeDataSendQueue.m_i8Rear = -1;
    
    // Initialize each queue element
    for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < SIZE_GPRS_DATA_NODE_QUEUE; ui8LoopCount++)
    {
        g_stPipeDataSendQueue.m_stArrGPRSDataNode[ui8LoopCount].m_ui32ItemWeight = 0;
        g_stPipeDataSendQueue.m_stArrGPRSDataNode[ui8LoopCount].m_ui16SequenceNo = 0;
        g_stPipeDataSendQueue.m_stArrGPRSDataNode[ui8LoopCount].m_ui32TimeStamp  = 0;
        g_stPipeDataSendQueue.m_stArrGPRSDataNode[ui8LoopCount].m_ui8RejectionStatus = 0;
    }

}

unsigned int8 IsPipeDataQueueEmpty(void)
{
    unsigned int8 ui8QueueStatus = 0;
    if (-1 == g_stPipeDataSendQueue.m_i8Front)
    {
        ui8QueueStatus = 1; 
    }
    else
    {
        ui8QueueStatus = 0;
    }
    return ui8QueueStatus;
}


void InsertInPipeDataQueue(void)
{
    if (-1 == g_stPipeDataSendQueue.m_i8Front)
    {
        g_stPipeDataSendQueue.m_i8Front = 0;
    }
    
    g_stPipeDataSendQueue.m_i8Rear = (g_stPipeDataSendQueue.m_i8Rear + 1) % SIZE_GPRS_DATA_NODE_QUEUE;
    
    // Insert Data
    g_stPipeDataSendQueue.m_stArrGPRSDataNode[g_stPipeDataSendQueue.m_i8Rear].m_ui16SequenceNo = g_stProdModeCurrBoxData.m_ui16SequenceNo;
    g_stPipeDataSendQueue.m_stArrGPRSDataNode[g_stPipeDataSendQueue.m_i8Rear].m_ui32ItemWeight = (unsigned int32)g_stProdModeCurrBoxData.m_i32PipeWeight;
    g_stPipeDataSendQueue.m_stArrGPRSDataNode[g_stPipeDataSendQueue.m_i8Rear].m_ui8RejectionStatus = (unsigned int8)g_stProdModeCurrBoxData.m_ui16RejectionStatus;
    g_stPipeDataSendQueue.m_stArrGPRSDataNode[g_stPipeDataSendQueue.m_i8Rear].m_ui32TimeStamp = g_stProdModeCurrBoxData.m_ui32TimeStamp;    
}


void SendDataFromQ(void)
{
    // Set web service URL.
    //fprintf(GSM_M66, "http://pipetrackerlive.com/api/Test/GetStringGetMethod?a=%u&b=%Lu&c=%Lu&d=%Lu&e=%Lu&m=%Lu$%Lu$%Lu$%u", g_stLANDataHeader.m_ui8MachineID, g_stLANDataHeader.m_ui16BasicMaterialStandard, g_stLANDataHeader.m_ui16PipeSpecification,g_stLANDataHeader.m_ui16MaxWeight,g_stLANDataHeader.m_ui16MinWeight,
    //                                                                                                                         g_stPipeDataSendQueue.m_stArrGPRSDataNode[g_stPipeDataSendQueue.m_i8Front].m_ui32TimeStamp, g_stPipeDataSendQueue.m_stArrGPRSDataNode[g_stPipeDataSendQueue.m_i8Front].m_ui16SequenceNo, g_stPipeDataSendQueue.m_stArrGPRSDataNode[g_stPipeDataSendQueue.m_i8Front].m_ui32ItemWeight, g_stPipeDataSendQueue.m_stArrGPRSDataNode[g_stPipeDataSendQueue.m_i8Front].m_ui8RejectionStatus);

    fprintf(LAN_USR, "/api/ipptv2/?a=%u&b=%Lu&c=%Lu&d=%u&e=%Lu&f=%u&g=%Lu&h=%Lu&i=%Lu&j=%Lu&k=%Lu&l=%Lu&m=%Lu&n=%Lu&o=%u", g_stLANDataHeader.m_ui8MachineID, g_stLANDataHeader.m_ui16BasicMaterialStandard, g_stLANDataHeader.m_ui16StandardSpecification,g_stLANDataHeader.m_ui8PressureRating,g_stLANDataHeader.m_ui16PipeOD,g_stLANDataHeader.m_ui8PipeLength,g_stLANDataHeader.m_ui32MaxWeight,g_stLANDataHeader.m_ui32MinWeight,g_stLANDataHeader.m_ui16Unused1,g_stLANDataHeader.m_ui16Unused2,g_stLANDataHeader.m_ui16Unused3,
                                                                                                                          g_stPipeDataSendQueue.m_stArrGPRSDataNode[g_stPipeDataSendQueue.m_i8Front].m_ui32TimeStamp, g_stPipeDataSendQueue.m_stArrGPRSDataNode[g_stPipeDataSendQueue.m_i8Front].m_ui16SequenceNo, g_stPipeDataSendQueue.m_stArrGPRSDataNode[g_stPipeDataSendQueue.m_i8Front].m_ui32ItemWeight, g_stPipeDataSendQueue.m_stArrGPRSDataNode[g_stPipeDataSendQueue.m_i8Front].m_ui8RejectionStatus);
}


unsigned int8 CheckServerResponse(void)
{
    unsigned int8 ui8RetVal = 1;
    unsigned int8 ui8LoopCounter = 0;
    
    // Loop through receive buffer.
    for(ui8LoopCounter = 0; ui8LoopCounter < SIZE_LAN_COMM_RCV_BUFFER; ui8LoopCounter++)
    {
        // Check for expected response.
        if(g_ui8LANCommRxBuffer[ui8LoopCounter - 3] == 'O' &&
           g_ui8LANCommRxBuffer[ui8LoopCounter - 2] == 'k' &&
           g_ui8LANCommRxBuffer[ui8LoopCounter - 1] == 'a' &&
           g_ui8LANCommRxBuffer[ui8LoopCounter - 0] == 'y')
        {
            // Expected response received.
            // Set return value.
            ui8RetVal = 0;
            // Reset receive buffer.
            ResetLANCommRxBuffer();            
            break;
        }
    }
        
    return ui8RetVal;
}


#endif	/* MZ_LAN_COMM_CONFIG_H */

