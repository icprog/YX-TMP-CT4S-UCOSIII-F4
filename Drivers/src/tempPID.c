/*
*********************************************************************************************************                                              
* File        : PID.c
* By          : Wenfei Tong
* Version     : V1.0
* Date        : 03-08-2016
* Description :
* ---------------
*   PID���ƺ�������������PID���Ƶ��ں�����PWM����
*********************************************************************************************************
*/
#include "tempPID.h"
#include "global.h"
#include <string.h>
/*
********************************************************************************
�������ƣ�void TEM_PID_ReSet(PID_STRUCT *pp)
�������ܣ�ʵ��PID�ģ��ռ�����
��ڲ�����PID�ṹ��
���ڲ�������
********************************************************************************
*/
void TEM_PID_ReSet(PID_STRUCT *pp) 
{ 
  memset(pp,0,sizeof(PID_STRUCT));     
}
/*
********************************************************************************
�������ƣ�void TEM_PID_Configuration(PID_STRUCT *pp,float goal,float P,float I,float D)
�������ܣ�ʵ��PID�ĳ�ʼ������
��ڲ�����PID�ṹ�壬goalĿ���¶ȣ�����P��I��D������ϵ��������ϵ��������ϵ����
���ڲ�������
********************************************************************************
*/
void TEM_PID_Configuration(PID_STRUCT *pp)
{
  pp->SetPoint = 20;			//��ʼ������ȫ��
  pp->Proportion = 10;      //���ñ���ϵ��0.05
  pp->Integral = 0.5;     //���û���ϵ��0.0003
  pp->Derivative = 0;     //���û���ϵ��0.00
}
/*
********************************************************************************
�������ƣ�float TEM_PID_Calculation(PID_STRUCT *pp,float NowTem)  
�������ܣ�ʵ��PID�ļ���
��ڲ�����PID�ṹ��,NowTem��ǰ�������¶�
���ڲ���������ռ�ձ�ϵ��0~1

����ʽPID�����㷨
m(t)=Kp*e(t-1)+Ki*[e(1)+e(2)+...+e(t)}+Kd{e(t-1)-e(t-2)]
Ki = Kp*T/Ti Ϊ����ϵ��		Kd = Kp*Td/T Ϊ΢��ϵ��
PID����Ҫ���ں��Ա�֤���ռ�ձ�ϵ��0-1
********************************************************************************
*/
float TEM_PID_Calculation(PID_STRUCT *pp,float NowTem) 
{ 
  //m(t)=Kp*e(n)+Ki*{e(1)+e(2)+...+e(n)}+Kd{e(n-1)-e(n-2)}
  float Error = 0; 
  Error = (pp->SetPoint - NowTem)*100;        //ƫ�� = �趨����-��ǰ����
  pp->Value=pp->Proportion *(Error-pp->LastError)\
    +pp->Integral * Error\
      +pp->Derivative *(Error-2*pp->LastError+pp->PrevError);
  pp->PrevError = pp->LastError;          //�ѵ�ǰƫ�����һ��ƫ��
  pp->LastError = Error;                  //��
  
  return (pp->Value);             
} 

