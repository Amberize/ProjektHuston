#ifndef DEFINES_H
#define DEFINES_H

//-----------------------------------------------------------------------------
//! ==-Config-==
//-----------------------------------------------------------------------------

//! Inverting controller axis
//#define INVERT_THROTTLE
#define INVERT_YAW
//#define INVERT_PITCH
#define INVERT_ROLL

//! Rounding area near middle value of data
#define STABLE_MID_ROUDING

//-----------------------------------------------------------------------------
//! Gui defines
//-----------------------------------------------------------------------------
#define S_TEST_ADDRESS          "127.0.0.1:5000"
#define I_LABELS_WIDTH          40

//-----------------------------------------------------------------------------
//! Controller defines
//-----------------------------------------------------------------------------
#define I_DATA_UPDATE_TIME      50 //ms

#define I_THROTTLE_AXIS         2
#define I_YAW_AXIS              0
#define I_PITCH_AXIS            3
#define I_ROLL_AXIS             4

#define A_THROTTLE              0
#define A_YAW                   1
#define A_PITCH                 2
#define A_ROLL                  3

//-----------------------------------------------------------------------------
//! Data counting
//-----------------------------------------------------------------------------
#define I_MAX_DATA              1900
#define I_MIN_DATA              1100
#define I_MID_DATA              1500

#define I_INT_MAX               32767

//! In this area arount middle value, final data will be rounding
#define D_DELTA_MID             0.012




#endif // DEFINES_H
