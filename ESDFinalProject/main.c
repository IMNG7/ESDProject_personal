/*
 * ********* Fall 2019 ESD - Final Project *************
 * ********** Maximum Power Point Tracker **************
 * Name: main.c
 * Author: Jerome Hittle, Nitik Gupta, Rakesh Kumar
 * Tools Used: Code Composer Studio 9.2.0
 * Leveraged Code:
 * Links:
 *
 */

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
/* Local Includes */
#include "systemClocks.h"
#include "powerSupply.h"
#include "adc.h"
#include "lcd16x2_msp43x.h"

int main(void)
{
    /* Halting the watchdog */
    WDT_A_holdTimer();

    volatile uint16_t a;

    initSystemClocks();
    initPowerSupply();
    initADC();
    lcd16x2_Init();
    MAP_Interrupt_enableMaster();
    Print_Welcome();
    while (1)
    {
        serviceADC();
        servicePowerSupply();
        Print_ADC_Data();
        //hd44780_deinit();
    }
}
