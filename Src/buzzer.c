
#include "buzzer.h"


void Buzzer_Init(void)
{
  Buzzer_Off();
}

void Buzzer_On(uint16_t frequency)
{
  uint32_t timer_freq = 25000;  // 25khz

  // 주파수 범위 제한
  if(frequency < 25) frequency = 25;
  if(frequency > 12500) frequency = 12500;

  uint32_t arr_Value = (timer_freq / frequency) - 1;

  // ARR 값 범위 제한
  if(arr_Value < 1) arr_Value = 1;
  if(arr_Value > 65535) arr_Value = 65535;

  // 타이머 설정
  __HAL_TIM_SET_AUTORELOAD(&htim2, arr_Value);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, arr_Value / 2);  // 50% duty

  // counter reset
  __HAL_TIM_SET_COUNTER(&htim2, 0);
}

void Buzzer_Off(void)
{
  // duty -> 0
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
}

void Buzzer_Beep(uint16_t frequency, uint16_t duration)
{
  Buzzer_On(frequency);
  HAL_Delay(duration);
  Buzzer_Off();
}

// 거리에 따른 BEEP 간격 계산
uint16_t Calculate_Beep_Interval(uint8_t distance)
{
  if(distance >= DISTANCE_FAR)
  {
    return BEEP_INTERVAL_FAR;
  }
  else if(distance >= DISTANCE_MID)
  {
    return BEEP_INTERVAL_MID;
  }
  else if(distance >= DISTANCE_NEAR)
  {
    return BEEP_INTERVAL_NEAR;
  }
  else if(distance >= DISTANCE_VNEAR)
  {
    return BEEP_INTERVAL_VNEAR;
  }
  else if(distance >= DISTANCE_CRITICAL)
  {
    return BEEP_INTERVAL_CRITICAL;
  }
  else
  {
    return 50;  // 매우 위험
  }
}

// 거리에 따른 BEEP 주파수 계산
uint16_t Calculate_Beep_Frequency(uint8_t distance)
{
  if(distance >= DISTANCE_FAR)
  {
    return BUZZER_FREQ;
  }
  else if(distance >= DISTANCE_MID)
  {
    return BUZZER_FREQ;
  }
  else
  {
    return BUZZER_FREQ;
  }
}





















