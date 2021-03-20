/* 
 * File:   vmM0ADCModuleCommon.h
 * Author: OEM
 *
 * Created on December 13, 2018, 4:59 PM
 */

#ifndef VM_M0_ADC_MODULE_COMMON_H
#define	VM_M0_ADC_MODULE_COMMON_H

#define PIPE_ACCEPTED              2
#define PIPE_REJECTED              0
#define PIPE_OVER_WEIGHT           1
#define PIPE_UNDER_WEIGHT          0

#define SYS_MODE_NOTHING        0
#define SYS_MODE_HOME           1
#define SYS_MODE_PRE_PROD       2
#define SYS_MODE_PROD           3
#define SYS_MODE_CONFIG         4
#define SYS_MODE_CALIB          5
#define SYS_MODE_POST_PROD      6
#define SYS_MODE_WAKE_UP        7
#define SYS_MODE_PRE_PROD_SCRAP 8

#define ADC_MEAN_DATA_INTERMEDIATE_WEIGHT   31
#define ADC_MEAN_DATA_INTERMEDIATE_TARE     32
#define ADC_MEAN_DATA_FINAL_WEIGHT          33
#define ADC_MEAN_DATA_FINAL_TARE            34
#define ADC_MEAN_DATA_CALIB_WEIGHT          35
#define ADC_MEAN_DATA_NOTHING               36

unsigned int8 g_ui8ADCMeanDataState = ADC_MEAN_DATA_NOTHING;

#define FLAG_WT_CALCULATION_DONE     1
#define FLAG_WT_CALCULATION_NOT_DONE 0

unsigned int8 g_fWtCalculationState = FLAG_WT_CALCULATION_NOT_DONE;

#define IBU_RESET_CALIB_OFF    0
#define IBU_RESET_CALIB_ON     1

#define HMI_REJECTION_AUTO_CALIB_MODE   0
#define HMI_REJECTION_FIXED_CALIB_MODE  1


unsigned int16 g_ui16RejectionStatus      = PIPE_REJECTED;
unsigned int8  g_ui8SystemMode            = SYS_MODE_NOTHING;
unsigned int32 g_ui32DynamicTareValue     = 0;
unsigned int32 g_ui32DynamicTareADCValue  = 0;
unsigned int16 g_ui16PipeCount            = 0;

unsigned int16 g_ui16PrevDynamicTareValue = 0;
int32          g_i32StaticTareValue       = 0;  // TODO Should keep in EEPROM
unsigned int8  g_fResetCalibStatus        = IBU_RESET_CALIB_OFF;

int32          g_i32ADCSumValue          = 0;
int32          g_i32ADCAvgValue          = 0;
unsigned int16 g_ui16ADCCalibValueEEPROM = 0;
unsigned int16 g_ui16CalibPointEEPROM    = 0;
int16          g_i16CalibResetPrevValue  = 0;
int16          g_i16DynamicErrValue      = 0;


unsigned int16 g_ui16PipePresenceDelayTime = 0;
unsigned int16 g_ui16PipeStabilizationDelayTime = 0;
unsigned int16 g_ui16PipeTransferDelayTime = 0;
unsigned int16 g_ui16WeighingCylinderUpActivationDelay = 0;

typedef struct _ST_CUR_BATCH_INFO
{
    int32 m_i32AvgWeight;
    int32 m_i32MaxWt;
    int32 m_i32MinWt;
} ST_CUR_BATCH_INFO;

ST_CUR_BATCH_INFO g_stCurBatchInfo;

      
#define STATE_HOME          0
#define STATE_PRE_PROD      1
#define STATE_PROD          2
#define STATE_POST_PROD     3
#define STATE_CALIB         4
#define STATE_NONE          5

unsigned int8  g_ui8ExpectedNextState[2] = {0};

#endif	/* VM_M0_ADC_MODULE_COMMON_H */

