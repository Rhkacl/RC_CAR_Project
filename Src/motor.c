
#include "motor.h"

static uint16_t motorSpeed = 500; // 기본 속도


void MotorInit()
{

}

// 모터 속도 설정 + PWM 업데이트
void SetSpeed(uint16_t speed)
{
  if(speed > 1000) speed = 1000;
  motorSpeed = speed;
  UpdatePWM();
}

// 현재 모터 속도
uint16_t GetSpeed()
{
  return motorSpeed;
}

// CCR1,2에 적용
void UpdatePWM()
{
  TIM3->CCR1 = motorSpeed;
  TIM3->CCR2 = motorSpeed;
}

void Forward()
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);     // IN1
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);  // IN2
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);    // IN3
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);  // IN4

  UpdatePWM();
}

void Backward()
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);   // IN1
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);    // IN2
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);  // IN3
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);    // IN4

  UpdatePWM();
}

void Stop()
{
  TIM3->CCR1 = 0;
  TIM3->CCR2 = 0;
}

void Left()
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);   // IN1
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);    // IN2
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);    // IN3
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);  // IN4

  UpdatePWM();
}

void Right()
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);     // IN1
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);  // IN2
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);  // IN3
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);    // IN4

  UpdatePWM();
}


