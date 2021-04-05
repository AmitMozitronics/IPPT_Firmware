/* 
 * File:   vmGSMM66_GPRSConfig.h
 * Author: Mrinmoy
 *
 * Created on January 22, 2019, 6:50 PM
 */

#ifndef VM_GSMM66_GPRS_CONFIG_H
#define	VM_GSMM66_GPRS_CONFIG_H

//#include "vmSIM900_GPRSConfig.h"
//#include "vmRS485Config.h"
#include "vmM1Common.h"
#include "mzEEPROMConfig_MCU1.h"

// Stuff Machine ID and Variety type in GPRS Message header.
// Amta plant(Line 2) -> Machine ID - 2 and Marie Gold variety Type - 3.
// Delta Foods Machine ID - 4
// Super Snacks Machine ID - 5
#define  MACHINE_ID                15            
    
#define   SIZE_GSMM66_RX_BUFFER                 500
#define   SIZE_GPRS_DATA_NODE_QUEUE             100

#define   GPRS_DATA_SENDING_SEQUENCE_1          1
#define   GPRS_DATA_SENDING_SEQUENCE_2          2
#define   GPRS_DATA_SENDING_SEQUENCE_3          3
#define   GPRS_DATA_SENDING_SEQUENCE_4          4
#define   GPRS_DATA_SENDING_SEQUENCE_5          5
#define   GPRS_DATA_SENDING_SEQUENCE_6          6
#define   GPRS_DATA_SENDING_SEQUENCE_7          7
#define   GPRS_DATA_SENDING_SEQUENCE_8          8
#define   GPRS_DATA_SENDING_SEQUENCE_9          9


typedef struct _ST_GPRS_DATA_NODE
{
    unsigned int32 m_ui32TimeStamp;
    unsigned int16 m_ui16SequenceNo;
    unsigned int32 m_ui32ItemWeight;
    unsigned int8  m_ui8RejectionStatus;
} ST_GPRS_DATA_NODE;

typedef struct _ST_GPRS_MSG_SEND_QUEUE
{
    ST_GPRS_DATA_NODE m_stArrGPRSDataNode[SIZE_GPRS_DATA_NODE_QUEUE];
    int8              m_i8Front;
    int8              m_i8Rear;
} ST_GPRS_MSG_SEND_QUEUE;

typedef struct _ST_PROD_M_CURR_BOX_DATA
{
    int32             m_i32PipeWeight;
    unsigned int16    m_ui16SequenceNo;
    unsigned int16    m_ui16RejectionStatus;
    unsigned int32    m_ui32TimeStamp;
} ST_PROD_M_CURR_BOX_DATA;

// *****************************************************************************
// ***************** Below -> Global Variable Declarations *********************
// *****************************************************************************
unsigned int8       g_ui8M66RxBuffer[SIZE_GSMM66_RX_BUFFER]               = {0};
unsigned int16      g_ui16M66RxByteCount                                  = 0;
unsigned int8       g_ui8URLInputByteLength                               = 0;
unsigned int8       g_ui8URLInputTime                                     = 0;
unsigned int16      g_ui16Itm1SequenceNumber                              = 1;
unsigned int16      g_ui16Itm2SequenceNumber                              = 2;
unsigned int16      g_ui16ItemWeight                                      = 1999;
unsigned int8       g_ui8DataQueueLen                                     = 0;
unsigned int8       g_fGPRSSequence                                       = 0;
unsigned int8       g_fGPRSFailureBit                                     = 0;

//ST_GPRS_MSG_SEND_QUEUE   g_stGPRSMsgSendQueue;
ST_PROD_M_CURR_BOX_DATA  g_stProdModeCurrBoxData;
// *****************************************************************************
// ***************** Above -> Global Variable Declarations *********************
// *****************************************************************************

void ResetGSMM66RxBuffer(void)
{
    // Reset whole buffer.
    memset(g_ui8M66RxBuffer, 0, SIZE_GSMM66_RX_BUFFER);
    
    // Reset byte count
    g_ui16M66RxByteCount = 0;
}

