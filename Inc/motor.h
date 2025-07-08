

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "main.h"

void MotorInit();
void Forward();
void Backward();
void Stop();
void Left();
void Right();
void SetSpeed(uint16_t speed);  // 모터 속도 설정 + PWM 업데이트
uint16_t GetSpeed();  // 현재 모터 속도
void UpdatePWM(); // CCR1,2에 적용

#endif /* INC_MOTOR_H_ */
