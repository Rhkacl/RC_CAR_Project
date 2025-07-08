/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "motor.h"
#include "Ultrasonic.h"
#include "buzzer.h"
#include "led.h"
#include "button.h"
#include "usart.h"
#include "delay_us.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

extern uint8_t txData;

uint16_t IC_Value1 = 0;
uint16_t IC_Value2 = 0;
uint16_t echoTime1 = 0;
uint8_t captureFlag1 = 0;
uint8_t distance1 = 0;

uint16_t IC_Value3 = 0;
uint16_t IC_Value4 = 0;
uint16_t echoTime2 = 0;
uint8_t captureFlag2 = 0;
uint8_t distance2 = 0;

uint16_t IC_Value5 = 0;
uint16_t IC_Value6 = 0;
uint16_t echoTime3 = 0;
uint8_t captureFlag3 = 0;
uint8_t distance3 = 0;

uint32_t buzzer_beep_time = 0;
uint8_t buzzer_state = 0; // 0: off, 1: on
uint8_t current_beep_interval = 0;
uint8_t current_beep_frequency = 0;

uint8_t mode = 10;

static uint16_t speed = 500;  // 모터 변수

uint8_t led1_State = 0;
uint8_t led2_State = 0;
uint8_t led1_Blink = 0;
uint8_t led2_Blink = 0;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART1)
    {
      switch(txData)
      {
        case 'w' :
        {
          mode = 0;
          break;
        }

        case 's' :
          {
            mode = 1;
            break;
          }

        case 'a' :
          {
            mode = 2;
            break;
          }

        case 'd' :
          {
            mode = 3;
            break;
          }

        case 't' :
          {
            mode = 4;
            //speed += 100; SetSpeed(speed);
            break;
          }

        case 'x' :
          {
            mode = 5;
//            speed = (speed >= 100) ? speed - 100 : 0;
//            SetSpeed(speed);
            break;
          }

        case 'q' :
        {
          mode = 6;
          break;
        }

        default: break;
      }
      // 다음 문자 수신 대기
      HAL_UART_Receive_IT(&huart1, &txData, sizeof(txData));
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
  {
    if(captureFlag1 == 0)  // 아직 캡처를 안했다면
    {
      IC_Value1 = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_1);
      captureFlag1 = 1;    // 캡처 했음
      // 캡처에 대한 극성을 rising 에서 falling 으로 바꿈
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
    }
    else if(captureFlag1 == 1)   // 캡처를 했다면
    {
      IC_Value2 = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_1);
      //__HAL_TIM_SET_COUNTER(&htim4, 0);

      // echo 시간 계산
      if(IC_Value2 > IC_Value1)
      {
        echoTime1 = IC_Value2 - IC_Value1;
      }
      else if(IC_Value1 > IC_Value2)
      {
        echoTime1 = (0xffff - IC_Value1) + IC_Value2;
      }
      // 거리 계산
      distance1 = echoTime1 / 58;

      // 거리 값 검증
      if(distance1 < 2 || distance1 > 400)
      {
        distance1 = 0;  // 오류 값
      }

      captureFlag1 = 0;
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
      __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_CC1);
    }
  }

  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
      if(captureFlag2 == 0)  // 아직 캡처를 안했다면
      {
        IC_Value3 = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_2);
        captureFlag2 = 1;    // 캡처 했음
        // 캡처에 대한 극성을 rising 에서 falling 으로 바꿈
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
      }
      else if(captureFlag2 == 1)   // 캡처를 했다면
      {
        IC_Value4 = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_2);
        //__HAL_TIM_SET_COUNTER(&htim4, 0);

        // echo 시간 계산
        if(IC_Value4 > IC_Value3)
        {
          echoTime2 = IC_Value4 - IC_Value3;
        }
        else if(IC_Value3 > IC_Value4)
        {
          echoTime2 = (0xffff - IC_Value3) + IC_Value4;
        }

        distance2 = echoTime2 / 58;

        if(distance2 < 2 || distance2 > 400)
        {
          distance2 = 0;
        }

        captureFlag2 = 0;
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
        __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_CC2);
      }
    }

  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
      if(captureFlag3 == 0)  // 아직 캡처를 안했다면
      {
        IC_Value5 = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_3);
        captureFlag3 = 1;    // 캡처 했음
        // 캡처에 대한 극성을 rising 에서 falling 으로 바꿈
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
      }
      else if(captureFlag3 == 1)   // 캡처를 했다면
      {
        IC_Value6 = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_3);
        //__HAL_TIM_SET_COUNTER(&htim4, 0);

        // echo 시간 계산
        if(IC_Value6 > IC_Value5)
        {
          echoTime3 = IC_Value6 - IC_Value5;
        }
        else if(IC_Value5 > IC_Value6)
        {
          echoTime3 = (0xffff - IC_Value5) + IC_Value6;
        }

        distance3 = echoTime3 / 58;

        if(distance3 < 2 || distance3 > 400)
        {
          distance3 = 0;
        }

        captureFlag3 = 0;
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
        __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_CC3);
      }
    }

}

