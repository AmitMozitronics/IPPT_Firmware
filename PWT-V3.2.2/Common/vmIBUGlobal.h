#ifndef _VM_IBU_GOBAL_H_
#define _VM_IBU_GOBAL_H_

//#include "vmModbusGlobal.h"

#define IBU_SEND_LOCK                   1
#define IBU_SEND_UNLOCK                 0

#define STX                             0xA5    
#define ETX                             0xB5
#define MULTI_PROD_MAX_VARIETY          4

#define PIPE_PRESENCE_DELAY             1
#define PIPE_STABILIZATION_DELAY        2
#define PIPE_TRANSFER_DELAY             3

typedef enum _EN_IBU_COMMANDS
{
    IBU_CMD_MODE_HOME                   = 100,
    // Calibration Mode
    IBU_CMD_MODE_CALIB                  = 101,
    IBU_CMD_START_CALIBRATION           = 102,
//    IBU_CMD_RESET_CALIB_POINT_TARE      = 103,
    IBU_CMD_RESET_CALIB_DATA            = 104,
    IBU_CMD_RESET_CALIB_POINT           = 105,
//    IBU_CMD_RESET_CALIB_POINT_WEIGHT    = 106, 
    IBU_CMD_TARE_REQUEST                = 107,
    IBU_CMD_SET_LOAD_CELL_FACTOR        = 108,

    // Pre-Production Mode
    IBU_CMD_MODE_PRE_PROD               = 111,
    IBU_CMD_PRE_PROD_AVG_WEIGHT         = 112,
    IBU_CMD_PRE_PROD_TOLERANCE          = 113,
    IBU_CMD_MODE_AUTO_CALIB             = 114,
    IBU_CMD_MODE_FIXED_CALIB            = 115,
    IBU_CMD_SELEC_PROD_TYPE             = 116,
    IBU_CMD_CUR_BATCH_PROD_INDEX        = 117,
    IBU_CMD_PRE_PROD_SCRAP_MODE         = 118,
 
    // Production Mode
    IBU_CMD_MODE_PROD                   = 130,
    IBU_CMD_START_PROD                  = 131,
    IBU_CMD_PROD_WEIGH_DATA             = 132,
    IBU_CMD_PROD_BOX_COUNT              = 133,

    IBU_CMD_PROD_TARE_DATA              = 134,
    IBU_CMD_PROD_AVG_WT_DATA            = 135,
            
    IBU_CMD_PROD_INTMD_DATA             = 140,
    IBU_CMD_MODE_CONFIG                 = 141,
    IBU_CMD_VAR_1_AVG_WT                = 142,
    IBU_CMD_VAR_1_TOL_NODE              = 143,
    IBU_CMD_VAR_2_AVG_WT                = 144,
    IBU_CMD_VAR_2_TOL_NODE              = 145,
    IBU_CMD_VAR_3_AVG_WT                = 146,
    IBU_CMD_VAR_3_TOL_NODE              = 147,
    IBU_CMD_VAR_4_AVG_WT                = 148,
    IBU_CMD_VAR_4_TOL_NODE              = 149,
            
    IBU_CMD_MODE_POST_PROD              = 150,
    IBU_CMD_PRE_PROD_TOL_STORE_ACK      = 151,
    IBU_CMD_STORE_PROD_DELAY_TIMERS     = 152,
    IBU_CMD_AIR_PRESSURE_ALARM          = 153,
    IBU_CMD_RESET_AIR_PRESSURE_ALARM    = 154,
    IBU_CMD_WEGHNG_CYLDR_UP_ACT_DELAY   = 155
            
            
} EN_IBU_COMMANDS;

// Mode type
enum boardModeType
{
    BOARD_MODE_TYPE_NOTHING         = 0,
    BOARD_MODE_TYPE_HOME            = 1,
    BOARD_MODE_TYPE_PRE_PRODUCTION  = 2,
	BOARD_MODE_TYPE_PRODUCTION      = 3,
    BOARD_MODE_TYPE_CONFIGURATION   = 4,
    BOARD_MODE_TYPE_CALIBRATION     = 5,
    BOARD_MODE_TYPE_POST_PROD       = 6
};



