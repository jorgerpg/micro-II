#include "app.hpp"
#include "main.h"

#include "../Bases/bases.cpp"
#include "../ADC/adc.cpp"

Base BASE1;
Base BASE2;
Base BASE5;
Base BASE05;

int main()
{
	mainInit();
    adc(adc_teste);
}

void app_init()
{
	  (void)BASE1.start(1000);
	  (void)BASE2.start(2000);
	  (void)BASE5.start(5000);
	  (void)BASE05.start(500);
}

void adc_teste(uint16_t adcteste )
{
	// TURN BLUE LED ON
//	RCC->AHBENR |= (1 << 19); // Enable GPIOC clock (RCC_AHBENR_GPIOCEN)
//	GPIOC->MODER &= ~(3 << (2 * 7)); // Clear mode bits for pin 7
//	GPIOC->MODER |= (1 << (2 * 7)); // Set pin 7 as output mode
//	GPIOC->ODR |= (1 << 7); // Set pin 7 to high

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