//unsigned int8 CountDataSize(unsigned int32 ui32Data)
//{
//    unsigned int8 ui8DataLen = 0;
//    
//    if(ui32Data <= 9)
//    {
//        ui8DataLen = 1;
//    }
//    else if(ui32Data <= 99)
//    {
//        ui8DataLen = 2;
//    }
//    else if(ui32Data <= 999)
//    {
//        ui8DataLen = 3;
//    }
//    else if(ui32Data <= 9999)
//    {
//        ui8DataLen = 4;
//    }
//    else if(ui32Data <= 99999)
//    {
//        ui8DataLen = 5;
//    }
//    else if(ui32Data <= 999999)
//    {
//        ui8DataLen = 6;
//    }
//    else if(ui32Data <= 9999999)
//    {
//        ui8DataLen = 7;
//    }
//    else if(ui32Data <= 99999999)
//    {
//        ui8DataLen = 8;
//    }
//    else if(ui32Data <= 999999999)
//    {
//        ui8DataLen = 9;
//    }
//    else
//    {
//        ui8DataLen = 10;
//    }
//
//    
//    return ui8DataLen;
//}

//void CheckURLInputByteLength(void)
//{
//    unsigned int8 ui8DataByte = 0;
//    
//    // Basic byte count is 75 bytes.
//    // Here the dynamic/ changing variables are considered to count byte length.
//
//    // Check for Machine ID.
//    ui8DataByte = CountDataSize(g_stGPRSMsgHeader.m_ui8MachineID);
//    // Add up data byte.
//    g_ui8URLInputByteLength = g_ui8URLInputByteLength + ui8DataByte;
//    // Check for header parameter - 1.
//    ui8DataByte = CountDataSize(g_stGPRSMsgHeader.m_ui16BasicMaterialStandard);
//    // Add up data byte.
//    g_ui8URLInputByteLength = g_ui8URLInputByteLength + ui8DataByte;
//    // Check for header parameter - 2.
//    ui8DataByte = CountDataSize(g_stGPRSMsgHeader.m_ui16PipeSpecification);
//    // Add up data byte.
//    g_ui8URLInputByteLength = g_ui8URLInputByteLength + ui8DataByte;
//    // Check for header parameter - 3.
//    ui8DataByte = CountDataSize(g_stGPRSMsgHeader.m_ui16MaxWeight);
//    // Add up data byte.
//    g_ui8URLInputByteLength = g_ui8URLInputByteLength + ui8DataByte;
//        // Check for header parameter - 4.
//    ui8DataByte = CountDataSize(g_stGPRSMsgHeader.m_ui16MinWeight);
//    // Add up data byte.
//    g_ui8URLInputByteLength = g_ui8URLInputByteLength + ui8DataByte;
//    
//    
//    // Check for TimeStamp.
//    ui8DataByte = CountDataSize(g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp);
//    // Add up data byte.
//    g_ui8URLInputByteLength = g_ui8URLInputByteLength + ui8DataByte;
//    // Check for Sequence number.
//    ui8DataByte = CountDataSize(g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front ].m_ui16SequenceNo);
//    // Add up data byte.
//    g_ui8URLInputByteLength = g_ui8URLInputByteLength + ui8DataByte;
//    // Check for Item weight.
//    ui8DataByte = CountDataSize(g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32ItemWeight);
//    // Add up data byte.
//    g_ui8URLInputByteLength = g_ui8URLInputByteLength + ui8DataByte;
//    // Check for Rejection status.
//    ui8DataByte = CountDataSize(g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui8RejectionStatus);
//    // Add up data byte.
//    g_ui8URLInputByteLength = g_ui8URLInputByteLength + ui8DataByte;
//
//}


//void InitializeGPRSMsgQueue(void)
//{
//    // Initialize front and rear of the queue
//    g_stGPRSMsgSendQueue.m_i8Front = -1;
//    g_stGPRSMsgSendQueue.m_i8Rear = -1;
//    
//    // Initialize each queue element
//    for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < SIZE_GPRS_DATA_NODE_QUEUE; ui8LoopCount++)
//    {
//        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[ui8LoopCount].m_ui32ItemWeight = 0;
//        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[ui8LoopCount].m_ui16SequenceNo = 0;
//        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[ui8LoopCount].m_ui32TimeStamp  = 0;
//        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[ui8LoopCount].m_ui8RejectionStatus = 0;
//    }

//}

//unsigned int8 IsGPRSMsgQueueEmpty(void)
//{
//    unsigned int8 ui8QueueStatus = 0;
//    if (-1 == g_stGPRSMsgSendQueue.m_i8Front)
//    {
//        ui8QueueStatus = 1; 
//    }
//    else
//    {
//        ui8QueueStatus = 0;
//    }
//    return ui8QueueStatus;
//}


