/*
*********************************************************************************************************
*                                               
* File        : TempControlTask.c
* By          : Wenfei Tong
* Version     : V1.0
* Date        : 03-08-2016
* Description :
* ---------------
*   �¶ȿ���������,�¶ȷ�Χ(15��~30��)��PWM����
*********************************************************************************************************
*/
#include "task.h"
#include "ads1248.h"
#include "global.h"
#include "tempPID.h"
OS_TCB TempControlTaskTCB;
//�����ջ	
CPU_STK TEMPCONTROL_TASK_STK[TEMPCONTROL_STK_SIZE];
float last_PT100_Temp[2],res[2];
uint8_t testcount[2]={0};
uint32_t ads1248_buf[2]={0};
void TempControlTask(void *pdata)
{
  OS_ERR err;
  pdata=pdata;
  while(1)
  {
    float temp[2]={0};
    
     
     ads1248_buf[0]=ADS1248_ReadADC(TEM1);
     ads1248_buf[1]=ADS1248_ReadADC(TEM2);
	res[0]=ads1248_buf[0]/ 24672.376470588235294117647058824;
	res[1]=ads1248_buf[1]/ 24672.376470588235294117647058824;
     temp[0]=ADS1248_ResToTemp(res[0]);

     temp[1]=ADS1248_ResToTemp(res[1]);
     if(temp[0]>-50)
     {
       PT100_Temp[0]=temp[0];
       if(testcount[0]<5)
       {
	 last_PT100_Temp[0]=PT100_Temp[0];
	 testcount[0]++;
       }
     }
      if(temp[1]>-50)
     {
       PT100_Temp[1]=temp[1];
       if(testcount[1]<5)
       {
	 last_PT100_Temp[1]=PT100_Temp[1];
	 testcount[1]++;
       }
     }
     if(fabs(last_PT100_Temp[0]-PT100_Temp[0])>3)
     {
       
      PT100_Temp[0]=last_PT100_Temp[0];
     }
     if(fabs(last_PT100_Temp[1]-PT100_Temp[1])>3)
     {
      
      PT100_Temp[1]=last_PT100_Temp[1];
     }
     last_PT100_Temp[0]=PT100_Temp[0];
     last_PT100_Temp[1]=PT100_Temp[1];
     
    TEM_Compare(&TempPID_struct,PT100_Temp[0]);
    
   OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
  }
}

void CreateTempControlTask(void)
{
  OS_ERR err;
  OSTaskCreate((OS_TCB 	  *)&TempControlTaskTCB,		//������ƿ�
               (CPU_CHAR  *)"TempControl Task", 		//��������
               (OS_TASK_PTR)TempControlTask, 			//������
               (void      *)0,					//���ݸ��������Ĳ���
               (OS_PRIO     )TEMPCONTROL_TASK_PRIO,     //�������ȼ�
               (CPU_STK   * )&TEMPCONTROL_TASK_STK[0],	//�����ջ����ַ
               (CPU_STK_SIZE)TEMPCONTROL_STK_SIZE/10,	//�����ջ�����λ
               (CPU_STK_SIZE)TEMPCONTROL_STK_SIZE,		//�����ջ��С
               (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
               (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
               (void      *)0,					//�û�����Ĵ洢��
               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
               (OS_ERR     *)&err);	
}