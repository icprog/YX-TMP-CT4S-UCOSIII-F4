#include "timer.h"

//������ʱ��6�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��6!
uint32_t Definite_Count[4]={0};
uint32_t Intervals_Count[4]={0};
bool isPause=false;
uint16_t cnt_buf=0;
void TIM6_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE); //��ʱ��6ʱ��ʹ��
  
  TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //���÷�Ƶֵ��10khz�ļ���Ƶ��
  TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
  TIM_TimeBaseInitStructure.TIM_Period=arr;  //�Զ���װ��ֵ ������5000Ϊ500ms
  TIM_TimeBaseInitStructure.TIM_ClockDivision=0; //ʱ�ӷָ�:TDS=Tck_Tim
  TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);
  
  TIM_ITConfig(TIM6,TIM_IT_Update|TIM_IT_Trigger,ENABLE); //ʹ��TIM6�ĸ����ж�
  
  NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn; //TIM6�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //��ռ���ȼ�1��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;  //�����ȼ�3��
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; //ʹ��ͨ��
  NVIC_Init(&NVIC_InitStructure);
  
  TIM_Cmd(TIM6,ENABLE); //��ʱ��6ʹ��
  isPause=true;
  cnt_buf=0;
}


void TIM6_DAC_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM6,TIM_IT_Update)!=RESET)
  {

    for(uint8_t i=0;i<4;i++)
    {
      if(Sampling_State[i]==Runing)Definite_Count[i]++;
      if(Intervals_State[i])Intervals_Count[i]++;
    }
   
    
  }
  TIM_ClearITPendingBit(TIM6,TIM_IT_Update); //����жϱ�־λ
}



void Start_Sampling(uint8_t CH)
{
  switch(CH)
  {
     case CH1:
    {
       if((Sampling_State[0]==Stop)||(Sampling_State[0]==Pause))
    {
      Sampling_State[0]=Runing;
    }
    }break;
     case CH2:
    {
      if((Sampling_State[1]==Stop)||(Sampling_State[1]==Pause))
    {
      Sampling_State[1]=Runing;
    }
    }break;
     case CH3:
    {
      if((Sampling_State[2]==Stop)||(Sampling_State[2]==Pause))
    {
      Sampling_State[2]=Runing;
    }
    }break;
     case CH4:
    {
       if((Sampling_State[3]==Stop)||(Sampling_State[3]==Pause))
    {
      Sampling_State[3]=Runing;
    }
    }break;
  }
 
    
    
   

}

