/**
  ******************************************************************************
  * @file    Project/STM32L1xx_StdPeriph_Templates/stm32l1xx_it.c 
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    13-April-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_it.h"
#include <stdio.h>
#include "dynament.h"
#include "dynamentMonitor.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32L1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l1xx_xx.s).                                            */
/******************************************************************************/
void EXTI0_IRQHandler(void)
{

}

/**
  * @brief  This function handles DMA1_Channel1 exception.
  * @param  None
  * @retval None
  */
void DMA1_Channel1_IRQHandler(void)
{
    #ifdef DYNAMENT_DEBUG
    printf("DMA1_Channel1_IRQHandler.\r\n");
    #endif
    DMA_ClearFlag(DMA1_IT_TC1);
    SetADCDMA_TransferComplete();
}

void DMA1_Channel6_IRQHandler(void)
{
//    printf("DMA1_Channel6_IRQHandler\r\n");
//    DMA_ClearFlag(DMA1_IT_TC6);
}


void EXTI15_10_IRQHandler(void)
{

}
/******************************************************************
*                        TIM2_IRQHandler                           *
*                        Timer 2 IRQ Handler                       * 
******************************************************************/
void TIM2_IRQHandler(void)						  //apply to every timer interrupt ISR
{	

}
/******************************************************************
*                        TIM3_IRQHandler                           *
*                        Timer 3 IRQ Handler                       * 
******************************************************************/
void TIM3_IRQHandler(void)						  //apply to every timer interrupt ISR
{

}
/******************************************************************
*                        TIM4_IRQHandler                           *
*                        Timer 4 IRQ Handler                       * 
******************************************************************/
void TIM4_IRQHandler(void)						  //apply to every timer interrupt ISR
{

}
/******************************************************************
*                        TIM5_IRQHandler                           *
*                        Timer 5 IRQ Handler                       * 
******************************************************************/
void TIM5_IRQHandler(void)						  //apply to every timer interrupt ISR
{

}
/******************************************************************
*                        TIM6_IRQHandler                           *
*                        Timer 6 IRQ Handler                       * 
******************************************************************/
void TIM6_IRQHandler(void)						  //apply to every timer interrupt ISR
{

}
/******************************************************************
*                        TIM7_IRQHandler                           *
*                        Timer 7 IRQ Handler                       * 
******************************************************************/
void TIM7_IRQHandler(void)						  //apply to every timer interrupt ISR
{

}
/******************************************************************
*                        TIM9_IRQHandler                           *
*                        Timer 9 IRQ Handler                       * 
******************************************************************/
void TIM9_IRQHandler(void)						  //apply to every timer interrupt ISR
{

}
/******************************************************************
*                        TIM10_IRQHandler                           *
*                        Timer 10 IRQ Handler                       * 
******************************************************************/
void TIM10_IRQHandler(void)						  //apply to every timer interrupt ISR
{

}
/******************************************************************
*                        TIM11_IRQHandler                           *
*                        Timer 11 IRQ Handler                       * 
******************************************************************/
void TIM11_IRQHandler(void)						  //apply to every timer interrupt ISR
{

}
void RTC_WKUP_IRQHandler(void)
{

}
void EXTI9_5_IRQHandler(void)
{
	#ifdef BOARD_REDHARE_V3_0
		
		if(EXTI_GetITStatus(PT_EXTI_LineBTSYNC) != RESET) //PC5 51822通知STM32发送数据
		{
			 /* Clear the EXTI line10 pending bit */
			EXTI_ClearITPendingBit(PT_EXTI_LineBTSYNC);
			gWakeUpNeedOLEDON = SET;
			TS_SendEvent(gTsSyncDataTaskID_c,gSyncDataEventSPISent);   //触发SPI发送数据事件
		}
	#endif
}

void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
         /* Clear the EXTI line10 pending bit */
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        
        dynament_usart_rx_buffer[dynament_usart_rx_index] = USART_ReceiveData(USART2);
        if(dynament_usart_rx_index)
        {
            if(dynament_usart_rx_buffer[dynament_usart_rx_index - 1] == DYNAMENT_DLE)
            {
                if(dynament_usart_rx_buffer[dynament_usart_rx_index] == DYNAMENT_DAT)
                {
                    dynament_usart_rx_index = 1;
                }
                else if(dynament_usart_rx_buffer[dynament_usart_rx_index] == DYNAMENT_DLE)
                {
                    dynament_usart_rx_index --;
                }
            }
        }
        dynament_usart_rx_index ++;
        
        if(dynament_usart_rx_index >= (dynament_usart_rx_buffer[2] + 7))
        {
            dynament_rx.pu8Data = dynament_usart_rx_buffer;
            dynament_rx.len = dynament_usart_rx_buffer[2] + 7;
            dynament_usart_rx_flag = 1;
            dynament_usart_rx_index = 0;
            printf("dynament_usart_rx_flag = 1\r\n");
        }
        else if(dynament_usart_rx_index >= DYNAMENT_USART_RX_MAX_LEN)
        {
            dynament_usart_rx_index = 0;
        }
    }
}

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
