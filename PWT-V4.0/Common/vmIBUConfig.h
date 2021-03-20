#ifndef _VI_IBU_CONFIG_H_
#define _VI_IBU_CONFIG_H_

#include "vmCRCConfig.h"
#include "vmIBUGlobal.h"
#include "vmCircularQ.h"

#define IBU_MSG_BYTE_COUNT                       9 //5
//#define IBU_DATA_BITS_LENGTH                     10
//#define IBU_DATA_BYTES_LENGTH                    10
//#define IBU_BOARD_ID                             1

#define IBU_WAIT_FOR_ACK_TIMEOUT_OFF             0
#define IBU_WAIT_FOR_ACK_TIMEOUT_ON              1
#define DELAY_IBU_MSG_ACK_NOT_RCV                30


#ifdef M1_COMM_MODULE
    #define IBU_RX_1_BT_CUSTOMISED_VAL_9600        (0xFFFF - 0x0039 + 1)  // 78us Instead of 104us we load 91.2 for per bit time.
#else
    #define IBU_RX_1_BT_CUSTOMISED_VAL_9600        (0xFFFF - 0x0039 + 1)//   IBU_RX_BIT_TIME_T1LOAD_VAL_9600  // 104 us
#endif
//#define IBU_RX_1_BT_CUSTOMISED_VAL_9600           (0xFFFF - 0x0031 + 1)  // Instead of 104us we load 78 us for per bit time.
#define IBU_RX_BIT_TIME_1_2_T1LOAD_VAL_9600       (0xFFFF - 0x004E + 1)
#define IBU_RX_BIT_TIME_1_3_T1LOAD_VAL_9600       (0xFFFF - 0x0051 + 1)
#define IBU_RX_BIT_TIME_1_4_T1LOAD_VAL_9600       (0xFFFF - 0x005B + 1)


#define IBU_RX_BIT_TIME_1_5_T1LOAD_VAL_19200     (0xFFFF - 0x0031 + 1) // For 19200
#define IBU_RX_BIT_TIME_T1LOAD_VAL_19200         (0xFFFF - 0x0041 + 1) // For 19200

#define SIZE_IBU_SEND_BYTE_SEGMENT                15  //1+3+8+3, 1 Start Bit, 3 Sequence Bits, 8 bits of Data and 3 Stop Bits; Total 15bits in one segment/ custised byte

#define IBU_TIMER_0_CRITICAL_ON                   0
#define IBU_TIMER_0_CRITICAL_OFF                  1

#define IBU_RCV_COMPLETE                          10
#define IBU_RDY_TO_RCV_NEXT                       11
#define IBU_SENDING_BUSY                          1
#define IBU_SENDING_FREE                          0
#define SIZE_IBU_SEND_RCV_ARRAY                   4 + IBU_MSG_BYTE_COUNT // 1+1+IBU_MSG_BYTE_COUNT+2
#define SIZE_IBU_COMPLETE_SEND_MSG                SIZE_IBU_SEND_RCV_ARRAY + 5 // 3 Byte STX and 2 Byte ETX
#define IBU_MSG_TYPE_MSG                          33
#define IBU_MSG_TYPE_ACK                          44

#define SIZE_CAM_RCV_BUFF                         20
//*****************************************************************************
// Below -> Data Structure for Interboard Communication ***********************
//*****************************************************************************
unsigned int8 g_ui8CameraRcvBuffer[SIZE_CAM_RCV_BUFF] = {0};
unsigned int8 g_ui8CamRcvByteCount                    = 0;
//*****************************************************************************
// Above -> Data Structure for Interboard Communication ***********************
//*****************************************************************************





//*****************************************************************************
// Below -> Interboard Communication  RX #  From M1_COMM **********************
//*****************************************************************************

// CHANGE V9.0
typedef struct _ST_IBU_DATA_NODE
{
    unsigned int8  m_ui8SeqNum;
    unsigned int8  m_ui8MsgType;    // Either MSG or ACK
    unsigned int8  g_ui8ArrIBUData[IBU_MSG_BYTE_COUNT];
    unsigned int16 m_CRC;
} ST_IBU_DATA_NODE;

//typedef struct _ST_IBU_DATA_NODE
//{
//    unsigned int8 g_ui8ArrIBUData[IBU_MSG_BYTE_COUNT];
//} ST_IBU_DATA_NODE;

#define SIZE_IBU_DATA_SEND_QUEUE            15

typedef struct _ST_IBU_MSG_QUEUE
{
    ST_IBU_DATA_NODE m_stArrIBUDataNode[SIZE_IBU_DATA_SEND_QUEUE];
    int8             m_i8Front;
    int8             m_i8Rear;
} ST_IBU_MSG_QUEUE;
//*****************************************************************************
// Below -> Global Variable Declaration ***************************************
//*****************************************************************************

//UN_INT_BOARD_BYTE_TYPE    g_unIBURcvData;
unsigned int8             g_ui8IBURcvBitCount = 0;
unsigned int8             g_ui8IBUTxBitCount  = 0;

// TESTING
unsigned int8             g_ui8IBUDataToSend = 0xA5;
unsigned int8             g_fIBUCommStatus   = 0;

ST_IBU_DATA_NODE          g_stIBURcvData;
ST_IBU_DATA_NODE          g_stIBUSendData;
unsigned int8             g_ui8ArrSendSingleMSG[IBU_MSG_BYTE_COUNT*(SIZE_IBU_SEND_BYTE_SEGMENT)] = {0}; //
unsigned int8             g_fIBUDataReceived = 0;

//ST_IBU_DATA_SEND_QUEUE    g_stIBUDataSendQueue;
unsigned int8             g_fIBUSendBusy = IBU_SENDING_FREE;
//UN_IBU_RCV_BYTE_NODE      g_unIBURcvByteNode;

