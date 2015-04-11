#include "key.h"


uint8_t  cKeyTimer=0;
uint8_t  cKey_Status=NOWAIT;
uint16_t iKeyTempHis=0x0000;
/****************************************************************************
* ��    �ƣ�void KeyGPIOColConfig(void)
* ��    �ܣ�������̳�ʼ��
* ��ڲ����a
* ���ڲ�������
* ˵    �����������������
* ���÷�����KeyGPIOConfig();
****************************************************************************/
void KeyGPIOConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);	
//PC12 �����ϵƿ���LEDon
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;						//���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            //����
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;					    //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOC,GPIO_Pin_12);
	
	//��������ʹ�� PC0~PC7 ��COL1~COL7 ��������ģʽ��PC8~PC11 ��ROW1~ROW4 �������ģʽ    
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|//��
	                                GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;							//����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;            //��©
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;					//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;//��
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;						//���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            //����
//	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;					    //����
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
	
}
/****************************************************************************
* ��    �ƣ�uint16_t KeyScan(void)
* ��    �ܣ��������ɨ��
* ��ڲ����a
* ���ڲ�������
* ˵    ����
* ���÷�����KeyScan();
****************************************************************************/
uint16_t KeyScan(void)
{
  uint16_t iReadData=0,iColData=0,iRowShift=0xfeff;
	uint8_t   i=0,j=0;

	if(cKey_Status == NOWAIT)
	{
		iReadData=GPIO_ReadInputData(GPIOC) & 0xf000;//��ȡGPIOC��4λ����
		GPIO_Write(GPIOC,iReadData | 0x00ff);//row��ȫΪ0��colȫΪ1����4λ����
		for(i=0;i<20;i++);
		iReadData=GPIO_ReadInputData(GPIOC) & 0x00ff;//��ȡGPIOC��8λ����
		if(iReadData != 0x00ff) //�м�����
		{
			cKeyTimer   = 0;
			cKey_Status = WAIT;
		}
		else
		{
			iKeyTempHis=0;//�ް������£�������ʷ����
			return 0;
		}
	}
	
	else
	{
		if(cKeyTimer > KeyDelayTime)//����ʱ�䵽��
		{
			cKey_Status = NOWAIT;
			iReadData=GPIO_ReadInputData(GPIOC) & 0xf000;//��ȡGPIOC��4λ����
			GPIO_Write(GPIOC,iReadData | 0x00ff);//row��ȫΪ0��colȫΪ1����4λ����
			for(i=0;i<20;i++);
			iReadData=GPIO_ReadInputData(GPIOC) & 0x00ff;//��ȡGPIOC��8λ����
			if(iReadData != 0x00ff) //�м�����
			{
				iColData=iReadData;//������ֵ	
				iRowShift=0x0eff;
				for(j=0;j<4;j++)
				{					
					iReadData=GPIO_ReadInputData(GPIOC)&0xf000;//��ȡGPIOC��4λ����
					GPIO_Write(GPIOC,iReadData | iRowShift);//row1~row4����Ϊ0,1110 1101 1011 0111
					for(i=0;i<20;i++);
					iReadData=GPIO_ReadInputData(GPIOC) & 0x00ff;//��ȡGPIOC��8λ����
					if(iReadData != 0x00ff)
						return ((iRowShift & 0x0f00) | iColData);				
					iRowShift=((iRowShift<<1) | 0x01) & 0x0fff;//������1λ
				}
			}
			else
				return 0;			
		}
		
	}
	
	return 0;
}

/****************************************************************************
* ��    �ƣ�uint8_t GetKeyValue(void)
* ��    �ܣ�����
* ��ڲ����a
* ���ڲ�������
* ˵    ����
* ���÷�����GetKeyValue();
****************************************************************************/
uint8_t GetKeyValue(void)
{
	uint16_t iKeyTempNum=0; 
	uint8_t  cKeyNum=0;
	
	iKeyTempNum=KeyScan();
	if(iKeyTempNum>0)
	{
		if(iKeyTempNum != iKeyTempHis)//���ڱ�������ʷ�����Ƿ���ͬ��
		{
			iKeyTempHis=iKeyTempNum;//�洢����
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


