/**
 * @file main.cpp
 * @brief Application main file containing state machine, initialization functions, and LED control.
 */

#include "app.hpp" // Include header file for application-specific functions

/**
 * @brief Entry point of the application.
 */
int main()
{
    stateMachine(); // Start the state machine
}

/**
 * @brief State machine implementation controlling application flow.
 */
void stateMachine()
{
    uint8_t STATE = INIT_STATE; // Initialize state machine with INIT_STATE

    while(1){ // Infinite loop for state machine operation
        switch(STATE){ // Switch based on current state
            case INIT_STATE:
            {
                mainInit(); // Initialize main peripherals
                appInit();  // Initialize application-specific peripherals
                STATE = CONFIG_STATE; // Move to CONFIG_STATE
                break;
            }
            case CONFIG_STATE:
            {
            	configADC(); // Configure ADC
                STATE = CONVERT_STATE; // Move to CONVERT_STATE
                break;
            }
            case  CONVERT_STATE:
            {
            	convertADC(); // start ADC conversion
                STATE = READ_STATE; // Move to READ_STATE
                break;
            }
            case READ_STATE:
            {
            	adcValue = readADC(); // read ADC value
                STATE = DIGITAL_STATE; // Move to DIGITAL_STATE
                break;
            }
            case DIGITAL_STATE:
            {
                // Call function which uses the ADC value
                appFunction(adcValue);

                STATE = READ_STATE; // Move back to READ_STATE for next iteration
                break;
            }
        }
    }
}

/**
 * @brief Initialize GPIO pins for LEDs.
 */
void initLEDS()
{
	// Enable GPIOC clock (RCC_AHBENR_GPIOCEN)
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; //(1 << 19);

    // Configure LD3_Pin, LD4_Pin, LD5_Pin, LD6_Pin as output

	GPIOC->MODER &= ~(3 << (2 * LEDRED_PIN)); // Clear mode bits for pin 6/LEDRED_PIN
	GPIOC->MODER &= ~(3 << (2 * LEDBLE_PIN)); // Clear mode bits for pin 7/LEDBLE_PIN
	GPIOC->MODER &= ~(3 << (2 * LEDORG_PIN)); // Clear mode bits for pin 8/LEDORG_PIN
	GPIOC->MODER &= ~(3 << (2 * LEDGRN_PIN)); // Clear mode bits for pin 9/LEDGRN_PIN

	GPIOC->MODER |= (1 << (2 * LEDRED_PIN)); // Set pin 6/LEDRED_PIN as output mode
	GPIOC->MODER |= (1 << (2 * LEDBLE_PIN)); // Set pin 7/LEDBLE_PIN as output mode
	GPIOC->MODER |= (1 << (2 * LEDORG_PIN)); // Set pin 8/LEDORG_PIN as output mode
	GPIOC->MODER |= (1 << (2 * LEDGRN_PIN)); // Set pin 9/LEDGRN_PIN as output mode
}

/**
 * @brief Write state to LED.
 * @param pin Pin number of the LED.
 * @param state State to set the LED (GPIO_PIN_SET or GPIO_PIN_RESET).
 */
void writeLED(uint32_t pin, GPIO_PinState state)
{
    if (state == GPIO_PIN_SET) {
        GPIOC->ODR |= (1 << pin); // Set corresponding bit in ODR register to turn on LED
    } else {
        GPIOC->ODR &= ~(1 << pin); // Clear corresponding bit in ODR register to turn off LED
    }
}

/**
 * @brief Initialize application-specific peripherals.
 */
void appInit()
{
    adcValue = 0; // Initialize ADC value

    // Initialize application-specific peripherals
    initLEDS();
    initADC();
}

/**
 * @brief Application function to control LEDs based on ADC value.
 * @param value ADC value to determine LED states.
 */
void appFunction(uint16_t value)
{
    // Define the ADC value thresholds for LEDs
    uint16_t thresholds[] = {1000, 2000, 3000, 4000};
    uint32_t pins[] = {LEDRED_PIN, LEDORG_PIN, LEDGRN_PIN, LEDBLE_PIN};

    // Iterate through the thresholds and turn on/off the LEDs accordingly
    for (int i = 0; i < 4; i++) {
        if (value > thresholds[i]) {
            writeLED(pins[i], GPIO_PIN_SET); // Turn LED on
        } else {
            writeLED(pins[i], GPIO_PIN_RESET); // Turn LED off
        }
    }

    // Add a small delay for stability (you can adjust the delay as needed)
    HAL_Delay(100);
}

/**
 * @brief  Function to initialize GPIOA and ADC1 clocks
 */
void initADC()
{

    // Enable the clock for GPIOA
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // Enable the clock for ADC
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
}

/**
 * @brief  Function to configure GPIOA and ADC1
 */
void configADC() {
    // Configure PA1 as an analog input
    GPIOA->MODER |= (0x3 << (1 * 2));

    // Disable pull-up and pull-down resistors for PA1
    GPIOA->PUPDR &= ~(0x3 << (1 * 2));

    // Clear the RES[1:0] bits in the ADC_CFGR1 register
    ADC1->CFGR1 &= ~(0x3 << 3);

    // Set the RES[1:0] bits to 00 (12-bit resolution) max value is 4092
    ADC1->CFGR1 |= (0x0 << 3);

    // Configure ADC channel sequence and sampling time
    ADC1->CHSELR = ADC_CHSELR_CHSEL1; // Select channel 1 (pin A1)
    ADC1->SMPR &= ~ADC_SMPR_SMP; // Clear sample time bits
    ADC1->SMPR |= (0x7); // Set the SMP[2:0] bits to 111 for 239.5 ADC clock cycles sampling time
}

/**
 * @brief Function to start ADC conversion
 */
void convertADC()
{
    // Calibrate the ADC
    ADC1->CR &= ~ADC_CR_ADEN; // Ensure ADC is disabled
    ADC1->CR |= ADC_CR_ADCAL; // Start calibration
    while (ADC1->CR & ADC_CR_ADCAL); // Wait for calibration to complete

    // Enable continuous conversion mode
    ADC1->CFGR1 |= ADC_CFGR1_CONT; // Set the CONT bit to enable continuous conversion mode

    // Enable the ADC
    ADC1->CR |= ADC_CR_ADEN; // Enable the ADC
    while (!(ADC1->ISR & ADC_ISR_ADRDY)); // Wait for ADC to be ready

    // Start ADC conversion
    ADC1->CR |= ADC_CR_ADSTART;
}

/**
* @brief Function to read ADC value
*/
uint16_t readADC()
{
    // Wait for conversion to complete
    while (!(ADC1->ISR & ADC_ISR_EOC));

    uint16_t adcValue = ADC1->DR; // Read ADC value

    // Clear end of conversion flag
    ADC1->ISR &= ~ADC_ISR_EOC;

    return adcValue;
}
