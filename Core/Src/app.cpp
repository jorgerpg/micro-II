#include "app.hpp"
#include "main.h"

#include "../Bases/bases.cpp"

Base BASE1;
Base BASE2;
Base BASE5;
Base BASE05;

uint16_t adcteste;

int main()
{
	mainInit();
	ADC_Init();

	while(true)
	{
		adc_teste();
	}
}

void app_init()
{
	  (void)BASE1.start(1000);
	  (void)BASE2.start(2000);
	  (void)BASE5.start(5000);
	  (void)BASE05.start(500);
}

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
uint16_t Read_ADC() {
    // Start ADC conversion
    ADC1->CR |= ADC_CR_ADSTART;

    // Wait for conversion to complete
    while (!(ADC1->ISR & ADC_ISR_EOC));

    // Read ADC value
    uint16_t adc_value = ADC1->DR;

    // Clear end of conversion flag
    ADC1->ISR &= ~ADC_ISR_EOC;

    return adc_value;
}

void adc_teste()
{
	// TURN BLUE LED ON
//	RCC->AHBENR |= (1 << 19); // Enable GPIOC clock (RCC_AHBENR_GPIOCEN)
//	GPIOC->MODER &= ~(3 << (2 * 7)); // Clear mode bits for pin 7
//	GPIOC->MODER |= (1 << (2 * 7)); // Set pin 7 as output mode
//	GPIOC->ODR |= (1 << 7); // Set pin 7 to high

	// Read ADC value from pin A1
	adcteste = Read_ADC();

	// Define the ADC value thresholds for LEDs
	uint16_t thresholds[] = {1000, 2000, 3000, 4000};
	uint16_t pins[] = {LD3_Pin, LD4_Pin, LD5_Pin, LD6_Pin};

	// Iterate through the thresholds and set/reset the LEDs
	for (int i = 0; i < 4; i++) {
		if (adcteste > thresholds[i]) {
			HAL_GPIO_WritePin(GPIOC, pins[i], GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOC, pins[i], GPIO_PIN_RESET);
		}
	}

	// Add a small delay for stability (you can adjust the delay as needed)
	HAL_Delay(100);
}


void bases_teste()
{

    if(BASE1.get()){
      HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
	  (void)BASE1.restart();
    }

    if(BASE2.get()){
      HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
	  (void)BASE2.restart();
    }

    if(BASE5.get()){
	  HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
	  (void)BASE5.restart();
    }

    if(BASE05.get()){
	  HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
	  (void)BASE05.restart();
    }
}

void app_loop()
{
}