//typedef struct _ST_IBU_PROD_M_WEIGHT_DATA
//{
//    unsigned int16 m_ui16BoxCount;
//    int16 m_i16BoxWeight;
//} ST_IBU_PROD_M_WEIGHT_DATA;
//
//
//typedef union _UN_IBU_PROD_M_WEIGHT_DATA
//{
//    ST_IBU_PROD_M_WEIGHT_DATA m_stWeighingData;
//    unsigned int8             m_ui8Arr[4];
//} UN_IBU_PROD_M_WEIGHT_DATA;

//typedef struct _ST_IBU_PROD_M_BOX_COUNT
//{
//    unsigned int16 m_ui16BoxCount;
//    unsigned int16 m_ui16Padding;
//}ST_IBU_PROD_M_BOX_COUNT;
//
//typedef union _UN_IBU_PROD_M_BOX_COUNT
//{
//    ST_IBU_PROD_M_BOX_COUNT m_stBoxCount;
//    unsigned int8           m_ui8Arr[4];
//}UN_IBU_PROD_M_BOX_COUNT;

typedef struct _ST_IBU_PROD_M_WEIGHING_STATUS_DATA
{
    int32           m_i32PipeWeight;
    unsigned int16  m_ui16PipeCount;
    unsigned int16  m_ui16PassStatus;
}ST_IBU_PROD_M_WEIGHING_STATUS_DATA;

typedef union _UN_IBU_PROD_M_WEIGHING_STATUS_DATA
{
    ST_IBU_PROD_M_WEIGHING_STATUS_DATA m_stWeighingStatusData;
    unsigned int8                      m_ui8Arr[8];
}UN_IBU_PROD_M_WEIGHING_STATUS_DATA;


typedef struct _ST_IBU_PROD_M_TARE_DATA
{
    int32 m_i32Padding;
    int32 m_i32TareData;
} ST_IBU_PROD_M_TARE_DATA;

typedef union _UN_IBU_PROD_M_TARE_DATA
{
    ST_IBU_PROD_M_TARE_DATA m_stTareData;
    unsigned int8           m_ui8Arr[8];
} UN_IBU_PROD_M_TARE_DATA;

typedef struct _ST_IBU_TOL_NODE
{
    int32 m_i32MaxWt;
    int32 m_i32MinWt;
} ST_IBU_TOL_NODE;

typedef union _UN_IBU_TOLERANCE_NODE
{
    ST_IBU_TOL_NODE m_stIBUTolNode;
    unsigned int8   m_ui8Arr[8];
}UN_IBU_TOLERANCE_NODE;

typedef struct _ST_IBU_RESET_CALIB_POINT
{
    int16         m_i16CalibPoint;
    int16         m_i16CurrCalibPoint;
} ST_IBU_RESET_CALIB_POINT;

typedef union _UN_IBU_RESET_CALIB_POINT
{
    ST_IBU_RESET_CALIB_POINT m_stIBUResetCalibPoint;
    unsigned int8            m_ui8Arr[4];
} UN_IBU_RESET_CALIB_POINT;


typedef struct _ST_IBU_RESET_CALIB_DATA
{
    unsigned int32 m_ui32CalibADCForTare;
    unsigned int32 m_ui32CalibADCForWeight;
} ST_IBU_RESET_CALIB_DATA;

typedef union _UN_IBU_RESET_CALIB_DATA
{
    ST_IBU_RESET_CALIB_DATA    m_stIBUResetCalibData;
    unsigned int8              m_ui8Arr[8];
} UN_IBU_RESET_CALIB_DATA;

typedef struct _ST_IBU_RESET_CALIB_POINT_WEIGHT
{
    unsigned int32 m_ui32CalibADCForWeight;
}ST_IBU_RESET_CALIB_POINT_WEIGHT;

typedef union _UN_IBU_RESET_CALIB_POINT_WEIGHT
{
    ST_IBU_RESET_CALIB_POINT_WEIGHT m_stIBUResetCalibWeight;
    unsigned int8                 m_ui8Arr[4];
}UN_IBU_RESET_CALIB_POINT_WEIGHT;

