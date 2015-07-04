#include "dynament.h"
#include "board.h"
#include "usart.h"
#include <stdio.h>
#include "delay.h"

dynament_rx_t dynament_rx;
int8_t dynament_usart_rx_index = 0;
int8_t dynament_usart_rx_flag = 0;
uint8_t dynament_usart_rx_buffer[DYNAMENT_USART_RX_MAX_LEN] = {0};
uint8_t tx_buffer[7];

/*******************************************************************************
* @brief   Convert a 8-bit array to a 32-bit variable.
* @param   u32Temp:the src data to be converted
* @param   pu8Array:the dest data addr
* @retval  a 32-bit variable
*******************************************************************************/
uint32_t Convertu8ArrayTou32(uint8_t *pu8Array)
{
    APP_ASSERT(pu8Array != NULL);
    
    return (((uint32_t)(*pu8Array)) + (((uint32_t)(*(pu8Array + 1))) << 8) + 
            ((uint32_t)(*(pu8Array + 2)) << 16) + ((uint32_t)(*(pu8Array + 3)) << 24));
}

/*******************************************************************************
* @brief   Compute the sum of the specified buffer.
* @param   buffer:  the specifid buffer
* @param   size:    the size to compute
* @retval  a 16-bit sum
*******************************************************************************/
uint16_t ComputeCheckSum(uint8_t * buffer,int16_t size)
{
    uint16_t checksum = 0;
    
    APP_ASSERT(buffer != NULL);
    
    for(int16_t i = 0;i < size;i ++)
    {
        checksum += *(buffer + i);
    }
    
    return checksum;
}

int8_t CheckSum(uint8_t *buffer,int16_t size,uint16_t checksum)
{
    int8_t state = 0;
    uint16_t checksum_tmp = 0;
    
    APP_ASSERT(buffer != NULL);
    
    checksum_tmp = ComputeCheckSum(buffer,size);
    if(checksum_tmp == checksum)
    {
        #ifdef DYNAMENT_DEBUG
            printf("Check OK.\r\n");
        #endif
        
        state = 0;
    }
    else
    {
        #ifdef DYNAMENT_DEBUG
            printf("Check error:checksum_tmp = 0x%x,checksum = 0x%x\r\n",checksum_tmp,checksum);
        #endif
        
        state = -1;
    }
    
    return state;
}

void ReadLiveDataSimple(void)
{
    uint16_t checksum = 0;

    tx_buffer[0] = DYNAMENT_DLE;
    tx_buffer[1] = DYNAMENT_RD;
    tx_buffer[2] = DYNAMENT_READ_LIVEDATA_SIMPLE;
    tx_buffer[3] = DYNAMENT_DLE;
    tx_buffer[4] = DYNAMENT_EOF;
    
    checksum = ComputeCheckSum(tx_buffer, 5);
    tx_buffer[5] = checksum >> 8;
    tx_buffer[6] = checksum;
    
    #ifdef DYNAMENT_DEBUG    
        printf("send data:\r\n");
        for(int i = 0;i < sizeof(tx_buffer);i ++)
        {
            printf("0x%x,",tx_buffer[i]);
        }
        printf("\r\n");
    #endif 
    
    Usart2_SendData(tx_buffer, sizeof(tx_buffer));
}

/*******************************************************************************
* @brief   Convert a unsigned int 32-bit  to a float.
* @param   temp:  the data to be converted
* @retval  a float data
*******************************************************************************/
float Convertu32Tofloat(uint32_t temp)
{
    u32orfloat_u u32orfloat;
    
    u32orfloat.u32Value = temp;
    
    return (u32orfloat.fValue);
}

/*******************************************************************************
* @brief   Convert a 8-bit array to a 32-bit variable.
* @param   void
* @retval  void
*******************************************************************************/
void GetLiveData(void)
{
    uint16_t timeout = 0;
    float fGasReading = 0;
    
    ReadLiveDataSimple();
    timeout = 10;
    while((!dynament_usart_rx_flag) && (--timeout))
    {
        printf("dynament_usart_rx_flag = %d,timeout = %d.\r\n",dynament_usart_rx_flag,timeout);
        Delay_ms(100);
    }
    dynament_usart_rx_flag = 0;
    
    if(timeout)
    {
        uint16_t checksum = (dynament_usart_rx_buffer[dynament_rx.len - 2] << 8) | dynament_usart_rx_buffer[dynament_rx.len - 1];
        if(!CheckSum(dynament_usart_rx_buffer,dynament_rx.len - 2,checksum))
        {
            printf("Check OK.\r\n");
        }
        printf("receive data:\r\n");
        for(int i = 0;i < DYNAMENT_USART_RX_MAX_LEN;i ++)
        {
             printf("%d = 0x%x.\r\n",i,dynament_usart_rx_buffer[i]);
        }
        fGasReading = Convertu32Tofloat(Convertu8ArrayTou32(&dynament_usart_rx_buffer[7]));
        printf("fGasReading = %f.\r\n",fGasReading);
        printf("\r\n");
    }
    else
    {
        printf("timeout.\r\n");
    }
}

