#include "delay.h"

/*******************************************************************************
Function:          Delay_ms0(uint32_t ms)
Description:       延时函数
Calls:             无               //被本函数调用的函数清单
Called by:         main()           //调用本函数的函数清单
Table Accessed：   无               //被访问的表
Table Updated：    无               //被修改的表
Input：            
                   param1：ms   0~65535
Output：           无
Return：           无 
Others：           1ms
*******************************************************************************/
void Delay_ms(uint32_t ms)
{
  uint16_t x,y;
  for(x=ms;x>0;x--)
    for(y=32000;y>0;y--);
}
