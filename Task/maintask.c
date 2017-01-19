/*******************************************************************************
*CopyRight Reserved:	
*Filename��			maintask.c
*Description��     	        ������
*Date:
*Author:		
*******************************************************************************/

#include "task.h"
#include "sys.h"
#include "global.h"
#include "port.h"
//�������ȼ�

//������ƿ�
OS_TCB MainTaskTCB;
//�����ջ	
 CPU_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//������
void MainTask(void *p_arg);
/*
********************************************************************************
�������ƣ�void MainTask(void *pdata)
�������ܣ���������
��ڲ�������
���ڲ�������
********************************************************************************
*/
extern SYSTEM_CALIBRATION Calibrate_buff;

uu Ushort_To_Double;
void MainTask(void *p_arg)
{
  OS_ERR err;
  
  p_arg = p_arg;
  
  
  //����ι��
  //  OS_CRITICAL_ENTER();	//�����ٽ���
  //  CreateRTCTask();
  //  CreateKeyTask();
  //  CreateTempControlTask();
  //  CreateTempPressTask();
  //  CreateTemTask();
  //  CreateGUITask();
  //  CreateADTask();
  //  CreatePOWERTask();
  //  OS_CRITICAL_EXIT();	//�����ٽ���
  //  Disp_SampWindow();
  //  Disp_Sys_Info();
  //  Disp_Data();
  //  Disp_DetailWindow();
  //  Disp_ConfigWindow();
  //  Disp_SetWindow();
  while(1)
  {
    
    GPIO_ToggleBits(GPIOC,GPIO_Pin_13);
    if(Calibration_Ch>0&&Calibration_Ch<=4)
    {
          
          for(uint8_t i=0;i<7;i++)
          {
            Ushort_To_Double.u[0]=usReg_CalibrationBuf[4*i+3];
            Ushort_To_Double.u[1]=usReg_CalibrationBuf[4*i+2];
            Ushort_To_Double.u[2]=usReg_CalibrationBuf[4*i+1];
            Ushort_To_Double.u[3]=usReg_CalibrationBuf[4*i];
            System_Calibration.Calibration_Point[Calibration_Ch-1][i]=Ushort_To_Double.d;
            //System_Calibration.Calibration_Point[Calibration_Ch-1][i]=(((uint64_t)usReg_CalibrationBuf[4*i]<<48)+((uint64_t)usReg_CalibrationBuf[4*i+1]<<32)+((uint64_t)usReg_CalibrationBuf[4*i+2]<<16)+((uint64_t)usReg_CalibrationBuf[4*i+3]));
          }
          
          FM24C64_WRITE_MUL(CALIBRATION_STORAGE_ADD+(Calibration_Ch-1)*56,(uint8_t *)&System_Calibration.Calibration_Point[Calibration_Ch-1][0],56);
          FM24C64_READ_MUL(CALIBRATION_STORAGE_ADD+(Calibration_Ch-1)*56,(uint8_t *)&Calibrate_buff.Calibration_Point[Calibration_Ch-1][0],56);
       Calibration_Ch=0;
     
    }
    //Feed_Dog();                 //����ι��
    OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
  }
}

/*
********************************************************************************
�������ƣ�void CreateMainTask(void)
�������ܣ�����������
��ڲ�������
���ڲ�������
********************************************************************************
*/
void CreateMainTask(void)
{
  OS_ERR err;
  OSTaskCreate((OS_TCB 	  *)&MainTaskTCB,		//������ƿ�
               (CPU_CHAR  *)"Main Task", 		//��������
               (OS_TASK_PTR)MainTask, 			//������
               (void      *)0,					//���ݸ��������Ĳ���
               (OS_PRIO     )MAIN_TASK_PRIO,     //�������ȼ�
               (CPU_STK   * )&MAIN_TASK_STK[0],	//�����ջ����ַ
               (CPU_STK_SIZE)MAIN_STK_SIZE/10,	//�����ջ�����λ
               (CPU_STK_SIZE)MAIN_STK_SIZE,		//�����ջ��С
               (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
               (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
               (void      *)0,					//�û�����Ĵ洢��
               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
               (OS_ERR     *)&err);				//��Ÿú�������ʱ�ķ���ֵ
  
}