//void InsertInGPRSMsgQueue()
//{
//    if (-1 == g_stGPRSMsgSendQueue.m_i8Front)
//    {
//        g_stGPRSMsgSendQueue.m_i8Front = 0;
//    }
//    
//    g_stGPRSMsgSendQueue.m_i8Rear = (g_stGPRSMsgSendQueue.m_i8Rear + 1) % SIZE_GPRS_DATA_NODE_QUEUE;
//    
//    // Insert Data
//    g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui16SequenceNo = g_stProdModeCurrBoxData.m_ui16SequenceNo;
//    g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui32ItemWeight = (unsigned int32)g_stProdModeCurrBoxData.m_i32PipeWeight;
//    g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui8RejectionStatus = (unsigned int8)g_stProdModeCurrBoxData.m_ui16RejectionStatus;
//    g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui32TimeStamp = g_stProdModeCurrBoxData.m_ui32TimeStamp;
//    //g_stGPRSMsgSendQueue.m_ui8QArr[g_stGPRSMsgSendQueue.m_ui8Rear] = ui8Element;
//}

//void InsertInGPRSQueue(void)
//{
//    if (-1 == g_stGPRSMsgSendQueue.m_i8Front)
//    {
//        g_stGPRSMsgSendQueue.m_i8Front = 0;
//    }
//    
//    g_stGPRSMsgSendQueue.m_i8Rear = (g_stGPRSMsgSendQueue.m_i8Rear + 1) % SIZE_GPRS_DATA_NODE_QUEUE;
//    
//    // Insert Data
//    g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui16SequenceNo = 999;
//    g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui32ItemWeight = 999;
//    g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui8RejectionStatus = 99;
//    g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui32TimeStamp = 999;    
//}


void InitializeGPRSMsgHeader(void)
{
    // Stuff Machine ID and Variety type in GPRS Message header.
    // Amta plant(Line 2) -> Machine ID - 2 and Marie Gold variety Type - 3.
    // Delta Foods Machine ID - 4
    // Super Snacks Machine ID - 5
    

    // Fetch Machine ID from memory.
    g_ui8MachineID = ReadMachineIDFromEEPROM();    
    //g_stGPRSMsgHeader.m_ui8MachineID = g_ui8MachineID;  // Delta
}



// ################################################################# Below GPRS Connection Driver #############################################################

//unsigned int8 SetPDPContext(void)
//{
//    unsigned int8 ui8IsOKFound = 1;
//    g_fIsTimedOut = 0;
//    
//    // Reset Rx buffer.
//    ResetGSMM66RxBuffer();
//    // Restart WDT.
//    restart_wdt();
//    // Set AT command for define PDP Context.    
//    if(1 == g_ui8SIMOperatorID)
//    {
//        fputs("AT+CGDCONT=1,\"IP\",\"airtelgprs.com\"\r", GSM_M66);    // For Airtel SIM.
//    }
//    else if (2 == g_ui8SIMOperatorID)
//    {
//        fputs("AT+CGDCONT=1,\"IP\",\"internet\"\r", GSM_M66);          // For Idea SIM.
//    }
//    else if (3 == g_ui8SIMOperatorID)
//    {
//        fputs("AT+CGDCONT=1,\"IP\",\"www\"\r", GSM_M66);               // For Vodafone SIM.
//    }
//    else
//    {
//        /* Do Nothing */
//    }                                     
//    // Set timeout.
//    SetTimeout(2);
//    // Wait for response.
//    while(g_fIsTimedOut == 0 && ui8IsOKFound == 1)
//    {
//        // Loop through entire receive buffer.
//        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
//        {
//            // Check for "OK".
//            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'K' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'O')
//               {
//                   // Set flag denoting 'OK' is found.
//                   ui8IsOKFound = 0;
//                   break;
//               }
//        }
//    }
//    
//    return ui8IsOKFound;
//}

