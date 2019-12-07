/*
 * Name: systemClocks.h
 * Author: Jerome Hittle
 * Tools Used: CCS
 * Leveraged Code:
 * Links:
 *
 */

#ifndef SYSTEMCLOCKS_H_
#define SYSTEMCLOCKS_H_


/* Function: initSystemClocks
 * Description: Initializes the onboard clocks as follows
 *      MCLK = 48MHz <- Do NOT change this as power supply duty cycle
 *              depends on this frequency.
 *      HSMCLK = 48MHz
 *      SMCLK = 48MHz <- Do NOT change this as power supply duty cycle
 *              depends on this frequency.
 *      Initializes the SysTick at 4Hz, LED P1.0 flashes at 2Hz.
 * Inputs:  none
 * Outputs: none
 * Author:  J. Hittle
 * Last Updated: 11/21/2019
 */
void initSystemClocks(void);

#endif /* SYSTEMCLOCKS_H_ */
