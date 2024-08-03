/**
 * @file ItemProgressIntMinMax.h
 * @brief This file contains the declaration of the ItemProgressIntMinMax class.
 */

#ifndef ItemProgress_H
#define ItemProgress_H

#undef MIN_PROGRESS
#define MIN_PROGRESS -32768
#undef MAX_PROGRESS
#define MAX_PROGRESS 65536

#include <MenuItem.h>
#include <utils/utils.h>

/**
 * @class ItemProgressIntMinMax
 * 
 * @brief ItemProgressIntMinMax
 *  indicates that the current item is a list.
 */
class ItemProgressIntMinMax : public MenuItem
{
   private:
    fptrMapping mapping = NULL;  ///< Pointer to a mapping function
    fptrInt callback = NULL;     ///< Pointer to a callback function
    uint16_t progress = 0;       ///< The progress
    uint8_t stepLength = 1;
    uint16_t minval;
    uint16_t maxval;
    uint16_t correction = 0;     ///< Correction value for use negative numbers.                                    
    const char *measure = NULL;  ///< Pointer to a measure string

   public:
    /**
     * @brief Constructs a new ItemProgressIntMinMax object.
     *
     * @param key The key of the menu item.
     * @param measure The measure of the menu item
     * @param minval The minimal progress value.
     * @param maxval The maximal progress value.
     * @param start The starting progress value.
     * @param stepLength The length of steps to the next position.
     * @param callback A pointer to the callback function to execute when this menu item is selected.
     */
    ItemProgressIntMinMax
    (const char* key, const char *measure, int16_t minval, int16_t maxval, int16_t start, uint8_t stepLength,
                 fptrInt callback)
        : MenuItem(key, MENU_ITEM_PROGRESS),
          callback(callback),
          stepLength(stepLength),
          measure(measure)
    {
        if(minval < 0)
        {
            this->correction = abs(minval);
            this->minval = 0;
            this->maxval = maxval + correction;
            this->progress = start + correction;
        }
        else 
        {
            this->minval = minval;
            this->maxval = maxval;
            this->progress = start;
        }
    }

    /**
     * @brief Increments the progress of the list.
     */
    void increment() override
    {
        if (progress >= maxval) return;
        progress += stepLength;
    }

    /**
     * @brief Decrements the progress of the list.
     */
    void decrement() override 
    {
        if (progress <= minval) return;
        progress -= stepLength;
    }

    /**
     * Set the progress on the item
     * @param uint16_t progress for the item
     */
    void setProgress(uint16_t value) override 
    {
        if (progress < minval || progress > maxval) return;
        progress = value + correction;  // correct negative values
    }

    /**
     * Return the progress
     */
    uint16_t getItemIndex() override { return progress - correction; }

    /**
     * Return the callback
     */
    fptrInt getCallbackInt() override { return callback; }

    /**
     * @brief Returns the value to be displayed.
     *        If there's no mapping, it returns the progress
     *        and if measure not NULL then concate the measure
     *        string behind the progress
     *        else it returns the mapped progress
     *
     * @return A pointer to the value of the current list item.
     */
    char* getValue() override
    {
        if (mapping == NULL)
        {
            static char buffer[10];
            itoa((int)progress - correction, buffer, 10);
            if(measure != NULL)
            {
                concat(buffer, measure, buffer);
            }
            return buffer;
        }
        else 
        {
            return mapping(progress);
        }
    }
};

#define ITEM_PROGRESS_INT_MINMAX(...) (new ItemProgressIntMinMax(__VA_ARGS__))

#endif  // ItemProgress_H