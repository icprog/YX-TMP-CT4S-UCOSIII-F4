 /*
 *CopyRight Reserved:	���ǿƼ�
 *Filename��		adtask.c
 *Description��     	AD�ɼ�����         
 *Date:
 *Author:				
*/

#include "task.h"
#include "adc.h"
#include "global.h"
//������ƿ�
OS_TCB ADCTaskTCB;
//�����ջ	
 CPU_STK ADC_TASK_STK[ADC_STK_SIZE];
//������
void ADCTask(void *p_arg);



void ADCTask(void *pdata)
{
   OS_ERR err;
  pdata=pdata;
  while(1)
  {
    
    System_Power=ReadADCAverageValue();
    
    
    OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
  }
}

void CreateADCTask(void)
{
   OS_ERR err;
  OSTaskCreate((OS_TCB 	  *)&ADCTaskTCB,		//������ƿ�
               (CPU_CHAR  *)"ADC Task", 		//��������
               (OS_TASK_PTR)ADCTask, 			//������
               (void      *)0,					//���ݸ��������Ĳ���
               (OS_PRIO     )ADC_TASK_PRIO,     //�������ȼ�
               (CPU_STK   * )&ADC_TASK_STK[0],	//�����ջ����ַ
               (CPU_STK_SIZE)ADC_STK_SIZE/10,	//�����ջ�����λ
               (CPU_STK_SIZE)ADC_STK_SIZE,		//�����ջ��С
               (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
               (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
               (void      *)0,					//�û�����Ĵ洢��
               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
               (OS_ERR     *)&err);				//��Ÿú�������ʱ�ķ���ֵ
}
















