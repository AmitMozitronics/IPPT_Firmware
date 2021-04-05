#ifndef VM_IMW_COMMON_H
#define VM_IMW_COMMON_H

#define MODE_PRODUCTION                  0 
#define MODE_CALIBRATION                 1

#define PIN_HIGH                         0  
#define PIN_LOW                          1 

//************************** End ***************************************

//******************************************************
//************* Shift Register Delays ******************
//******************************************************

#define SOLENOID_BACK_START_TIME_COUNT    1
#define SOLENOID_BACK_STOP_TIME_COUNT     0

#define REJECTION_UNIT_ENGAGE				 75
#define REJECTION_UNIT_FREE					 76

//*******************************************************
//************** End ************************************
//*******************************************************

#define LDCD_FILTER_TOLERANCE 5   // 50% Tolerance for Filter Value (Base is 10, Means 10 = 100%)
#define LDCD_FILTER_MEAN      50  // 80% Max is 100

#endif // VM_IMW_COMMON_H