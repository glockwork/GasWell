#ifndef __BOARD_H__
#define __BOARD_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h" 

#define DYNAMENT_DEBUG

/******************************* Define for Dynament ****************************/
#define STM32_SUCCESS                   (0)
#define STM32_FAIL                      (-1)

/******************************* Define for Dynament ****************************/
#define DYNAMENT_USART                  USART2

// Dynament USART pins
#define GPIO_Port_DYNAMENT_USART        GPIOD
#define GPIO_Pin_DYNAMENT_TX            GPIO_Pin_5
#define GPIO_Pin_DYNAMENT_RX            GPIO_Pin_6

// Dynament Power pins
#define GPIO_Port_DYNAMENT_3V3_EN       GPIOB
#define GPIO_Pin_DYNAMENT_3V3_EN        GPIO_Pin_4

// Dynament ADC pins
#define GPIO_Port_ANALOG_IN             GPIOA
#define GPIO_Pin_ANALOG_IN              GPIO_Pin_1

/******************************* Define for  ****************************/
#define PRINTF_USART                    USART3

// USART pins
#define GPIO_Port_PRINTF                GPIOD
#define GPIO_Pin_PRINTF_TX              GPIO_Pin_8

/******************************* Define for  ****************************/
#define GPIO_Port_9V_EN                 GPIOC
#define GPIO_Pin_9V_EN                  GPIO_Pin_1


/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_ASSERT
/******************************************************************************
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
******************************************************************************/
  #define assert(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert(expr) ((void)0)
#endif /* USE_ASSERT */

#endif /* __BOARD_H__ */
