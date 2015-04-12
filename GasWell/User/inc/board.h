#ifndef __BOARD_H__
#define __BOARD_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h" 

#define DYNAMENT_DEBUG

/******************************* Define for Dynament ****************************/
#define DYNAMENT_USART                  USART2

// USART pins
#define GPIO_Port_DYNAMENT_USART        GPIOD
#define GPIO_Pin_DYNAMENT_TX            GPIO_Pin_5
#define GPIO_Pin_DYNAMENT_RX            GPIO_Pin_6

#define GPIO_Port_DYNAMENT_3V3_EN       GPIOB
#define GPIO_Pin_DYNAMENT_3V3_EN        GPIO_Pin_4

/******************************* Define for  ****************************/
#define PRINTF_USART                    USART3

// USART pins
#define GPIO_Port_PRINTF                GPIOD
#define GPIO_Pin_PRINTF_TX              GPIO_Pin_8

/******************************* Define for  ****************************/
#define GPIO_Port_9V_EN                 GPIOC
#define GPIO_Pin_9V_EN                  GPIO_Pin_1


#endif /* __BOARD_H__ */
