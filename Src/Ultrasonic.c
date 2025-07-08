

#include "Ultrasonic.h"
#include "tim.h"
#include "delay_us.h"


#define TRIG_PORT GPIOA
#define TRIG1_PIN  GPIO_PIN_5
#define TRIG2_PIN  GPIO_PIN_6
#define TRIG3_PIN  GPIO_PIN_7




// 초음파 센서 초기화
void Ultrasonic_Init()
{

}

void HCSR04_TRIG()
{
  HAL_GPIO_WritePin(TRIG_PORT, TRIG1_PIN, GPIO_PIN_SET);
  delay_us(10);
  HAL_GPIO_WritePin(TRIG_PORT, TRIG1_PIN, GPIO_PIN_RESET);

  __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC1);

  delay_us(1000);

  HAL_GPIO_WritePin(TRIG_PORT, TRIG2_PIN, GPIO_PIN_SET);
  delay_us(10);
  HAL_GPIO_WritePin(TRIG_PORT, TRIG2_PIN, GPIO_PIN_RESET);

  __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC2);

  delay_us(1000);

  HAL_GPIO_WritePin(TRIG_PORT, TRIG3_PIN, GPIO_PIN_SET);
  delay_us(10);
  HAL_GPIO_WritePin(TRIG_PORT, TRIG3_PIN, GPIO_PIN_RESET);

  __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC3);
}


// 거리 측정 함수 (cm)
//uint32_t Ultrasonic_Read()
//{
//  HCSR04_TRIG();
//  HAL_Delay(50);
//  return distance;
//}






















