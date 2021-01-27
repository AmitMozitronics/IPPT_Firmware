#ifndef _VI_RS485_CONFIG_H_
#define _VI_RS485_CONFIG_H_

#include "../Common/vmCRCConfig.h"
#include "../Common/vmModbusGlobal.h"
#include "../Common/vmIBUConfig.h"
#include "vmM1Common.h"
#include "mzEEPROMConfig_MCU1.h"


//#define PIN_HIGH     			0
//#define PIN_LOW      			1

#define SLAVE_ID                                            0x1

#define MODE_ADDRESS                                        0x05
#define ADDRESS_FOR_SELECT_PRODUCTION_TYPE                  0x04
#define ADDRESS_FOR_SCRAP_ON_MODE                           0x99
#define ADRESS_FOR_HMI_TIMESTAMP                            0x47
#define ADDRESS_FOR_MACHINE_ID                              0x90


#define ADDRESS_FOR_SET_LOAD_CELL_FACTOR                    0x282
#define ADDRESS_FOR_CALIB_TARE_ADC_VALUE                    0x296
#define ADDRESS_FOR_CALIB_WEIGHT_ADC_VALUE                  0x298

#define ADDRESS_FOR_CURRENT_CALIBRATION_WT                  0x310
#define ADDRESS_FOR_START_CALIBRATION                       0x299
#define ADDRESS_FOR_RUNNING_VAR_PROD_DETAILS                0x420
#define ADDRESS_FOR_SELECTED_PRODUCT_INDEX                  0x540
#define ADDRESS_FOR_CURRENT_CBB_PRODUCT_INDEX               0x250 

// use for production
#define ADDRESS_FOR_CBB_COUNT_IN_PRODUCTION                 0x560
#define ADDRESS_FOR_CURRENT_WT_IN_PRODUCTION                0x562
#define ADDRESS_FOR_PASS_STATUS_WT_IN_PRODUCTION            0x564 // SET 1 for accept
#define ADDRESS_FOR_RUNNING_AVG_WT_IN_PRODUCTION            0x566
#define ADDRESS_FOR_TARE_VALUE_IN_PRODUCTION                0x568
#define ADDRESS_FOR_WRITE_TARE_VALUE                        0x570
#define ADDRESS_FOR_HMI_DATA_UPDATE_TRIGGER                 0x760

// use for preproduction
#define ADDRESS_FOR_PRODUCTION_INDEX_PRE_PRODUCTION         0x500
#define ADDRESS_FOR_PRODUCTION_AVG_WT_PRE_PRODUCTION        0x550
#define ADDRESS_FOR_PRODUCTION_PV_TOL_PRE_PRODUCTION        0x552
#define ADDRESS_FOR_PRODUCTION_NV_TOL_PRE_PRODUCTION        0x554

#define ADDRESS_FOR_CALCULATED_MIN_MAX_WT                   0x50
#define ADDRESS_FOR_TOL_STORE_SEEKING_ACK                   0x88
#define ADDRESS_FOR_TOL_STORE_SETTING_ACK                   0x89
#define ADDRESS_FOR_PRODUCT_BASIC_MATERIAL_STANDARD         0x190
#define ADDRESS_FOR_PIPE_SPECIFICATION                      0x193
#define ADDRESS_FOR_PRE_PROD_MAX_MIN_WT                     0x52
#define ADDRESS_FOR_AT_OK_TIMER_SETTING                     0x53
#define ADDRESS_FOR_AT_DATA_INTERVAL_TIMER_SETTING          0x54
#define ADDRESS_FOR_HTTP_GET_SESSION_TIMER_SETTING          0x55
#define ADDRESS_FOR_WIRELESS_MODEM_RESET_TIMER_SETTING      0x56
#define ADDRESS_FOR_WIRELESS_MODEM_NETWORK_RECONNECTION_HALT_TIME      0x57
#define ADDRESS_FOR_PIPE_PRESENSE_DELAY_TIME                0x58
#define ADDRESS_FOR_PIPE_STABILIZATION_DELAY_TIME           0x59
#define ADDRESS_FOR_PIPE_TRANSFER_DELAY_TIME                0x60
#define ADDRESS_FOR_WIRELESS_MODEM_RESET_INTERVAL_TIME      0x61
#define ADDRESS_FOR_SHIFT_NUMBER                            0x62
#define ADDRESS_FOR_SHIFT_DURATION                          0x63
#define ADDRESS_FOR_SELECT_SIM_OPERATOR                     0x64
#define ADDRESS_FOR_AIR_PRESSURE_ALARM                      0x65
#define ADDRESS_FOR_WEIGHING_CYLINDER_UP_ACTIVATION_DELAY   0x66

// use for calibration type set
#define ADDRESS_FOR_CALIBRATION_TYPE                        0x710
#define ADDRESS_FOR_AUTO_CALIB_SAMPLE_COUNT                 0x711

// RS485 Communication related stuff
#define SIZE_RS485_RX_BUFF                                  50
#define SIZE_RS485_TX_BUFF                                  50
#define ISR_1ST_BYTE_RECEIVED                               0
#define ISR_1ST_BYTE_PROCESSED                              1
#define MODBUS_FNCODE_READ_COIL_STATUS                      1
#define MODBUS_FNCODE_READ_HOLDING_REGISTER                 3
#define MODBUS_FNCODE_WRITE_SINGLE_REGISTER                 6
#define MODBUS_FNCODE_WRITE_MULTIPLE_REGISTER               16
#define MODBUS_SINGLE_REG_READ_WRITE_RX_BYTE_COUNT          8
#define MODBUS_RESPONSE_TO_MASTER_FOR_READ                  9
#define MODBUS_RESPONSE_TO_MASTER_FOR_WRITE                 10
#define MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL            0


////////////////////////////////////////////////////////////////////////////////
#define SIZE_TEMP_ARR_CRC_VALID                        15
#define MSG_QUEUE_EMPTY                                 0
#define MSG_QUEUE_NOT_EMPTY                             1
#define MODBUS_MASTER_QUERY_LENGTH_MIN                  8
#define MODBUS_MASTER_QUERY_LENGTH_MIX                 12
#define MODBUS_MASTER_QUERY_READ_REG_LENGTH             8
#define MODBUS_MASTER_QUERY_SINGLE_WRITE_REG_LENGTH     8

#define STATUS_ON                                       10
#define STATUS_OFF                                      11


//#define RS485_RX_BUFF_SIZE 		50

//unsigned int8 g_RS485RxBuffer[RS485_RX_BUFF_SIZE] = {0};

typedef struct _ST_ISR_QUEUE_MEMBERS
{
    unsigned int8 m_ui8Arr[RS485_RX_BUFF_SIZE];
    int8 m_i8Front;
    int8 m_i8Rear;
    
}ST_ISR_QUEUE_MEMBERS;


//ST_ISR_QUEUE_MEMBERS g_stUART2RcvBuffModbus;
unsigned int8 g_ui8TempByte = 0;
//unsigned int8 g_ui8TempArrForCRCValidation[SIZE_TEMP_ARR_CRC_VALID] = {0};


////////////////////////////////////////////////////////////////////////////////

#define REG_DATA_LO_TO_HI                       1 // 1 = lo byte first, 2 = lo byte last

unsigned int8 g_ui8RS485Rxbuffer[SIZE_RS485_RX_BUFF] = {0};
unsigned int8 g_ui8RS485Txbuffer[SIZE_RS485_TX_BUFF] = {0};
unsigned int8 g_ui8RS485RxByteCount                  = 0;
unsigned int8 g_fModbusMultiRegWriteTimeCountStats   = STATUS_OFF;
unsigned int8 g_fISRFirstBytesStatus                 = ISR_1ST_BYTE_PROCESSED;

unsigned int8 g_ui8Test                  = 0;
unsigned int16 g_ui16BitFillingTemp      = 0;
unsigned int32 g_ui32BitFillingTemp      = 0;


// Initialize RS485 receive interrupt.
void InitRS485RxInterrupt(void)
{
    // Enable interrupt. 
    clear_interrupt(INT_RDA3);
	enable_interrupts(INT_RDA3);
}

