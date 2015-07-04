#include "usr_trace.h"
#include "stm32l1xx.h"
#include <stdio.h>
#include "board.h"

void usr_trace_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    
    /* Enable USART3 port clocks */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD , ENABLE);
    
    /* Enable PRINTF_USART TX clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    /* Changes the mapping of the PD8 pin */
    GPIO_PinAFConfig(GPIOD ,GPIO_PinSource8,GPIO_AF_USART3);

    /* Configure PRINTF_USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_PRINTF_TX;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
    GPIO_Init(GPIO_Port_PRINTF, &GPIO_InitStructure); 

    /* PRINTF_USART mode config */
    USART_InitStructure.USART_BaudRate              = 115200;
    USART_InitStructure.USART_WordLength            = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits              = USART_StopBits_1;
    USART_InitStructure.USART_Parity                = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                  = USART_Mode_Tx;
    USART_Init(PRINTF_USART, &USART_InitStructure); 
    
    /* Enable USART3 */
    USART_Cmd(PRINTF_USART, ENABLE);
}

/*******************************************************************************
* @brief   Mapping to printf
* @param   ch:  the char to be transmitted
* @param   f:  
* @retval  the char to be transmitted
*******************************************************************************/
int fputc(int ch, FILE *f)
{
	uint32_t  TimeCnt=0;
	
	USART_SendData(PRINTF_USART, (unsigned char) ch);
	TimeCnt = 16000;
	while ((!(USART3->SR & USART_FLAG_TXE)) && (TimeCnt != 0))
	{
		TimeCnt--;
	}
    return (ch);
}
