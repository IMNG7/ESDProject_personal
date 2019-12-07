/*
 * Name: powerPWM.h
 * Author: Jerome Hittle
 * Tools Used: Code Composer Studio 9.2.0
 * Leveraged Code:
 * Links:
 *
 * (c) j. hittle 2019 - All rights reserved.
 * This file may not be reused, shared, or copied without written permission
 *   of the author for any reason, except where applicable by law.  The
 *   author retains all rights to the intellectual property herein.
 */

#ifndef POWERPWM_H_
#define POWERPWM_H_

/*
 *  Function Name: initPowerPWM
 *  Function Description: Initializes the power supply PWM signals
 *  Inputs:  none
 *  Outputs: none
 *  Author: Jerome Hittle
 *  Last updated: 11/25/2019
 */
void initPowerPWM(void);

/*
 *  Function Name: incrementDutyCycle
 *  Function Description: Increments the duty cycle of the power supply
 *      by one step of resolution.
 *  Inputs:  none
 *  Outputs: none
 *  Author: Jerome Hittle
 *  Last updated: 11/25/2019
 */
void incrementDutyCycle(void);

/*
 *  Function Name: decrementDutyCycle
 *  Function Description: Decrements the duty cycle of the power supply
 *      by one step of resolution.
 *  Inputs:  none
 *  Outputs: none
 *  Author: Jerome Hittle
 *  Last updated: 11/25/2019
 */
void decrementDutyCycle(void);

#endif /* POWERPWM_H_ */
