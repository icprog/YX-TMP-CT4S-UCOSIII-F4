#include "flow_PID.h"
#include "string.h"
#include "global.h"
/*
********************************************************************************
�������ƣ�void Flow_PID_Init(PID *pp)
�������ܣ�ʵ��PID�ĳ�ʼ�����ռ�����
��ڲ�����PID�ṹ��
���ڲ�������
********************************************************************************
*/
void Flow_PID_Init(PID_STRUCT *pp) 
{ 
  memset(pp,0,sizeof(PID_STRUCT)); 
  pp->SetPoint = 0;			//��ʼ������ȫ��
  pp->Proportion = 0.5;      //���ñ���ϵ��0.05
  pp->Integral = 0.21;     //���û���ϵ��0.0003
  pp->Derivative = 0.00;     //����΢��ϵ��0.00
} 
/*
********************************************************************************
�������ƣ�float Flow_PID_Calculation(PID_STRUCT *pp,float NowPoint) 
�������ܣ�ʵ��PID�ļ���
��ڲ�����PID�ṹ�壬NowPoint��ǰ����
���ڲ�������������ϵ��0~1000

λ��ʽPID�����㷨
u(t)=kp[e(t)+1/Ti��e(t)dt+Td*de(t)/dt]
kp ����ϵ��	, Ti �������Ļ���ʱ�� , Td ��������΢��ʱ�䡣
m(t)=Kp*e(t)+Ki*[e(1)+e(2)+...+e(t)}+Kd{e(t-1)-e(t-2)]
Ki = Kp*T/Ti Ϊ����ϵ��		Kd = Kp*Td/T Ϊ΢��ϵ��
********************************************************************************
*/
float Flow_PID_Calculation(PID_STRUCT *pp,float NowPoint) 
{ 
  //m(t)=Kp*e(n)+Ki*{e(1)+e(2)+...+e(n)}+Kd{e(n-1)-e(n-2)}
  float dError = 0;
  float Error = 0; 
  Error = pp->SetPoint - NowPoint;        //ƫ�� = �趨����-��ǰ����
  pp->SumError += Error;                  //��ƫ����л��ֵ���
  dError = pp->LastError - pp->PrevError; //΢�� = ��ǰƫ�-1�� - ��һ��ƫ�-2�� 
  pp->PrevError = pp->LastError;          //�ѵ�ǰƫ�����һ��ƫ��
  pp->LastError = Error;                  //��
  
  return (pp->Proportion * Error          //����
          + pp->Integral * pp->SumError           //������
            + pp->Derivative * dError);             //΢���� 
} 
/*
********************************************************************************
�������ƣ�void Flow_PWM_Init(void)
�������ܣ�����PWM����ʼ����TIM3��CH1~CH4(PC6~PC9)
��ڲ�������
���ڲ�������
********************************************************************************
*/
/*�����TMP�¶ȿ��Ƶ�PWM����*/
void Flow_PWM_Init(void)
{
	//�˲������ֶ��޸�IO������
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//ʹ��PORTFʱ��	
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3); //GPIOF9����Ϊ��ʱ��14
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3); //GPIOF9����Ϊ��ʱ��14
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3); //GPIOF9����Ϊ��ʱ��14
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3); //GPIOF9����Ϊ��ʱ��14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOC,&GPIO_InitStructure);              //��ʼ��PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=6;  //��ʱ����Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
  TIM_TimeBaseStructure.TIM_Period=999;   //�Զ���װ��ֵ
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
  
  
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//��ʼ����ʱ��14
  
  //��ʼ��TIM3 Channel1 PWMģʽ	 
 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 4OC1
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
  
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 4OC1
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
  
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 4OC1
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
  
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 4OC1
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
 
  TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
  
 
//  TIM_SetCompare1(TIM3,500);
//  TIM_SetCompare2(TIM3,500);
//  TIM_SetCompare3(TIM3,500);
//  TIM_SetCompare4(TIM3,500);
  Flow_PID_Init(&FlowPID_struct[0]);
  Flow_PID_Init(&FlowPID_struct[1]);
  Flow_PID_Init(&FlowPID_struct[2]);
  Flow_PID_Init(&FlowPID_struct[3]);
}
/*
********************************************************************************
�������ƣ�void Flow_PWM_Config(FLOW_STRUCT *FLOW)
�������ܣ�PWMΪTIM3 Channel1~4  PC6~PC9
��ڲ������������ƽṹ��ָ��
���ڲ�������
********************************************************************************
*/
/*�����TMP�¶ȿ��Ƶ�PWM����*/
void Flow_PWM_Config(FLOW_STRUCT *FLOW)
{
  TIM_SetCompare1(TIM3,FLOW->Flow_Duty1);
  TIM_SetCompare2(TIM3,FLOW->Flow_Duty2);
  TIM_SetCompare3(TIM3,FLOW->Flow_Duty3);
  TIM_SetCompare4(TIM3,FLOW->Flow_Duty4);
}
