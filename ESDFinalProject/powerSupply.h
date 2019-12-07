/*
 * Name: powerSupply.h
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

#ifndef POWERSUPPLY_H_
#define POWERSUPPLY_H_

/*
 *  Function Name: initPowerSupply
 *  Function Description: Initializes the Power supply
 *  Inputs:  none
 *  Outputs: none
 *  Author: Jerome Hittle
 *  Last updated: 11/25/2019
 */
void initPowerSupply(void);

/*
 *  Function Name: servicePowerSupply
 *  Function Description: Function to service the Power Supply, should be called
 *      in regularly to keep the Power Supply operating properly.  Changes the duty
 *      cycle as required.
 *  Inputs:  none
 *  Outputs: none
 *  Author: Jerome Hittle
 *  Last updated: 11/25/2019
 */
void servicePowerSupply (void);

#endif /* POWERSUPPLY_H_ */
