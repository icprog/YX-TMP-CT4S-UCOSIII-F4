/*
*********************************************************************************************************
*                                               
* File        : KeyTask.c
* By          : Wenfei Tong
* Version     : V1.0
* Date        : 03-08-2016
* Description :
* ---------------
*   ��������������
*********************************************************************************************************
*/
#include "task.h"
#include "global.h"
#include "key.h"
uint8_t last_key,now_key;

//������ƿ�
OS_TCB KeyTaskTCB;
//�����ջ	
CPU_STK KEY_TASK_STK[KEY_STK_SIZE];
#include "lcd.h"

//������
void KeyTask(void *pdata);
void KeyTask(void *pdata)
{
  OS_ERR err;
  pdata=pdata;
  while(1)
  {
    Button_Detect();
    if(!LCD_BL_State)
    {
    if((KEY_FIFO.Read < KEY_FIFO.Write)&&KEY_FIFO.Write>0)
    {bsp_keyRead();
   OSTmrStop((OS_TMR		*)&tmr1,		//��ʱ��1
		(OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
		(void	    *)0,			//����Ϊ0
	        (OS_ERR	    *)&err);		//���صĴ�����
	TIM_SetCompare3(TIM4,1000);
        LCD_BL_State=1;
    }
    }
    else
    {
      if((OSTmrStateGet(&tmr1,&err)==OS_TMR_STATE_RUNNING))
      {
        if((KEY_FIFO.Read < KEY_FIFO.Write)&&(KEY_FIFO.Write>0))
    {
        OSTmrStop((OS_TMR		*)&tmr1,		//��ʱ��1
		(OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
		(void	    *)0,			//����Ϊ0
	        (OS_ERR	    *)&err);		//���صĴ�����
	TIM_SetCompare3(TIM4,1000);
        LCD_BL_State=1;
        OSTmrStart((OS_TMR		*)&tmr1,//��ʱ��1
		   (OS_ERR	        *)&err);
    }
		//���صĴ�����
      }
      else if((OSTmrStateGet(&tmr1,&err)==OS_TMR_STATE_UNUSED)||(OSTmrStateGet(&tmr1,&err)==OS_TMR_STATE_STOPPED))
      {
	OSTmrStart((OS_TMR		*)&tmr1,//��ʱ��1
		   (OS_ERR	        *)&err);
		//���صĴ�����
      }
      
    }
    
    OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
  }
}

void CreateKeyTask(void)
{
  OS_ERR err;
  OSTaskCreate((OS_TCB 	  *)&KeyTaskTCB,		//������ƿ�
	       (CPU_CHAR  *)"Key Task", 		//��������
	       (OS_TASK_PTR)KeyTask, 			//������
	       (void      *)0,					//���ݸ��������Ĳ���
	       (OS_PRIO     )KEY_TASK_PRIO,     //�������ȼ�
	       (CPU_STK   * )&KEY_TASK_STK[0],	//�����ջ����ַ
	       (CPU_STK_SIZE)KEY_STK_SIZE/10,	//�����ջ�����λ
	       (CPU_STK_SIZE)KEY_STK_SIZE,		//�����ջ��С
	       (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
	       (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
	       (void      *)0,					//�û�����Ĵ洢��
	       (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
	       (OS_ERR     *)&err);	
}