//unsigned int8 ActivatePDP(void)
//{
//    unsigned int8 ui8IsOKFound = 1;
//    g_fIsTimedOut = 0;
//    
//    // Reset Rx buffer.
//    ResetGSMM66RxBuffer();
//    // Restart WDT.
//    restart_wdt();    
//    // Set AT command to activate PDP context.
//    fputs("AT+CGACT=1,1\r", GSM_M66);
//    // Set timeout.
//    SetTimeout(5);
//    // Wait for response.
//    while(g_fIsTimedOut == 0 && ui8IsOKFound == 1)
//    {
//        // Restart WDT.
//        restart_wdt();
//        // Loop through entire receive buffer.
//        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
//        {
//            // Check for "OK".
//            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'K' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'O')
//               {
//                   // Set flag denoting 'OK' is found.
//                   ui8IsOKFound = 0;
//                   break;
//               }
//        }
//    }
//    
//    return ui8IsOKFound;
//} 

//unsigned int8 ShowActivatedAddress(void)
//{
//    unsigned int8 ui8IsResponseFound            = 1;
//    unsigned int8 ui8FirstCharSequenceFound     = 0;
//    unsigned int8 ui8FirstCharSeqIndex          = 0;
//    unsigned int8 ui8SecondCharSequenceFound    = 0;
//    unsigned int8 ui8DoubleQuoteCount           = 0;
//   
//    
//    // Reset Rx buffer.
//    ResetGSMM66RxBuffer();
//    // Restart WDT.
//    restart_wdt();    
//    // Set AT command to Show activated PDP address.
//    fputs("AT+CGPADDR=1\r",GSM_M66);
//    mz_delay_ms(2000);
//    // Set timeout.
//    SetTimeout(2);
//    // Reset timeout flag.
//     g_fIsTimedOut = 0;
//    // Wait for response.
//    while(g_fIsTimedOut == 0 && ui8IsResponseFound == 1)
//    {
//        // Reset WDT
//        restart_wdt();
//        // Reset variables.
//        ui8FirstCharSequenceFound  = 0;
//        ui8FirstCharSeqIndex       = 0;
//        ui8SecondCharSequenceFound = 0;
//        
//        // Loop through entire receive buffer.
//        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount <= g_ui16M66RxByteCount; ui16ByteCount++)
//        {
//            // Check for first character sequence.
//            if(ui8FirstCharSequenceFound == 0 && ui8SecondCharSequenceFound == 0)
//            {
//                if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == ':' &&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 'R' &&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'D' &&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'D' &&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 4)] == 'A' &&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 5)] == 'P' &&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 6)] == 'G' &&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 7)] == 'C' &&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 8)] == '+')
//                {
//                    // Set flag denoting first character sequence has found.
//                    ui8FirstCharSequenceFound = 1;
//                    // Copy the first character sequence index.
//                    ui8FirstCharSeqIndex = (ui16ByteCount - 0);
//                }
//            }
//            
//            // Check for second character sequence set.
//            else if(ui8FirstCharSequenceFound == 1 && ui8SecondCharSequenceFound == 0)
//            {
//                if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == '"')
//                {
//                    // Increment double quote count.
//                    ui8DoubleQuoteCount++;
//                    // Check double quote count.
//                    if(2 == ui8DoubleQuoteCount)
//                    {
//                        // Reset double quote count.
//                        ui8DoubleQuoteCount = 0;
//                        // loop through to check second quote count.
//                        for(unsigned int8 ui8LoopCount = ui8FirstCharSeqIndex; ui8LoopCount < (ui16ByteCount - 0); ui8LoopCount++)
//                        {
//                            // Check for stop count.
//                            if(g_ui8M66RxBuffer[(ui8LoopCount - 0)] == '.')
//                            {
//                                // Increase stop.
//                                ui8DoubleQuoteCount++;
//                                // Check limiting stop count.
//                                if(3 == ui8DoubleQuoteCount)
//                                {
//                                    // Set flag denoting second character sequence has found.
//                                    ui8SecondCharSequenceFound = 1;
//                                    break;
//                                }
//                            }    
//                        }
//                        
//                        
//                    }
//                }
//            }
//            
//            // Check for OK.
//            else if(ui8FirstCharSequenceFound == 1 && ui8SecondCharSequenceFound == 1)
//            {
//                if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'K' &&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'O')
//               {
//                   // Set flag denoting 'OK' is found.
//                   ui8IsResponseFound = 0;
//                   break;
//               }
//            }
//        }
//        
//    }
//    
//    return ui8IsResponseFound;
//}

//void SendGPRSConnectionQuery(void)
//{
//    g_fIsTimedOut = 0;
//    
//    
//    // Reset Rx buffer.
//    ResetGSMM66RxBuffer();
//    // Check GPRS Connection.
//    fputs("AT+CGACT?\r", GSM_M66);
//    // Set timeout.
//    //SetTimeout(3);
//
//}

