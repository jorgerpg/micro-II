#include "app.hpp"
#include "main.h"

#include "../Bases/bases.cpp"

Base BASE1;
Base BASE2;
Base BASE5;
Base BASE10;

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
	  (void)BASE1.start(1000);
	  (void)BASE2.start(2000);
	  (void)BASE5.start(5000);
	  (void)BASE10.start(10000);
}

void app_loop()
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

    if(BASE10.get()){
	  HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
	  (void)BASE10.restart();
    }
}
