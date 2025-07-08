#ifndef BUZZER_H
#define BUZZER_H

#include "main.h"
#include "tim.h"


// 부저 주파수 정의
#define BUZZER_FREQ   		2000

// 거리 범위 정의
#define DISTANCE_FAR        40
#define DISTANCE_MID        30
#define DISTANCE_NEAR       20
#define DISTANCE_VNEAR      10
#define DISTANCE_CRITICAL   5

// 부저 간격 정의 (ms)
#define BEEP_INTERVAL_FAR        2000  // 2초 간격
#define BEEP_INTERVAL_MID        1000  // 1초 간격
#define BEEP_INTERVAL_NEAR       500   // 0.5초 간격
#define BEEP_INTERVAL_VNEAR      200   // 0.2초 간격
#define BEEP_INTERVAL_CRITICAL   100   // 0.1초 간격 (연속음)

// 부저 지속시간 정의 (ms)
#define BEEP_DURATION_SHORT  100
#define BEEP_DURATION_LONG   200

// 함수 선언
void Buzzer_Init(void);
void Buzzer_On(uint16_t frequency);
void Buzzer_Off(void);
void Buzzer_Beep(uint16_t frequency, uint16_t duration);
uint16_t Calculate_Beep_Interval(uint8_t distance);
uint16_t Calculate_Beep_Frequency(uint8_t distance);

#endif


