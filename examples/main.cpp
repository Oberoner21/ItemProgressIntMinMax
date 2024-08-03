/*
    LCD-Menu with progress integer menu items that define
    minimum and maximum values. Negative values ​​are also possible.

    Tested on Arduino Uno R3 with rotery encoder.

    Oberoner21, Version 1.0
    03.08.2024
*/

#include <Wire.h> 
#include <SimpleRotary.h>
#include "SmartDebug.h"

#include <ItemSubMenu.h>
#include <ItemToggle.h>
#include "ItemProgressIntMinMax.h"
#include <LcdMenu.h>

// Example for Arduino UNO with input signals on pin 2, 3 and switch on pin 4
const uint8_t PIN_IN1 = 3;  
const uint8_t PIN_IN2 = 2;
const uint8_t PIN_SW = 4;

// Constants
const uint8_t LCD_ROWS = 4;
const uint8_t LCD_COLS = 20;

// Setup the SimpleRotary encoder
SimpleRotary encoder = SimpleRotary(PIN_IN1, PIN_IN2, PIN_SW);
LcdMenu menu(LCD_ROWS, LCD_COLS);

// Declare the call back functions
void toggleBacklight(uint16_t isOn);
void bassCallback(uint16_t value);
void middleCallback(uint16_t value);
void trebleCallback(uint16_t value);
void balanceCallback(uint16_t value);

int16_t intCallbackCorrection(uint16_t value);

extern MenuItem* settingsMenu[];

// Define the main menu
MAIN_MENU(
    ITEM_SUBMENU("Settings", settingsMenu),
    ITEM_PROGRESS_INT_MINMAX("Bass   ", "dB", -21, 6, 0, 1, bassCallback),
    ITEM_PROGRESS_INT_MINMAX("Mitten ", "dB", -21, 6, 0, 1, middleCallback),
    ITEM_PROGRESS_INT_MINMAX("Hoehen ", "dB", -21, 6, 0, 1, trebleCallback),
    ITEM_PROGRESS_INT_MINMAX("Balance", "dB", -21, 21, 0, 1, balanceCallback),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random")
);
/**
 * Create submenu and precise its parent
 */
SUB_MENU(settingsMenu, mainMenu,
    ITEM_TOGGLE("Backlight", toggleBacklight),
    ITEM_BASIC("Contrast")
);

void setup()
{
  DEBUG_BEGIN(9600);
  
  menu.setupLcdWithMenu(0x27, mainMenu);
  // set initial values of menu items
  menu[1]->getSubMenu()[1]->setIsOn(true);
  menu[2]->setProgress(-5);
  menu[3]->setProgress(0);
  menu[4]->setProgress(4);
  menu[5]->setProgress(0);
  menu.update();
}

/*
    Main loop
*/
void loop()
{
    /* Setups the menu to be used with an encoder.
    * You will need the SimpleRotarty Library for it work
    * switches the menu based on: 
    * A: CW or CCW rotation for UP and DOWN
    * B: short or Long SW press for ENTER and BACK 
    */
    byte i, j;

    // i will be: 0 = not turning, 1 = CW, 2 = CCW
    i = encoder.rotate();

    // j will be: 0 = not pushed, 1 = pushed, 2 = long pushed
    j = encoder.pushType(650);      //sets the millsecond button  
                                    //has to be pushed for it to 
                                    //beconsidered a long push.

    if (i == 1) 
    {
      if(menu.isInEditMode())
      {
          menu.right();             // increment the menu item value
      } 
      else 
      {
          menu.up();
      }
    } 
    else if (i == 2) 
    {
        if(menu.isInEditMode())
        {
            menu.left();            // decrement the menu item value
        }
        else
        {
            menu.down();
        }

    } 
    else if (j == 1) 
    {
      menu.enter();                 // short push on encoder switch
    } 
    else if (j == 2) 
    {
      menu.back();                  // long push
    }
}

/**
 * Define menu callbacks
 */
void toggleBacklight(uint16_t isOn)
{
    menu.setBacklight(isOn);
}

void bassCallback(uint16_t value) 
{
    DEBUG_PRINTLN_VALUE("Bass", intCallbackCorrection(value));
}

void middleCallback(uint16_t value)
{
    DEBUG_PRINTLN_VALUE("Mitten", intCallbackCorrection(value));
}

void trebleCallback(uint16_t value)
{
    DEBUG_PRINTLN_VALUE("Höhen", intCallbackCorrection(value));
}

void balanceCallback(uint16_t value)
{
    DEBUG_PRINTLN_VALUE("Balance", intCallbackCorrection(value));
}

/*
    Correction for use negative values
*/
int16_t intCallbackCorrection(uint16_t value)
{
    int16_t result = value;
    if(value > 32767)
    {
        result = value - 65536;
    }
    return result;
}

// The End