void Pause_Sampling(uint8_t CH)
{
  switch(CH)
  {
  case CH1:
    {
       if(Sampling_State[0]==Runing)
   {
     Sampling_State[0]=Pause;
   }
    }break;
    case CH2:
    {
      if(Sampling_State[1]==Runing)
   {
     Sampling_State[1]=Pause;
   }
    }break;
    case CH3:
    {
      if(Sampling_State[2]==Runing)
   {
     Sampling_State[2]=Pause;
   }
    }break;
    case CH4:
    {
       if(Sampling_State[3]==Runing)
   {
     Sampling_State[3]=Pause;
   }
    }break;
  }
  
   
   
  
}
void Stop_Sampling(uint8_t CH)
{
  switch(CH)
  {
  case CH1:
    {
      if(Sampling_State[0]==Runing)
   {
     Sampling_State[0]=Stop;
     Definite_Count[0]=0;
   }
    }break;
    case CH2:
    {
      if(Sampling_State[1]==Runing)
   {
     Sampling_State[1]=Stop;
      Definite_Count[1]=0;
   }
    }break;
    case CH3:
    {
      if(Sampling_State[2]==Runing)
   {
     Sampling_State[2]=Stop;
      Definite_Count[2]=0;
   }
    }break;
    case CH4:
    {
      if(Sampling_State[3]==Runing)
   {
     Sampling_State[3]=Stop;
      Definite_Count[3]=0;
   }
    }break;
  }
   
   
   
   
}
void Multiple_Start_Sampling(FLOW_CHN CH)
{
  switch(CH)
  {
     case Flow_CH1:
    {
      Start_Sampling(CH1);
      start[0]=true;
    }break;
     case Flow_CH2:
    {
      Start_Sampling(CH2);
      start[1]=true;
    }break;
     case Flow_CH2_1:
    {
      Start_Sampling(CH1);
      Start_Sampling(CH2);
      start[0]=true;
      start[1]=true;
    }break;
     case Flow_CH3:
    {
      Start_Sampling(CH3);
      start[2]=true;
    }break;
     case Flow_CH3_1:
    {
      Start_Sampling(CH1);
      Start_Sampling(CH3);
      start[0]=true;
      start[2]=true;
    }break;
     case Flow_CH3_2:
    {
      Start_Sampling(CH2);
      Start_Sampling(CH3);
      start[1]=true;
      start[2]=true;
    }break;
     case Flow_CH3_2_1:
    {
      Start_Sampling(CH1);
      Start_Sampling(CH2);
      Start_Sampling(CH3);
      start[0]=true;
      start[1]=true;
      start[2]=true;
    }break;
     case Flow_CH4:
    {
      Start_Sampling(CH4);
       start[3]=true;
    }break;
     case Flow_CH4_1:
    {
      Start_Sampling(CH1);
      Start_Sampling(CH4);
       start[0]=true;
        start[3]=true;
    }break;
     case Flow_CH4_2:
    {
      Start_Sampling(CH2);
      Start_Sampling(CH4);
       start[1]=true;
        start[3]=true;
    }break;
      case Flow_CH4_2_1:
    {
      Start_Sampling(CH1);
      Start_Sampling(CH2);
      Start_Sampling(CH4);
       start[0]=true;
        start[1]=true;
	 start[3]=true;
    }break;
      case Flow_CH4_3:
    {
      Start_Sampling(CH3);
      Start_Sampling(CH4);
       start[2]=true;
        start[3]=true;
    }break;
      case Flow_CH4_3_1:
    {
      Start_Sampling(CH1);
      Start_Sampling(CH3);
      Start_Sampling(CH4);
       start[0]=true;
        start[2]=true;
	 start[3]=true;
    }break;
      case Flow_CH4_3_2:
    {
      Start_Sampling(CH2);
      Start_Sampling(CH3);
      Start_Sampling(CH4);
       start[1]=true;
        start[2]=true;
	 start[3]=true;
    }break;
      case Flow_CH4_3_2_1:
    {
      Start_Sampling(CH1);
      Start_Sampling(CH2);
      Start_Sampling(CH3);
      Start_Sampling(CH4);
       start[0]=true;
        start[1]=true;
	 start[2]=true;
	  start[3]=true;
    }break;
   case Flow_CH_None:
    {
    }break;
  }
}
void Multiple_Pause_Sampling(FLOW_CHN CH)
{
    switch(CH)
  {
     case Flow_CH1:
    {
      Pause_Sampling(CH1);
      start[0]=false;
    }break;
     case Flow_CH2:
    {
      Pause_Sampling(CH2);
      start[1]=false;
    }break;
     case Flow_CH2_1:
    {
      Pause_Sampling(CH1);
      Pause_Sampling(CH2);
      start[0]=false;
      start[1]=false;
    }break;
     case Flow_CH3:
    {
      Pause_Sampling(CH3);
      start[2]=false;
    }break;
     case Flow_CH3_1:
    {
      Pause_Sampling(CH1);
      Pause_Sampling(CH3);
      start[0]=false;
      start[2]=false;
    }break;
     case Flow_CH3_2:
    {
      Pause_Sampling(CH2);
      Pause_Sampling(CH3);
      start[1]=false;
      start[2]=false;
    }break;
     case Flow_CH3_2_1:
    {
      Pause_Sampling(CH1);
      Pause_Sampling(CH2);
      Pause_Sampling(CH3);
      start[0]=false;
      start[1]=false;
      start[2]=false;
    }break;
     case Flow_CH4:
    {
      Pause_Sampling(CH4);
      start[3]=false;
    }break;
     case Flow_CH4_1:
    {
      Pause_Sampling(CH1);
      Pause_Sampling(CH4);
      start[0]=false;
      start[3]=false;
    }break;
     case Flow_CH4_2:
    {
      Pause_Sampling(CH2);
      Pause_Sampling(CH4);
      start[1]=false;
      start[3]=false;
    }break;
      case Flow_CH4_2_1:
    {
      Pause_Sampling(CH1);
      Pause_Sampling(CH2);
      Pause_Sampling(CH4);
      start[0]=false;
      start[1]=false;
      start[3]=false;
    }break;
      case Flow_CH4_3:
    {
      Pause_Sampling(CH3);
      Pause_Sampling(CH4);
      start[2]=false;
      start[3]=false;
    }break;
      case Flow_CH4_3_1:
    {
      Pause_Sampling(CH1);
      Pause_Sampling(CH3);
      Pause_Sampling(CH4);
      start[0]=false;
      start[2]=false;
      start[2]=false;
    }break;
      case Flow_CH4_3_2:
    {
      Pause_Sampling(CH2);
      Pause_Sampling(CH3);
      Pause_Sampling(CH4);
      start[1]=false;
      start[2]=false;
      start[3]=false;
    }break;
      case Flow_CH4_3_2_1:
    {
      Pause_Sampling(CH1);
      Pause_Sampling(CH2);
      Pause_Sampling(CH3);
      Pause_Sampling(CH4);
      start[0]=false;
      start[1]=false;
      start[2]=false;
      start[3]=false;
    }break;
   case Flow_CH_None:
    {
    }break;
  }
}
void Multiple_Stop_Sampling(FLOW_CHN CH)
{
    switch(CH)
  {
     case Flow_CH1:
    {
      Stop_Sampling(CH1);
    }break;
     case Flow_CH2:
    {
      Stop_Sampling(CH2);
    }break;
     case Flow_CH2_1:
    {
      Stop_Sampling(CH1);
      Stop_Sampling(CH2);
    }break;
     case Flow_CH3:
    {
      Stop_Sampling(CH3);
    }break;
     case Flow_CH3_1:
    {
      Stop_Sampling(CH1);
      Stop_Sampling(CH3);
    }break;
     case Flow_CH3_2:
    {
      Stop_Sampling(CH2);
      Stop_Sampling(CH3);
    }break;
     case Flow_CH3_2_1:
    {
      Stop_Sampling(CH1);
      Stop_Sampling(CH2);
      Stop_Sampling(CH3);
    }break;
     case Flow_CH4:
    {
      Stop_Sampling(CH4);
    }break;
     case Flow_CH4_1:
    {
      Stop_Sampling(CH1);
      Stop_Sampling(CH4);
    }break;
     case Flow_CH4_2:
    {
      Stop_Sampling(CH2);
      Stop_Sampling(CH4);
    }break;
      case Flow_CH4_2_1:
    {
      Stop_Sampling(CH1);
      Stop_Sampling(CH2);
      Stop_Sampling(CH4);
    }break;
      case Flow_CH4_3:
    {
      Stop_Sampling(CH3);
      Stop_Sampling(CH4);
    }break;
      case Flow_CH4_3_1:
    {
      Stop_Sampling(CH1);
      Stop_Sampling(CH3);
      Stop_Sampling(CH4);
    }break;
      case Flow_CH4_3_2:
    {
      Stop_Sampling(CH2);
      Stop_Sampling(CH3);
      Stop_Sampling(CH4);
    }break;
      case Flow_CH4_3_2_1:
    {
      Stop_Sampling(CH1);
      Stop_Sampling(CH2);
      Stop_Sampling(CH3);
      Stop_Sampling(CH4);
    }break;
   case Flow_CH_None:
    {
    }break;
  }
}