ST_IBU_MSG_QUEUE          g_stIBUSendQueue;
ST_IBU_MSG_QUEUE          g_stIBURcvQueue;

unsigned int8             g_ui8fTimer0State    = IBU_TIMER_0_CRITICAL_OFF;
unsigned int8             g_ui8IBURcvByteCount = 0;
unsigned int8             g_fIBUDataRcvStatus  = 0;

int8                      g_fIBUMsgAckDelayStatus      = IBU_WAIT_FOR_ACK_TIMEOUT_OFF;
unsigned int16            g_ui16IBUWaitForACKTikCount  = 0;
unsigned int8             g_ui8IBURcvDataTemp[SIZE_IBU_SEND_RCV_ARRAY];
//*****************************************************************************
// Above -> Global Variable Declaration ***************************************
//*****************************************************************************


void InitIBURxInterrupt(void)
{
    // Enable IBU Rcv interrupt.
    clear_interrupt(INT_RDA);
	enable_interrupts(INT_RDA);
}

#ifdef M0_ADC_MODULE
// ############### Should not be here; Must be in specific camera config header.############################ //
void InitCamRxInterrupt(void)
{
    // Enable camera communication receive interrupt.
    clear_interrupt(INT_RDA4);
	enable_interrupts(INT_RDA4);    
}

#INT_RDA4
void rda4_isr(void)
{
    // Receive buffer.
    g_ui8CameraRcvBuffer[g_ui8CamRcvByteCount] = fgetc(CAMERA);
    
    // Increment receive byte count.
    g_ui8CamRcvByteCount++;
    
    // Check with buffer size.
    if(g_ui8CamRcvByteCount >= SIZE_CAM_RCV_BUFF)
    {
        // Reset byte count.
        g_ui8CamRcvByteCount = 0;
    }
    
    // Clear interrupt.
    clear_interrupt(INT_RDA4);
}

#endif

/////////////////// Camera config header TESTING part : Temporary ///////////////////////////////////////////


////////////// Common QUEUE for Data Sending and Receiving START ///////////////

void InitializeDataQueue(ST_IBU_MSG_QUEUE *pstIBUMSGQueue)
{
    int iLoopCount1 = 0;
    int iLoopCount2 = 0;
    pstIBUMSGQueue->m_i8Front = pstIBUMSGQueue->m_i8Rear = -1;
    
    for (iLoopCount1 = 0; iLoopCount1 < SIZE_IBU_DATA_SEND_QUEUE; iLoopCount1++)
    {
        for (iLoopCount2 = 0; iLoopCount2 < IBU_MSG_BYTE_COUNT; iLoopCount2++)
        {
           pstIBUMSGQueue->m_stArrIBUDataNode[iLoopCount1].g_ui8ArrIBUData[iLoopCount2] = 0;
        }
    }
}

void CopyIBUMsg(ST_IBU_DATA_NODE* pstIBUSource, ST_IBU_DATA_NODE* pstIBUDDest)
{
    int8 i8LoopCount = 0;
    
    for (i8LoopCount = 0; i8LoopCount < IBU_MSG_BYTE_COUNT; i8LoopCount++)
    {
        pstIBUDDest->g_ui8ArrIBUData[i8LoopCount] = pstIBUSource->g_ui8ArrIBUData[i8LoopCount];
    }
}

/* Function to create Circular queue */
void InsertIBUMsgQueue(ST_IBU_MSG_QUEUE *pstIBUMSGQueue, ST_IBU_DATA_NODE* pstIBUData) 
{
    if (!pstIBUData)
    {
        return;
    }
    
//    if ((pstIBUMSGQueue->m_i8Front == 0 && pstIBUMSGQueue->m_i8Rear == SIZE_IBU_DATA_SEND_QUEUE-1) || 
//            (pstIBUMSGQueue->m_i8Rear == (pstIBUMSGQueue->m_i8Front-1)%(SIZE_IBU_DATA_SEND_QUEUE-1))) 
//    { 
//        //printf("\nQueue is Full"); 
//        return;
//    }
    if ((pstIBUMSGQueue->m_i8Front == 0 && pstIBUMSGQueue->m_i8Rear == SIZE_IBU_DATA_SEND_QUEUE-1) || 
           (pstIBUMSGQueue->m_i8Rear == (pstIBUMSGQueue->m_i8Front-1))) 
    { 
        //printf("\nQueue is Full"); 
        return;
    }    
    
    else if (pstIBUMSGQueue->m_i8Front == -1) /* Insert First Element */
    { 
        pstIBUMSGQueue->m_i8Front = 0;
        pstIBUMSGQueue->m_i8Rear  = 0;
        
        // Insert Data
        //arr[g_stIBUDataSendQueue.m_i8Rear] = value;
        CopyIBUMsg(pstIBUData, &(pstIBUMSGQueue->m_stArrIBUDataNode[pstIBUMSGQueue->m_i8Rear]));
    } 
  
    else if (pstIBUMSGQueue->m_i8Rear == SIZE_IBU_DATA_SEND_QUEUE-1 && pstIBUMSGQueue->m_i8Front != 0) 
    { 
        pstIBUMSGQueue->m_i8Rear = 0; 
        CopyIBUMsg(pstIBUData, &(pstIBUMSGQueue->m_stArrIBUDataNode[pstIBUMSGQueue->m_i8Rear]));
    } 
  
    else
    { 
        pstIBUMSGQueue->m_i8Rear++; 
        CopyIBUMsg(pstIBUData, &(pstIBUMSGQueue->m_stArrIBUDataNode[pstIBUMSGQueue->m_i8Rear]));
    }
    
//    if (IBU_CMD_PROD_WEIGH_DATA == pstIBUData->g_ui8ArrIBUData[0])
//    {
//        output_toggle(PIN_OUT_DO_00);
//    }
} 