//unsigned int8 CheckGPRSConnection(void)
//{
//    unsigned int8 ui8IsResponseFound = 5;
//    unsigned int8 ui8IsFirstCharSequenceFound = 0;
//    unsigned int8 ui8FirstCharSequenceIndex = 0;
//    g_fIsTimedOut = 0;
//    
//    // Reset Rx buffer.
//    ResetGSMM66RxBuffer();
//    // Check GPRS Connection.
//    fputs("AT+CGACT?\r", GSM_M66);
//    // Set timeout.
//    SetTimeout(2);
//    // Wait for response.
//    while(g_fIsTimedOut == 0 && ui8IsResponseFound == 5)
//    {
//        // Reset variables.
//        ui8IsFirstCharSequenceFound = 0;
//        ui8FirstCharSequenceIndex = 0;
//        
//        // Loop through entire receive buffer.
//        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
//        {
//            // Check for first character sequence.
//            if(ui8IsFirstCharSequenceFound == 0)
//            {
//                if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == ':'&&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 'T'&&
//                   g_ui8M66RxBuffer[(ui16ByteCount-  2)] == 'C'&&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'A'&&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 4)] == 'G'&&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 5)] == 'C'&&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 6)] == '+')
//                {
//                    // Set flag denoting first character sequence found.
//                    ui8IsFirstCharSequenceFound = 1;
//                    // Copy the index of the first character of the sequence.
//                    ui8FirstCharSequenceIndex = (ui16ByteCount - 0);
//                }
//            }
//            // Check for second character sequence.
//            else if(ui8IsFirstCharSequenceFound == 1)
//            {
//                if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == '0'&&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 1)] == ','&&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 2)] == '1')
//                {
//                    // Set flag denoting particular response found.
//                    // and the response says GPRS not connected.
//                    ui8IsResponseFound = 0;
//                    break;
//                }
//                else if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == '1'&&
//                        g_ui8M66RxBuffer[(ui16ByteCount - 1)] == ','&&
//                        g_ui8M66RxBuffer[(ui16ByteCount - 2)] == '1')
//                {
//                    // Set flag denoting particular response found.
//                    // and the response says GPRS connected.
//                    ui8IsResponseFound = 1;
//                    break;
//                }
//            }
//        }
//    }
//    
//    return ui8IsResponseFound;
//}

//unsigned int8 CheckGPRSConnectionQueryResponse(void)
//{
//    unsigned int8 ui8IsResponseFound = 5;
//    unsigned int8 ui8IsFirstCharSequenceFound = 0;
//    unsigned int8 ui8FirstCharSequenceIndex = 0;
//    
//    
//    // Loop through entire receive buffer.
//    for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
//    {
//        // Check for first character sequence.
//        if(ui8IsFirstCharSequenceFound == 0)
//        {
//            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == ':'&&
//               g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 'T'&&
//               g_ui8M66RxBuffer[(ui16ByteCount-  2)] == 'C'&&
//               g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'A'&&
//               g_ui8M66RxBuffer[(ui16ByteCount - 4)] == 'G'&&
//               g_ui8M66RxBuffer[(ui16ByteCount - 5)] == 'C'&&
//               g_ui8M66RxBuffer[(ui16ByteCount - 6)] == '+')
//            {
//                // Set flag denoting first character sequence found.
//                ui8IsFirstCharSequenceFound = 1;
//                // Copy the index of the first character of the sequence.
//                ui8FirstCharSequenceIndex = (ui16ByteCount - 0);
//            }
//        }
//        // Check for second character sequence.
//        else if(ui8IsFirstCharSequenceFound == 1)
//        {
//            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == '0'&&
//               g_ui8M66RxBuffer[(ui16ByteCount - 1)] == ','&&
//               g_ui8M66RxBuffer[(ui16ByteCount - 2)] == '1')
//            {
//                // Set flag denoting particular response found.
//                // and the response says GPRS not connected.
//                ui8IsResponseFound = 0;
//                break;
//            }
//            else if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == '1'&&
//                    g_ui8M66RxBuffer[(ui16ByteCount - 1)] == ','&&
//                    g_ui8M66RxBuffer[(ui16ByteCount - 2)] == '1')
//            {
//                // Set flag denoting particular response found.
//                // and the response says GPRS connected.
//                ui8IsResponseFound = 1;
//                break;
//            }
//        }
//    }
//    
//    return ui8IsResponseFound;
//}

