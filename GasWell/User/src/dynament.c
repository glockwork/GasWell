#include "dynament.h"
#include "usart.h"
#include <stdio.h>

dynament_rx_status_t dynament_usart_rx_status;
int8_t dynament_usart_rx_index = 0;
int8_t dynament_usart_rx_flag = 0;
uint8_t dynament_usart_rx_buffer[DYNAMENT_USART_RX_MAX_LEN] = {0};
uint8_t tx_buffer[7];

uint16_t GetCheckSum(uint8_t *buffer,int16_t size)
{
    uint16_t checksum = 0;
    
    for(int16_t i = 0;i < size;i ++)
    {
        checksum += *(buffer + i);
    }
    
    return checksum;
}

int8_t CheckSum(uint8_t *buffer,int16_t size,uint16_t checksum)
{
    int8_t state = 0;
    
    if(GetCheckSum(buffer,size) == checksum)
    {
        state = 0;
    }
    else
    {
        state = -1;
    }
    
    return state;
}

void ReadLiveData(void)
{
    uint16_t checksum = 0;

    tx_buffer[0] = DYNAMENT_DLE;
    tx_buffer[1] = DYNAMENT_RD;
    tx_buffer[2] = DYNAMENT_READ_LIVEDATA_SIMPLE;
    tx_buffer[3] = DYNAMENT_DLE;
    tx_buffer[4] = DYNAMENT_EOF;
    
    checksum = GetCheckSum(tx_buffer, 5);
    tx_buffer[5] = checksum >> 8;
    tx_buffer[6] = checksum;
    
    printf("send data:\r\n");
    for(int i = 0;i < sizeof(tx_buffer);i ++)
    {
        printf("0x%x,",tx_buffer[i]);
    }
    printf("\r\n");
    
    Usart2_SendData(tx_buffer, sizeof(tx_buffer));
}
