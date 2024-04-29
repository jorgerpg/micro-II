/**
 * @file bases.cpp
 * @author Jorge Ricarte Passos Gonçalves (jorgericartepg@gmail.com)
 * @brief Implementation file for software timers management.
 * @version 0.1
 * @date 2024-04-28
 *
 * @copyright Copyright (c) 2024
 *
 * This file provides functions to manage software timers.
 *
 * @section functionalities Functionalities:
 * - Analogic Digital Convertion
 *
 * @section how_to_use How to use this driver:
 *
 * @code
 * #include "adc.h"
 *
 * int main()
 * {
 *     adc(user_function);
 *
 *     return 0;
 * }
 * @endcode
 */

#define INIT_STATE 0
#define CONFIG_STATE 1
#define READ_STATE 2
#define CONVERT_STATE 3
#define DIGITAL_STATE 4


// Function to initialize the ADC at the register level
void ADC_Init() {
        // Enable the clock for GPIOA
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // Configure PA1 as an analog input
    // Set the MODER bits for PA1 to 11 (analog mode)
    GPIOA->MODER |= (0x3 << (1 * 2));

    // Disable pull-up and pull-down resistors for PA1
    // Set the PUPDR bits for PA1 to 00 (no pull-up or pull-down)
    GPIOA->PUPDR &= ~(0x3 << (1 * 2));

    // Enable the clock for ADC
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    // Configure ADC resolution and data alignment

    // Clear the RES[1:0] bits in the ADC_CFGR1 register
    ADC1->CFGR1 &= ~(0x3 << 3);

    // Set the RES[1:0] bits to 00 (12-bit resolution)
    ADC1->CFGR1 |= (0x0 << 3);

    // Configure ADC channel sequence and sampling time
    ADC1->CHSELR = ADC_CHSELR_CHSEL1; // Select channel 1 (pin A1)
    ADC1->SMPR &= ~ADC_SMPR_SMP; // Clear sample time bits

    // Set the SMP[2:0] bits to 111 for 239.5 ADC clock cycles sampling time
    ADC1->SMPR |= (0x7);

    // Calibrate the ADC
    ADC1->CR &= ~ADC_CR_ADEN; // Ensure ADC is disabled
    ADC1->CR |= ADC_CR_ADCAL; // Start calibration
    while (ADC1->CR & ADC_CR_ADCAL); // Wait for calibration to complete

    // Enable continuous conversion mode
    ADC1->CFGR1 |= ADC_CFGR1_CONT; // Set the CONT bit to enable continuous conversion mode

    // Enable the ADC
    ADC1->CR |= ADC_CR_ADEN; // Enable the ADC
    while (!(ADC1->ISR & ADC_ISR_ADRDY)); // Wait for ADC to be ready

}

// Function to read ADC value from pin A1
// uint16_t Read_ADC() {
//     // Start ADC conversion
//     ADC1->CR |= ADC_CR_ADSTART;

//     // Wait for conversion to complete
//     while (!(ADC1->ISR & ADC_ISR_EOC));

//     // Read ADC value
//     uint16_t adc_value = ADC1->DR;

//     // Clear end of conversion flag
//     ADC1->ISR &= ~ADC_ISR_EOC;

//     return adc_value;
// }

void adc(void (*userFunction)()){
    int STATE = 0;

    while(1){
        switch(STATE){
            case INIT_STATE: 
            {
                ADC_Init();
                STATE = 1;
                break;
            }
            case CONFIG_STATE:
            {
                STATE = 2;
                break;
            }
            case READ_STATE:
            {
                // Start ADC conversion
                ADC1->CR |= ADC_CR_ADSTART;

                STATE = 3;
                break;
            }
            case CONVERT_STATE:
            {
                // Wait for conversion to complete
                while (!(ADC1->ISR & ADC_ISR_EOC));
                
                STATE = 4;
                break;
            }
            case DIGITAL_STATE:
            {
                //call user function
                userFunction(ADC1->DR);

                // Clear end of conversion flag
                ADC1->ISR &= ~ADC_ISR_EOC;

                STATE = 2;
                break;
            }
        }
    }
}