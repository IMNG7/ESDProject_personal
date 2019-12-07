/***************************************************************************************
 * LCD 16x2 / 16x4 using 4bit mode - lcd16x2_msp43x.c
 * - Developed focusing on the MSP430 and MSP432, but can be adapted to other hw
 * - Easy to configure and adapt in each new project (changing some #define in .h)
 * - with configuration error detection (will show some error if something go wrong)
 * - with simple conversion of integer and float to print on lcd
 * - - I encourage you to use my "Embedded_Printf" together (convert the number to string)
 *
 *  author: Haroldo Amaral - agaelema@globo.com
 *  v0.2.1 - 2017/12/28
 ***************************************************************************************
 *  log:
 *    v0.1      . Initial version without configuration macros
 *    v0.2.1    + add macros to facilitate the configuration
 *              . update some functions
 *    v0.2.2    - Remove unused parts
 ***************************************************************************************/

#include "lcd16x2_msp43x.h"
#include <stdint.h>
#include "adc.h"

/***************************************************************************************
 *                          MACROS - Do not change
 ***************************************************************************************/
#define     _SET_PIN(port, pin)     P ## port ## OUT |= BIT ## pin
#define     _CLEAR_PIN(port, pin)   P ## port ## OUT &= ~BIT ## pin

#define     _EN_SET(port, pin)      _SET_PIN(port, pin)
#define     EN_SET                  _EN_SET(_EN_PORT, _EN_PIN)          // macro to set EN pin
#define     _EN_CLEAR(port, pin)    _CLEAR_PIN(port, pin)
#define     EN_CLEAR                _EN_CLEAR(_EN_PORT, _EN_PIN)        // macro to clear EN pin

#define     _RS_SET(port, pin)      _SET_PIN(port, pin)
#define     RS_SET                  _RS_SET(_RS_PORT, _RS_PIN)          // macro to set RS pin
#define     _RS_CLEAR(port, pin)    _CLEAR_PIN(port, pin)
#define     RS_CLEAR                _RS_CLEAR(_RS_PORT, _RS_PIN)        // macro to clear RS pin

#define     _LCD_DATA_PORT(port)    P ## port ## OUT
#define     _LCD_DATA_OUT(port)     _LCD_DATA_PORT(port)
#define     LCD_DATA_OUT            _LCD_DATA_OUT(_LCD_4BIT_PORT)       // macro defining data port

#define     LCD_EN_STROBE           EN_CLEAR; EN_SET; EN_CLEAR;


/***************************************************************************************
 *
 ***************************************************************************************/
void lcd16x2_Wait(volatile uint_fast16_t wait_1)
{
    volatile int wait_2;
    for (wait_1; wait_1>1; wait_1--)
    {
        for (wait_2 = 110; wait_2 >= 0; wait_2--);
    }
}


/***************************************************************************************
 *
 ***************************************************************************************/
void lcd16x2_Init(void)
{    CS->CTL0  = CS_CTL0_DCORSEL_4;

    P7DIR = 0xFF;                                           // P1 as output
    P7OUT = 0x00;                                           // reset all pins
    P2DIR = 0x03;

    EN_CLEAR;
    RS_CLEAR;

    lcd16x2_Wait(1100);                                         // initialization delay - wait +- 15ms

    lcd16x2_Cmd(LCD_CMD_INIT_8BIT);                             // force 4bit mode after power up

    LCD_EN_STROBE;
    lcd16x2_Wait(150);
    LCD_EN_STROBE;
    lcd16x2_Wait(150);
    LCD_EN_STROBE;
    lcd16x2_Wait(150);

    lcd16x2_Cmd(LCD_CMD_RETURN_HOME);                           // move cursor to home
    LCD_EN_STROBE;

    lcd16x2_Wait(100);

    lcd16x2_Cmd(LCD_CMD_CLEAR_LCD);                             // clear display
    lcd16x2_Wait(100);

    lcd16x2_Cmd(LCD_CMD_2LINES_8BIT);                           // configure in 4bit mode 2 lines
    lcd16x2_Wait(100);

    lcd16x2_Cmd(LCD_CMD_DISPLAY_ON_CURSOR_ON);                  // display ON, cursor ON
    lcd16x2_Wait(100);

//    lcd16x2_Cmd(LCD_CMD_DISPLAY_ON_CURSOR_BLINK);               // display ON, cursor BLINK
//    lcd16x2_Wait(100);

    lcd16x2_Cmd(LCD_CMD_ENTRY_INCREMENT_SHIFT_OFF);             // increment address without shift
    lcd16x2_Wait(100);

    lcd16x2_Cmd(LCD_CMD_CURSOR_1ST_LINE);                       // put cursor at first line
    lcd16x2_Wait(100);


}