void DeleteIBUMsgQueue(ST_IBU_MSG_QUEUE *pstIBUMSGQueue, ST_IBU_DATA_NODE* pstIBUData) 
{
    if (pstIBUMSGQueue->m_i8Front == -1)
    { 
        //printf("\nQueue is Empty"); 
        return; 
    } 

    // Copy Message Node from Front
    CopyIBUMsg(&(pstIBUMSGQueue->m_stArrIBUDataNode[pstIBUMSGQueue->m_i8Front]), pstIBUData);

    if (pstIBUMSGQueue->m_i8Front == pstIBUMSGQueue->m_i8Rear) 
    { 
        pstIBUMSGQueue->m_i8Front = -1; 
        pstIBUMSGQueue->m_i8Rear  = -1; 
    } 
    else if (pstIBUMSGQueue->m_i8Front == SIZE_IBU_DATA_SEND_QUEUE-1) 
        pstIBUMSGQueue->m_i8Front = 0; 
    else
        pstIBUMSGQueue->m_i8Front++; 
  
    return;
}

void GetIBUMsgQueue(ST_IBU_MSG_QUEUE *pstIBUMSGQueue, ST_IBU_DATA_NODE* pstIBUData) 
{
    if (pstIBUMSGQueue->m_i8Front == -1)
    { 
        //printf("\nQueue is Empty"); 
        return; 
    } 

    // Copy Message Node from Front
    CopyIBUMsg(&(pstIBUMSGQueue->m_stArrIBUDataNode[pstIBUMSGQueue->m_i8Front]), pstIBUData);
  
}

// This Function will remove the first Node from Message Queue
void RemoveIBUMsgQueue(ST_IBU_MSG_QUEUE *pstIBUMSGQueue) 
{
    if (pstIBUMSGQueue->m_i8Front == -1)
    { 
        //printf("\nQueue is Empty"); 
        return; 
    } 

    if (pstIBUMSGQueue->m_i8Front == pstIBUMSGQueue->m_i8Rear) 
    { 
        pstIBUMSGQueue->m_i8Front = -1; 
        pstIBUMSGQueue->m_i8Rear  = -1; 
    } 
    else if (pstIBUMSGQueue->m_i8Front == SIZE_IBU_DATA_SEND_QUEUE-1) 
        pstIBUMSGQueue->m_i8Front = 0; 
    else
        pstIBUMSGQueue->m_i8Front++; 
  
}

unsigned int8 IsMSGInIBUQueue(ST_IBU_MSG_QUEUE *pstIBUMSGQueue)
{
    unsigned int8 ui8RetVal = 0;
    if (pstIBUMSGQueue->m_i8Front == -1)
    {
        ui8RetVal = 0;
    }
    else
    {
        ui8RetVal = 1;
    }
    
    return ui8RetVal;
}

////////////// Common QUEUE for Data Sending and Receiving END /////////////////
// CHANGE V9.0
#define MAX_SEND_TRY            10
unsigned int8 g_ui8UARTMsgSeq = 0;
unsigned int8 g_ui8ArrSendBuffer[SIZE_IBU_SEND_RCV_ARRAY] = {0}; // Seq No -> 1 Byte, MSG Type -> 1 Byte, Data -> 9 Byte, CRC -> 2 Byte
unsigned int16 g_ui16CRCValue = 0;
unsigned int8  g_ui8SendTryCount = 0;

void SendIBUMSGFromQ()
{
    int8          i8LoopCount    = 0;
    
    // Check if it is the last message does not get ACK
    if (g_ui8SendTryCount >= MAX_SEND_TRY)
    {
        // Ignore the Message and Proceed for next Message
        g_ui8SendTryCount = 0;
        
        // Remove the message from Send Queue
        RemoveIBUMsgQueue(&g_stIBUSendQueue);
        
        // Reset flag denoting IBU is free now
        g_fIBUSendBusy = IBU_SENDING_FREE;
        return;
    }

    // Increment the Send Message Try Count
    g_ui8SendTryCount++;
    
    // Initialize the Send Array
    for (i8LoopCount = 0; i8LoopCount < IBU_MSG_BYTE_COUNT; i8LoopCount++)
    {
        g_stIBUSendData.g_ui8ArrIBUData[i8LoopCount] = 0;
    }

    // Get Node from Queue
    //DeleteIBUMsgQueue(&g_stIBUSendQueue, &g_stIBUSendData);
    GetIBUMsgQueue(&g_stIBUSendQueue, &g_stIBUSendData);
    
    // Insert the Sequence Number
    if (1 == g_ui8SendTryCount)
    {
        if (255 == g_ui8UARTMsgSeq)
        {
          g_ui8UARTMsgSeq = 0;  
        }
        else
        {
          ++g_ui8UARTMsgSeq;  
        }       
    }
    
    g_stIBUSendData.m_ui8SeqNum = g_ui8UARTMsgSeq;
    
    g_ui8ArrSendBuffer[0]  = g_ui8UARTMsgSeq;
    g_ui8ArrSendBuffer[1]  = IBU_MSG_TYPE_MSG;
    
    g_ui8ArrSendBuffer[2]  = g_stIBUSendData.g_ui8ArrIBUData[0];
    g_ui8ArrSendBuffer[3]  = g_stIBUSendData.g_ui8ArrIBUData[1];
    g_ui8ArrSendBuffer[4]  = g_stIBUSendData.g_ui8ArrIBUData[2];
    g_ui8ArrSendBuffer[5]  = g_stIBUSendData.g_ui8ArrIBUData[3];
    g_ui8ArrSendBuffer[6]  = g_stIBUSendData.g_ui8ArrIBUData[4];
    g_ui8ArrSendBuffer[7]  = g_stIBUSendData.g_ui8ArrIBUData[5];
    g_ui8ArrSendBuffer[8]  = g_stIBUSendData.g_ui8ArrIBUData[6];
    g_ui8ArrSendBuffer[9]  = g_stIBUSendData.g_ui8ArrIBUData[7];
    g_ui8ArrSendBuffer[10] = g_stIBUSendData.g_ui8ArrIBUData[8];
    //g_ui8ArrSendBuffer[11] = g_stIBUSendData.g_ui8ArrIBUData[9];
    
//    g_ui8ArrSendBuffer[12] = 0;
//    g_ui8ArrSendBuffer[13] = 0;
    
    
    // Insert MSG CRC
    // CRC calculation
    g_ui16CRCValue = calculateCrc(g_ui8ArrSendBuffer, SIZE_IBU_SEND_RCV_ARRAY - 2);

    // Stuff CRC
    g_ui8ArrSendBuffer[11] = (unsigned int8)((g_ui16CRCValue >> 0) & 0xFF);

    g_ui8ArrSendBuffer[12] = (unsigned int8)((g_ui16CRCValue >> 8) & 0xFF);

    // Send Start Text to protect spurious data.
    fputc(STX, UART);
    fputc(STX, UART);
    fputc(STX, UART);
    // Send Original Data.
    for (i8LoopCount = 0; i8LoopCount < SIZE_IBU_SEND_RCV_ARRAY; i8LoopCount++)
    {
        fputc(g_ui8ArrSendBuffer[i8LoopCount], UART);
        //delay_us(1);
    }
    fputc(ETX, UART);
    fputc(ETX, UART);
    delay_us(5);
    
    // Start the Timer for Ack
    g_fIBUMsgAckDelayStatus = IBU_WAIT_FOR_ACK_TIMEOUT_ON;
    
    // Reset flag denoting IBU is free now.
    //g_fIBUSendBusy = IBU_SENDING_FREE;
}

