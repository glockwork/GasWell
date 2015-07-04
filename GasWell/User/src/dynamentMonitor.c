#include "DynamentMonitor.h"
#include "stm32l1xx_adc.h"
#include "board.h"
#include <stdio.h>

#define ADC_CONV_BUFF_SIZE              6

volatile FlagStatus flag_ADCDMA_TransferComplete;
uint16_t T_StartupTimeDelay;
uint16_t ADC_ConvertedValueBuff[ADC_CONV_BUFF_SIZE] = {0};

ADC_InitTypeDef ADC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;
DMA_InitTypeDef DMA_InitStructure;

void acquireVoltageValue(void);
void clearADCDMA_TransferComplete(void);

/*******************************************************************************
* @brief   Set the flag_ADCDMA_TransferComplete flag
* @param   void
* @retval  void
*******************************************************************************/
void SetADCDMA_TransferComplete(void)
{
    flag_ADCDMA_TransferComplete = SET;
}

/*******************************************************************************
* @brief   Clears the ag_ADCDMA_TransferComplete flag
* @param   void
* @retval  void
*******************************************************************************/
void ClearADCDMA_TransferComplete(void)
{
    flag_ADCDMA_TransferComplete = RESET;
}

/*******************************************************************************
* @brief   Configures the GPIO for ADC1
* @param   void
* @retval  void
*******************************************************************************/
void configureGPIO_ADC(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
   
    /* Enable GPIOs clock */ 	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    
    /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_ANALOG_IN;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIO_Port_ANALOG_IN, &GPIO_InitStructure);
}

/*******************************************************************************
* @brief   Configures the DMA for ADC1
* @param   void
* @retval  void
*******************************************************************************/
void configureDMA_ADC(void)
{
    /* Declare NVIC init Structure */
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable DMA1 clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* De-initialise  DMA */
    DMA_DeInit(DMA1_Channel1);

    /* DMA1 channel1 configuration */
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);	        // Set DMA channel Peripheral base address to ADC Data register
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValueBuff;   // Set DMA channel Memeory base addr to ADC_ConvertedValueBuff address
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          // Set DMA channel direction to peripheral to memory
    DMA_InitStructure.DMA_BufferSize = ADC_CONV_BUFF_SIZE;                      // Set DMA channel buffersize to peripheral to ADC_CONV_BUFF_SIZE
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	        // Disable DMA channel Peripheral address auto increment
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     // Enable Memeory increment (To be verified ....)
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // set Peripheral data size to 8bit 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	        // set Memeory data size to 8bit 
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                               // Set DMA in normal mode
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;	                        // Set DMA channel priority to High
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                // Disable memory to memory option 
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                // Use Init structure to initialise channel1 (channel linked to ADC)

    /* Enable Transmit Complete Interrup for DMA channel 1 */ 
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

    /* Setup NVIC for DMA channel 1 interrupt request */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* @brief   Configures the ADC module
* @param   void
* @retval  void
*******************************************************************************/
void configureADC_BAT(void)
{
    /*Enable the HSI oscillator*/
    RCC_HSICmd(ENABLE);
    
    /* Enable ADC clock & SYSCFG */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    /* Enable the internal connection of Temperature sensor and with the ADC channels*/
    ADC_TempSensorVrefintCmd(ENABLE); 

    /* Wait until ADC + Temp sensor start */
    T_StartupTimeDelay = 1024;
    while (T_StartupTimeDelay--);
    
    /* Setup ADC common init struct */
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_CommonInit(&ADC_CommonInitStructure);

    /* Initialise the ADC1 by using its init structure */
    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;	                // Set conversion resolution to 12bit
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                            // Enable Scan mode (single conversion for each channel of the group)
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;			            // Disable Continuous conversion
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None; // Disable external conversion trigger
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                  // Set conversion data alignement to right
    ADC_InitStructure.ADC_NbrOfConversion = ADC_CONV_BUFF_SIZE;             // Set conversion data alignement to ADC_CONV_BUFF_SIZE
    ADC_Init(ADC1, &ADC_InitStructure);

    /* ADC1 regular from channel1 to channel5 and internal reference channel17 configuration */ 
    for (uint32_t ch_index = 1; ch_index < ADC_CONV_BUFF_SIZE; ch_index++)
    {
        ADC_RegularChannelConfig(ADC1, ADC_Channel_NAP55_AD_IN, ch_index, ADC_SampleTime_384Cycles);
    }
    ADC_RegularChannelConfig(ADC1, ADC_Channel_INTERNAL_REF, ADC_CONV_BUFF_SIZE, ADC_SampleTime_384Cycles);   // Internal reference voltage
}

