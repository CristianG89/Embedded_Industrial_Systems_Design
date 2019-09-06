#ifndef PID_H
#define PID_H

#include "stdint.h"

// Maximum value of variables
#define MAX_INT         INT16_MAX		//Max int16 = 0x7fff (no uint!)
#define MAX_LONG        INT32_MAX		//Max int32 = 0x7fffffff (no uint!)
#define MAX_I_TERM		(MAX_LONG/2)


//PID structure
typedef struct {
	int16_t lastProcessValue;
	int32_t sumError;
	int16_t P_Factor;
	int16_t I_Factor;
	int16_t D_Factor;
	int16_t maxError;
	int32_t maxSumError;
} pidData_t;


/****************************************************************************
Set up the PID and calculate its parameters
****************************************************************************/
void pid_init(pidData_t *pid, uint16_t frequency);


/****************************************************************************
Update the controlled output of the PID
****************************************************************************/
int16_t pid_controller(pidData_t *pid_st, uint8_t setPoint, int16_t processValue);

#endif //PID_H