void ResetIBURcvBuffer(void)
{
    int8 i8LoopCount = 0;
    for(i8LoopCount = 0; i8LoopCount < IBU_MSG_BYTE_COUNT; i8LoopCount++)
    {
        g_stIBURcvData.g_ui8ArrIBUData[i8LoopCount] = 0;
    }
    
    // Reset rcv buffer byte count.
    g_ui8IBURcvByteCount = 0;
}


void ResetIBUSendBuffer(void)
{
    int8 i8LoopCount = 0;
    for(i8LoopCount = 0; i8LoopCount < IBU_MSG_BYTE_COUNT; i8LoopCount++)
    {
        g_stIBUSendData.g_ui8ArrIBUData[i8LoopCount] = 0;
    }
}

#define SIZE_RCV_BUFFER           100
#define FLAG_STX_FOUND              1
#define FLAG_STX_NOT_FOUND          0
#define FLAG_ETX_FOUND              1
#define FLAG_ETX_NOT_FOUND          0
unsigned int8 g_ui8ArrRCVBuffer[SIZE_RCV_BUFFER + 2] = {0}; // 2 Bytes extra just to prevent
                                                            // unauthorized access beyond Full Array Length
unsigned int8 g_ui8RCVQFront         = 0;
unsigned int8 g_ui8RCVQRear          = 0;
unsigned int8 g_ui8FlagETXFound      = 0;
unsigned int8 g_ui8FlagSTXFound      = 0;
unsigned int8 g_ui8TempRcvMSG[SIZE_IBU_SEND_RCV_ARRAY] = {0};
//unsigned int8 g_ui8SendMsgSeq = 0;

void ProcessACK(unsigned int8 ui8MsgSeqNo)
{
    // Try to match the Send Message Sequence Number for which we are waiting for Ack
    if (g_ui8UARTMsgSeq == ui8MsgSeqNo)
    {
        // Remove the first message node from Send MSG Queue
        RemoveIBUMsgQueue(&g_stIBUSendQueue);
        
        // Reset Send Try
        g_ui8SendTryCount = 0;
        
        // Reset Timer
        g_fIBUMsgAckDelayStatus      = IBU_WAIT_FOR_ACK_TIMEOUT_OFF;
        g_ui16IBUWaitForACKTikCount  = 0;
        
        // Reset flag denoting IBU is free now.
        g_fIBUSendBusy = IBU_SENDING_FREE;
    }
    else
    {
        // Some Acknowledgment has arrived but somehow the sequence number got corrupted
        // Need to Resend the message Again
        
        // Reset Timer
        g_fIBUMsgAckDelayStatus      = IBU_WAIT_FOR_ACK_TIMEOUT_OFF;
        g_ui16IBUWaitForACKTikCount  = 0;
        
        // Reset flag denoting IBU is free now
        g_fIBUSendBusy = IBU_SENDING_FREE;
    }
}