void ResetRS485RXBuffer(void)
{
    // clear buffer.
    memset(g_ui8RS485Rxbuffer, 0, SIZE_RS485_RX_BUFF);
    // Reset byte count.
    g_ui8RS485RxByteCount = 0;
    
}

#INT_RDA3
void RS485_RxByteReady_ISR3(void)
{
    
    // Checking for Is ISR ready flag.
    if(ISR_IS_NOT_READY == g_fIsReadyToReceiveByte && g_fIsValidSlaveID == MODBUS_IS_VALID_SLAVE_ID)
    {
        ++g_ui8Test;
        g_ui8TempByte = fgetc(RS485);
        
        // Checking for is the request/response for valid slave..i.e. slaveID
        if(SLAVE_ID == g_ui8TempByte)
        {
            g_fIsReadyToReceiveByte = ISR_IS_READY;
            g_ui8RS485Rxbuffer[g_ui8RS485RxByteCount] = g_ui8TempByte;
            g_ui8RS485RxByteCount++;
            g_fISRFirstBytesStatus = ISR_1ST_BYTE_RECEIVED;            
        }
        else
        {
            // change flag status if slave id does not match.
            g_fIsValidSlaveID = MODBUS_IS_NOT_VALID_SLAVE_ID;
        }

    }
    else if(g_fIsValidSlaveID == MODBUS_IS_VALID_SLAVE_ID)
    {
        g_ui8TempByte = fgetc(RS485);
        
        // Checking for ISR 2nd received byte i.e. function code.
        if(ISR_1ST_BYTE_RECEIVED == g_fISRFirstBytesStatus)
        {
            // change flag status.
            g_fISRFirstBytesStatus = ISR_1ST_BYTE_PROCESSED;
            
            // Set ISR all byte expected receive timing.
            if(g_ui8TempByte == MODBUS_FNCODE_READ_HOLDING_REGISTER || g_ui8TempByte == MODBUS_FNCODE_WRITE_SINGLE_REGISTER)
            {
                // Load the time.
                g_ui16ISRAllBytesExpectedRcvTime = ISR_TOTAL_8_BYTE_RECEIVE_TIME;
            }
            else if(g_ui8TempByte == MODBUS_FNCODE_WRITE_MULTIPLE_REGISTER)
            {
                // Set flag to calculate and  load receive time.
                g_fModbusMultiRegWriteTimeCountStats = STATUS_ON;
                
                // Load the time.
                //g_ui16ISRAllBytesExpectedRcvTime = ISR_TOTAL_19_BYTE_RECEIVE_TIME;
            }
            else
            {
                /* Do Nothing */
            }
        }
        
        // Check multi register receive time calculation status.(This will depend upon number of register requested for by master)
        if(STATUS_ON == g_fModbusMultiRegWriteTimeCountStats)
        {
            // Check byte count.
            if(7 == g_ui8RS485RxByteCount)
            {
                // Load the time.
                g_ui16ISRAllBytesExpectedRcvTime = (RS485_9600_1_BYTE_RCV_TIME * (RS485_MULTIPLE_REG_BASIC_RCV_BYTE_COUNT + g_ui8RS485Rxbuffer[g_ui8RS485RxByteCount - 1]));

                // Manipulate data.
                g_ui16ISRAllBytesExpectedRcvTime = (g_ui16ISRAllBytesExpectedRcvTime / 1000);
                g_ui16ISRAllBytesExpectedRcvTime = g_ui16ISRAllBytesExpectedRcvTime + 2;

                // Reset flag status.
                g_fModbusMultiRegWriteTimeCountStats = STATUS_OFF;
            }            
        }
        
        // Store the data byte in receive buffer.
        g_ui8RS485Rxbuffer[g_ui8RS485RxByteCount] = g_ui8TempByte;
    
        // Increment receive byte count.
        g_ui8RS485RxByteCount++;
        
        // Check received byte count with buffer size.
        if(g_ui8RS485RxByteCount >= SIZE_RS485_RX_BUFF)
        {
            // Reset byte count.
            g_ui8RS485RxByteCount = 0;
        }
    
    }
    else
    {
        /* Do Nothing */
    }
    
    // TESTING
    if(g_ui8RS485RxByteCount == 8)
    {
        g_ui8RS485RxByteCount = 8;
    }
    
    
	// Clear interrupt.
	clear_interrupt(INT_RDA3);
}

void SetProductSpecificationForServer(void)
{
    unsigned int16 ui162ndAPIParameter = 0;
    unsigned int16 ui163rdAPIParameter = 0;
    unsigned int8  ui8Multiplier       = 10;

    
    // Accumulate individual data from HMI for server API string.
    // Take basic material and standard of production.
    ui162ndAPIParameter = g_ui8BasicMaterialStandard;
    
    // Now combine specification of that standard used.
    if(g_ui8StandardSpecification <= 9)
    {
        // One digit number.
        // Set multiplier.
        ui8Multiplier = 10;
        
    }
    else if(g_ui8StandardSpecification <= 99)
    {
        // Two digit  number.
        // Set multiplier.
        ui8Multiplier = 100;        
    }
    else
    {
        /* Do Nothing */
    }
    
    // Combine the two specification.
    ui162ndAPIParameter = ui162ndAPIParameter * ui8Multiplier;
    ui162ndAPIParameter = ui162ndAPIParameter + g_ui8StandardSpecification;

    // Now combine Pressure rating specification used for for that standard.
    if(g_ui8PressureRating <= 9)
    {
        // One digit number.
        // Set multiplier.
        ui8Multiplier = 10;
        
    }
    else if(g_ui8PressureRating <= 99)
    {
        // Two digit  number.
        // Set multiplier.
        ui8Multiplier = 100;        
    }
    else
    {
        /* Do Nothing */
    }    
    
    // Now combine pressure specification of that production.
    ui162ndAPIParameter = ui162ndAPIParameter * ui8Multiplier;
    ui162ndAPIParameter = ui162ndAPIParameter + g_ui8PressureRating;
    
    // second parameter of API string is ready.
    // Put it at GPRS message header.
    g_stLANDataHeader.m_ui16BasicMaterialStandard = ui162ndAPIParameter;
    
    // Now make 3rd parameter of API string.
    // Take pipe OD.
    ui163rdAPIParameter = g_ui8PipeOD;
    
    // Now combine Pipe length used for for that standard.
    if(g_ui8PipeLength <= 9)
    {
        // One digit number.
        // Set multiplier.
        ui8Multiplier = 10;
        
    }
    else if(g_ui8PipeLength <= 99)
    {
        // Two digit  number.
        // Set multiplier.
        ui8Multiplier = 100;        
    }
    else
    {
        /* Do Nothing */
    }    
    
    // Now combine pipe length.
    ui163rdAPIParameter = ui163rdAPIParameter * ui8Multiplier;
    ui163rdAPIParameter = ui163rdAPIParameter + g_ui8PipeLength;
    
    // Third parameter is ready.
    // Put it in GPRS message header.
    g_stLANDataHeader.m_ui16PipeSpecification = ui163rdAPIParameter;
    
}





