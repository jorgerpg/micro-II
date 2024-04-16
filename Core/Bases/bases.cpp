/**
 * @file bases.cpp
 * @author Jorge Ricarte Passos Gonçalves (jorgericartepg@gmail.com)
 * @brief Implementation file for software timers management.
 * @version 0.1
 * @date 2024-03-08
 *
 * @copyright Copyright (c) 2024
 *
 * This file provides functions to manage software timers.
 *
 * @section functionalities Functionalities:
 * - Start a timer with a specified interval.
 * - Check if a timer has reached its interval.
 * - Restart a timer for the next cycle.
 *
 * @section how_to_use How to use this driver:
 *
 * @code
 * #include "bases.h"
 *
 * int main()
 * {
 *     Base timer;
 *
 *     // Setting the interval to 1000 milliseconds (1 second)
 *     timer.start(1000);
 *
 *     // Waiting until the interval is reached
 *     if(timer.get())
 *     {
 *     	// The interval is reached, perform the desired action
 *     	toogle.led();
 *     	// Restart the timer for the next cycle
 *     	timer.restart();
 *     }
 *
 *
 *     return 0;
 * }
 * @endcode
 */

#ifndef BASE_H
#define BASE_H

#include "stm32f0xx_hal.h"

class Base {
public:
    Base() : intervalMs_(0), elapsedTime_(0)
    {
        startTime_ = HAL_GetTick(); // Using getTick() to get the initial time
    }
    void start(uint32_t interval) {
        intervalMs_ = interval;
    }

    bool get() const {
        auto currentTime = getTick(); // Get the current time

        // Handle overflow
        if (currentTime < startTime_) {
            // Overflow occurred, adjust the difference
            elapsedTime_ = UINT32_MAX - startTime_ + currentTime;
        } else {
            elapsedTime_ = currentTime - startTime_;
        }

        return elapsedTime_ >= intervalMs_;
    }

    void restart() {
        startTime_ = getTick(); // Using getTick() to get the initial time
    }

protected:
    uint32_t getTick() const override {
            return HAL_GetTick();
        }
        
private:
    uint32_t intervalMs_; // Interval in milliseconds
    uint32_t startTime_;   // Timer start time
    mutable uint32_t elapsedTime_; // Time elapsed since start
};

#endif // BASE_H
