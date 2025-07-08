
#include "button.h"

uint8_t Read_BTN1()
{
	return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5);
}

uint8_t Read_BTN2()
{
	return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);
}


