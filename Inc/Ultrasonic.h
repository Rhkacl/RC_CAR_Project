

#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "main.h"

void Ultrasonic_Init();     // 초음파 센서 초기화
void HCSR04_TRIG();
uint32_t Ultrasonic_Read(); // 거리 측정 함수 (cm)




#endif /* INC_ULTRASONIC_H_ */
