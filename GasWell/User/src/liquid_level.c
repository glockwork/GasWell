#include "liquid_level.h"
#include "stm32l1xx_conf.h"
#include "board.h"
#include <stdio.h>

liquid_level_state_e liquid_level_state;

/**
  * @brief  Configures the function of  detecting the liquid level.
  * @param  None
  * @retval None
  */
void LIQUID_LEVEL_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // GPIO_Pin_LIQUID_LEVEL_EN
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_LIQUID_LEVEL;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
    GPIO_Init(GPIO_Port_LIQUID_LEVEL, &GPIO_InitStructure);
    
    /* Selects the GPIO pin used as EXTI Line.*/
    SYSCFG_EXTILineConfig(EXTI_PortSource_LIQUID_LEVEL, EXTI_PinSource_LIQUID_LEVEL);
    
    /* Configure EXTI lines */
    EXTI_InitStructure.EXTI_Line    = EXTI_Line_LIQUID_LEVEL;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    /* Enable the EXTI9_5_IRQn Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                      = EXTI9_5_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Handle the interrupt of liquid level alarm.
  * @param  None
  * @retval None
  */
void LIQUID_LEVEL_Interrupt_Handler(void)
{
    printf("EXTI9_5_IRQHandler,liquid level alarm !!!\r\n");
}
