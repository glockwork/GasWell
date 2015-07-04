/**
  ******************************************************************************
  * @file    main.c
  * @author  
  * @version V1.0
  * @date    2014-01-23
  * @brief   主函数
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32l1xx.h"
#include "board.h"
#include "delay.h"
#include "usart.h"
#include "liquid_level.h"
#include "dynament.h"
#include "dynamentMonitor.h"

/* Private function prototypes -----------------------------------------------*/
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable the GPIOs Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);  
    
    /* Enable the GPIOs Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_40MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_40MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_40MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_40MHz;
    GPIO_Init( GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_4|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_40MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure);
    
    // GPIO_Pin_9V_EN
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_9V_EN;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_40MHz;
    GPIO_Init(GPIO_Port_9V_EN, &GPIO_InitStructure);
    
    // GPIO_Pin_DYNAMENT_3V3_EN
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_DYNAMENT_3V3_EN;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_40MHz;
    GPIO_Init(GPIO_Port_DYNAMENT_3V3_EN, &GPIO_InitStructure);
    
    // LED pin ENABLE
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_LED1;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_40MHz;
    GPIO_Init(GPIO_Port_LED1, &GPIO_InitStructure);

    LIQUID_LEVEL_Init();
}
/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;

    /* 2 bit for pre-emption priority, 2 bits for subpriority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 

//    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

    /* Enable the Usart2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                      = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);    
}

//void MCO_OutPut(void)
//{
//    GPIO_InitTypeDef        GPIO_InitStructure;

//    /* GPIOA Periph clock enable */
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

//    /* Configure PA8 in MCO output mode */
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);

//    GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_MCO);
//    RCC_MCOConfig(RCC_MCOSource_SYSCLK,RCC_MCODiv_1);  
//}
/****************************************************************************
* 名    称：int main(void)
* 功    能：主函数
* 入口参数：无
* 出口参数：无
* 说    明： 
* 调用方法：不需要调用
* 创建时间：2014-01-23
****************************************************************************/
int main(void)
{	
    GPIO_Configuration();
    USART_Configuration();
    NVIC_Configuration();
    Delay_ms(10);
    printf("Hello.\r\n");
    
    GPIO_ResetBits(GPIOC, GPIO_Pin_4);
    
    /* Enable the power of dynament sensor. */
    GPIO_ResetBits(GPIO_Port_9V_EN, GPIO_Pin_9V_EN);
    GPIO_SetBits(GPIO_Port_DYNAMENT_3V3_EN, GPIO_Pin_DYNAMENT_3V3_EN);
    
    /* Enable the power of NP55 sensor. */
    GPIO_SetBits(GPIO_Port_NAP55_3V3_EN, GPIO_Pin_NAP55_3V3_EN); 
    GPIO_SetBits(GPIOB, GPIO_Pin_10);
    for(int i = 0;i < 100;i ++)
    {
        printf("%d\r\n",i);
        Delay_ms(1000);
    }

    dynament_usart_rx_index = 0;
    while(1)
	{
//        DynamentVoltDetect();
        GetLiveData();
        Delay_ms(500);
	}
}


#ifdef USE_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    
    printf("Wrong parameters value: file %s on line %d\r\n", file, line) ;
    
    /* Infinite loop */
    while (1)
    {
    }
}
#endif

#ifdef APP_USE_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void app_assert(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    printf("Wrong parameters value: file %s on line %d\r\n", file, line) ;
}
#endif
