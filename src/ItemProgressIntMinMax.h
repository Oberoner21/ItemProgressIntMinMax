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
        fptrInt callback = NULL; ///< Pointer to a callback function
        uint16_t progress = 0;   ///< The progress
        uint8_t stepLength = 1;
        uint16_t minval;
        uint16_t maxval;
        const char *measure = NULL; ///< Pointer to a measure string

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
        ItemProgressIntMinMax(const char *key, const char *measure, uint16_t minval, uint16_t maxval, uint16_t start, uint8_t stepLength,
                            fptrInt callback)
            : MenuItem(key, MENU_ITEM_PROGRESS),
            callback(callback),
            progress(start),
            stepLength(stepLength),
            minval(minval),
            maxval(maxval),
            measure(measure) {}

    /**
     * @brief Increments the progress value.
     */
    void increment() override
    {
        if ((int16_t)progress >= (int16_t)maxval) return;
        progress += stepLength;
    }

    /**
     * @brief Decrements the progress value.
     */
    void decrement() override
    {
        if ((int16_t)progress <= (int16_t)minval) return;
        progress -= stepLength;
    }

    /**
     * Set the progress on the item
     * @param uint16_t progress for the item
     */
    void setProgress(uint16_t value) override
    {
        if ((int16_t)progress < (int16_t)minval || (int16_t)progress > (int16_t)maxval) return;
        progress = value;
    }

    /**
     * Return the progress with correction then use negative values
     */
    uint16_t getItemIndex() override { return progress; }

    /**
     * Return the callback
     */
    fptrInt getCallbackInt() override { return callback; }

    /**
     * @brief Returns the value to be displayed.
     *        Returns the progress and if measure not
     *        NULL then adds the measure string behind the
     *        progress value.
     *
     * @return A pointer to the value of the current list item.
     */
    char *getValue() override
    {
        static char buffer[10];
        itoa((int16_t)progress, buffer, 10);
        if (measure != NULL)
        {
            concat(buffer, measure, buffer);
        }
        return buffer;
    }
};

#define ITEM_PROGRESS_INT_MINMAX(...) (new ItemProgressIntMinMax(__VA_ARGS__))

#endif // ItemProgress_H