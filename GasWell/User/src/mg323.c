#include "mg323.h"
#include "include.h"
#include "stm32l1xx_gpio.h"

/*******************************************************************************
* @brief   Init MG323 module
* @param   void
* @retval  void
*******************************************************************************/
void MG323_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable the GPIOs Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); 
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE); 
    
    // MG323 Hardware Reset pin Enable
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_MG323_RESET;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_40MHz;
    GPIO_Init(GPIO_Port_MG323_RESET, &GPIO_InitStructure);
    
}

/*******************************************************************************
* @brief   Reset MG323 module
* @param   void
* @retval  void
*******************************************************************************/
void ResetMG323(void)
{
    GPIO_SetBits(GPIO_Port_MG323_RESET, GPIO_Pin_MG323_RESET);
    Delay_us(100);
    GPIO_ResetBits(GPIO_Port_MG323_RESET, GPIO_Pin_MG323_RESET);
}
