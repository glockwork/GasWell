#include "key.h"


uint8_t  cKeyTimer=0;
uint8_t  cKey_Status=NOWAIT;
uint16_t iKeyTempHis=0x0000;
/****************************************************************************
* 名    称：void KeyGPIOColConfig(void)
* 功    能：矩阵键盘初始化
* 入口参数
* 出口参数：无
* 说    明：行输出，列输入
* 调用方法：KeyGPIOConfig();
****************************************************************************/
void KeyGPIOConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);	
//PC12 键盘上灯开关LEDon
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;						//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            //推挽
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;					    //上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOC,GPIO_Pin_12);
	
	//键盘引脚使能 PC0~PC7 列COL1~COL7 配置输入模式；PC8~PC11 列ROW1~ROW4 配置输出模式    
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|//列
	                                GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;							//输入
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;            //开漏
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;					//浮空
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;//行
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;						//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            //推挽
//	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;					    //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
	
}
/****************************************************************************
* 名    称：uint16_t KeyScan(void)
* 功    能：矩阵键盘扫描
* 入口参数
* 出口参数：无
* 说    明：
* 调用方法：KeyScan();
****************************************************************************/
uint16_t KeyScan(void)
{
  uint16_t iReadData=0,iColData=0,iRowShift=0xfeff;
	uint8_t   i=0,j=0;

	if(cKey_Status == NOWAIT)
	{
		iReadData=GPIO_ReadInputData(GPIOC) & 0xf000;//获取GPIOC高4位数据
		GPIO_Write(GPIOC,iReadData | 0x00ff);//row行全为0，col全为1，高4位不变
		for(i=0;i<20;i++);
		iReadData=GPIO_ReadInputData(GPIOC) & 0x00ff;//读取GPIOC低8位数据
		if(iReadData != 0x00ff) //有键按下
		{
			cKeyTimer   = 0;
			cKey_Status = WAIT;
		}
		else
		{
			iKeyTempHis=0;//无按键按下，清零历史编码
			return 0;
		}
	}
	
	else
	{
		if(cKeyTimer > KeyDelayTime)//消抖时间到？
		{
			cKey_Status = NOWAIT;
			iReadData=GPIO_ReadInputData(GPIOC) & 0xf000;//获取GPIOC高4位数据
			GPIO_Write(GPIOC,iReadData | 0x00ff);//row行全为0，col全为1，高4位不变
			for(i=0;i<20;i++);
			iReadData=GPIO_ReadInputData(GPIOC) & 0x00ff;//读取GPIOC低8位数据
			if(iReadData != 0x00ff) //有键按下
			{
				iColData=iReadData;//保存列值	
				iRowShift=0x0eff;
				for(j=0;j<4;j++)
				{					
					iReadData=GPIO_ReadInputData(GPIOC)&0xf000;//获取GPIOC高4位数据
					GPIO_Write(GPIOC,iReadData | iRowShift);//row1~row4依次为0,1110 1101 1011 0111
					for(i=0;i<20;i++);
					iReadData=GPIO_ReadInputData(GPIOC) & 0x00ff;//读取GPIOC低8位数据
					if(iReadData != 0x00ff)
						return ((iRowShift & 0x0f00) | iColData);				
					iRowShift=((iRowShift<<1) | 0x01) & 0x0fff;//行左移1位
				}
			}
			else
				return 0;			
		}
		
	}
	
	return 0;
}

/****************************************************************************
* 名    称：uint8_t GetKeyValue(void)
* 功    能：解码
* 入口参数
* 出口参数：无
* 说    明：
* 调用方法：GetKeyValue();
****************************************************************************/
uint8_t GetKeyValue(void)
{
	uint16_t iKeyTempNum=0; 
	uint8_t  cKeyNum=0;
	
	iKeyTempNum=KeyScan();
	if(iKeyTempNum>0)
	{
		if(iKeyTempNum != iKeyTempHis)//现在编码与历史编码是否相同？
		{
			iKeyTempHis=iKeyTempNum;//存储编码
			switch(iKeyTempNum)
			{
				case 0x0efe:cKeyNum=1;break;
				case 0x0efd:cKeyNum=2;break;
				case 0x0efb:cKeyNum=3;break;
				case 0x0ef7:cKeyNum=4;break;
				case 0x0eef:cKeyNum=5;break;
				case 0x0edf:cKeyNum=6;break;
				case 0x0ebf:cKeyNum=7;break;
				case 0x0e7f:cKeyNum=8;break;
				
				case 0x0dfe:cKeyNum=9;break;
				case 0x0dfd:cKeyNum=10;break;
				case 0x0dfb:cKeyNum=11;break;
				case 0x0df7:cKeyNum=12;break;
				case 0x0def:cKeyNum=13;break;
				case 0x0ddf:cKeyNum=14;break;
				case 0x0dbf:cKeyNum=15;break;
				
				case 0x0bfe:cKeyNum=16;break;
				case 0x0bfd:cKeyNum=17;break;
				case 0x0bfb:cKeyNum=18;break;
				case 0x0bf7:cKeyNum=19;break;
				case 0x0bef:cKeyNum=20;break;
				case 0x0bdf:cKeyNum=21;break;
				case 0x0bbf:cKeyNum=22;break;
				
				case 0x07fe:cKeyNum=23;break;
				case 0x07fd:cKeyNum=24;break;
				case 0x07fb:cKeyNum=25;break;
				case 0x07f7:cKeyNum=26;break;
				case 0x07ef:cKeyNum=27;break;
				case 0x07df:cKeyNum=28;break;
				case 0x07bf:cKeyNum=29;break;
							
				default:break;
			}
		}
		else 
			return 0;
	}	
	else 
		return 0;
	
	return cKeyNum;
}


