#include "stm32l1xx.h"
#include "usart.h"
#include <stdio.h>
#include "board.h"

/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Uasrt Config
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* Enable Dynament RX, TX clocks */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD , ENABLE);
    
    /* Enable DYNAMENT_USART RX, TX clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
    
    /* Enable DMA1 clock */
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    /* Changes the mapping of the PD5,PD6 pin */
    GPIO_PinAFConfig(GPIOD ,GPIO_PinSource5,GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD ,GPIO_PinSource6,GPIO_AF_USART2);

    /* Configure DYNAMENT_USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure); 
    
    /* Configure DYNAMENT_USART Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure); 
    
    /* DYNAMENT_USART mode config */
    USART_InitStructure.USART_BaudRate              = 38400;
    USART_InitStructure.USART_WordLength            = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits              = USART_StopBits_1;
    USART_InitStructure.USART_Parity                = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                  = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(DYNAMENT_USART, &USART_InitStructure); 

    USART_ITConfig(DYNAMENT_USART, USART_IT_RXNE, ENABLE);
    
//    /* De-initialise  DMA */
//    DMA_DeInit(DMA1_Channel6);

//    /* DMA1 channel1 configuration */
//    DMA_StructInit(&DMA_InitStructure);
//    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);	        // Set DMA channel Peripheral base address to ADC Data register
//    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&USART_RX_Buffer;          // Set DMA channel Memeory base addr to ADC_ConvertedValueBuff address
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          // Set DMA channel direction to peripheral to memory
//    DMA_InitStructure.DMA_BufferSize = USART2_RX_MAX_LEN;                       // Set DMA channel buffersize to peripheral to ADC_CONV_BUFF_SIZE
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	        // Disable DMA channel Peripheral address auto increment
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     // Enable Memeory increment (To be verified ....)
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // set Peripheral data size to 8bit 
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	        // set Memeory data size to 8bit 
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                               // Set DMA in normal mode
//    DMA_InitStructure.DMA_Priority = DMA_Priority_High;	                        // Set DMA channel priority to High
//    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                // Disable memory to memory option 
//    DMA_Init(DMA1_Channel6, &DMA_InitStructure);                                // Use Init structure to initialise channel1 (channel linked to ADC)
//   
//    /* Enable Transmit Complete Interrup for DMA channel 1 */ 
//    DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);
//    
//    DMA_Cmd(DMA1_Channel6, ENABLE);

//    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
//    
    /* Enable DYNAMENT_USART */
    USART_Cmd(DYNAMENT_USART, ENABLE);
}

/*******************************************************************************
* Function Name  : USART3_Configuration
* Description    : Uasrt3 Config
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART_Configuration(void)
{
    /* Enable USART3 for dynament's sensor */
    USART2_Configuration();
}


/*******************************************************************************
* @brief   Send data through Usart2
* @param   str:  the buffer to be transmitted
* @param   len:  the size to be transmitted
* @retval  void
*******************************************************************************/
void Usart2_SendData(uint8_t *str, int16_t len)
{
    uint8_t i=0;

    if((str != NULL) && (len > 0))
    {
        while(i <= len)
        {
            USART_SendData(USART2,*(str + i));
            i ++;
            while (!(USART2->SR & USART_FLAG_TXE));
        }
    }
}