/* USER CODE END Variables */
/* Definitions for Motor_Task1 */
osThreadId_t Motor_Task1Handle;
const osThreadAttr_t Motor_Task1_attributes = {
  .name = "Motor_Task1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ultrasonic_Task */
osThreadId_t ultrasonic_TaskHandle;
const osThreadAttr_t ultrasonic_Task_attributes = {
  .name = "ultrasonic_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for BUZZER_Task1 */
osThreadId_t BUZZER_Task1Handle;
const osThreadAttr_t BUZZER_Task1_attributes = {
  .name = "BUZZER_Task1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Signal_Task01 */
osThreadId_t Signal_Task01Handle;
const osThreadAttr_t Signal_Task01_attributes = {
  .name = "Signal_Task01",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Motor_Task01(void *argument);
void Ultrasonic_Task(void *argument);
void Buzzer_Task01(void *argument);
void signal_task01(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Motor_Task1 */
  Motor_Task1Handle = osThreadNew(Motor_Task01, NULL, &Motor_Task1_attributes);

  /* creation of ultrasonic_Task */
  ultrasonic_TaskHandle = osThreadNew(Ultrasonic_Task, NULL, &ultrasonic_Task_attributes);

  /* creation of BUZZER_Task1 */
  BUZZER_Task1Handle = osThreadNew(Buzzer_Task01, NULL, &BUZZER_Task1_attributes);

  /* creation of Signal_Task01 */
  Signal_Task01Handle = osThreadNew(signal_task01, NULL, &Signal_Task01_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Motor_Task01 */
/**
  * @brief  Function implementing the Motor_Task1 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Motor_Task01 */
void Motor_Task01(void *argument)
{
  /* USER CODE BEGIN Motor_Task01 */
  /* Infinite loop */
  for(;;)
  {
    if(mode == 0)
    {
      Forward();
      mode = 10;
    }

    if(mode == 1)
    {
      Backward();
      mode = 10;
    }

    if(mode == 2)
    {
      Left();
      mode = 10;
    }

    if(mode == 3)
    {
      Right();
      mode = 10;
    }

    if(mode == 4)
    {
      speed += 100; SetSpeed(speed);
      mode = 10;
    }

    if(mode == 5)
    {
      speed = (speed >= 100) ? speed - 100 : 0;
      SetSpeed(speed);
      mode = 10;
    }

    if(mode == 6)
    {
      Stop();
      mode = 10;
    }

    osDelay(1);
  }
  /* USER CODE END Motor_Task01 */
}

/* USER CODE BEGIN Header_Ultrasonic_Task */
/**
* @brief Function implementing the ultrasonic_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Ultrasonic_Task */
void Ultrasonic_Task(void *argument)
{
  /* USER CODE BEGIN Ultrasonic_Task */
  /* Infinite loop */
  for(;;)
  {
    HCSR04_TRIG();
    delay_us(1000);
    delay_us(2000);
    //printf("Sensor1 : %d cm, Sensor2 : %d cm, Sensor3 : %d cm\r\n", distance1, distance2, distance3);
    //delay_us(100);
    osDelay(1);
  }
  /* USER CODE END Ultrasonic_Task */
}

/* USER CODE BEGIN Header_Buzzer_Task01 */
/**
* @brief Function implementing the BUZZER_Task1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Buzzer_Task01 */
void Buzzer_Task01(void *argument)
{
  /* USER CODE BEGIN Buzzer_Task01 */
  /* Infinite loop */
  for(;;)
  {
    // 가장 가까운 거리 찾기
    uint8_t current_distance = distance1;

    // 유효한 거리 값이 있을 때만 부저 제어
    if(current_distance > 0 && current_distance <= DISTANCE_FAR)
    {
      uint16_t beep_interval = Calculate_Beep_Interval(current_distance);
      uint16_t beep_frequency = Calculate_Beep_Frequency(current_distance);

      uint32_t current_time = HAL_GetTick();

      // 부저 상태 관리
      if(buzzer_state == 0)
      {
        if(current_time - buzzer_beep_time >= beep_interval)
        {
          // 부저 켜기
          Buzzer_On(beep_frequency);
          buzzer_state = 1;
          buzzer_beep_time = current_time;

          // 거리에 따른 비프 지속시간 설정
          if(current_distance <= DISTANCE_CRITICAL)
          {
            current_beep_interval = BEEP_DURATION_LONG;
          }
          else
          {
            current_beep_interval = BEEP_DURATION_SHORT;
          }
        }
      }
      // 부저가 켜져 있는 상태
      else
      {
        if(current_time - buzzer_beep_time >= current_beep_interval)
        {
          Buzzer_Off();
          buzzer_state = 0;
          buzzer_beep_time = current_time;
        }
      }
    }
    else
    {
      // 너무 멀 때 부저 끄기
      Buzzer_Off();
      buzzer_state = 0;
    }

    printf("Distance : %d cm, Beep interval : %d ms\r\n", current_distance, Calculate_Beep_Interval(current_distance));
    osDelay(10);
  }
  /* USER CODE END Buzzer_Task01 */
}

/* USER CODE BEGIN Header_signal_task01 */
/**
* @brief Function implementing the Signal_Task01 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_signal_task01 */
void signal_task01(void *argument)
{
	uint8_t prev_btn1 = 0, prev_btn2 = 0;
	uint32_t last_blink_time1 = 0, last_blink_time2 = 0;
  /* USER CODE BEGIN signal_task01 */
  /* Infinite loop */
  for(;;)
  {
	  //uint32_t now = HAL_GetTick();
	  uint8_t btn1 = Read_BTN1();	// 0 : 눌림, 1 : 안눌림
	  uint8_t btn2 = Read_BTN2();

	  if(btn1 == 0 && prev_btn1 == 1)	// 버튼 1이 새로 눌림
	  {
		  led1_Blink ^= 1;	// 깜빡임 ON/OFF 토글

		  if(led1_Blink == 0)
		  {
			  // 깜빡이 정지 시 LED OFF
			  LED1_OFF();
			  led1_State = 0;
		  }
	  }

	  if(btn2 == 0 && prev_btn2 == 1)
	  {
		  led2_Blink ^= 1;

		  if(led2_Blink == 0)
		  {
			  // 깜빡이 정지 시 LED OFF
			  LED2_OFF();
			  led2_State = 0;
		  }
	  }

	  // LED1 제어
	  if(led1_Blink)
	  {
		  if(HAL_GetTick() - last_blink_time1 >= 500)
		  {
			  led1_State ^= 1;
			  if(led1_State)
				  LED1_ON();
			  else
				  LED1_OFF();

			  last_blink_time1 = HAL_GetTick();
		  }
	  }

	  // LED2 제어
	  if(led2_Blink)
	  {
		  if(HAL_GetTick() - last_blink_time2 >= 500)
		  {
			  led2_State ^= 1;
			  if(led2_State)
				  LED2_ON();
			  else
				  LED2_OFF();

			  last_blink_time2 = HAL_GetTick();
		  }
	  }
	  prev_btn1 = btn1;
	  prev_btn2 = btn2;

    osDelay(50);
  }
  /* USER CODE END signal_task01 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