void CheckIBUReceiveQ()
{
    unsigned int8 ui8CurPosition = g_stRcvQueue.m_ui8Front;
    unsigned int8 ui8Count       = 0;
    unsigned int8 ui8CurNextPos = g_stRcvQueue.m_ui8Front;
    unsigned int8 ui8STXPos       = 0;
    
    g_ui8RCVQFront = g_stRcvQueue.m_ui8Front;
    g_ui8RCVQRear  = g_stRcvQueue.m_ui8Rear;
    
    g_ui8FlagSTXFound = FLAG_STX_NOT_FOUND;
    g_ui8FlagETXFound = FLAG_ETX_NOT_FOUND;
    
    // Check for the at least two consecutive STXs
    while (g_ui8ElementInQ)
    {
        // Find the next Position
        ui8CurNextPos = (g_stRcvQueue.m_ui8Front + 1) % SIZE_RCV_QUEUE;
        
        if (STX == g_stRcvQueue.m_ui8QArr[g_stRcvQueue.m_ui8Front] &&
            STX == g_stRcvQueue.m_ui8QArr[ui8CurNextPos])
        {
            // We got Two Consecutive STXs
            break;
        }
        else
        {
            // Update Queue Front
            g_stRcvQueue.m_ui8Front = ui8CurNextPos;
            g_ui8ElementInQ--;
        }
    }
    
    // Now Check how we exit from the above loop
    // If there is no STX found through out the stretch
    if (0 == g_ui8ElementInQ)
    {
        // The message exhausted
        return;
    }
    // So we found two consecutive STXs. There might be more STXs
    // Now check for the final STX and ETX pair
    // STX STX STX SEQ ACK D1 D2 D3 D4 D5  D6  D7  D8  D9  CRC1 CRC2 ETX ETX
    //  0   1   2   3   4   5  6  7  8  9  10  11  12  13   14   15   16  17
    while (g_ui8ElementInQ > SIZE_IBU_SEND_RCV_ARRAY)
    {
        // Get the Expected STX Position
        ui8STXPos = (g_stRcvQueue.m_ui8Front + 14) % SIZE_RCV_QUEUE;
        if (STX == g_stRcvQueue.m_ui8QArr[g_stRcvQueue.m_ui8Front])
        {
            if (ETX == g_stRcvQueue.m_ui8QArr[ui8STXPos])
            {
                // Perfect Message Found
                break;
            }
            else
            {
                g_stRcvQueue.m_ui8Front = (g_stRcvQueue.m_ui8Front + 1) % SIZE_RCV_QUEUE;
                g_ui8ElementInQ--;
            }
        }
        else
        {
            // STX Train Exhausted, No chance to get a complete message
            return;
        }
    }
    
    g_stRcvQueue.m_ui8Front = (g_stRcvQueue.m_ui8Front + 1) % SIZE_RCV_QUEUE;
    g_ui8ElementInQ--;
    // We have got a complete message
    // Copy the message from Receive Queue to Receive Buffer
    for (ui8Count = 0; ui8Count < SIZE_IBU_SEND_RCV_ARRAY; ui8Count++)
    {
        g_ui8IBURcvDataTemp[ui8Count] = g_stRcvQueue.m_ui8QArr[g_stRcvQueue.m_ui8Front];
        g_stRcvQueue.m_ui8Front = (g_stRcvQueue.m_ui8Front + 1) % SIZE_RCV_QUEUE;
        g_ui8ElementInQ--;
    }
    
    
    // Move the front upto next STX or end of current element count
    while (g_ui8ElementInQ)
    {
        if (STX == g_stRcvQueue.m_ui8QArr[g_stRcvQueue.m_ui8Front])
        {
            // It might be STX of next message
            // Queue front is pointing to this STX and let us return from here
            break;
        }
        g_stRcvQueue.m_ui8Front = (g_stRcvQueue.m_ui8Front + 1) % SIZE_RCV_QUEUE;
        g_ui8ElementInQ--;
    }
    
    // Get Next Rear Position
    ui8CurNextPos = (g_stRcvQueue.m_ui8Rear + 1) % SIZE_RCV_QUEUE;
    if (g_stRcvQueue.m_ui8Front == ui8CurNextPos)
    {
        // No farther elements in the queue
        g_stRcvQueue.m_ui8Front = g_stRcvQueue.m_ui8Rear = -1;
    }
    
    // Now let us process the Received Message
    // There are two types of IBU Messages
    //     1. Acknowledgment against a Fresh Message
    //     2. Fresh Message Received
    // SEQ ACK D1 D2 D3 D4 D5 D6 D7 D8 D9 CRC1 CRC2 
    //  0   1   2  3  4  5  6  7  8  9 10  11   12  
    // For ACK Message
    if (IBU_MSG_TYPE_ACK == g_ui8IBURcvDataTemp[1])
    {
        // Send the MSG Sequence Number to process Acknowledgment
        ProcessACK(g_ui8IBURcvDataTemp[0]);
        return;
    }
    
    // For Fresh Message Received
    if (IBU_MSG_TYPE_MSG == g_ui8IBURcvDataTemp[1])
    {
        // Check the Message CRC
        // Calculate CRC
        g_ui16CRCValue = calculateCrc(g_ui8IBURcvDataTemp, SIZE_IBU_SEND_RCV_ARRAY - 2);
        
        // Validate CRC.
        if(((g_ui16CRCValue >> 8) & 0xFF) == g_ui8IBURcvDataTemp[12] &&
           ((g_ui16CRCValue >> 0) & 0xFF) == g_ui8IBURcvDataTemp[11])
        {
            // Insert the Message into IBU Receive Queue
            g_stIBURcvData.g_ui8ArrIBUData[0] = g_ui8IBURcvDataTemp[2];
            g_stIBURcvData.g_ui8ArrIBUData[1] = g_ui8IBURcvDataTemp[3];
            g_stIBURcvData.g_ui8ArrIBUData[2] = g_ui8IBURcvDataTemp[4];
            g_stIBURcvData.g_ui8ArrIBUData[3] = g_ui8IBURcvDataTemp[5];
            g_stIBURcvData.g_ui8ArrIBUData[4] = g_ui8IBURcvDataTemp[6];
            g_stIBURcvData.g_ui8ArrIBUData[5] = g_ui8IBURcvDataTemp[7];
            g_stIBURcvData.g_ui8ArrIBUData[6] = g_ui8IBURcvDataTemp[8];
            g_stIBURcvData.g_ui8ArrIBUData[7] = g_ui8IBURcvDataTemp[9];
            g_stIBURcvData.g_ui8ArrIBUData[8] = g_ui8IBURcvDataTemp[10];

            // Insert Message into IBC Receive Queue
            InsertIBUMsgQueue(&g_stIBURcvQueue, &g_stIBURcvData);

            // Send Acknowledgment
            g_ui8IBURcvDataTemp[1] = IBU_MSG_TYPE_ACK;
            
            // No need to hold the Send Lock, because this function is called from main thread
            // and only main thread can send IBU Data
            // Send Start Text to protect spurious data.
            fputc(STX, UART);
            fputc(STX, UART);
            fputc(STX, UART);
            // Send Original Data.
            for (ui8Count = 0; ui8Count < SIZE_IBU_SEND_RCV_ARRAY; ui8Count++)
            {
                fputc(g_ui8IBURcvDataTemp[ui8Count], UART);
            }
            fputc(ETX, UART);
            fputc(ETX, UART);
        }
    }
    else
    {
        // For Invalid CRC
        // Nothing to Do
    }
    
    // All Elements exhausted g_ui8ElementInQ == 0
    return;
}

