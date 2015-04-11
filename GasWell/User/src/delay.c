#include "delay.h"

/*******************************************************************************
Function:          Delay_ms0(uint32_t ms)
Description:       ��ʱ����
Calls:             ��               //�����������õĺ����嵥
Called by:         main()           //���ñ������ĺ����嵥
Table Accessed��   ��               //�����ʵı�
Table Updated��    ��               //���޸ĵı�
Input��            
                   param1��ms   0~65535
Output��           ��
Return��           �� 
Others��           1ms
*******************************************************************************/
void Delay_ms(uint32_t ms)
{
  uint16_t x,y;
  for(x=ms;x>0;x--)
    for(y=32000;y>0;y--);
}