void SendMulResponseToMaster(unsigned int16 ui16StartAddress, unsigned int8 ui8DataBytesToFollow)
{
    unsigned int8  ui8SendByteCount         = 0;
    unsigned int8  ui8DataBytesToSend       = 0;
    unsigned int16 ui16CRCValue             = 0;
        
    // Stuff generic slave ID for all responses.
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = SLAVE_ID;
    // Increment counter.
    ui8DataBytesToSend++;
    // Stuff  the function code.
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = MODBUS_FNCODE_READ_HOLDING_REGISTER;
    // Increment counter.
    ui8DataBytesToSend++;
    // Stuff number of data bytes to follow.
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = ui8DataBytesToFollow;
    // Increment counter.
    ui8DataBytesToSend++;

    // For CBB Count. Register-> 560 & 561
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((g_ui16PipeCount >> 8) & 0xFF);
    // Increment counter.
    ui8DataBytesToSend++;
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((g_ui16PipeCount >> 0) & 0xFF);
    // Increment counter.
    ui8DataBytesToSend++;
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = 0;
    // Increment counter.
    ui8DataBytesToSend++;
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = 0;
    // Increment counter.
    ui8DataBytesToSend++;
    
    // For Current CBB Weight. Register-> 562 & 563
    g_ui32BitFillingTemp = (unsigned int32)((g_ui32CurrentWt << 16) & 0xFFFF0000);
    g_ui16BitFillingTemp = (unsigned int16)((g_ui32BitFillingTemp >> 16) & 0x0000FFFF);
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((g_ui16BitFillingTemp >> 8) & 0xFF);
    // Increment counter.
    ui8DataBytesToSend++;
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((g_ui16BitFillingTemp >> 0) & 0xFF);
    // Increment counter.
    ui8DataBytesToSend++;
    g_ui16BitFillingTemp = (unsigned int16)((g_ui32CurrentWt >> 16) & 0xFFFF);
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((g_ui16BitFillingTemp >> 8) & 0xFF);
    // Increment counter.
    ui8DataBytesToSend++;
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((g_ui16BitFillingTemp >> 0) & 0xFF);
    // Increment counter.
    ui8DataBytesToSend++;

    // For Pass CBB Status. Register-> 564 & 565
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((g_ui16CurrentPassStatus >> 8) & 0xFF);
    // Increment counter.
    ui8DataBytesToSend++;
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((g_ui16CurrentPassStatus >> 0) & 0xFF);
    // Increment counter.
    ui8DataBytesToSend++;
    
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = 0;
    // Increment counter.
    ui8DataBytesToSend++;
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = 0;
    // Increment counter.
    ui8DataBytesToSend++; 

    // For Production Running Average. Register-> 566 & 567
    // Hold the MSB first
    g_ui32BitFillingTemp = (unsigned int32)((g_ui32RunningAvgWt << 16) & 0xFFFF0000);
    g_ui16BitFillingTemp = (unsigned int16)((g_ui32BitFillingTemp >> 16) & 0x0000FFFF);
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((g_ui16BitFillingTemp >> 8) & 0xFF);
    // Increment counter.
    ui8DataBytesToSend++;
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((g_ui16BitFillingTemp >> 0) & 0xFF);
    // Increment counter.
    ui8DataBytesToSend++;
    
    g_ui16BitFillingTemp = (unsigned int16)((g_ui32RunningAvgWt >> 16) & 0xFFFF);
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((g_ui16BitFillingTemp >> 8) & 0xFF);
    // Increment counter.
    ui8DataBytesToSend++;
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((g_ui16BitFillingTemp >> 0) & 0xFF);
    // Increment counter.
    ui8DataBytesToSend++; 
    
    // For Running Tare Weight. Register-> 568 & 569
    // Example: If the four bytes are like 1234 then it will be filled as 3412 
    // Hold the MSB first
    g_ui32BitFillingTemp = (unsigned int32)((g_ui32RunningTareValue << 16) & 0xFFFF0000);
    g_ui16BitFillingTemp = (unsigned int16)((g_ui32BitFillingTemp >> 16) & 0x0000FFFF);
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((g_ui16BitFillingTemp >> 8) & 0xFF);
    // Increment counter.
    ui8DataBytesToSend++;
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((g_ui16BitFillingTemp >> 0) & 0xFF);
    // Increment counter.
    ui8DataBytesToSend++;
    
    g_ui16BitFillingTemp = (unsigned int16)((g_ui32RunningTareValue >> 16) & 0xFFFF);
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((g_ui16BitFillingTemp >> 8) & 0xFF);;
    // Increment counter.
    ui8DataBytesToSend++;
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((g_ui16BitFillingTemp >> 0) & 0xFF);;
    // Increment counter.
    ui8DataBytesToSend++;
    
    // CRC calculation.
    ui16CRCValue = calculateCrc(g_ui8RS485Txbuffer, ui8DataBytesToSend);

    // Stuff CRC
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((ui16CRCValue >> 0) & 0xFF);
    // Increment counter.
    ui8DataBytesToSend++;                
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((ui16CRCValue >> 8) & 0xFF);
    // Increment counter.
    ui8DataBytesToSend++; 
    
    
    // Data Sending procedure.
    // Make RTS pin transmit active.
    output_bit(PIN_RS485_RTS, 1);
    delay_ms(2);
    
    // Send bytes.
    for(ui8SendByteCount = 0; ui8SendByteCount < ui8DataBytesToSend; ui8SendByteCount++)
    {
        fputc(g_ui8RS485Txbuffer[ui8SendByteCount], RS485);
    }
    
    delay_ms(3);
    // Make RTS pin transmit inactive..receive mode on.
    output_bit(PIN_RS485_RTS, 0);    
    
}

void SendResponseToMaster(unsigned int8 ui8ResponseType, unsigned int8 ui8FuncCode, unsigned int16 ui16StartAddress, unsigned int8 ui8DataBytesToFollow, int32 ui32RegData)
{
    unsigned int16 ui16NoOfRegisterToFollow = 0;
    unsigned int16 ui16CRCValue             = 0;
    unsigned int8  ui8SendByteCount         = 0;
    unsigned int8  ui8DataBytesToSend       = 0;
    unsigned int16 ui16RegisterData         = 0;
    
    // Stuff generic slave ID for all responses.
    g_ui8RS485Txbuffer[ui8DataBytesToSend] = SLAVE_ID;
    // Increment counter.
    ui8DataBytesToSend++;
    
    // Step 1: Parse response type
    if(MODBUS_RESPONSE_TO_MASTER_FOR_READ == ui8ResponseType)
    {
        // Step 2: Parse function code
        switch(ui8FuncCode)
        {
            case MODBUS_FNCODE_READ_HOLDING_REGISTER:
                // Stuff  the function code.
                g_ui8RS485Txbuffer[ui8DataBytesToSend] = MODBUS_FNCODE_READ_HOLDING_REGISTER;
                // Increment counter.
                ui8DataBytesToSend++;
                
                // Parse the no of registers to response
                if(2 == ui8DataBytesToFollow) // For WORD.
                {
                    // Stuff number of data bytes to follow.
                    g_ui8RS485Txbuffer[ui8DataBytesToSend] = ui8DataBytesToFollow;
                    // Increment counter.
                    ui8DataBytesToSend++;
                    // Stuff the data.
                    ui16RegisterData = (unsigned int16)(ui32RegData);
                    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((ui16RegisterData >> 8) & 0xFF);
                    //Increment counter. 
                    ui8DataBytesToSend++;
                    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((ui16RegisterData >> 0) & 0xFF);
                    //Increment counter. 
                    ui8DataBytesToSend++;                    
                }
                else if(4 == ui8DataBytesToFollow) // For DOUBLE WORD
                {
                    // Stuff number of data bytes to follow.
                    g_ui8RS485Txbuffer[ui8DataBytesToSend] = ui8DataBytesToFollow;
                    // Increment counter.
                    ui8DataBytesToSend++;
                    // Stuff the data.
                    ui16RegisterData = (unsigned int16)(ui32RegData);  // 1st WORD of the DOUBLE WORD
                    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((ui16RegisterData >> 8) & 0xFF);
                    //Increment counter. 
                    ui8DataBytesToSend++;
                    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((ui16RegisterData >> 0) & 0xFF);
                    //Increment counter. 
                    ui8DataBytesToSend++; 
                    ui16RegisterData = (unsigned int16)(ui32RegData >> 16);  // 2nd WORD of the DOUBLE WORD
                    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((ui16RegisterData >> 8) & 0xFF);
                    //Increment counter. 
                    ui8DataBytesToSend++;
                    g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((ui16RegisterData >> 0) & 0xFF);
                    //Increment counter. 
                    ui8DataBytesToSend++;                    
                }
                else
                {
                    /* Do Nothing */
                } 
                
                // CRC calculation.
                ui16CRCValue = calculateCrc(g_ui8RS485Txbuffer, ui8DataBytesToSend);
                
                // Stuff CRC value.
                g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((ui16CRCValue >> 0) & 0xFF);
                // Increment counter.
                ui8DataBytesToSend++;                
                g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((ui16CRCValue >> 8) & 0xFF);
                // Increment counter.
                ui8DataBytesToSend++;                
                break;
            default:
                break;
        }
    }
    else // Respond against write.
    {
        // Step 2: Parse function code
        switch(ui8FuncCode)
        {
            case MODBUS_FNCODE_WRITE_SINGLE_REGISTER:
                // Stuff reply in RS485 transmit buffer.
                memcpy(g_ui8RS485Txbuffer, g_ui8RS485Rxbuffer, g_ui8RS485RxByteCount);
                ui8DataBytesToSend = g_ui8RS485RxByteCount;
                break;
            case MODBUS_FNCODE_WRITE_MULTIPLE_REGISTER:
                // Stuff function code.
                g_ui8RS485Txbuffer[ui8DataBytesToSend] = ui8FuncCode;
                // Increment counter.
                ui8DataBytesToSend++;
                
                // Stuff data address of the first register.
                g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((ui16StartAddress >> 8) & 0xFF);
                // Increment counter.
                ui8DataBytesToSend++;                
                g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((ui16StartAddress >> 0) & 0xFF);
                // Increment counter.
                ui8DataBytesToSend++;                

                // Stuff no of registers written.
                ui16NoOfRegisterToFollow = (unsigned int16)(ui8DataBytesToFollow / 2);
                g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((ui16NoOfRegisterToFollow >> 8) & 0xFF);
                // Increment counter.
                ui8DataBytesToSend++;                
                g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((ui16NoOfRegisterToFollow >> 0) & 0xFF);
                // Increment counter.
                ui8DataBytesToSend++;                

                // CRC calculation.
                ui16CRCValue = calculateCrc(g_ui8RS485Txbuffer, 6);
                
                // Stuff CRC
                g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((ui16CRCValue >> 0) & 0xFF);
                // Increment counter.
                ui8DataBytesToSend++;                
                g_ui8RS485Txbuffer[ui8DataBytesToSend] = (unsigned int8)((ui16CRCValue >> 8) & 0xFF);
                // Increment counter.
                ui8DataBytesToSend++;                
                
                break;
            default:
                break;
        }
    }
    
    // Data Sending procedure.
    // Make RTS pin transmit active.
    output_bit(PIN_RS485_RTS, 1);
    delay_ms(2);
    
    // Send bytes.
    for(ui8SendByteCount = 0; ui8SendByteCount < ui8DataBytesToSend; ui8SendByteCount++)
    {
        fputc(g_ui8RS485Txbuffer[ui8SendByteCount], RS485);
    }
    
    delay_ms(3);
    // Make RTS pin transmit inactive..receive mode on.
    output_bit(PIN_RS485_RTS, 0);
}



