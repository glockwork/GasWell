/**
  ******************************************************************************
  * @file    main.c
  * @author  冉景海
  * @version V1.0
  * @date    2014-01-23
  * @brief   主函数
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "include.h"
#include "board.h"
#include "usart.h"
#include "dynament.h"

#define ADC_MEASUREMENT_PIN              GPIO_Pin_0           /* PF.11 */
#define ADC_MEASUREMENT_GPIO_PORT        GPIOB                /* GPIOF */
#define ADC_MEASUREMENT_ADC_CHANNEL      ADC_Channel_8        /* ADC channel1b */

dynament_rx_simple_t dynament_rx_simple,* pDynament_rx_simple;

uint32_t ADCValue=0,ADCValue1=0;
/* Private function prototypes -----------------------------------------------*/

void IDD_Measurement_ADC_Config(void);
uint16_t IDD_Measurement_ADC_Supply(void);
uint32_t IDD_Measurement_VDD(void);
uint32_t IDD_Measurement_ADC_ReadValue(void);

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Enable the GPIOs Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);  
    
    /* Enable the GPIOs Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init( GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure);
    
    // GPIO_Pin_9V_EN
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9V_EN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(GPIO_Port_9V_EN, &GPIO_InitStructure);
    
    // GPIO_Pin_DYNAMENT_3V3_EN
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_DYNAMENT_3V3_EN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(GPIO_Port_DYNAMENT_3V3_EN, &GPIO_InitStructure);
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
    
    /* Enable the Ethernet global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
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
    
    GPIO_ResetBits(GPIO_Port_9V_EN, GPIO_Pin_9V_EN);
    GPIO_SetBits(GPIO_Port_DYNAMENT_3V3_EN, GPIO_Pin_DYNAMENT_3V3_EN);
    
    dynament_usart_rx_index = 0;
    while(1)
	{
        ReadLiveData();
        printf("receive data:\r\n");
        if(dynament_usart_rx_flag == 2)                           // usart receive data
        {
            dynament_usart_rx_flag = 0;
            uint16_t checksum = (dynament_usart_rx_buffer[17] << 8) | dynament_usart_rx_buffer[18];
            if(!CheckSum(dynament_usart_rx_buffer,17,checksum))
            {
                printf("Check OK.\r\n");
            }
            printf("receive dynament data.\r\n");
            for(int i = 0;i < DYNAMENT_USART_RX_MAX_LEN;i ++)
            {
                 printf("%d = 0x%x.\r\n",i,dynament_usart_rx_buffer[i]);
            }
            printf("\r\n");
        }
        Delay_ms(2000);
	}
}

void IDD_Measurement_ADC_Config(void)
{
  ADC_InitTypeDef  ADC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOB Clock */  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /* Configure IDD Measurement pin (ADC Channelxx) as analog input -----------*/
  GPIO_InitStructure.GPIO_Pin = ADC_MEASUREMENT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_Init(ADC_MEASUREMENT_GPIO_PORT, &GPIO_InitStructure);
  
/* ADC1 configuration --------------------------------------------------------*/
  /* Enable HSI clock for ADC clock */
  RCC_HSICmd(ENABLE);

  /*!< Wait till HSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
  {}
    
/* Enable ADC clock ----------------------------------------------------------*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

/* de-initialize ADC ---------------------------------------------------------*/
  ADC_DeInit(ADC1);

/*  ADC configured as follows:
  - NbrOfChannel = 1 - ADC_Channel_1b
  - Mode = Single ConversionMode(ContinuousConvMode Enabled)
  - Resolution = 12Bits
  - Prescaler = /1
  - Sampling time 192 */

  /* ADC Configuration */
  ADC_BankSelection(ADC1, ADC_Bank_A);//ADC_Bank_A/ADC_Bank_B
  
  /* ADC Configuration */
  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel4 configuration */
  ADC_RegularChannelConfig(ADC1, ADC_MEASUREMENT_ADC_CHANNEL, 1, ADC_SampleTime_192Cycles);

  ADC_DelaySelectionConfig(ADC1, ADC_DelayLength_Freeze);

  ADC_PowerDownCmd(ADC1, ADC_PowerDown_Idle_Delay, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  
  /* Wait until ADC1 ON status */
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
  {
  }
}
uint32_t IDD_Measurement_ADC_ReadValue(void)
{
  uint16_t adcvalue = 0x0, i = 0x00;

  /* ADC1 configuration ------------------------------------------------------*/
  IDD_Measurement_ADC_Config();

  /* Start ADC1 Conversion using Software trigger */
  ADC_SoftwareStartConv(ADC1);
     
  for (i = 4; i > 0; i--)
  {  
    /* Wait until ADC Channel 15 end of conversion */
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
    {}
    /* Read ADC conversion result */
    adcvalue += ADC_GetConversionValue(ADC1);
  }
  return  (adcvalue >> 2);
}

uint16_t IDD_Measurement_ADC_Supply(void)
{
  uint16_t adcdata = 0x0, i = 0x00;

  /* ADC1 configuration ------------------------------------------------------*/
  IDD_Measurement_ADC_Config();
 
  ADC_TempSensorVrefintCmd(ENABLE);

  /* ADC1 regular channel 17 for VREF configuration */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 1, ADC_SampleTime_192Cycles);

  /* Start ADC conversion by software */
  ADC_SoftwareStartConv(ADC1);  

  /* Initialize result */
  for(i = 4; i > 0; i--)
  {
    /* Wait until end-of-conversion */
    while( ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
    {
    }

    /* Read ADC conversion result */
    adcdata += ADC_GetConversionValue(ADC1);
  }
	
  /* Deinitialize ADC */
  ADC_TempSensorVrefintCmd(DISABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);
  
  return (adcdata >> 2);
}

uint32_t IDD_Measurement_VDD(void)
{
  uint16_t adcdata;
  
  /* Read the VREF value on ADC converted value. */
  adcdata = IDD_Measurement_ADC_Supply();	
  
  /* Compute the VDD in mV using the theorical VREF value */
  adcdata = (1224 * 4096) / adcdata;
	
  return adcdata;
}


