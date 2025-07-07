#include "Infrared.h"

/****************************************************
函数功能：红外初始化函数
参    数：None
返 回 值：None
备    注：DATA---PA1----TIM2_CH2
*****************************************************/
void Infrared_Init(void)
{
	//开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct = {0};	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	TIM2_Config();//定时器配置
}


INFRARED_INFO Infrared_info_t = {0};

//TIM2中断服务函数
void TIM2_IRQHandler(void)
{
	static u8 i =0;
	static u8 StartFlag = 0;
	//更新中断
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除中断标志位
		if(StartFlag == 1)//采集开始
		{
			//采集过程中，没有边沿时间大于10ms，所以当采集开始后
			//进入到更新中断说明采集完成
			StartFlag = 0;
			Infrared_info_t.EdgeCnt = i;//边沿数
			i = 0;
			if(Infrared_info_t.EdgeCnt > 10) //过滤干扰信号
			Infrared_info_t.CaptureOver = 1;//捕获结束
		}
	}
	//通道2的捕获中断
	else if(TIM_GetITStatus(TIM2,TIM_IT_CC2) == SET)
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_CC2);//清除中断标志位
		if(TIM2->CCER&(1<<5))//下降沿触发捕获
		{
			TIM2->CCER &= ~(1<<5);//改成上升沿触发
			if(StartFlag == 0)//第一次捕获进入
			{
				StartFlag = 1;//采集开始标志
				TIM2->CNT = 0;//计数器清零
			}
			else
			{
				Infrared_info_t.TimeCnt[i++] = TIM_GetCapture2(TIM2);//保存高电平时间
				TIM2->CNT = 0;//计数器清零
			}
		}
		else//上升沿触发捕获
		{
			TIM2->CCER |= (1<<5);//改成下降沿触发
			Infrared_info_t.TimeCnt[i++] = TIM_GetCapture2(TIM2);
			TIM2->CNT = 0;
		}
	}
}


/****************************************************
函数功能：解析红外NEC位
参    数：类型 type 1  67个
返 回 值：0成功   1失败
备    注：逻辑1：560us低+1680us高
		  逻辑0：560us低+560us高
*****************************************************/
u8 Infrared_AnalysisNECBit(u8 type)
{
	u8 i = 0,j = 0;
	u8 EdgeCnt = 0;
	switch(type)
	{
		case 1:EdgeCnt = 67;break;//理想边沿数
	}
	if(Infrared_info_t.EdgeCnt != EdgeCnt)
		return 1;
	for(i=3;i<EdgeCnt;i+=2)
	{
		if(Infrared_info_t.TimeCnt[i]>1380 && Infrared_info_t.TimeCnt[i]<1980)//1  ---- 1680us
			Infrared_info_t.NECBit[j++] = 1;//如果在1380~1980为逻辑1
		else
			Infrared_info_t.NECBit[j++] = 0;//否则为逻辑0
		printf("%d ",Infrared_info_t.NECBit[j-1]);
	}
	printf("\r\n");
	return 0;
}


/****************************************************
函数功能：解析红外NEC码
参    数：类型 type 1  67个
返 回 值：0成功   1失败
备    注：同步头:9ms低电平+4.5ms高电平
          逻辑1：560us低+1680us高
		  逻辑0：560us低+560us高
*****************************************************/
u8 Infrared_AnalysisNECCode(u8 type)
{
	u8 i ,j = 0;
	u8 ret = 0;
	switch(type)
	{
		//XYD红外遥控----同步码
		case 1:if((Infrared_info_t.TimeCnt[0] >8500 && Infrared_info_t.TimeCnt[0] <9500)
			&& (Infrared_info_t.TimeCnt[1] >4000 && Infrared_info_t.TimeCnt[1] <5000))
			{
				ret = Infrared_AnalysisNECBit(type);
				if(ret) return 1;
				for(i=0;i<4;i++)
				{
					for(j = 0;j<8;j++)
					{
						//Bit低位先出
						Infrared_info_t.NECCode[i]<<=1;//空出最低位
						Infrared_info_t.NECCode[i] |= Infrared_info_t.NECBit[8*i + 7-j];//将NECBit从低位往前装
					}
					printf("%#x  ",Infrared_info_t.NECCode[i]);
				}
				printf("\r\n");
				Infrared_info_t.ContorlCode = Infrared_info_t.NECCode[2];//控制码为第三个字节
			}
			break;
	}
	return 0;
}