// ########################################################## Above GPRS Connection Driver ###############################################################

// ########################################################## Below HTTP Application Driver ##############################################################

//unsigned int8 SetHTTPURL(void)
//{
//    unsigned int8 ui8IsSetURLOk = 1;
//    unsigned int8 ui8IsConnectionDone = 0;
//    g_ui8URLInputTime           = 60;
//    g_ui8URLInputByteLength     = 93;
//    g_fIsTimedOut               = 0;
//    
//    // Reset Rx buffer.
//    ResetGSMM66RxBuffer();
//    // Increment variables.
//    g_ui16Itm1SequenceNumber += 2;
//    g_ui16Itm2SequenceNumber += 2;
//    g_ui16ItemWeight++;
//    // Check URL input byte length.
//    CheckURLInputByteLength();
//    // Set AT Commands to define web service URL.
//    fprintf(GSM_M66, "AT+QHTTPURL=%u,%u\r", g_ui8URLInputByteLength, g_ui8URLInputTime);
//    // Set timeout.
//    SetTimeout(2);
//    // Wait for response.
//    while(g_fIsTimedOut == 0 && ui8IsSetURLOk == 1)
//    {
//        // Loop through entire receive buffer.
//        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
//        {
//            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 'T' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 'C' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'E' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'N' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 4)] == 'N' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 5)] == 'O' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 6)] == 'C')
//            {
//                // Set flag denote connection is established.
//                ui8IsConnectionDone = 1;
//                break;
//            }       
//        }
//        
//        // set URL in buffer.
//        if(ui8IsConnectionDone)
//        {
//            // Set web service URL.
//            fprintf(GSM_M66, "http://mwm.itsinindia.com/api/Test/GetStringGetMethod?a=2&b=1&c=5&m=1000$%Lu$%Lu$1|1500$%Lu$%Lu$1", g_ui16Itm1SequenceNumber, g_ui16ItemWeight, g_ui16Itm2SequenceNumber, ++g_ui16ItemWeight);
//            delay_ms(300);
//            // Check for "OK".
//            for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
//            {
//                if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'K' &&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'O')
//                {
//                    // Set flag denoting Set URL successfully.
//                    ui8IsSetURLOk = 0;
//                    break;
//                }
//            }    
//        }
//        
//       
//    }
//    
//    return ui8IsSetURLOk;
//    
//}

//void SetHTTPURLParameter(void)
//{
//    g_ui8URLInputTime           = 60;
//    g_ui8URLInputByteLength     = 75;
//    g_fIsTimedOut               = 0;
//    
//    // Reset Rx buffer.
//    ResetGSMM66RxBuffer();
//    
//    // Check URL input byte length.
//    CheckURLInputByteLength();
//    
//    // Set AT Commands to define web service URL.
//    fprintf(GSM_M66, "AT+QHTTPURL=%u,%u\r", g_ui8URLInputByteLength, g_ui8URLInputTime);
//    // Set timeout.
//    //SetTimeout(2);
//}

//unsigned int8 SetHTTPURLL(void)
//{
//    unsigned int8  ui8IsSetURLOk       = 1;
//    unsigned int8  ui8IsConnectionDone = 0;
//    unsigned int16 ui16ByteCount       = 0;
//    
//    // Loop through entire receive buffer.
//    for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
//    {
//        if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 'T' &&
//           g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 'C' &&
//           g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'E' &&
//           g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'N' &&
//           g_ui8M66RxBuffer[(ui16ByteCount - 4)] == 'N' &&
//           g_ui8M66RxBuffer[(ui16ByteCount - 5)] == 'O' &&
//           g_ui8M66RxBuffer[(ui16ByteCount - 6)] == 'C')
//        {
//            // Set flag denote connection is established.
//            ui8IsConnectionDone = 1;
//            break;
//        }       
//    }
//    
//    // set URL in buffer.
//    if(ui8IsConnectionDone)
//    {
//
//        // Set web service URL.
//        fprintf(GSM_M66, "http://pipetrackerlive.com/api/Test/GetStringGetMethod?a=%u&b=%Lu&c=%Lu&d=%Lu&e=%Lu&m=%Lu$%Lu$%Lu$%u", g_stGPRSMsgHeader.m_ui8MachineID, g_stGPRSMsgHeader.m_ui16BasicMaterialStandard, g_stGPRSMsgHeader.m_ui16PipeSpecification,g_stGPRSMsgHeader.m_ui16MaxWeight,g_stGPRSMsgHeader.m_ui16MinWeight,
//                                                                                                                                 g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16SequenceNo, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32ItemWeight, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui8RejectionStatus);
//
//        
//        //delay_ms(300);
//        mz_delay_ms(300);
//        // Check for "OK".
//        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
//        {
//            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'K' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'O')
//            {
//                // Set flag denoting Set URL successfully.
//                ui8IsSetURLOk = 0;
//                break;
//            }
//        }    
//    }
//
//    return ui8IsSetURLOk;
//}

