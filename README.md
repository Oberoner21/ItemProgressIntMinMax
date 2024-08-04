# ItemProgressIntMinMax
This is a new menu item class to use with LcdMenu from [forntoh](https://github.com/forntoh/LcdMenu). This item class displays and make changeable int values in range with defined minimal and maximal values. Negative values are also possible.

-----
## Arduino code
### Getting started
Include LcdMenu.h and ItemProgressIntMinMax. Make sure that the library required for the interface of your LCD display is also available.(LiquidCrystal.h or LiquidCrystal_I2C.h).
```
#include <ItemProgressIntMinMax.h>
#include <LcdMenu.h>
```
### Create the menu
Create your menu with menu items class ItemProgressIntMinMax (and other). The parameters of the new class item in order are:
- key - The menu item displayed text.
- measure - The measure of menu item value is printed behind them. Can be NULL (not displayed).
- minval - The minimum that can be the item value.
- maxval - The maximum that can be the item value.
- start - The item value, when the menu is created.
- stepLength - The step value with which the item value is incremented or decremented
- callback - The function that is called when editing the menu item is finished. 
```
// Declare the menu call back functions
void bassCallback(uint16_t value);
void middleCallback(uint16_t value);
void trebleCallback(uint16_t value);
void balanceCallback(uint16_t value);
void menuExitCallback();

// Define the main menu
MAIN_MENU(
    ITEM_SUBMENU("Settings", settingsMenu),
    ITEM_PROGRESS_INT_MINMAX("Bass   ", "dB", -21, 6, 0, 1, bassCallback),
    ITEM_PROGRESS_INT_MINMAX("Middle ", "dB", -21, 6, 0, 1, middleCallback),
    ITEM_PROGRESS_INT_MINMAX("Treble ", "dB", -21, 6, 0, 1, trebleCallback),
    ITEM_PROGRESS_INT_MINMAX("Balance", NULL, -21, 21, 0, 1, balanceCallback),
    ITEM_COMMAND("Exit", menuExitCallback)
);
```