void CheckIBUReceiveBuffer()
{
    unsigned int8 ui8CurPosition = g_ui8RCVQFront;
    unsigned int8 ui8Count       = 0;
    
    g_ui8FlagSTXFound = FLAG_STX_NOT_FOUND;
    g_ui8FlagETXFound = FLAG_ETX_NOT_FOUND;
    
    // Step 1: Check for three consecutive STX
    for (ui8CurPosition = 0; ui8CurPosition < g_ui8RCVQRear; ui8CurPosition++)
    {
        if (STX == g_ui8ArrRCVBuffer[ui8CurPosition])
        {
            // Check for STX in Next two Position
            if (STX == g_ui8ArrRCVBuffer[ui8CurPosition + 1] &&
                STX == g_ui8ArrRCVBuffer[ui8CurPosition + 2])
            {
                // Got Three STXs
                g_ui8FlagSTXFound = FLAG_STX_FOUND;
                break;
            }
        }
    }
    
    // If the Buffer exhausted and STX not found
    if (ui8CurPosition == g_ui8RCVQRear)
    {
        // Update front and rear and exit, may be we got spurious data
        g_ui8RCVQFront = 0;
        g_ui8RCVQRear  = 0;
        
        return;
    }
    
    // STX STX STX SEQ ACK D1 D2 D3 D4 D5  D6  D7  D8  D9  CRC1 CRC2 ETX  ETX
    //  0   1   2   3   4   5  6  7  8  9  10  11  12  13   14   15   16   17
    
    // Now Check for the ETX
    if (ETX == g_ui8ArrRCVBuffer[ui8CurPosition + 2 + SIZE_IBU_SEND_RCV_ARRAY + 1])
    {
        g_ui8FlagETXFound = FLAG_ETX_FOUND;  //TODO: No use of this variable, can be removed
    }
    else
    {
        // No ETX Found
        // Receive message got corrupted
        // Reset Receive MSG Array
        g_ui8RCVQFront = 0;
        g_ui8RCVQRear  = 0;
        
        return;
    }
    
    // STX and ETX Both are found
    // Now check MSG Type
    if(IBU_MSG_TYPE_ACK == g_ui8ArrRCVBuffer[ui8CurPosition + 4])
    {
        // This is an Acknowledgment Message
        // STX STX STX SEQ ACK D1 D2 D3 D4 D5  D6  D7  D8  D9  CRC1 CRC2 ETX  ETX
        //  0   1   2   3   4   5  6  7  8  9  10  11  12  13   14   15   16   17

        ProcessACK(g_ui8ArrRCVBuffer[ui8CurPosition + 3]);
        g_ui8RCVQFront = 0;
        g_ui8RCVQRear  = 0;
        return;
    }
    
    // If the MSG is not ACK that means fresh MSG received
    // Now check for CRC
    // STX STX STX SEQ ACK D1 D2 D3 D4 D5  D6  D7  D8  D9  CRC1 CRC2 ETX  ETX
    //  0   1   2   3   4   5  6  7  8  9  10  11  12  13   14   15   16   17
    for (ui8Count = 0; ui8Count < SIZE_IBU_SEND_RCV_ARRAY - 2; ui8Count++)
    {
        g_ui8TempRcvMSG[ui8Count] = g_ui8ArrRCVBuffer[ui8CurPosition + 3 + ui8Count];
    }
    
//    g_ui8TempRcvMSG[ui8Count]     = 0;
//    g_ui8TempRcvMSG[ui8Count + 1] = 0;
    
    // Calculate CRC
    g_ui16CRCValue = calculateCrc(g_ui8TempRcvMSG, SIZE_IBU_SEND_RCV_ARRAY - 2);
    
    // Validate CRC.
    if(((g_ui16CRCValue >> 8) & 0xFF) == g_ui8ArrRCVBuffer[ui8CurPosition + 3 + ui8Count + 1] &&
       ((g_ui16CRCValue >> 0) & 0xFF) == g_ui8ArrRCVBuffer[ui8CurPosition + 3 + ui8Count])
    {
        // Insert the Message into IBU Receive Queue
        g_stIBURcvData.g_ui8ArrIBUData[0] = g_ui8TempRcvMSG[2];
        
        g_stIBURcvData.g_ui8ArrIBUData[1] = g_ui8TempRcvMSG[3];
        g_stIBURcvData.g_ui8ArrIBUData[2] = g_ui8TempRcvMSG[4];
        g_stIBURcvData.g_ui8ArrIBUData[3] = g_ui8TempRcvMSG[5];
        g_stIBURcvData.g_ui8ArrIBUData[4] = g_ui8TempRcvMSG[6];
        
        g_stIBURcvData.g_ui8ArrIBUData[5] = g_ui8TempRcvMSG[7];
        g_stIBURcvData.g_ui8ArrIBUData[6] = g_ui8TempRcvMSG[8];
        g_stIBURcvData.g_ui8ArrIBUData[7] = g_ui8TempRcvMSG[9];
        g_stIBURcvData.g_ui8ArrIBUData[8] = g_ui8TempRcvMSG[10];


        // Insert Message into IBC Receive Queue
        InsertIBUMsgQueue(&g_stIBURcvQueue, &g_stIBURcvData);

        // Reset receive buffer.
        ResetIBURcvBuffer();
        
        // Send Acknowledgment
        g_ui8TempRcvMSG[1] = IBU_MSG_TYPE_ACK;
        g_ui8TempRcvMSG[11] = g_ui8ArrRCVBuffer[ui8CurPosition + 3 + ui8Count];
        g_ui8TempRcvMSG[12] = g_ui8ArrRCVBuffer[ui8CurPosition + 3 + ui8Count + 1];
        
        // No need to hold the Send Lock, because this function is called from main thread
        // and only main thread can send IBU Data
        // Send Start Text to protect spurious data.
        fputc(STX, UART);
        fputc(STX, UART);
        fputc(STX, UART);
        // Send Original Data.
        for (ui8Count = 0; ui8Count < SIZE_IBU_SEND_RCV_ARRAY; ui8Count++)
        {
            fputc(g_ui8TempRcvMSG[ui8Count], UART);
        }
        fputc(ETX, UART);
        fputc(ETX, UART);
        delay_us(5);
    }
    else  // For Invalid CRC
    {
        // Receive message got corrupted
        // Reset Receive MSG Array
        g_ui8RCVQFront = 0;
        g_ui8RCVQRear  = 0;
    }
    
    g_ui8RCVQFront = 0;
    g_ui8RCVQRear  = 0;
}

