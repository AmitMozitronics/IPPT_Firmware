/* 
 * File:   vmM0Common.h
 * Author: mrinmoy
 *
 * Created on August 10, 2019, 9:35 AM
 */

#ifndef VMM0COMMON_H
#define	VMM0COMMON_H

//unsigned int8 g_ui8IBCTareDataSendDone = FALSE;

#define STATE_NOTHING            190
#define STATE_CALIB_STEP_WAITING 130
#define STATE_CALIB_STEP_1       131
#define STATE_CALIB_STEP_2       132
#define STATE_CALIB_STEP_3       133
#define STATE_CALIB_STEP_4       134
#define STATE_CALIB_STEP_5       135
#define STATE_CALIB_STEP_6       136
#define STATE_CALIB_STEP_7       137
#define STATE_CALIB_STEP_8       138
#define STATE_CALIB_STEP_9       139
#define STATE_CALIB_STEP_10      140

unsigned int8 g_ui8CurrentState  = 0;
unsigned int8 g_ui8StateWaiting = STATE_NOTHING;

unsigned int32 g_ui32CurPipeADCValue = 0;
unsigned int32 g_ui32CurPipeWeight   = 0;

#endif	/* VMM0COMMON_H */

