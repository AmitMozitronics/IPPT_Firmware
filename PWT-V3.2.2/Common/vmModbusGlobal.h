#ifndef _VI_MODBUS_GOBAL_H_
#define _VI_MODBUS_GOBAL_H_

#include "vmIBUGlobal.h"

#define MODBUS_SERIAL_RX_BUFFER_SIZE    64  
#define MODBUS_REG_BUFFER_SIZE          10  
#define MAX_RUNNING_PRODUCTS            8 // Maximum is 7..but here 8 is taken to maintain correspondenc between product ID & array index..i.e. index 0 is not used.
                                          // data storing starts from index 1 and ends at index 7.
#define MODBUS_IS_VALID_SLAVE_ID             12
#define MODBUS_IS_NOT_VALID_SLAVE_ID         13




typedef struct _ST_CUR_BATCH_INFO
{
    int16 m_i16AvgWeight;
    int16 m_i16PositiveTol;
    int16 m_i16NegativeTol;
} ST_CUR_BATCH_INFO;


//unsigned int16 g_ui16RegisterCurrentCalibrationWeight;
//unsigned int16 g_ui16RegisterCurrentCalibrationPoint;
unsigned int32 g_ui32TareADCValueForCalibration = 0;
unsigned int32 g_ui32gWeightADCValueForCalibration = 0;
//unsigned int16 g_ui16gAvgWtForCalibration;


// use for production 
unsigned int16 g_ui16PipeCount = 0;
unsigned int16 g_ui16PipeCountPrev;
unsigned int32 g_ui32CurrentWt = 0;
unsigned int16 g_ui16CurrentPassStatus = 0;
unsigned int32 g_ui32RunningAvgWt = 0;
unsigned int32 g_ui32RunningTareValue = 0;
//unsigned int16 g_ui16ServerStatus;

//unsigned int16 g_ui16FlagForSetTareValue;



// use for pre production
//unsigned int16 g_ui16CurrProdIndex = 0;
unsigned int16 g_ui16CurrProdAvgWt = 0;
unsigned int16 g_ui16CurrProdPvTol = 0;
unsigned int16 g_ui16CurrProdNvTol = 0;

// Use for Current Mode
unsigned int16 g_ui16CurrentMode = 0;

// Use for detect start calibration.
unsigned int8  g_fStartCalibration = 0;


// Use for store load cell factor.
unsigned int16 g_ui16CurrLoadCellFactor = 0;

// Use for Store Running Production Type Selection.
//unsigned int8  g_ui8CurrProductionType = 0;

// Use for Store HMI TimeStamp
unsigned int16  g_ui16HMITimeStamp = 0;

// Use for Passing Box Product Index.
//unsigned int16  g_ui16CurrProdIndex = 0;

// Use for Tolerance store acknowledge.
unsigned int8   g_ui8ToleranceStoreAck = 0;

// Use for Server Machine ID.
unsigned int8   g_ui8MachineID = 0;

// Use for AT-OK response check time.
unsigned int8  g_ui8ATResponseCheckTime = 0;

// Use for data sending interval time.
unsigned int8 g_ui8DataSendingIntervalTime = 0;

// Use for HTTP GET session time.
unsigned int8 g_ui8HTTPGETSessionTime = 0;

// Use for Wireless Modem power cut time for reset.
unsigned int8 g_ui8WirelessModemResetTime = 0;

// Use for Wireless modem network reconnection halt time.
unsigned int8 g_ui8WirelessModemNWReccntHaltTime = 0;

// Use for Pipe presence delay time.
unsigned int16 g_ui16PipePresenceDelayTime = 0;

// Use for pipe stabilization delay time.
unsigned int16 g_ui16PipeStabilizationDelayTime = 0;

// Use for Pipe transfer delay time.
unsigned int16 g_ui16PipeTransferDelayTime = 0;

// Use for wireless modem periodic reset interval time.
unsigned int8  g_ui8WirelessModemPeriodicResetIntervalTime = 0;

// Use for Shift number storing.
unsigned int8  g_ui8ShiftNumber = 0;

// Use for Actual Machine Up time storing in one shift.
unsigned int16 g_ui16TotalMachineUpTime = 0;

// Use for Air-pressure below rated alarm .
unsigned int8  g_ui8AirPressureAlarm = 0;

// Use for SIM Operator ID.
unsigned int8 g_ui8SIMOperatorID = 0;

// Use for Weighing cylinder up activation delay.
unsigned int16 g_ui16WeighingCylinderUpActivationDelay = 0;

// Use for product basic material and standard and standard specification and pressure rating.
unsigned int8  g_ui8BasicMaterialStandard = 0;
unsigned int8  g_ui8StandardSpecification = 0;
unsigned int8  g_ui8PressureRating        = 0;

// Use for pipe specification.
unsigned int8  g_ui8PipeOD                = 0;
unsigned int8  g_ui8PipeLength            = 0;

//void ResetIBUPreProdDataNode(void)
//{
//    unsigned int8 ui8LoopCount = 0;
//    for(ui8LoopCount = 0; ui8LoopCount < MULTI_PROD_MAX_VARIETY; ui8LoopCount++)
//    {   
//        // Tolerance Node.
//        g_unIBUArrTolNode[ui8LoopCount].m_stIBUTolNode.m_i32MaxWt = 0;
//        g_unIBUArrTolNode[ui8LoopCount].m_stIBUTolNode.m_i32MinWt = 0;
//    }
//}




#endif /* _VI_MODBUS_GOBAL_H_ */

