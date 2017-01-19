/* Includes ------------------------------------------------------------------*/
#include "main.h"



//�������ȼ�

//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);
OS_TMR	tmr1;	//����һ����ʱ��

SYSTEM_CALIBRATION Calibrate_buff;

int main(void)
{
  OS_ERR err;
  
  
  delay_init(168);  	//ʱ�ӳ�ʼ��
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
  uart_init(115200);  //���ڳ�ʼ��
  LED_Init();         //LED��ʼ��
  Pump_Init();
  ADS1248_Init();
  SPI2_Init();
  LCD_Init();
  
  Ds1302_GPIO_Init();
  uint8_t check_value=DS1302_Check();
  if(check_value)Ds1302_Time_Init();
  
  FM24C64_Init();
//  uint8_t *earse_buff=(uint8_t *)malloc(160);
//  memset(earse_buff,0,160);
//  FM24C64_WRITE_MUL(CALIBRATION_STORAGE_ADD,earse_buff,160);
//  for(uint8_t i=0;i<4;i++)
//  {
//    System_Calibration.Calibration_Point[i][2]=1.0;
////    System_Calibration.Calibration_Flow[i][0]=200;
////    System_Calibration.Calibration_Flow[i][1]=300;
////    System_Calibration.Calibration_Flow[i][2]=600;
////    System_Calibration.Calibration_Flow[i][3]=900;
//  }
//   System_Calibration.Calibration_Point[0][0]=-2.19753794064977;
//   System_Calibration.Calibration_Point[0][1]=54.2283299577478;
//   System_Calibration.Calibration_Point[0][2]=-544.537479284230;
//   System_Calibration.Calibration_Point[0][3]=2891.86868738164;
//   System_Calibration.Calibration_Point[0][4]=-8765.05599820211;
//   System_Calibration.Calibration_Point[0][5]=15141.6918323016;
//   System_Calibration.Calibration_Point[0][6]=-13628.2240784771;
//   System_Calibration.Calibration_Point[0][7]=4872.88768692642;
//   
//   
//   							
//   System_Calibration.Calibration_Point[1][0]=-4.75271397602781;
//   System_Calibration.Calibration_Point[1][1]=112.427473730970;
//   System_Calibration.Calibration_Point[1][2]=-1095.76977300455;
//   System_Calibration.Calibration_Point[1][3]=5694.18653609531;
//   System_Calibration.Calibration_Point[1][4]=-16994.2449640936;
//   System_Calibration.Calibration_Point[1][5]=29059.8200561913;
//   System_Calibration.Calibration_Point[1][6]=-26126.4473840401;
//   System_Calibration.Calibration_Point[1][7]=9400.49716959420;
//   
//   
//   							
//   System_Calibration.Calibration_Point[2][0]=37.0641742875286;
//   System_Calibration.Calibration_Point[2][1]=-851.720379900497;
//   System_Calibration.Calibration_Point[2][2]=8192.25126734981;
//   System_Calibration.Calibration_Point[2][3]=-42592.8764290560;
//   System_Calibration.Calibration_Point[2][4]=128625.871996733;
//   System_Calibration.Calibration_Point[2][5]=-223982.296809053;
//   System_Calibration.Calibration_Point[2][6]=206100.532752370;
//   System_Calibration.Calibration_Point[2][7]=-75960.0853645296;
//   
//   
//   							
//   System_Calibration.Calibration_Point[3][0]=21.5649106392322;
//   System_Calibration.Calibration_Point[3][1]=-486.895445950732;
//   System_Calibration.Calibration_Point[3][2]=4611.01610602489;
//   System_Calibration.Calibration_Point[3][3]=-23650.9411683683;
//   System_Calibration.Calibration_Point[3][4]=70589.8835191975;
//   System_Calibration.Calibration_Point[3][5]=-121657.596007612;
//   System_Calibration.Calibration_Point[3][6]=110968.232296739;
//   System_Calibration.Calibration_Point[3][7]=-40620.7061019803;
   
 
//   FM24C64_WRITE_MUL(CALIBRATION_STORAGE_ADD,(uint8_t *)&System_Calibration,256);
// 
//   FM24C64_READ_MUL(CALIBRATION_STORAGE_ADD,(uint8_t *)&System_Calibration,256);
//   FM24C64_READ_MUL(CALIBRATION_STORAGE_ADD,(uint8_t *)&Calibrate_buff,256);
  //check_value=FM24C64_Check();
  MS561101BA_Init();
  W25QXX_Init();
  Flow_PWM_Init();
  TEM_PWM_Init();
  TIM6_Init(8399,999);
  PanakeyVar_Init();
  ADC1_Init();
   eMBInit(MB_RTU, 0x0A, 1, 115200, MB_PAR_NONE);  //��ʼ��FreeModbus

    eMBEnable();  //����FreeModbus
  Disp_MainWindow();
  CPU_SR_ALLOC();
  //Mem_Init();                                                 /* Initialize Memory Managment Module                   */
  /* Disable all Interrupts.                              */
  
  OSInit(&err);                                               /* Init uC/OS-III.                                      */
  // App_OS_SetAllHooks();
  CPU_CRITICAL_ENTER();
  OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
               (CPU_CHAR	* )"start task", 		//��������
               (OS_TASK_PTR )start_task, 			//������
               (void		* )0,					//���ݸ��������Ĳ���
               (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
               (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
               (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
               (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
               (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
               (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
               (void   	* )0,					//�û�����Ĵ洢��
               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
               (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
  
  OSStart(&err);  //����UCOSIII
  CPU_CRITICAL_EXIT();	//�˳��ٽ���	 
  while(1);
}

//��ʼ������
void start_task(void *p_arg)
{
  OS_ERR err;
  CPU_SR_ALLOC();
  CPU_CRITICAL_ENTER();
  p_arg = p_arg;
  
  CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
  OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
  
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
  CPU_IntDisMeasMaxCurReset();	
#endif
  
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
  //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
  OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
  
  CPU_CRITICAL_ENTER();	//�����ٽ���
  //����LED0����
  //������ʱ��1
  OSTmrCreate((OS_TMR		*)&tmr1,		//��ʱ��1
	      (CPU_CHAR	*)"tmr1",		//��ʱ������
	      (OS_TICK	 )0,			//0ms
	      (OS_TICK	 )6000,          //6000*10=60000ms
	      (OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
	      (OS_TMR_CALLBACK_PTR)tmr1_callback,//��ʱ��1�ص�����
	      (void	    *)0,			//����Ϊ0
              (OS_ERR	    *)&err);		//���صĴ�����
  CreateMainTask();
  CreateTempTask();
  CreateGUITask();
  CreateTempPressTask();
  CreateRTCTask();
  CreateKeyTask();
  CreateMODBUSTask();
  //CreateBATTERTask();
  CreateFlowControlTask();
  CreateTempControlTask();
  // CreateUsartTask();
  
  OSTaskDel((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
  CPU_CRITICAL_EXIT();	//�����ٽ���
}
float adcvaule[4]={0};


//��ʱ��1�Ļص�����
void tmr1_callback(void *p_tmr,void *p_arg)
{
  OS_ERR err;
  if(OSTmrStateGet(&tmr1,&err)==OS_TMR_STATE_RUNNING)
  {
  TIM_SetCompare3(TIM4,0);
  //CLR_LCD_LED();
  LCD_BL_State=0;
  }
  
  
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
