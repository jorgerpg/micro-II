#include "app.hpp"
#include "main.h"

#include "../Bases/bases.cpp"

extern ADC_HandleTypeDef hadc;
extern DMA_HandleTypeDef hdma_adc;

Base BASE1;
Base BASE2;
Base BASE5;
Base BASE05;

uint32_t adcteste;

int main()
{
	mainInit();
	app_init();

	while(true)
	{
		app_loop();
	}
}

void app_init()
{
	  HAL_ADC_Start_DMA(&hadc, &adcteste, 1);
	  (void)BASE1.start(1000);
	  (void)BASE2.start(2000);
	  (void)BASE5.start(5000);
	  (void)BASE05.start(500);
}

void adc_teste()
{
	  if(adcteste > 1000)
	  	  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
	  if(adcteste > 2000)
	  	  HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET);
	  if(adcteste > 3000)
	  	  HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_SET);
	  if(adcteste > 4000)
	  	  HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, GPIO_PIN_SET);

	  if(adcteste < 1000)
	  	  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
	  if(adcteste < 2000)
	  	  HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
	  if(adcteste < 3000)
	  	  HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET);
	  if(adcteste < 4000)
	  	  HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, GPIO_PIN_RESET);
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
	adc_teste();
}
