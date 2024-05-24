/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_HPP
#define __APP_HPP

#include <cstdint>
#include "main.h"
#include "../Bases/bases.cpp"

#define LEDRED_PIN 6
#define LEDBLE_PIN 7
#define LEDORG_PIN 8
#define LEDGRN_PIN 9

// Define states
enum MSTATES {
    INIT_STATE,
    CONFIG_STATE,
    CONVERT_STATE,
    READ_STATE,
    DIGITAL_STATE
};

void initLEDS();
void writeLED();
void appInit();
void stateMachine();
void appFunction(uint16_t value);

void initADC();
void configADC();
void convertADC();
uint16_t readADC();

uint16_t adcValue;

#endif /* __APP_HPP */