////////void CheckIBUReceiveBuffer()
////////{
////////    unsigned int8 ui8CurPosition = g_ui8RCVQFront;
////////    unsigned int8 ui8Count       = 0;
////////    
////////    g_ui8FlagSTXFound = FLAG_STX_NOT_FOUND;
////////    g_ui8FlagETXFound = FLAG_ETX_NOT_FOUND;
////////    
////////    // Step 1: Check for three consecutive STX
////////    for (ui8CurPosition = 0; ui8CurPosition < g_ui8RCVQRear; ui8CurPosition++)
////////    {
////////        if (STX == g_ui8ArrRCVBuffer[ui8CurPosition])
////////        {
////////            // Check for STX in Next two Position
////////            if (STX == g_ui8ArrRCVBuffer[ui8CurPosition + 1] &&
////////                STX == g_ui8ArrRCVBuffer[ui8CurPosition + 2])
////////            {
////////                // Got Three STXs
////////                g_ui8FlagSTXFound = FLAG_STX_FOUND;
////////                break;
////////            }
////////        }
////////    }
////////    
////////    // If the Buffer exhausted and STX not found
////////    if (ui8CurPosition == g_ui8RCVQRear)
////////    {
////////        // Update front and rear and exit, may be we got spurious data
////////        g_ui8RCVQFront = 0;
////////        g_ui8RCVQRear  = 0;
////////        
////////        return;
////////    }
////////    
////////    // STX STX STX SEQ ACK D1 D2 D3 D4 D5 CRC1 CRC2 ETX ETX
////////    //  0   1   2   3   4   5  6  7  8  9  10   11   12  13
////////    
////////    // Now Check for the ETX
////////    if (ETX == g_ui8ArrRCVBuffer[ui8CurPosition + 2 + SIZE_IBU_SEND_RCV_ARRAY + 1])
////////    {
////////        g_ui8FlagETXFound = FLAG_ETX_FOUND;
////////    }
////////    else
////////    {
////////        // No ETX Found
////////        // Receive message got corrupted
////////        // Reset Receive MSG Array
////////        g_ui8RCVQFront = 0;
////////        g_ui8RCVQRear  = 0;
////////        
////////        return;
////////    }
////////    
////////    // STX and ETX Both are found
////////    // Now check MSG Type
////////    if(IBU_MSG_TYPE_ACK == g_ui8ArrRCVBuffer[ui8CurPosition + 4])
////////    {
////////        // This is an Acknowledgment Message
////////        // STX STX STX SEQ ACK D1 D2 D3 D4 D5 CRC1 CRC2 ETX ETX
////////        //  0   1   2   3   4   5  6  7  8  9  10   11   12  13
////////        ProcessACK(g_ui8ArrRCVBuffer[ui8CurPosition + 3]);
////////        g_ui8RCVQFront = 0;
////////        g_ui8RCVQRear  = 0;
////////        return;
////////    }
////////    
////////    // If the MSG is not ACK that means fresh MSG received
////////    // Now check for CRC
////////    // STX STX STX SEQ ACK D1 D2 D3 D4 D5 CRC1 CRC2 ETX ETX
////////    //  0   1   2   3   4   5  6  7  8  9  10   11   12  13
////////    for (ui8Count = 0; ui8Count < SIZE_IBU_SEND_RCV_ARRAY - 2; ui8Count++)
////////    {
////////        g_ui8TempRcvMSG[ui8Count] = g_ui8ArrRCVBuffer[ui8CurPosition + 3 + ui8Count];
////////    }
////////    
//////////    g_ui8TempRcvMSG[ui8Count]     = 0;
//////////    g_ui8TempRcvMSG[ui8Count + 1] = 0;
////////    
////////    // Calculate CRC
////////    g_ui16CRCValue = calculateCrc(g_ui8TempRcvMSG, SIZE_IBU_SEND_RCV_ARRAY - 2);
////////    
////////    // Validate CRC.
////////    if(((g_ui16CRCValue >> 8) & 0xFF) == g_ui8ArrRCVBuffer[ui8CurPosition + 3 + ui8Count + 1] &&
////////       ((g_ui16CRCValue >> 0) & 0xFF) == g_ui8ArrRCVBuffer[ui8CurPosition + 3 + ui8Count])
////////    {
////////        // Insert the Message into IBU Receive Queue
////////        g_stIBURcvData.g_ui8ArrIBUData[0] = g_ui8TempRcvMSG[2];
////////        g_stIBURcvData.g_ui8ArrIBUData[1] = g_ui8TempRcvMSG[3];
////////        g_stIBURcvData.g_ui8ArrIBUData[2] = g_ui8TempRcvMSG[4];
////////        g_stIBURcvData.g_ui8ArrIBUData[3] = g_ui8TempRcvMSG[5];
////////        g_stIBURcvData.g_ui8ArrIBUData[4] = g_ui8TempRcvMSG[6];
////////
////////        // Insert Message into IBC Receive Queue
////////        InsertIBUMsgQueue(&g_stIBURcvQueue, &g_stIBURcvData);
////////
////////        // Reset receive buffer.
////////        ResetIBURcvBuffer();
////////        
////////        // Send Acknowledgment
////////        g_ui8TempRcvMSG[1] = IBU_MSG_TYPE_ACK;
////////        g_ui8TempRcvMSG[7] = g_ui8ArrRCVBuffer[ui8CurPosition + 3 + ui8Count];
////////        g_ui8TempRcvMSG[8] = g_ui8ArrRCVBuffer[ui8CurPosition + 3 + ui8Count + 1];
////////        
////////        // No need to hold the Send Lock, because this function is called from main thread
////////        // and only main thread can send IBU Data
////////        // Send Start Text to protect spurious data.
////////        fputc(STX, UART);
////////        fputc(STX, UART);
////////        fputc(STX, UART);
////////        // Send Original Data.
////////        for (ui8Count = 0; ui8Count < SIZE_IBU_SEND_RCV_ARRAY; ui8Count++)
////////        {
////////            fputc(g_ui8TempRcvMSG[ui8Count], UART);
////////        }
////////        fputc(ETX, UART);
////////        fputc(ETX, UART);
////////        delay_us(5);
////////    }
////////    else  // For Invalid CRC
////////    {
////////        // Receive message got corrupted
////////        // Reset Receive MSG Array
////////        g_ui8RCVQFront = 0;
////////        g_ui8RCVQRear  = 0;
////////    }
////////    
////////    g_ui8RCVQFront = 0;
////////    g_ui8RCVQRear  = 0;
////////}

