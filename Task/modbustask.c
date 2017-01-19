 /*
 *CopyRight Reserved:	���ǿƼ�
 *Filename��		adtask.c
 *Description��     	AD�ɼ�����         
 *Date:
 *Author:				
*/

#include "task.h"
#include "port.h"
#include "mb.h"
#include "global.h"
//������ƿ�
OS_TCB MODBUSTaskTCB;
//�����ջ	
 CPU_STK MODBUS_TASK_STK[MODBUS_STK_SIZE];
//������
void MODBUSTask(void *p_arg);



void MODBUSTask(void *pdata)
{
   OS_ERR err;
  pdata=pdata;
  while(1)
  {
    
    eMBPoll(); 
//    usRegAd_VauleBuf[0]=*(uint16_t *)&testfloat[0];
//    usRegAd_VauleBuf[1]=*(((uint16_t *)&testfloat[0])+1);
    OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
  }
}

void CreateMODBUSTask(void)
{
   OS_ERR err;
  OSTaskCreate((OS_TCB 	  *)&MODBUSTaskTCB,		//������ƿ�
               (CPU_CHAR  *)"MODBUS Task", 		//��������
               (OS_TASK_PTR)MODBUSTask, 			//������
               (void      *)0,					//���ݸ��������Ĳ���
               (OS_PRIO     )MODBUS_TASK_PRIO,     //�������ȼ�
               (CPU_STK   * )&MODBUS_TASK_STK[0],	//�����ջ����ַ
               (CPU_STK_SIZE)MODBUS_STK_SIZE/10,	//�����ջ�����λ
               (CPU_STK_SIZE)MODBUS_STK_SIZE,		//�����ջ��С
               (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
               (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
               (void      *)0,					//�û�����Ĵ洢��
               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
               (OS_ERR     *)&err);				//��Ÿú�������ʱ�ķ���ֵ
}
















