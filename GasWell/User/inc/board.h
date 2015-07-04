#ifndef __BOARD_H__
#define __BOARD_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h" 

#define DYNAMENT_DEBUG

/******************************* Define for Dynament ****************************/
#define STM32_SUCCESS                           (0)
#define STM32_FAIL                              (-1)

/******************************* Define for Dynament ****************************/
#define DYNAMENT_USART                          USART2

// Dynament USART pins
#define GPIO_Port_DYNAMENT_USART                GPIOD
#define GPIO_Pin_DYNAMENT_TX                    GPIO_Pin_5
#define GPIO_Pin_DYNAMENT_RX                    GPIO_Pin_6

// Dynament Power pins
#define GPIO_Port_DYNAMENT_3V3_EN               GPIOB
#define GPIO_Pin_DYNAMENT_3V3_EN                GPIO_Pin_4

// Dynament ADC pins
#define GPIO_Port_ANALOG_IN                     GPIOA
#define GPIO_Pin_ANALOG_IN                      GPIO_Pin_3
#define GPIO_Port_NAP55_3V3_EN                  GPIOD
#define GPIO_Pin_NAP55_3V3_EN                   GPIO_Pin_3
#define ADC_Channel_NAP55_AD_IN                 ADC_Channel_3
#define ADC_Channel_INTERNAL_REF                ADC_Channel_17

// LED1 pin
#define GPIO_Port_LED1                          GPIOC
#define GPIO_Pin_LED1                           GPIO_Pin_4


// LED S1_ENABLE pins
#define GPIO_Port_LED_S1_ENABLE                 GPIOE
#define GPIO_Pin_LED_S1_ENABLE                  GPIO_Pin_2

// LED S2_ENABLE pins
#define GPIO_Port_LED_S2_ENABLE                 GPIOE
#define GPIO_Pin_LED_S2_ENABLE                  GPIO_Pin_3

// LED S3_ENABLE pins
#define GPIO_Port_LED_S3_ENABLE                 GPIOE
#define GPIO_Pin_LED_S3_ENABLE                  GPIO_Pin_4

// LED S4_ENABLE pins
#define GPIO_Port_LED_S4_ENABLE                 GPIOE
#define GPIO_Pin_LED_S4_ENABLE                  GPIO_Pin_5

// LED S4_ENABLE pins
#define GPIO_Port_LED_S4_ENABLE                 GPIOE
#define GPIO_Pin_LED_S4_ENABLE                  GPIO_Pin_5

// LED S4_ENABLE pins
#define GPIO_Port_LED_S4_ENABLE                 GPIOE
#define GPIO_Pin_LED_S4_ENABLE                  GPIO_Pin_5

// LED S4_ENABLE pins
#define GPIO_Port_LED_S4_ENABLE                 GPIOE
#define GPIO_Pin_LED_S4_ENABLE                  GPIO_Pin_5

// LED S4_ENABLE pins
#define GPIO_Port_LED_S4_ENABLE                 GPIOE
#define GPIO_Pin_LED_S4_ENABLE                  GPIO_Pin_5

// LED S4_ENABLE pins
#define GPIO_Port_LED_S4_ENABLE                 GPIOE
#define GPIO_Pin_LED_S4_ENABLE                  GPIO_Pin_5

// Liquid level pins
#define GPIO_Port_LIQUID_LEVEL              GPIOB
#define GPIO_Pin_LIQUID_LEVEL               GPIO_Pin_5
#define EXTI_PortSource_LIQUID_LEVEL        EXTI_PortSourceGPIOB
#define EXTI_PinSource_LIQUID_LEVEL         GPIO_PinSource5
#define EXTI_Line_LIQUID_LEVEL              EXTI_Line5

/******************************* Define for PRINTF****************************/
#define PRINTF_USART                    USART3

// USART pins
#define GPIO_Port_PRINTF                GPIOD
#define GPIO_Pin_PRINTF_TX              GPIO_Pin_8

/******************************* Define for  ****************************/
#define GPIO_Port_9V_EN                 GPIOC
#define GPIO_Pin_9V_EN                  GPIO_Pin_1

/**************************** Define for MG323 Module ***********************/
#define GPIO_Port_MG323_RESET           GPIOD
#define GPIO_Pin_MG323_RESET            GPIO_Pin_14




/* Exported macro ------------------------------------------------------------*/
#ifdef  APP_USE_ASSERT
/******************************************************************************
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
******************************************************************************/
  #define APP_ASSERT(expr) ((expr) ? (void)0 : app_assert((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void app_assert(uint8_t* file, uint32_t line);
#else
  #define APP_ASSERT(expr) ((void)0)
#endif /* APP_ASSERT */

#endif /* __BOARD_H__ */