/***************************************************************************************
 *
 ***************************************************************************************/
void lcd16x2_Cmd(uint_fast8_t Data)
{
	RS_CLEAR;
	EN_CLEAR;

//    LCD_DATA_OUT &= ~(_LCD_4BIT_HIGH << _LCD_DATA_BASE);                    // reset data pins
//    LCD_DATA_OUT |= (((Data >> 4) & _LCD_4BIT_HIGH) << _LCD_DATA_BASE);     // send MSB first

//	EN_SET;
//	lcd16x2_Wait(6);
//	EN_CLEAR;

	LCD_DATA_OUT &= ~(_LCD_4BIT_HIGH << _LCD_DATA_BASE);                    // reset data pins
	LCD_DATA_OUT |= (Data & _LCD_4BIT_HIGH) << _LCD_DATA_BASE;              // send the LSB

	EN_SET;
	lcd16x2_Wait(6);
	EN_CLEAR;
}


/***************************************************************************************
 *
 ***************************************************************************************/
void lcd16x2_Data(char char_lcd)
{
    RS_SET;
    EN_CLEAR;

//    LCD_DATA_OUT &= ~ (_LCD_4BIT_HIGH << _LCD_DATA_BASE);                   // reset data pins
//    LCD_DATA_OUT = (((char_lcd >> 4) & _LCD_4BIT_HIGH) << _LCD_DATA_BASE); // send MSB first

//    EN_SET;
//	lcd16x2_Wait(6);
//	EN_CLEAR;

	LCD_DATA_OUT &= ~ (_LCD_4BIT_HIGH << _LCD_DATA_BASE);                   // reset data pins
	LCD_DATA_OUT = ((char_lcd & _LCD_4BIT_HIGH));        // send LSB

	EN_SET;
	lcd16x2_Wait(6);
	EN_CLEAR;
}


/***************************************************************************************
 *
 ***************************************************************************************/
volatile uint16_t column_pos=0;
volatile uint16_t row_pos=1;
void lcd16x2_SetPosition(uint_fast8_t row, uint_fast8_t coll)
{       column_pos=coll;
        row_pos=row;
        switch(row)
        {
            case 1: coll|=0x80;
                    break;
            case 2: coll+=63;
                    coll|=0x80;
                    break;
            case 3: coll+=14;
                    coll|=0x80;
                    break;
            case 4: coll+=79;
                    coll|=0x80;
                    break;
        }
        lcd16x2_Cmd(coll);
}


/***************************************************************************************
 *
 ***************************************************************************************/
void lcd16x2_PrintString(char *str)
{
	while (*str)                                            // while different of "\0"
	{
		lcd16x2_Data(*str++);									// print the char
		column_pos++;
		if(column_pos==16)
		{
		    row_pos++;
		    column_pos=0;
		    lcd16x2_SetPosition(row_pos,column_pos);
		}
		//str++;											    // increment pointer
	}
}


/*
 * versão muito limitada
 * - indicar o uso da embedded_printf
 * Converte um número inteiro para ser escrito no LCD de -9999 a 9999
 */