void ProcessUART2ReceivedData(void)
{
    unsigned int16 ui16ModbusCRCvalue       = 0;
    unsigned int8  fIsValidCRC              = 0;
    unsigned int16 ui16StartAddtess         = 0;
    unsigned int16 ui16NoOfRegistersToRead  = 0;
    unsigned int8  ui8LoopCount             = 0;
    unsigned int8  ui8NoOfDataBytesToFollow = 0;
    unsigned int8  ui8NoOfVariety           = 0;
    unsigned int8  ui8WriteMulRegParseByteIndx = 6;
    

    // Calculate CRC for data redundancy check.
    // Calculate CRC with all bytes except last two.
    ui16ModbusCRCvalue = calculateCrc(g_ui8RS485Rxbuffer, (g_ui8RS485RxByteCount - 2));    
    
    // Validate CRC.
    if(((ui16ModbusCRCvalue >> 8) & 0xFF) == g_ui8RS485Rxbuffer[g_ui8RS485RxByteCount - 1] &&
       ((ui16ModbusCRCvalue >> 0) & 0xFF) == g_ui8RS485Rxbuffer[g_ui8RS485RxByteCount - 2])
    {
        // Set flag denoting received byte string CRC validation successful.
        fIsValidCRC = 1;
    }
    else
    {
        // Error occured in received data bytes
        ResetRS485RXBuffer();
    }
    
    // If CRC validation turns out to true.
    if(fIsValidCRC)
    {
        // Parse function code and process accordingly.
        switch(g_ui8RS485Rxbuffer[1])
        {
            case MODBUS_FNCODE_READ_COIL_STATUS:
                
                break;
            case MODBUS_FNCODE_READ_HOLDING_REGISTER:
                // Accumulate register address.
                ui16StartAddtess = g_ui8RS485Rxbuffer[2];
                ui16StartAddtess = ((ui16StartAddtess << 8) & 0xFF00);
                ui16StartAddtess = (ui16StartAddtess | g_ui8RS485Rxbuffer[3]);

                // Process the data at that address.
                switch(ui16StartAddtess)
                {
                    case ADDRESS_FOR_CALIB_TARE_ADC_VALUE:
                        // Step 1: Accumulate number of registers to read.
                        ui16NoOfRegistersToRead = g_ui8RS485Rxbuffer[4];
                        ui16NoOfRegistersToRead = ((ui16NoOfRegistersToRead << 8) & 0xFF00);
                        ui16NoOfRegistersToRead = (ui16NoOfRegistersToRead | g_ui8RS485Rxbuffer[5]);  

                        // Step 2: Send response to master with requested data.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_READ, MODBUS_FNCODE_READ_HOLDING_REGISTER, ui16StartAddtess, (ui16NoOfRegistersToRead * 2), g_ui32TareADCValueForCalibration);

                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();
                        
                        break;
                    case ADDRESS_FOR_CALIB_WEIGHT_ADC_VALUE:
                        // Step 1: Accumulate number of registers to read.
                        ui16NoOfRegistersToRead = g_ui8RS485Rxbuffer[4];
                        ui16NoOfRegistersToRead = ((ui16NoOfRegistersToRead << 8) & 0xFF00);
                        ui16NoOfRegistersToRead = (ui16NoOfRegistersToRead | g_ui8RS485Rxbuffer[5]);
                        
                        // Step 2: Send response to master with requested data.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_READ, MODBUS_FNCODE_READ_HOLDING_REGISTER, ui16StartAddtess, (ui16NoOfRegistersToRead * 2), g_ui32gWeightADCValueForCalibration);                        
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();
                        
                        break;
                    case ADDRESS_FOR_CBB_COUNT_IN_PRODUCTION:
                        // Step 1: Accumulate number of registers to read.
                        ui16NoOfRegistersToRead = g_ui8RS485Rxbuffer[4];
                        ui16NoOfRegistersToRead = ((ui16NoOfRegistersToRead << 8) & 0xFF00);
                        ui16NoOfRegistersToRead = (ui16NoOfRegistersToRead | g_ui8RS485Rxbuffer[5]);

                        // Check for number of registers requested for.
                        if(ui16NoOfRegistersToRead > 1)
                        {
                            // Step 2: Send response to master with requested data.
                            SendMulResponseToMaster(ui16StartAddtess, (ui16NoOfRegistersToRead * 2));
                        }
                        else
                        {                        
                            // Step 2: Send response to master with requested data.
                            SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_READ, MODBUS_FNCODE_READ_HOLDING_REGISTER, ui16StartAddtess, (ui16NoOfRegistersToRead * 2), g_ui16PipeCount);                                                
                        }
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();
                        
                        break;
                    case ADDRESS_FOR_CURRENT_WT_IN_PRODUCTION:
                        // Step 1: Accumulate number of registers to read.
                        ui16NoOfRegistersToRead = g_ui8RS485Rxbuffer[4];
                        ui16NoOfRegistersToRead = ((ui16NoOfRegistersToRead << 8) & 0xFF00);
                        ui16NoOfRegistersToRead = (ui16NoOfRegistersToRead | g_ui8RS485Rxbuffer[5]);
                        
                        // Step 2: Send response to master with requested data.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_READ, MODBUS_FNCODE_READ_HOLDING_REGISTER, ui16StartAddtess, (ui16NoOfRegistersToRead * 2), g_ui32CurrentWt);                                                                        
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();
                        
                        break;
                    case ADDRESS_FOR_RUNNING_AVG_WT_IN_PRODUCTION:
                        // Step 1: Accumulate number of registers to read.
                        ui16NoOfRegistersToRead = g_ui8RS485Rxbuffer[4];
                        ui16NoOfRegistersToRead = ((ui16NoOfRegistersToRead << 8) & 0xFF00);
                        ui16NoOfRegistersToRead = (ui16NoOfRegistersToRead | g_ui8RS485Rxbuffer[5]);
                        
                        // Step 2: Send response to master with requested data.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_READ, MODBUS_FNCODE_READ_HOLDING_REGISTER, ui16StartAddtess, (ui16NoOfRegistersToRead * 2), g_ui32RunningAvgWt);                                                                        
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();
                        
                        break;
                    case ADDRESS_FOR_TARE_VALUE_IN_PRODUCTION:
                        // Step 1: Accumulate number of registers to read.
                        ui16NoOfRegistersToRead = g_ui8RS485Rxbuffer[4];
                        ui16NoOfRegistersToRead = ((ui16NoOfRegistersToRead << 8) & 0xFF00);
                        ui16NoOfRegistersToRead = (ui16NoOfRegistersToRead | g_ui8RS485Rxbuffer[5]);
                        
                        // Step 2: Send response to master with requested data.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_READ, MODBUS_FNCODE_READ_HOLDING_REGISTER, ui16StartAddtess, (ui16NoOfRegistersToRead * 2), g_ui32RunningTareValue);                                                                        
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();
                        
                        break;
                    case ADDRESS_FOR_PASS_STATUS_WT_IN_PRODUCTION:
                        // Step 1: Accumulate number of registers to read.
                        ui16NoOfRegistersToRead = g_ui8RS485Rxbuffer[4];
                        ui16NoOfRegistersToRead = ((ui16NoOfRegistersToRead << 8) & 0xFF00);
                        ui16NoOfRegistersToRead = (ui16NoOfRegistersToRead | g_ui8RS485Rxbuffer[5]);
                        
                        // Step 2: Send response to master with requested data.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_READ, MODBUS_FNCODE_READ_HOLDING_REGISTER, ui16StartAddtess, (ui16NoOfRegistersToRead * 2), g_ui16CurrentPassStatus);                                                                        
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();                        
                        break;
                    case ADDRESS_FOR_TOL_STORE_SEEKING_ACK:
                        // Step 1: Accumulate number of registers to read.
                        ui16NoOfRegistersToRead = g_ui8RS485Rxbuffer[4];
                        ui16NoOfRegistersToRead = ((ui16NoOfRegistersToRead << 8) & 0xFF00);
                        ui16NoOfRegistersToRead = (ui16NoOfRegistersToRead | g_ui8RS485Rxbuffer[5]);                        
                        
                        // Step 2: Send response to master with requested data.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_READ, MODBUS_FNCODE_READ_HOLDING_REGISTER, ui16StartAddtess, (ui16NoOfRegistersToRead * 2), g_ui8ToleranceStoreAck);
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();                         
                        break;
                    case ADDRESS_FOR_AIR_PRESSURE_ALARM:
                        // Step 1: Accumulate number of registers to read.
                        ui16NoOfRegistersToRead = g_ui8RS485Rxbuffer[4];
                        ui16NoOfRegistersToRead = ((ui16NoOfRegistersToRead << 8) & 0xFF00);
                        ui16NoOfRegistersToRead = (ui16NoOfRegistersToRead | g_ui8RS485Rxbuffer[5]);
                        
                        // Step 2: Send response to master with requested data.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_READ, MODBUS_FNCODE_READ_HOLDING_REGISTER, ui16StartAddtess, (ui16NoOfRegistersToRead * 2), g_ui8AirPressureAlarm);
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();
                        break;                        
                    default:
                        break;
                }
                break;
            case MODBUS_FNCODE_WRITE_SINGLE_REGISTER:
                // Accumulate register address.
                ui16StartAddtess = g_ui8RS485Rxbuffer[2];
                ui16StartAddtess = ((ui16StartAddtess << 8) & 0xFF00);
                ui16StartAddtess = (ui16StartAddtess | g_ui8RS485Rxbuffer[3]);
                
                // Process data at that address.
                switch(ui16StartAddtess)
                {
                    case ADDRESS_FOR_SET_LOAD_CELL_FACTOR:
                        // Step 1: accumulate data.
                        g_ui16CurrLoadCellFactor = g_ui8RS485Rxbuffer[4];
                        g_ui16CurrLoadCellFactor = ((g_ui16CurrLoadCellFactor << 8) & 0xFF00);
                        g_ui16CurrLoadCellFactor = (g_ui16CurrLoadCellFactor | g_ui8RS485Rxbuffer[5]);
                        
                        // Step 2: Response back to master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_SINGLE_REGISTER, ui16StartAddtess, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);
                                
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();
                        
                        // Step 4: Send data to MCU0.
                        // Prepare IBU message node.
                        // a. Stuff IBU message header.
                        g_stIBUSendData.g_ui8ArrIBUData[0] = IBU_CMD_SET_LOAD_CELL_FACTOR;
                        
                        // b. Stuff data.
                        g_unIBUCurrLoadCellFactor.m_stLoadCellFactor.m_ui16CurrLoadCellFactor = g_ui16CurrLoadCellFactor;

                        // c. Stuff padding bytes; if needed.
                        g_unIBUCurrLoadCellFactor.m_stLoadCellFactor.m_ui16Padding = 0;
                        for(ui8LoopCount = 1; ui8LoopCount < IBU_MSG_BYTE_COUNT; ui8LoopCount++)
                        {
                            g_stIBUSendData.g_ui8ArrIBUData[ui8LoopCount] = g_unIBUCurrLoadCellFactor.m_ui8Arr[ui8LoopCount-1];
                        }
    
                        // d. Insert in IBU sent message queue.
                        InsertIBUMsgQueue(&g_stIBUSendQueue, &g_stIBUSendData);                        
                        break;
                    case MODE_ADDRESS:
                        // Step 1: accumulate data.
                        g_ui16CurrentMode = g_ui8RS485Rxbuffer[4];
                        g_ui16CurrentMode = ((g_ui16CurrentMode << 8) & 0xFF00);
                        g_ui16CurrentMode = (g_ui16CurrentMode | g_ui8RS485Rxbuffer[5]); 
                        
                        // Step 2: Response back to master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_SINGLE_REGISTER, ui16StartAddtess, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);                        
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();
                        
                        // Step 4: Send data to MCU0.
                        // Prepare IBU message node.                        
                        switch(g_ui16CurrentMode)
                        {
                            case BOARD_MODE_TYPE_NOTHING:
                                // Stuff IBU message header.
                                g_stIBUSendData.g_ui8ArrIBUData[0] = IBU_CMD_MODE_HOME;
                                
                                break;
                            case BOARD_MODE_TYPE_HOME:
                                // Stuff IBU message header.
                                g_stIBUSendData.g_ui8ArrIBUData[0] = IBU_CMD_MODE_HOME;
                                        
                                break;
                            case BOARD_MODE_TYPE_PRE_PRODUCTION:
                                // Stuff IBU message header.
                                g_stIBUSendData.g_ui8ArrIBUData[0] = IBU_CMD_MODE_PRE_PROD;
                                
                                break;
                            case BOARD_MODE_TYPE_PRODUCTION:
                                // Stuff IBU message header.
                                g_stIBUSendData.g_ui8ArrIBUData[0] = IBU_CMD_MODE_PROD;
                                
                                break;
                            case BOARD_MODE_TYPE_POST_PROD:
                                // Stuff IBU message header.
                                g_stIBUSendData.g_ui8ArrIBUData[0] = IBU_CMD_MODE_POST_PROD;
                                
                                break;
                            case BOARD_MODE_TYPE_CALIBRATION:
                                // Stuff IBU message header.
                                g_stIBUSendData.g_ui8ArrIBUData[0] = IBU_CMD_MODE_CALIB;
                                
                                break;
                            case BOARD_MODE_TYPE_CONFIGURATION:
                                // Stuff IBU message header.
                                g_stIBUSendData.g_ui8ArrIBUData[0] = IBU_CMD_MODE_CONFIG;
                                
                                break;
                            default:
                                break;
                        }
                        
                        // Stuff remaining padding bytes.
                        for(ui8LoopCount = 1; ui8LoopCount < IBU_MSG_BYTE_COUNT; ui8LoopCount++)
                        {
                            g_stIBUSendData.g_ui8ArrIBUData[ui8LoopCount] = 0;
                        }
                        
                        // Insert in IBU sent message queue.
                        InsertIBUMsgQueue(&g_stIBUSendQueue, &g_stIBUSendData);
                        break;
                    case  ADDRESS_FOR_START_CALIBRATION:
                        // Step 1: accumulate data.
                        g_fStartCalibration = g_ui8RS485Rxbuffer[4];
                        g_fStartCalibration = ((g_fStartCalibration << 8) & 0xFF00);
                        g_fStartCalibration = (g_fStartCalibration | g_ui8RS485Rxbuffer[5]);                         
                                
                        // Step 2: Response back to master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_SINGLE_REGISTER, ui16StartAddtess, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);                                                
                                
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();
                                
                        // Step 4: Send data to MCU0.
                        // Prepare IBU message node.
                        // a. Stuff IBU message header.
                        g_stIBUSendData.g_ui8ArrIBUData[0] = IBU_CMD_START_CALIBRATION;

                        // b. Stuff data.
                        // No data need to stuff

                        // c. Stuff padding bytes; if needed.
                        // Remaining all bytes are padding bytes; need to stuff.
                        for(ui8LoopCount = 1; ui8LoopCount < IBU_MSG_BYTE_COUNT; ui8LoopCount++)
                        {
                            g_stIBUSendData.g_ui8ArrIBUData[ui8LoopCount] = 0;
                        }                        

                        // d. Insert in IBU sent message queue.
                        InsertIBUMsgQueue(&g_stIBUSendQueue, &g_stIBUSendData);
                        
                        // Reset Calibration point values; i.e. Tare ADC & Weight ADC values.
                        g_ui32TareADCValueForCalibration = 0;
                        g_ui32gWeightADCValueForCalibration = 0;
                        break;
                    case ADDRESS_FOR_PRODUCTION_PV_TOL_PRE_PRODUCTION:
                        // Step 1: accumulate data.
                        g_ui16CurrProdPvTol = g_ui8RS485Rxbuffer[4];
                        g_ui16CurrProdPvTol = ((g_ui16CurrProdPvTol << 8) & 0xFF00);
                        g_ui16CurrProdPvTol = (g_ui16CurrProdPvTol | g_ui8RS485Rxbuffer[5]);                         
                        
                        // Step 2: Response back to master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_SINGLE_REGISTER, ui16StartAddtess, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);  
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();
                        
                        // Step 4: Send data to MCU0.
                        // Prepare IBU message node.
                        // a. Stuff IBU message header.
                        //g_stIBUSendData.g_ui8ArrIBUData[0] = IBU_CMD_PRE_PROD_TOLERANCE;
                        
                        // b. Stuff data.
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt = (int16)g_ui16CurrProdPvTol;
                        
                        // #####################################################
                        // Positive tolerance will be sent to MCU0 after negative
                        // tolerance will arrive so that can be sent accumulately.
                        // #####################################################
                        
//                        // c. Stuff padding bytes; if needed.
//                        g_unIBUAvgNode.m_stIBUAvgNode.m_i16Padding = 0;
//                        for(ui8LoopCount = 1; ui8LoopCount < IBU_MSG_BYTE_COUNT; ui8LoopCount++)
//                        {
//                            g_stIBUSendData.g_ui8ArrIBUData[ui8LoopCount] = g_unIBUAvgNode.m_ui8Arr[ui8LoopCount - 1];
//                        }                        
//                        
//                        // d. Insert in IBU sent message queue.
//                        InsertIBUMsgQueue(&g_stIBUSendQueue, &g_stIBUSendData);                         
                        break;
                    case ADDRESS_FOR_PRODUCTION_NV_TOL_PRE_PRODUCTION:
                        // Step 1: accumulate data.
                        g_ui16CurrProdNvTol = g_ui8RS485Rxbuffer[4];
                        g_ui16CurrProdNvTol = ((g_ui16CurrProdNvTol << 8) & 0xFF00);
                        g_ui16CurrProdNvTol = (g_ui16CurrProdNvTol | g_ui8RS485Rxbuffer[5]); 

                        // Step 2: Response back to master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_SINGLE_REGISTER, ui16StartAddtess, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);                                                
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();

                        // Step 4: Send data to MCU0.
                        // Prepare IBU message node.
                        // a. Stuff IBU message header.
                        g_stIBUSendData.g_ui8ArrIBUData[0] = IBU_CMD_PRE_PROD_TOLERANCE;  
                        
                        // b. Stuff data.
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt = (int16)g_ui16CurrProdNvTol;
                        
                        // c. Stuff padding bytes; if needed.
                        // Not Needed.
                        for(ui8LoopCount = 1; ui8LoopCount < IBU_MSG_BYTE_COUNT; ui8LoopCount++)
                        {
                            g_stIBUSendData.g_ui8ArrIBUData[ui8LoopCount] = g_unIBUTolNode.m_ui8Arr[ui8LoopCount - 1];
                        } 
                        
                        // d. Insert in IBU sent message queue.
                        InsertIBUMsgQueue(&g_stIBUSendQueue, &g_stIBUSendData);                         
                        break;
                    case ADDRESS_FOR_SCRAP_ON_MODE:
                        // Step 1: No data to parse..just send the mode to MCU0 for mode setting.
                        
                       // Step 2: Response back to master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_SINGLE_REGISTER, ui16StartAddtess, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL); 
                       
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();

                        // Step 4: Send data to MCU0.
                        // a. Stuff IBU message header.
                        g_stIBUSendData.g_ui8ArrIBUData[0] = IBU_CMD_PRE_PROD_SCRAP_MODE;
                        
                        // b. Stuff padding data.
                        for(ui8LoopCount = 1; ui8LoopCount < IBU_MSG_BYTE_COUNT; ui8LoopCount++)
                        {
                            g_stIBUSendData.g_ui8ArrIBUData[ui8LoopCount] = 0;
                        }

                        // c. Insert in IBU sent message queue.
                        InsertIBUMsgQueue(&g_stIBUSendQueue, &g_stIBUSendData);
                        break;
                    case ADDRESS_FOR_TOL_STORE_SETTING_ACK:
                        // No data to parse.
                        // Ack confirmation message.
                        // Reset ack register.
                        g_ui8ToleranceStoreAck = 0;
                        
                       // Step 2: Response back to master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_SINGLE_REGISTER, ui16StartAddtess, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL); 
                       
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();                        
                        break;
                    case ADDRESS_FOR_MACHINE_ID:
                        // Step 1: accumulate data.
                        g_ui8MachineID = g_ui8RS485Rxbuffer[5];
                        
                        // Step 2: Response back to master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_SINGLE_REGISTER, ui16StartAddtess, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);   
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();
                        
                        // Step 4: Set data.
                        g_stLANDataHeader.m_ui8MachineID = g_ui8MachineID;
                       
                        // Step 5: Store data in EEPROM.
                        WriteMachineIDToEEPROM(g_ui8MachineID);                        
                        break;
                    case ADDRESS_FOR_AT_DATA_INTERVAL_TIMER_SETTING:
                        // Step 1: accumulate data.
                        g_ui8DataSendingIntervalTime = g_ui8RS485Rxbuffer[5];

                        // Step 2: Response back to master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_SINGLE_REGISTER, ui16StartAddtess, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();

                        // Step 4: Store data in EEPROM.
                        WriteIntervalToEEPROM(g_ui8DataSendingIntervalTime);
                        break;                                
                    case ADDRESS_FOR_PIPE_PRESENSE_DELAY_TIME:
                        // Step 1: accumulate data.
                        g_ui16PipePresenceDelayTime = g_ui8RS485Rxbuffer[4];
                        g_ui16PipePresenceDelayTime = ((g_ui16PipePresenceDelayTime << 8) & 0xFF00);
                        g_ui16PipePresenceDelayTime = (g_ui16PipePresenceDelayTime | g_ui8RS485Rxbuffer[5]);

                        // Step 2: Response back to master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_SINGLE_REGISTER, ui16StartAddtess, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();

                        // Step 4: Send data to MCU0.
                        //a. Stuff message header.
                        g_stIBUSendData.g_ui8ArrIBUData[0] = IBU_CMD_STORE_PROD_DELAY_TIMERS;
                        
                        // b. Stuff data.
                        g_unProdDelayTimers.m_stProdDelayTimers.m_ui16DelayTimer = g_ui16PipePresenceDelayTime;
                        g_unProdDelayTimers.m_stProdDelayTimers.m_ui8MessageHeader = PIPE_PRESENCE_DELAY; // Means pipe presence delay time.
                        g_unProdDelayTimers.m_stProdDelayTimers.m_ui8Spare = 0;
                        
                        // c. Stuff in IBU data node.
                        for(ui8LoopCount = 1; ui8LoopCount < IBU_MSG_BYTE_COUNT; ui8LoopCount++)
                        {
                            g_stIBUSendData.g_ui8ArrIBUData[ui8LoopCount] = g_unProdDelayTimers.m_ui8Arr[ui8LoopCount - 1];
                        }

                        // c. Insert in IBU sent message queue.
                        InsertIBUMsgQueue(&g_stIBUSendQueue, &g_stIBUSendData);
                        break;
                    case ADDRESS_FOR_PIPE_STABILIZATION_DELAY_TIME:
                        // Step 1: accumulate data.
                        g_ui16PipeStabilizationDelayTime = g_ui8RS485Rxbuffer[4];
                        g_ui16PipeStabilizationDelayTime = ((g_ui16PipeStabilizationDelayTime << 8) & 0xFF00);
                        g_ui16PipeStabilizationDelayTime = (g_ui16PipeStabilizationDelayTime | g_ui8RS485Rxbuffer[5]);

                        // Step 2: Response back to master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_SINGLE_REGISTER, ui16StartAddtess, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();

                        // Step 4: Send data to MCU0.
                        //a. Stuff message header.
                        g_stIBUSendData.g_ui8ArrIBUData[0] = IBU_CMD_STORE_PROD_DELAY_TIMERS;
                        
                        // b. Stuff data.
                        g_unProdDelayTimers.m_stProdDelayTimers.m_ui16DelayTimer = g_ui16PipeStabilizationDelayTime;
                        g_unProdDelayTimers.m_stProdDelayTimers.m_ui8MessageHeader = PIPE_STABILIZATION_DELAY; // Means pipe stabilization delay time.
                        g_unProdDelayTimers.m_stProdDelayTimers.m_ui8Spare = 0;
                        
                        // c. Stuff in IBU data node.
                        for(ui8LoopCount = 1; ui8LoopCount < IBU_MSG_BYTE_COUNT; ui8LoopCount++)
                        {
                            g_stIBUSendData.g_ui8ArrIBUData[ui8LoopCount] = g_unProdDelayTimers.m_ui8Arr[ui8LoopCount - 1];
                        }

                        // c. Insert in IBU sent message queue.
                        InsertIBUMsgQueue(&g_stIBUSendQueue, &g_stIBUSendData);
                        break;
                    case ADDRESS_FOR_PIPE_TRANSFER_DELAY_TIME:
                        // Step 1: accumulate data.
                        g_ui16PipeTransferDelayTime = g_ui8RS485Rxbuffer[4];
                        g_ui16PipeTransferDelayTime = ((g_ui16PipeTransferDelayTime << 8) & 0xFF00);
                        g_ui16PipeTransferDelayTime = (g_ui16PipeTransferDelayTime | g_ui8RS485Rxbuffer[5]);

                        // Step 2: Response back to master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_SINGLE_REGISTER, ui16StartAddtess, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();

                        // Step 4: Send data to MCU0.
                        //a. Stuff message header.
                        g_stIBUSendData.g_ui8ArrIBUData[0] = IBU_CMD_STORE_PROD_DELAY_TIMERS;
                        
                        // b. Stuff data.
                        g_unProdDelayTimers.m_stProdDelayTimers.m_ui16DelayTimer = g_ui16PipeTransferDelayTime;
                        g_unProdDelayTimers.m_stProdDelayTimers.m_ui8MessageHeader = PIPE_TRANSFER_DELAY; // Means pipe transfer delay time.
                        g_unProdDelayTimers.m_stProdDelayTimers.m_ui8Spare = 0;
                        
                        // c. Stuff in IBU data node.
                        for(ui8LoopCount = 1; ui8LoopCount < IBU_MSG_BYTE_COUNT; ui8LoopCount++)
                        {
                            g_stIBUSendData.g_ui8ArrIBUData[ui8LoopCount] = g_unProdDelayTimers.m_ui8Arr[ui8LoopCount - 1];
                        }

                        // c. Insert in IBU sent message queue.
                        InsertIBUMsgQueue(&g_stIBUSendQueue, &g_stIBUSendData);
                        break;
                    case ADDRESS_FOR_SHIFT_NUMBER:
                        // Step 1: accumulate data.                        
                        g_ui8ShiftNumber = g_ui8RS485Rxbuffer[5];
                        
                        // Step 2: Response back to master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_SINGLE_REGISTER, ui16StartAddtess, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();                        
                        break;
                    case ADDRESS_FOR_SHIFT_DURATION:
                        // Step 1: accumulate data.
                        g_ui16TotalMachineUpTime = g_ui8RS485Rxbuffer[4];
                        g_ui16TotalMachineUpTime = ((g_ui16TotalMachineUpTime << 8) & 0xFF00);
                        g_ui16TotalMachineUpTime = (g_ui16TotalMachineUpTime | g_ui8RS485Rxbuffer[5]);
                        
                        // Step 2: Response back to master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_SINGLE_REGISTER, ui16StartAddtess, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();
                        
                        // Send the shift related data to server for report generation.
                        // Insert data in GPRS message queue.
                        //InsertInGPRSQueue();
                        break;
                    case ADDRESS_FOR_SELECT_SIM_OPERATOR:
                        // Step 1: accumulate data.
                        g_ui8SIMOperatorID = g_ui8RS485Rxbuffer[5];
                        
                        // Step 2: Response back to master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_SINGLE_REGISTER, ui16StartAddtess, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();

                        // Step 4: Store data in EEPROM.
                        WriteSIMOperatorIDToEEPROM(g_ui8SIMOperatorID);                        
                        break; 
                    case ADDRESS_FOR_WEIGHING_CYLINDER_UP_ACTIVATION_DELAY:
                        // Step 1: accumulate data.
                        g_ui16WeighingCylinderUpActivationDelay = g_ui8RS485Rxbuffer[4];
                        g_ui16WeighingCylinderUpActivationDelay = ((g_ui16WeighingCylinderUpActivationDelay << 8) & 0xFF00);
                        g_ui16WeighingCylinderUpActivationDelay = (g_ui16WeighingCylinderUpActivationDelay | g_ui8RS485Rxbuffer[5]);
                        
                        // Step 2: Response back to master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_SINGLE_REGISTER, ui16StartAddtess, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();

                        // Step 4: Send data to MCU0.
                        //a. Stuff message header.
                        g_stIBUSendData.g_ui8ArrIBUData[0] = IBU_CMD_WEGHNG_CYLDR_UP_ACT_DELAY;
                        
                        // b. Stuff data.
                        g_unWeighingCylinderUpActivationDelay.m_stWeighingCylinderUpActivationDelay.m_ui16DelayTime = g_ui16WeighingCylinderUpActivationDelay;
                        g_unWeighingCylinderUpActivationDelay.m_stWeighingCylinderUpActivationDelay.m_ui16Spare = 0;
                        
                        // c. Stuff in IBU data node.
                        for(ui8LoopCount = 1; ui8LoopCount < IBU_MSG_BYTE_COUNT; ui8LoopCount++)
                        {
                            g_stIBUSendData.g_ui8ArrIBUData[ui8LoopCount] = g_unWeighingCylinderUpActivationDelay.m_ui8Arr[ui8LoopCount - 1];
                        }

                        // c. Insert in IBU sent message queue.
                        InsertIBUMsgQueue(&g_stIBUSendQueue, &g_stIBUSendData);                        
                        break;                        
                    default:
                        break;
  
                }

                break;
            case MODBUS_FNCODE_WRITE_MULTIPLE_REGISTER:
                // Accumulate first register address to start write.
                ui16StartAddtess = g_ui8RS485Rxbuffer[2];
                ui16StartAddtess = ((ui16StartAddtess << 8) & 0xFF00);
                ui16StartAddtess = (ui16StartAddtess | g_ui8RS485Rxbuffer[3]);
                
                // Process data at that address.
                switch(ui16StartAddtess)
                {
                    case ADRESS_FOR_HMI_TIMESTAMP:
                        // Parse number of data bytes to follow.
                        ui8NoOfDataBytesToFollow = g_ui8RS485Rxbuffer[6];                        

                        // Year
                        g_stHMITime.tm_year = g_ui8RS485Rxbuffer[7];
                        g_stHMITime.tm_year = ((g_stHMITime.tm_year << 8) & 0xFF00);
                        g_stHMITime.tm_year = (g_stHMITime.tm_year | g_ui8RS485Rxbuffer[8]);
                        // Month
                        g_stHMITime.tm_mon = g_ui8RS485Rxbuffer[10] - 1;
                        // Day
                        g_stHMITime.tm_mday = g_ui8RS485Rxbuffer[12];
                        // Week Day
                        g_stHMITime.tm_wday = g_ui8RS485Rxbuffer[14];
                        // Hour
                        g_stHMITime.tm_hour = g_ui8RS485Rxbuffer[16];
                        // Minute
                        g_stHMITime.tm_min = g_ui8RS485Rxbuffer[18];
                        // Second
                        g_stHMITime.tm_sec = g_ui8RS485Rxbuffer[20];
                        
                        // Response Back to Master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_MULTIPLE_REGISTER, ui16StartAddtess, ui8NoOfDataBytesToFollow, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);

                        g_ui32UnixTimeInSec = mktime(&g_stHMITime);
                        
                        g_pstHMITimeTemp = localtime(&g_ui32UnixTimeInSec);
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();
                        break;
                    case ADDRESS_FOR_CALCULATED_MIN_MAX_WT:
                        // Parse number of data bytes to follow.
                        ui8NoOfDataBytesToFollow = g_ui8RS485Rxbuffer[6]; 

                        // Step 1: Accumulate data.
                        // Max. weight.
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt = g_ui8RS485Rxbuffer[9];
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt << 8);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt | g_ui8RS485Rxbuffer[10]);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt << 8);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt | g_ui8RS485Rxbuffer[7]);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt << 8);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt | g_ui8RS485Rxbuffer[8]);
                        
                        // Min. weight.
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt = g_ui8RS485Rxbuffer[13];
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt << 8);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt | g_ui8RS485Rxbuffer[14]);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt << 8);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt | g_ui8RS485Rxbuffer[11]);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt << 8);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt | g_ui8RS485Rxbuffer[12]);

                        // Step 2: Response Back to Master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_MULTIPLE_REGISTER, ui16StartAddtess, ui8NoOfDataBytesToFollow, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();

                        // Step 4: Send data to MCU0.
                        // Prepare IBU data node.
                        // a. Stuff IBU message header.
                        g_stIBUSendData.g_ui8ArrIBUData[0] = IBU_CMD_PRE_PROD_TOLERANCE;
                        
                        // b. stuff data in node.
                        for(ui8LoopCount = 1; ui8LoopCount < IBU_MSG_BYTE_COUNT; ui8LoopCount++)
                        {
                            g_stIBUSendData.g_ui8ArrIBUData[ui8LoopCount] = g_unIBUTolNode.m_ui8Arr[ui8LoopCount - 1];
                        }

                        // c. Insert in IBU message send queue.
                        InsertIBUMsgQueue(&g_stIBUSendQueue, &g_stIBUSendData);

                        // Set the values  in GPRS message header for server.
                        g_stLANDataHeader.m_ui32MaxWeight = (unsigned int32)g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt;
                        g_stLANDataHeader.m_ui32MinWeight = (unsigned int32)g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt;
                        break;
                    case ADDRESS_FOR_PRE_PROD_MAX_MIN_WT:
                        // Parse number of data bytes to follow.
                        ui8NoOfDataBytesToFollow = g_ui8RS485Rxbuffer[6]; 

                        // Step 1: Accumulate data.
                        // Max. weight.
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt = g_ui8RS485Rxbuffer[9];
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt << 8);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt | g_ui8RS485Rxbuffer[10]);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt << 8);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt | g_ui8RS485Rxbuffer[7]);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt << 8);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt | g_ui8RS485Rxbuffer[8]);
                        
                        // Min. weight.
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt = g_ui8RS485Rxbuffer[13];
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt << 8);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt | g_ui8RS485Rxbuffer[14]);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt << 8);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt | g_ui8RS485Rxbuffer[11]);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt << 8);
                        g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt = (g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt | g_ui8RS485Rxbuffer[12]);

                        // Step 2: Response Back to Master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_MULTIPLE_REGISTER, ui16StartAddtess, ui8NoOfDataBytesToFollow, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer(); 
                        
                        // Set the values  in GPRS message header for server.
                        g_stLANDataHeader.m_ui32MaxWeight = (unsigned int32)g_unIBUTolNode.m_stIBUTolNode.m_i32MaxWt;
                        g_stLANDataHeader.m_ui32MinWeight = (unsigned int32)g_unIBUTolNode.m_stIBUTolNode.m_i32MinWt;                        
                        break;
                    case ADDRESS_FOR_PRODUCT_BASIC_MATERIAL_STANDARD:
                        // Parse number of data bytes to follow.
                        ui8NoOfDataBytesToFollow = g_ui8RS485Rxbuffer[6];
                        
                        // Basic material and standard Specification and pressure rating of pipe has arrived.
                        // 1. First register(2 bytes) contains basic material and standard
                        // 2. Second register(2 bytes) contains standard specification.
                        // 3. Third register (2 bytes) contains pressure rating / type.
                        // Step 1: Accumulate data.
                        g_ui8BasicMaterialStandard = g_ui8RS485Rxbuffer[8];
                        g_ui8StandardSpecification = g_ui8RS485Rxbuffer[10];
                        g_ui8PressureRating        = g_ui8RS485Rxbuffer[12];
                        
                        // Step 2: Response Back to Master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_MULTIPLE_REGISTER, ui16StartAddtess, ui8NoOfDataBytesToFollow, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);
                        
                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();                        
                        break;
                    case ADDRESS_FOR_PIPE_SPECIFICATION:
                        // Parse number of data bytes to follow.
                        ui8NoOfDataBytesToFollow = g_ui8RS485Rxbuffer[6];

                        // Pipe specification has arrived.
                        // 1. First register(2 bytes) contains pipe's outer diameter(OD).
                        // 2. Second register(2 bytes) contains pipe's length.
                        // Step 1: Accumulate data.
                        g_ui8PipeOD = g_ui8RS485Rxbuffer[8];
                        g_ui8PipeLength = g_ui8RS485Rxbuffer[10];
                        
                        // Step 2: Response Back to Master.
                        SendResponseToMaster(MODBUS_RESPONSE_TO_MASTER_FOR_WRITE, MODBUS_FNCODE_WRITE_MULTIPLE_REGISTER, ui16StartAddtess, ui8NoOfDataBytesToFollow, MODBUS_RESPONSE_TO_MASTER_PARAMETER_NULL);

                        // Step 3: Clear receive buffer.
                        ResetRS485RXBuffer();

                        // Set all product specification in GPRS message header.
                        SetProductSpecificationForServer();
                        break;
                    default:
                        break;
                }
                break;

            default:
                break;
                
        }
    }
}


#endif /* _VI_RS485_CONFIG_H_ */

