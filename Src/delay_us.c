
#include "delay_us.h"



void delay_us(uint16_t us)
{
  // 호출하면 0부터 시작
  __HAL_TIM_SET_COUNTER(&htim11, 0);    // __ 이거 붙은 애들은 함수가 아닌 매크로
  while((__HAL_TIM_GET_COUNTER(&htim11)) < us);
}
