/*
 * Name: adc.c
 * Author: Jerome Hittle
 * Tools Used: CCS
 * Leveraged Code:  TI SimpleLink v3.30.00.13, MSP432P401R DriverLib Examples:
 *              adc14_multiple_channel_no_repeat.c
 * Links:
 *
 */


#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <string.h>
#include "adc.h"



#define NUM_ADC_CHANNELS    8       //Number of adc channels to measure
#define MAX_SAMPLES         8       //Number of samples to return with request

#define ADC_BATT_V  GPIO_PIN7       //A6    P4.7
#define ADC_BATT_I  GPIO_PIN6       //A7    P4.6

static uint16_t resultsBuffer[NUM_ADC_CHANNELS];
static uint16_t datamem[NUM_ADC_CHANNELS][MAX_SAMPLES];
volatile bool adc_conversion_complete_flag = false;

void initADC (void) {
    /* Zero-filling buffer */
    memset(resultsBuffer, 0x00, NUM_ADC_CHANNELS * sizeof(uint16_t));
    memset(datamem, 0x00, NUM_ADC_CHANNELS * sizeof(uint16_t) * sizeof(uint16_t));

    /* Setting reference voltage to 2.5  and enabling reference */
    MAP_REF_A_setReferenceVoltage(REF_A_VREF2_5V);
    MAP_REF_A_enableReferenceVoltage();

    /* Initializing ADC (SMCLK/1/1) */
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_SMCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1,
            0);

    /* Configuring GPIOs for Analog In */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                                                   ADC_BATT_V |
                                                   ADC_BATT_I |
                                                   GPIO_PIN5 |
                                                   GPIO_PIN4 |
                                                   GPIO_PIN3 |
                                                   GPIO_PIN2 |
                                                   GPIO_PIN1 |
                                                   GPIO_PIN0
                                                   , GPIO_TERTIARY_MODULE_FUNCTION);


    /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM7 (A6 - A13)  without repeat)
     * with internal 2.5v reference */
    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM7, false);
    MAP_ADC14_configureConversionMemory(ADC_MEM0,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A6, ADC_NONDIFFERENTIAL_INPUTS );
    MAP_ADC14_configureConversionMemory(ADC_MEM1,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A7, ADC_NONDIFFERENTIAL_INPUTS );
    MAP_ADC14_configureConversionMemory(ADC_MEM2,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A8, ADC_NONDIFFERENTIAL_INPUTS );
    MAP_ADC14_configureConversionMemory(ADC_MEM3,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS );
    MAP_ADC14_configureConversionMemory(ADC_MEM4,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A10, ADC_NONDIFFERENTIAL_INPUTS );
    MAP_ADC14_configureConversionMemory(ADC_MEM5,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A11, ADC_NONDIFFERENTIAL_INPUTS );
    MAP_ADC14_configureConversionMemory(ADC_MEM6,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A12, ADC_NONDIFFERENTIAL_INPUTS );
    MAP_ADC14_configureConversionMemory(ADC_MEM7,
            ADC_VREFPOS_INTBUF_VREFNEG_VSS,
            ADC_INPUT_A13, ADC_NONDIFFERENTIAL_INPUTS );

    /* Enabling the interrupt when a conversion on channel 13 (end of sequence)
     *  is complete and enabling conversions */
    MAP_ADC14_enableInterrupt(ADC_INT7 | ADC_OV_INT);

    /* Enabling Interrupts */
    MAP_Interrupt_enableInterrupt(INT_ADC14);


    /* Setting up the sample timer to automatically step through the sequence
     * convert.
     */
    MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    /* Triggering the start of the sample */
    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();

}
void serviceADC(void) {

    static uint8_t i=0;

    if(adc_conversion_complete_flag) {
        datamem[battV][i]   = resultsBuffer[battV];
        datamem[battI][i]   = resultsBuffer[battI];
        datamem[A8][i]      = resultsBuffer[A8];
        datamem[A9][i]      = resultsBuffer[A9];
        datamem[A10][i]     = resultsBuffer[A10];
        datamem[A11][i]     = resultsBuffer[A11];
        datamem[A12][i]     = resultsBuffer[A12];
        datamem[A13][i]     = resultsBuffer[A13];

        if (++i==MAX_SAMPLES) i = 0;
        adc_conversion_complete_flag = false;

    }
}

void startADCCapture (void) {
    /* Triggering the start of the sample */
    MAP_ADC14_toggleConversionTrigger();
}

uint16_t getVoltage (ADC_CHANNEL channel) {
    uint8_t i;
    uint32_t voltage = 0;

    for (i=0; i<MAX_SAMPLES; i++) {
        voltage += datamem[channel][i];
    }

    return (voltage / MAX_SAMPLES);
}

/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM13. This signals the end of conversion and the results array is
 * grabbed and placed in resultsBuffer */
void ADC14_IRQHandler(void)
{
    uint64_t status;

    status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    if(status & ADC_INT7) {
        MAP_ADC14_getMultiSequenceResult(resultsBuffer);
        adc_conversion_complete_flag = true;
    }

    if(status & ADC_OV_INT) {
        while(1);
    }

}