// Send Current Average Weight from MCU0 to MCU1, for IBU_CMD_PROD_AVG_WT_DATA
typedef struct _ST_IBU_PROD_MODE_CUR_AVG
{
    int32 m_i32CurAvg;
    int32 m_i32Padding;
} ST_IBU_PROD_MODE_CUR_AVG;

typedef union _UN_IBU_PROD_MODE_CUR_AVG
{
    ST_IBU_PROD_MODE_CUR_AVG m_stAvgData;
    unsigned int8            m_ui8Arr[8];
} UN_IBU_PROD_MODE_CUR_AVG;

typedef struct _ST_IBU_LOAD_CELL_FACTOR
{
    unsigned int16 m_ui16CurrLoadCellFactor;
    unsigned int16 m_ui16Padding;
    
}ST_IBU_LOAD_CELL_FACTOR;

typedef union _UN_IBU_LOAD_CELL_FACTOR
{
    ST_IBU_LOAD_CELL_FACTOR     m_stLoadCellFactor;
    unsigned int8               m_ui8Arr[4];
}UN_IBU_LOAD_CELL_FACTOR;


typedef struct _ST_IBU_PROD_DELAY_TIMERS
{
    unsigned int8  m_ui8MessageHeader;
    unsigned int16 m_ui16DelayTimer;
    unsigned int8  m_ui8Spare;
    
}ST_IBU_PROD_DELAY_TIMERS;

typedef union _UN_IBU_PROD_DELAY_TIMERS
{
    ST_IBU_PROD_DELAY_TIMERS  m_stProdDelayTimers;
    unsigned int8             m_ui8Arr[4];
}UN_IBU_PROD_DELAY_TIMERS;

typedef struct _ST_IBU_WEGHNG_CYLDR_UP_ACT_DELAY
{
    unsigned int16     m_ui16DelayTime;
    unsigned int16     m_ui16Spare;
}ST_IBU_WEGHNG_CYLDR_UP_ACT_DELAY;

typedef union _UN_IBU_WEGHNG_CYLDR_UP_ACT_DELAY
{
    ST_IBU_WEGHNG_CYLDR_UP_ACT_DELAY   m_stWeighingCylinderUpActivationDelay;
    unsigned int8                      m_ui8Arr[4];
}UN_IBU_WEGHNG_CYLDR_UP_ACT_DELAY;


//*****************************************************************************
// Below -> Global Variable Declaration ***************************************
//***************************************************************************** 
boardModeType           g_boardModeType;
boardModeType           g_boardModeTypePre;
unsigned int16          g_ui16DIStatus;
unsigned int16          g_ui16DOStatus;
unsigned int16          g_ui16ErrorStatus;
unsigned int8           g_ui8IncrementIndex;
int8                    g_fIBUSendLockStatus = IBU_SEND_UNLOCK;
unsigned int8           g_ui8IBUTempByte = 0;
unsigned int8           g_fDataReady  = 0;


UN_IBU_PROD_M_WEIGHING_STATUS_DATA  g_unIBUProdModeWtStsData;
UN_IBU_PROD_M_TARE_DATA             g_unIBUProdModeTrData;
UN_IBU_TOLERANCE_NODE               g_unIBUTolNode;
UN_IBU_RESET_CALIB_POINT            g_unIBUResetCalib;
UN_IBU_RESET_CALIB_DATA             g_unIBUResetCalibData;
UN_IBU_PROD_MODE_CUR_AVG            g_unIBUProdModeAvg;
UN_IBU_LOAD_CELL_FACTOR             g_unIBUCurrLoadCellFactor;
UN_IBU_RESET_CALIB_POINT_WEIGHT     g_unIBUResetCalibWeight;
UN_IBU_PROD_DELAY_TIMERS            g_unProdDelayTimers;
UN_IBU_WEGHNG_CYLDR_UP_ACT_DELAY    g_unWeighingCylinderUpActivationDelay;

//*****************************************************************************
// Above -> Global Variable Declaration ***************************************
//*****************************************************************************


#endif /* _VM_IBU_GOBAL_H_ */