/*******************************************************************************
* @brief   Disables the DMA module and clock for ADC to power down
* @param   void
* @retval  void
*******************************************************************************/
void powerDownADC(void)
{
    /* Disable DMA channel1 */
    DMA_Cmd(DMA1_Channel1, DISABLE);  
    
    /* Disable ADC1 */
    ADC_Cmd(ADC1, DISABLE);

    /* Disable ADC1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);  
    
    /* Disable DMA1 clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, DISABLE);

	#ifdef DYNAMENT_DEBUG
		printf("batt DMA disable \r\n");
	#endif 
}

/*******************************************************************************
* @brief   Computes the real voltage
* @param   void
* @retval  The ADC value
*******************************************************************************/
float ComputeRealVoltage(void)
{
    uint32_t temp  = 0;

    for(int32_t i = 0;i < (ADC_CONV_BUFF_SIZE - 1);i ++)
    {
        temp += *(ADC_ConvertedValueBuff + i);
    }
    
    float volADCvalue = ((temp * 1.2) / ((*(ADC_ConvertedValueBuff + 5))*5));
    
    return volADCvalue;
}

/*******************************************************************************
* @brief   Configures the GPIO for ADC1
* @param   void
* @retval  void
*******************************************************************************/
float ADC_SampleAndConv(ADC_TypeDef* ADCx)
{
    /* Check the parameters */
    APP_ASSERT(ADCx == ADC1);
    
	uint32_t TimeCnt=0;
	
	#ifdef DYNAMENT_DEBUG
		printf("batt require start \r\n");
	#endif 
    /* Re-enable DMA and ADC conf and start Temperature Data acquisition */ 
    acquireVoltageValue();
	
	TimeCnt = 160000;
    /* for DEBUG purpose uncomment the following line and comment the __WFI call to do not enter STOP mode */
    while ((!flag_ADCDMA_TransferComplete) && (TimeCnt !=0))
	{
			TimeCnt--;
	}
    
    if(!TimeCnt)
    {
        #ifdef DYNAMENT_DEBUG
		printf("ADC timeout.\r\n");
        #endif
    }
    
    /* Clear global flag for DMA transfert complete */
    ClearADCDMA_TransferComplete(); 
	
	#ifdef DYNAMENT_DEBUG
		printf("batt require stop \r\n");
	#endif 
	
    powerDownADC();
    
    #ifdef DYNAMENT_DEBUG
//    for(int32_t index = 0;index < ADC_CONV_BUFF_SIZE;index ++)
//    {
//        printf("ADC_ConvertedValueBuff[%d] = 0x%x.\r\n",index,ADC_ConvertedValueBuff[index]);
//    }
    #endif
    
    return (ComputeRealVoltage());
}

void acquireVoltageValue(void)
{
	uint32_t TimeCnt=0;
    
    /* Enable ADC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    /* Enable DMA1 clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

	TimeCnt = 6000;
    /* Wait until the ADC1 is ready */
    while((ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET) && (TimeCnt != 0))
	{
		TimeCnt--;
	}		

    /* re-initialize DMA -- is it needed ?*/
    DMA_DeInit(DMA1_Channel1);
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);
  
    /* Enable DMA channel 1 Transmit complete interrupt*/
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

    /* Disable DMA mode for ADC1 */ 
    ADC_DMACmd(ADC1, DISABLE);

    /* Enable DMA mode for ADC1 */  
    ADC_DMACmd(ADC1, ENABLE);

    /*Enable ADC1*/
    ADC_Cmd(ADC1,ENABLE);
    
    /*Wait until the ADC1 is ready*/
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_ADONS) == RESET);
    
    /* Clear global flag for DMA transfert complete */
    ClearADCDMA_TransferComplete(); 

    /* Start ADC conversion */
    ADC_SoftwareStartConv(ADC1);
}

void DynamentVoltDetect(void)
{
	/* Disable charge for improving the ADC value */

    #ifdef DYNAMENT_DEBUG
        printf("batt vol detect start \r\n");
    #endif 
    configureGPIO_ADC();
    configureDMA_ADC();
    configureADC_BAT();

    float fVoltage = ADC_SampleAndConv(ADC1);

    /* Enables charge */
    #ifdef DYNAMENT_DEBUG
        printf("The battery voltage is %f v.\r\n",fVoltage);
    #endif 
}