////////#INT_RDA HIGH
////////void uart1_Rx_isr(void)
////////{
////////    // Receive byte from interrupt.   
////////    g_ui8ArrRCVBuffer[g_ui8RCVQRear++] = fgetc(UART);
////////
////////    // Clear interrupt.
////////    clear_interrupt(INT_RDA);
////////}
#define ETX_FOUND      1
#define ETX_NOT_FOUND  0
unsigned int8 g_ui8CompleteMSGFound = ETX_FOUND;

#INT_RDA HIGH
void uart1_Rx_isr(void)
{
    // Receive byte from interrupt
    if (-1 == g_stRcvQueue.m_ui8Front)
    {
        g_stRcvQueue.m_ui8Front = 0;
    }
    g_stRcvQueue.m_ui8Rear = (g_stRcvQueue.m_ui8Rear + 1) % SIZE_RCV_QUEUE;
    
    g_stRcvQueue.m_ui8QArr[g_stRcvQueue.m_ui8Rear] = fgetc(UART);
    
    // Clear interrupt
    clear_interrupt(INT_RDA);
    

//    g_ui8ArrRCVBuffer[g_ui8RCVQRear++] = fgetc(UART);
//    
//    if ( (g_ui8RCVQRear - g_ui8RCVQFront) > 14 &&
//        g_ui8ArrRCVBuffer[g_ui8RCVQRear - 1] == ETX &&
//        g_ui8ArrRCVBuffer[g_ui8RCVQRear - 2] == ETX  )
//    {
//        g_ui8CompleteMSGFound = ETX_FOUND;
//    }
//
//    // Clear interrupt.
//    clear_interrupt(INT_RDA);
}

// Before 9.0
//#INT_RDA HIGH
//void uart1_Rx_isr(void)
//{
//    // Receive byte from interrupt.
//    g_ui8IBUTempByte = fgetc(UART);
//    
//    if(STX == g_ui8IBUTempByte)// && !g_fDataReady)
//    {
//        g_fDataReady = 1;
//        //disable_interrupts(INT_RDA3);
//    }
//    else if(g_fDataReady)
//    {
//        // Receive Original data byte.
//        g_stIBURcvData.g_ui8ArrIBUData[g_ui8IBURcvByteCount] = g_ui8IBUTempByte;
//        
//        // Increment byte count.
//        g_ui8IBURcvByteCount++;
//        
//        // Check if all bytes are received.
//        if(IBU_MSG_BYTE_COUNT == g_ui8IBURcvByteCount)
//        {
//            // Raise a flag to denote all data bytes are received..now process the data.
//            g_fIBUDataRcvStatus = IBU_RCV_COMPLETE;
//            
//            // Reset Flag.
//            g_fDataReady = 0;
//            
//            //enable_interrupts(INT_RDA3);
//            //clear_interrupt(INT_RDA3);
//        }
//    }
//    else
//    {
//        /* Do Nothing */
//    }
//
//    // Clear interrupt.
//    clear_interrupt(INT_RDA);
//}






#endif /* _VI_IBU_CONFIG_H_ */