/***************************************************************************************
 *
 ***************************************************************************************/
void lcd16x2_PrintInt(volatile int_fast16_t iNumber)
{
    uint_fast8_t thousands, hundreds, tens, ones;

	if (iNumber < 0)
	{
	    lcd16x2_Data('-');
	    iNumber = iNumber * (-1);
	}

	thousands = iNumber / 1000;
	lcd16x2_Data(thousands + '0');                          // convert the number in ascii code

	hundreds = ((iNumber - thousands*1000)-1) / 100;
	lcd16x2_Data(hundreds + '0');                           // convert the number in ascii code

	tens = (iNumber % 100) / 10;
	lcd16x2_Data(tens + '0');                               // convert the number in ascii code

	ones = iNumber % 10;
	lcd16x2_Data(ones + '0');                               // convert the number in ascii code
}


/*
 * utiliza "car_float" como double para garantir que o número não seja arredondado
 * rotina atual aceita caracteres de -9999.9999 a 9999.9999
 */
/***************************************************************************************
 *
 ***************************************************************************************/
void lcd16x2_PrintFloat(volatile float var_float)
{
    int_fast16_t var_int;

    if (var_float < 0)                                      // if negative
    {
        var_float = var_float * (-1);                       // multiply by -1
        lcd16x2_Data('-');                                  // put minus symbol
    }

	var_int = (int_fast16_t) var_float;						// typecast - convert to integer
	lcd16x2_PrintInt(var_int);								// use integer function

	lcd16x2_Wait(2);
	lcd16x2_Data('.');										// write the dot "." - 0x2E

	var_float = (var_float - var_int)*10000;			    // multiply the residual
	var_int = (int_fast16_t) var_float;						// typecast - convert to integer
	lcd16x2_PrintInt(var_int);								// use integer function
}


/***************************************************************************************
 *
 ***************************************************************************************/
void lcd16x2_ShitftRigh(uint_fast8_t times)
{
    while(times > 0)
    {
        lcd16x2_Cmd(LCD_CMD_SHIFT_DISPLAY_RIGHT);
        lcd16x2_Wait(2);
        times--;
    }
}


/***************************************************************************************
 *
 ***************************************************************************************/
void lcd16x2_ShitftLeft(uint_fast8_t times)
{
    while(times)
    {
        lcd16x2_Cmd(LCD_CMD_SHIFT_DISPLAY_LEFT);
        lcd16x2_Wait(2);
        times--;
    }
}
void Print_Welcome()
{
    lcd16x2_SetPosition(1,4);                               // set position
    lcd16x2_PrintString("Welcome");                  // write a string to lcd
                // write a string to lcd
    lcd16x2_SetPosition(2,1);
            lcd16x2_PrintString("BATTV:");
            lcd16x2_SetPosition(3,1);
            lcd16x2_Wait(10);
            lcd16x2_PrintString("BATTI:");
            lcd16x2_Wait(10);
}
uint16_t counter=0;
uint8_t i=0;
uint16_t BattVoltage=0;
uint16_t BattCurrent=0;
void Print_ADC_Data()
{
        for(i=0;i<250;i++)
        {
            BattVoltage+=getVoltage(battV);
            BattCurrent+=getVoltage(battI);
        }
        BattVoltage=BattVoltage/250;
        BattCurrent=BattCurrent/250;
        counter++;
        if(counter==100)
        {   lcd16x2_SetPosition(4, 1);

                        lcd16x2_SetPosition(2,7);
                        lcd16x2_PrintInt(BattVoltage);          // convet and print integer - internal function (limited)
                        lcd16x2_Wait(10);
                        lcd16x2_SetPosition(3,7);
                        lcd16x2_PrintInt(BattCurrent);
                        counter=0;
                        BattVoltage=0;
                        BattCurrent=0;
        }


}
