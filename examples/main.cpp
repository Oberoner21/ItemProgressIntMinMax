/*
    LCD-Menu with progress integer menu items that define
    minimum and maximum values. Negative values ​​are also possible.

    Tested on Arduino Uno R3 with standard rotery encoder.

    If the menu is in edit mode, you can use rotary to change the
    item value. A long push to rotary encoder button exits the
    menu edit mode and give back the current value of menu item.

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

// Display constants
const uint8_t LCD_ROWS = 4;
const uint8_t LCD_COLS = 20;

// Create the SimpleRotary encoder object
SimpleRotary encoder = SimpleRotary(PIN_IN1, PIN_IN2, PIN_SW);
// Create the LcdMenu object
LcdMenu menu(LCD_ROWS, LCD_COLS);

// Declare the menu call back functions
void toggleBacklight(uint16_t isOn);
void bassCallback(uint16_t value);
void middleCallback(uint16_t value);
void trebleCallback(uint16_t value);
void balanceCallback(uint16_t value);

// Define a pointer to the empty settings submenu menu item array.
extern MenuItem *settingsMenu[];
// Define the main menu
MAIN_MENU(
    ITEM_SUBMENU("Settings", settingsMenu),
    ITEM_PROGRESS_INT_MINMAX("Bass   ", "dB", -21, 6, 0, 1, bassCallback),
    ITEM_PROGRESS_INT_MINMAX("Mitten ", "dB", -21, 6, 0, 1, middleCallback),
    ITEM_PROGRESS_INT_MINMAX("Hoehen ", "dB", -21, 6, 0, 1, trebleCallback),
    ITEM_PROGRESS_INT_MINMAX("Balance", NULL, -21, 21, 0, 1, balanceCallback),
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
  menu[1]->getSubMenu()[1]->setIsOn(true);  // set Backlight item to "ON"
  menu[2]->setProgress(-5);                 // set the bass value to -5dB
  menu[3]->setProgress(0);                  // set the middle value to 0dB
  menu[4]->setProgress(4);                  // set the treble value to 4dB
  menu[5]->setProgress(0);                  // set the balance value to 0
  menu.update();
}

/*
    Main loop
*/
void loop()
{
    switch(encoder.rotate())
    {
        // encoder not turning
        case 0:
            break; 
        // encoder turns CW      
        case 1:
            if(menu.isInEditMode()) menu.right();   // increment the menu item value
            else menu.up();                         // navigate upwards
            break;
        // encoder turns CCW      
        case 2:
            if(menu.isInEditMode()) menu.left();    // decremet the menu item value
            else menu.down();                       // navigate downwards
            break;
    }

    switch(encoder.pushType(650))                   // detects a long push after button press for 650mS
    {
        // encoder button not pushed
        case 0:
            break;
        // short push
        case 1:
            menu.enter();                           // enter in menu edit mode               
            break;
        // long push
        case 2:
            menu.back();                            // exit menu edit mode               
            break;
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
    DEBUG_PRINTLN_VALUE("Bass", (int16_t)value);
}

void middleCallback(uint16_t value)
{
    DEBUG_PRINTLN_VALUE("Mitten", (int16_t)value);
}

void trebleCallback(uint16_t value)
{
    DEBUG_PRINTLN_VALUE("Höhen", (int16_t)value);
}

void balanceCallback(uint16_t value)
{
    DEBUG_PRINTLN_VALUE("Balance", (int16_t)value);
}


// The End