/*
********************************************************************************
�������ƣ�void TEM_PWM_Init(void)
�������ܣ�ʵ��TIM2��CH1��PA8����CH2(PA9)ͨ�����PWM�������Ƽ�������
��ڲ�������
���ڲ�������
********************************************************************************
*/
void TEM_PWM_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTFʱ��	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2  , ENABLE);  
  
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM2); //GPIOF9����Ϊ��ʱ��14
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM2); //GPIOF9����Ϊ��ʱ��14
  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;           //GPIOF9
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
  GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PF9
  
  
  
  TIM_TimeBaseStructure.TIM_Prescaler=83;  //��ʱ����Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
  TIM_TimeBaseStructure.TIM_Period=999;   //�Զ���װ��ֵ
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
  
  
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//��ʼ����ʱ��14
  
  //��ʼ��TIM2 Channel1 PWMģʽ	 
 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  
  TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 4OC1
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR1�ϵ�Ԥװ�ؼĴ���
  
  TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 4OC1
  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR1�ϵ�Ԥװ�ؼĴ���
  
  TIM_ARRPreloadConfig(TIM2,ENABLE);//ARPEʹ�� 
  
  TIM_Cmd(TIM2, ENABLE);
  TEM_PID_Configuration(&TempPID_struct);
}
/*
********************************************************************************
�������ƣ�void TEM_SetDuty(uint8_t state,uint16_t duty)
�������ܣ�ʵ��PWM�����������ã����PID�����¶�
��ڲ�����state 0:Ĭ�ϼ��ȹ��ܣ� 1�����书��; duty:0~1000��Ӧ0�� ~ 1000��ռ�ձ�
���ڲ�������
********************************************************************************
*/
void TEM_SetDuty(uint8_t state,uint16_t duty)
{
  if(state)
  {
    TIM_SetCompare3(TIM2,0);
    TIM_SetCompare4(TIM2,duty);
  }
  else 
  {
    TIM_SetCompare4(TIM2,0);
    TIM_SetCompare3(TIM2,duty);
  }
}
/*
********************************************************************************
�������ƣ�void TEM_PWM_ReSet(void)
�������ܣ��ر��¶ȿ��ƹ���
��ڲ�������
���ڲ�������
********************************************************************************
*/
void TEM_PWM_ReSet(void)
{
  TIM_SetCompare4(TIM2,0);
  TIM_SetCompare3(TIM2,0);
}
/*
********************************************************************************
�������ƣ�void TEM_Compare(PID_STRUCT *pp,float NowTem)
�������ܣ��趨�¶ȺͲ����¶Ƚ��бȽϣ�Ȼ�󾭹�PID�����ת��ΪPWM������
��ڲ�����PID�ṹ�壬�����¶�
���ڲ�������
********************************************************************************
*/
void TEM_Compare(PID_STRUCT *pp,float NowTem)
{
  
  //static uint8_t pid_flag=0;
  if((pp->SetPoint-NowTem)>1)                                 //�����¶ȵ����趨�¶�3�����ϣ����ؼ���
  {
    TEM_SetDuty(HOT,1000);
  }
  else if((NowTem-pp->SetPoint)>1)                            //�����¶ȸ����趨�¶�3�����ϣ���������
  {
    TEM_SetDuty(COOL,1000);
  }
  else if(((pp->SetPoint-NowTem)<=1)&&\
    ((pp->SetPoint-NowTem)>=0.2)) //�����¶ȵ����趨�¶�1��~7�����ڣ�PID���ڼ���
  {
    TEM_PID_Calculation(pp,NowTem);
    //   if(pp->Value>1)temp_data=1;
    //    else if(pp->Value<0)temp_data=0;
    //    else temp_data=pp->Value;
    TEM_SetDuty(HOT,(uint16_t)(250+250*pp->Value));
  }
  else if(((NowTem-pp->SetPoint)<=1)&&\
    ((NowTem-pp->SetPoint)>=0.2)) //�����¶ȸ����趨�¶�1��~7�����ڣ�PID��������
  {
    TEM_PID_Calculation(pp,NowTem);
    //    if(pp->Value>1)temp_data=1;
    //    else if(pp->Value<0)temp_data=0;
    //    else temp_data=pp->Value;
    TEM_SetDuty(COOL,(uint16_t)(750+250*pp->Value));
  }
  else if(((NowTem-pp->SetPoint)<0.2)&&((NowTem-pp->SetPoint)>-0.2))   //�����¶ȸ��������¶�1���ں��¿��ƣ�����΢�����䣩
  {
    TEM_SetDuty(COOL,CONST_COOL);
  }
  else if(((pp->SetPoint-NowTem)<0.2)&&((pp->SetPoint-NowTem)>-0.2))   //�����¶ȵ��������¶�1���ں��¿��ƣ�����΢�����ȣ�
  {
    TEM_SetDuty(HOT,CONST_HOT);
  }
  else   TEM_PWM_ReSet();                                          //ֹͣ��������
  
}