//void SendHTTPGetSessionRequest(void)
//{
//    g_fIsTimedOut = 0;
//    
//    // Reset Rx buffer.
//    ResetGSMM66RxBuffer();
//    // Set AT Command to start web service action.
//    fputs("AT+QHTTPGET=120\r", GSM_M66);
//    // Set timeout.
//    //SetTimeout(10);
//}

//unsigned int8 CheckHTTPGETRequestResponse(void)
//{
//    unsigned int8 ui8IsOKFound = 1;
//    unsigned int8 ui8IsFirstCharSeqFound  = 0;
//    unsigned int8 ui8IsSecondCharSeqFound = 0;
//    unsigned int8 ui8CharSeqIndex         = 0;
//    unsigned int8 ui8CommaCount           = 0;
//    
//    // Loop through entire receive buffer
//    for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
//    {
//        // Check for first character sequence.
//        if(ui8IsFirstCharSeqFound == 0 && ui8IsSecondCharSeqFound == 0)
//        {
//            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'K' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'O')
//            {
//                // Set flag denoting that first character sequence is found.
//                ui8IsFirstCharSeqFound = 1;
//            }
//        }
//
//        // Check for second character sequence.
//        else if(ui8IsFirstCharSeqFound == 1 && ui8IsSecondCharSeqFound == 0)
//        {
//            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == ':' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 'T' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'E' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'G' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 4)] == 'P' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 5)] == 'T' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 6)] == 'T' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 7)] == 'H' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 8)] == 'Q' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 9)] == '+' )
//            {
//                // Set flag denoting that second character sequence has found.
//                ui8IsSecondCharSeqFound = 1;
//            } 
//
//        }
//
//        // Check for third character sequence.
//        else if(ui8IsFirstCharSeqFound == 1 && ui8IsSecondCharSeqFound == 1)
//        {
//            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == ',')
//            {
//                // Increase comma count
//                ui8CommaCount++;
//                // Check for comma count level
//                if(ui8CommaCount == 2)
//                {
//                    // Check for success message code.
//                    if(g_ui8M66RxBuffer[(ui16ByteCount - 1)] == '0' &&
//                       g_ui8M66RxBuffer[(ui16ByteCount - 2)] == '0' &&
//                       g_ui8M66RxBuffer[(ui16ByteCount - 3)] == '2')
//                    {
//                        // Set flag denoting that success message code found.
//                        ui8IsOKFound = 0;
//                        break;
//                    }
//                }
//            }    
//        }
//
//    }
//    
//    return ui8IsOKFound;
//}

//unsigned int8 StartHTTPGETSession(void)
//{
//    unsigned int8 ui8IsOKFound = 1;
//    g_fIsTimedOut = 0;
//    
//    // Reset Rx buffer.
//    ResetGSMM66RxBuffer();
//    // Set AT Command to start web service action.
//    fputs("AT+QHTTPGET=120\r", GSM_M66);
//    // Set timeout.
//    SetTimeout(20);
//    // Wait for response.
//    while(g_fIsTimedOut == 0 && ui8IsOKFound == 1)
//    {
//        // Reset WDT
//        restart_wdt();
//        // Loop through entire receive buffer
//        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
//        {
//            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'K' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'O')
//            {
//                // Set flag denoting Set URL successfully.
//                ui8IsOKFound = 0;
//                break;
//            }
//        }
//    }
//    
//    return ui8IsOKFound;
//}


// ############################################################# Above HTTP Application driver ##########################################################
#endif	/* VM_GSMM66_GPRS_CONFIG_H */

