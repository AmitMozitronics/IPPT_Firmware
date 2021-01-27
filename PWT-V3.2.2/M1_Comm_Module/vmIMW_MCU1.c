#include "vmSystemConfig.h"
#include "vmM1Common.h"
#include "vmTimerConfig.h"
#include "vmIBCProcessM1.h"
#include "mzLANCommConfig.h"
#include "vmRS485Config.h"


void Initialize(void)
{
    
    // Set RS485 Transmit Inactive.
    output_bit(PIN_RS485_RTS,0);

    
    // Enable Global Interrupts.
    enable_interrupts(GLOBAL);
    

    // Initialize for GPRS message queue.
    InitializePipeDataQueue();
    InitializeLANMsgHeader();

    
    // Read set of Timer values from EEPROM. 
    g_ui8DataSendingIntervalTime = ReadIntervalFromEEPROM();
    g_ui32UnixTimeInSec = ReadStoredUnixTimeFromEEPROM();
    g_ui8SIMOperatorID  = ReadSIMOPeratorIDFromEEPROM();
    
    // Initialize the sending queue to M0
    //InitializeDataSendQueue();
    InitializeDataQueue(&g_stIBURcvQueue); 
    InitializeDataQueue(&g_stIBUSendQueue);
    
    // Initialize global variables for RS485 communication.    
    g_ui16PipeCount =  0;
    g_ui16PipeCountPrev = 0;
    g_ui32CurrentWt = 0;
    g_ui16CurrentPassStatus = 0;
    g_ui32RunningAvgWt = 0;
    g_ui32RunningTareValue = 0;
    //g_ui16ServerStatus = 0;
    g_ui16CurrentMode = 0;
        
    g_stIBURcvData.g_ui8ArrIBUData[0] = 0;
    g_stIBURcvData.g_ui8ArrIBUData[1] = 0;
    g_stIBURcvData.g_ui8ArrIBUData[2] = 0;
    g_stIBURcvData.g_ui8ArrIBUData[3] = 0;
    g_stIBURcvData.g_ui8ArrIBUData[4] = 0;
    
    
}	

