#include "app.hpp"
#include "main.h"
#include "drv_pit.hpp"

DrvPIT BASE1;
DrvPIT BASE2;
DrvPIT BASE5;
DrvPIT BASE10;

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
	  (void)BASE1.set(1000, MiliSec);
	  (void)BASE1.setAutoReload(true);
	  (void)BASE2.set(2000, MiliSec);
	  (void)BASE2.setAutoReload(true);
	  (void)BASE5.set(5000, MiliSec);
	  (void)BASE5.setAutoReload(true);
	  (void)BASE10.set(10000, MiliSec);
	  (void)BASE10.setAutoReload(true);
}

void app_loop()
{
    if(BASE1.get() == true){
      HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
    }

    if(BASE2.get() == true){
      HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    }

    if(BASE5.get() == true){
	  HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
    }

    if(BASE10.get() == true){
	  HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
    }
}
