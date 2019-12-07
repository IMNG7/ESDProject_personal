/*
 * Name: adc.h
 * Author: Jerome Hittle
 * Tools Used: CCS
 * Leveraged Code:
 * Links:
 *
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

//Enumerated type of the measurement on each ADC channel
typedef enum adc_channel {
    battV,
    battI,
    A8,
    A9,
    A10,
    A11,
    A12,
    A13
} ADC_CHANNEL;


/* Function: initADC
 * Description: Initializes the ADC to read channels A6 thru A13 in
 *      sequential mode, non-repeating.  14-bit
 * Inputs:  none
 * Outputs: none
 * Author:  J. Hittle
 * Last Updated: 11/21/2019
 */
void initADC (void);

/* Function: serviceADC
 * Description: services the ADC to get the data out of memory, should be called
 *      regularly to avoid overfilling ADC memory buffer.
 * Inputs:  none
 * Outputs: none
 * Author:  J. Hittle
 * Last Updated: 11/21/2019
 */
void serviceADC (void);

/* Function: startADCCapture
 * Description: starts an ADC capture
 * Inputs:  none
 * Outputs: none
 * Author:  J. Hittle
 * Last Updated: 11/21/2019
 */
void startADCCapture (void);

/* Function: getVoltage
 * Description: retrieves voltage from the ADC given a specific channel
 * Inputs:  channel: the channel voltage that should be returned
 * Outputs: Returns binary representation of the voltage that is being
 *      measured.
 * Author:  J. Hittle
 * Last Updated: 11/21/2019
 */
uint16_t getVoltage (ADC_CHANNEL channel);




#endif /* ADC_H_ */