void main()
{
    unsigned int8 ui8RetVal                         = 0;
    g_fDataSendingSequence                          = LAN_DATA_SENDING_SEQUENCE_1;
    
	// Set Watch Dog Timer ON.
    setup_wdt(WDT_4S | WDT_WINDOW_100_PERCENT);
    
	//Initialize 1 millisecond timer.
	Init1MSTimer();
    	
	//Initialize RS485 Receive interrupt.
	InitRS485RxInterrupt();
	
	//Initialize UART2UART Receive interrupt.
    InitIBURxInterrupt();
    
    // Initialize GPRS Module;EC20 Receive interrupt.
    //InitM66RxInterrupt();
    
    // Initialize LAN Module; USR_K3 receive interrupt.
    InitLANCommRxInterrupt();

	// Initialize Main Global variables 
	Initialize();
    
    // Initialize Receive Queue
    InitializeRcvQ();
    
    // Set GSM modem ready
    delay_ms(3000);
    

	// Reset WDT.
	restart_wdt();

	for(;;)
	{ 
        // step 1: Check if there is any message received in IBU through UART
        g_ui8ElementInQ = ElementsInQ();
        if (g_ui8ElementInQ >= SIZE_IBU_COMPLETE_SEND_MSG)
        {
            // Hopefully a Complete Message Received
            CheckIBUReceiveQ();
        }

        // step 2: Check if there is any message in IBC Receive Queue
        if (IsMSGInIBUQueue(&g_stIBURcvQueue))
        {
            // Process IBC 
            ProcessIBUMsg();
        } 
        
        // Step 3: check if there is any message in IBU send queue.
        // If available then process to send to UART.
        if(IsMSGInIBUQueue(&g_stIBUSendQueue) && 
          (IBU_SENDING_FREE == g_fIBUSendBusy))
        {
            // Set flag to denote IBU is busy in sending data.
            g_fIBUSendBusy = IBU_SENDING_BUSY;
            
            // Send data from queue.
            SendIBUMSGFromQ();
        }        
 
        // Step 5: Check if a byte received in Modbus..then process data.
        if(ISR_RECEIVED_ALL_BYTES == g_fISRReceivedByteStatus)
        { 
            // Process received data.
            ProcessUART2ReceivedData();

            // Reset flag.
            g_fISRReceivedByteStatus = ISR_PROCESSED_ALL_BYTES;
        }
        
        // Step 6: Check for unix time write to EEPROM signal.
        if(TIME_STAMP_SIGNAL_ON == g_fTSWriteToEEPROMSignal)
        {
            // Write data to EEPROM.
            WriteCurrentUnixTimeToEEPROM(g_ui32HookedUpTimeStampPerMinute);
            
            // Reset flag state.
            g_fTSWriteToEEPROMSignal = TIME_STAMP_SIGNAL_COMPLETE;
        }
        
//        // Step 7: Check if keep alive send status is active.
//        if(KEEP_ALIVE_SEND_PROCESS_ON == g_fKeepAliveSendStatus)
//        {
//            // Check if GPRS data queue is empty.
//            if(-1 == g_stGPRSMsgSendQueue.m_i8Front)
//            {
//                // Queue is empty.
//                // Stuff keep alive data in queue.
//                InsertInGPRSQueue();
//            }
//            
//            // Reset keep alive monitoring counter.
//            g_ui8KeepAliveMonitoringCounter = 180;
//            
//            // Reset flag status.
//            g_fKeepAliveSendStatus = KEEP_ALIVE_SEND_PROCESS_OFF;
//            
//            // Set flag to check query on time.
//            g_fKeepAliveQueryCheck = CHECK_FOR_QUERY_START;
//           
//        }
        
        // Here, pipe current weight or pipe data will be sent through LAN.
        // By modular data sending process we can uninterruptedly run peripheral
        // process along with data sending in parallel.
        
 // ################ Below steps are for GPRS modular data sending ######################
        
        // Step 1: Check modular data sending time interval & data sending sequence
        //         are allowing to initiate data sending process.
        if(SHOULD_SEND_DATA == g_fShouldSendData &&
           LAN_DATA_SENDING_SEQUENCE_1 == g_fDataSendingSequence)
        {
            // Check if new data available in data queue.
            ui8RetVal = IsPipeDataQueueEmpty();
            
            if(0 == ui8RetVal)  // Queue is not empty.
            {
                // Update sequence flag of sending.
                g_fDataSendingSequence = LAN_DATA_SENDING_SEQUENCE_2;
            }
            else    // Queue is empty.
            {
                /* Do Nothing; Wait until new data available */
            }
        }
        
        // Step 2: If data present in queue then fetch according to queue front
        //         and send.
        else if(LAN_DATA_SENDING_SEQUENCE_2 == g_fDataSendingSequence)
        {
            // Send data from queue.
            SendDataFromQ();
            
            // Set a timeout to receive success code.
            g_fIsTimedOut = 0;
            SetTimeout(SERVER_RESPONSE_RECEIVE_TIMEOUT);
            
            // Update sequence flag of data sending.
            g_fDataSendingSequence = LAN_DATA_SENDING_SEQUENCE_3;
        }
        
        // Step 3: check for return code from server upto set time.
        else if(LAN_DATA_SENDING_SEQUENCE_3 == g_fDataSendingSequence &&
                g_fIsTimedOut == 0)
        {
            // Check for server response.
            //ui8RetVal = CheckServerResponse();
            ui8RetVal = 0;
            
            // Check return value.
            if(ui8RetVal == 0)  // Server response received.
            {
                // Update sequence flag for data sending.
                g_fDataSendingSequence = LAN_DATA_SENDING_SEQUENCE_4;
            }
            else  // No response received after parsing.
            {
                /* Do Nothing */
            }
               
        } 
        
        // Step 4: check if set timeout for arriving server response; is over.
        else if(LAN_DATA_SENDING_SEQUENCE_3 == g_fDataSendingSequence &&
                g_fIsTimedOut == 1)
        {
            // No response received during set time interval for expected arrival.
            // Set a time interval to wait & try Once.
            g_fIsTimedOut = 0;
            SetTimeout(DATA_RESEND_INTERVAL_TIMEOUT);

            // Update sequence flag.
            g_fDataSendingSequence = LAN_DATA_SENDING_SEQUENCE_5;
        }
        
        // Step 5: If server response code received & parsed successfully.
        else if(LAN_DATA_SENDING_SEQUENCE_4 == g_fDataSendingSequence)
        {
            // Data from queue successfully sent to server and server response
            // is also received. 
            // Update flags upon successful data send.
            
            // Update data sending interval flag for next interval counting start.
            g_fShouldSendData = SHOULD_NOT_SEND_DATA;
            
            // Handle queue front.
            if (g_stPipeDataSendQueue.m_i8Front == g_stPipeDataSendQueue.m_i8Rear)
            {
                // Queue is empty
                g_stPipeDataSendQueue.m_i8Front = g_stPipeDataSendQueue.m_i8Rear = -1;
            }
            else // Queue is not empty
            {
                g_stPipeDataSendQueue.m_i8Front = (g_stPipeDataSendQueue.m_i8Front + 1) % SIZE_GPRS_DATA_NODE_QUEUE;
            } 
            
            // Update sequence flag of data sending.
            g_fDataSendingSequence = LAN_DATA_SENDING_SEQUENCE_1;
        }
        
        // Step 6: If proper response is not received; after waiting for some time
        //          resend data.
        else if(LAN_DATA_SENDING_SEQUENCE_5 == g_fDataSendingSequence &&
                1 == g_fIsTimedOut)
        {
            // Update sequence flag to resend data.
            g_fDataSendingSequence = LAN_DATA_SENDING_SEQUENCE_1;
        }
        else
        {
            /* Do Nothing */
        }
            
		// Reset WDT.
        restart_wdt();                
	}	
}
    	





	
