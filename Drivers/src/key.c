/*******************************************************************************
*CopyRight Reserved:	
*Filename��			key.c
*Description��     	        �����������
*Date:
*Author:		
*******************************************************************************/
#include "key.h"
#include "target.h"
//const uint8_t key_tab[]={11,22,44,88,
//                   10,20,40,80,
//                   9 ,18,36,72,
//                   13,26,52,104
//};//������


KEY_T s_KEY[16];
KEY_FIFO_T  KEY_FIFO;
uint8_t lastkey=0xff;
void  PanakeyVar_Init(void)  
{  
  for(uint8_t i=0;i<16;i++)
  {
    /* ��ʼ��USER����������֧�ְ��¡����𡢳��� */  
    s_KEY[i].IsKeyDown = 0;                /* �жϰ������µĺ��� */  
    s_KEY[i].FilterTime = BUTTON_FILTER_TIME;                /* �����˲�ʱ�� */  
    s_KEY[i].LongTime = BUTTON_LONG_TIME;                        /* ����ʱ�� */  
    s_KEY[i].Count = s_KEY[i].FilterTime / 2;                /* ����������Ϊ�˲�ʱ���һ�� */  
    s_KEY[i].State = 0;                                                        /* ����ȱʡ״̬��0Ϊδ���� */  
    s_KEY[i].KeyCodeDown = KEY_DOWN;                        /* �������µļ�ֵ���� */  
    s_KEY[i].KeyCodeUp =0;                                /* ��������ļ�ֵ���� */  
    s_KEY[i].KeyCodeLong = 0;                        /* �������������µļ�ֵ���� */  
    s_KEY[i].RepeatSpeed = 50;                                                /* �����������ٶȣ�0��ʾ��֧������ */  
    s_KEY[i].RepeatCount = 0;   /* ���������� */  
  }  
  s_KEY[3].KeyCodeLong=KEY_LONG;
  s_KEY[3].RepeatSpeed=10;
  s_KEY[3].RepeatCount=10;
  s_KEY[7].KeyCodeLong=KEY_LONG;
  s_KEY[7].RepeatSpeed=10;
  s_KEY[7].RepeatCount=10;
  s_KEY[11].KeyCodeLong=KEY_LONG;
  s_KEY[11].RepeatSpeed=10;
  s_KEY[11].RepeatCount=10;
  s_KEY[15].KeyCodeLong=KEY_LONG;
  s_KEY[15].RepeatSpeed=10;
  s_KEY[15].RepeatCount=10;
  KEY_FIFO .Read = 0;
  KEY_FIFO.Write  = 0;
  
  for(uint8_t i = 0; i< KEY_FIFO_SIZE ; i++)
    
    KEY_FIFO.Buf[i] = 0;
}  
void Pannelkey_Put( unsigned char key_Value)
{
  if(KEY_FIFO.Write   <  KEY_FIFO_SIZE)
  {
    KEY_FIFO.Buf[KEY_FIFO.Write] = key_Value;
    KEY_FIFO.Write ++ ;   
    
  }
}
void Key_Init(void)
{ 
  
   GPIO_InitTypeDef      GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  
  //	GPIO_InitTypeDef  GPIO_InitStructure;
  // 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;				//PORTA.8 �������
  // 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		  
  // 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  // 	GPIO_Init(GPIOA, &GPIO_InitStructure);
  //         
  //        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;				
  // 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
  // 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  // 	GPIO_Init(GPIOC, &GPIO_InitStructure);
  //      
  //        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;				
  // 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
  // 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  // 	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11 ; //GPIOA9��GPIOA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//���ù���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
 
  
  GPIO_Init(GPIOE,&GPIO_InitStructure); //��ʼ��PA9��PA10
  
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 ; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��TEM2_DRDY
  GPIO_SetBits(GPIOE,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
  
  
  
  
}
inline void Key_Col_Init(void)
{ 
  //    GPIO_InitTypeDef  GPIO_InitStructure;
  // 	
  // 	
  //	
  // 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;				//PORTA.8 �������
  // 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
  // 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  // 	GPIO_Init(GPIOA, &GPIO_InitStructure);
  //        
  //       
  //        
  //        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;				//PORTA.8 �������
  // 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		  
  // 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  // 	GPIO_Init(GPIOC, &GPIO_InitStructure);
  //        
  //        
  //        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;				//PORTA.8 �������
  // 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		  
  // 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  // 	GPIO_Init(GPIOB, &GPIO_InitStructure);
  //  
  GPIO_InitTypeDef      GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11 ; //GPIOA9��GPIOA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ù���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
  
  GPIO_Init(GPIOE,&GPIO_InitStructure); //��ʼ��PA9��PA10
  
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 ; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��TEM2_DRDY
  GPIO_SetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
  
  
  
  
}




uint8_t Key_Scan(void)
{
  uint8_t row=0,col=0,temp=0;
  Key_Init();
  Delayms(5);
  row=(GPIO_ReadInputData(GPIOE)>>8)&0x0f;
  if(row!=0)
  {
    
    							//�����ж�
    Key_Col_Init();
    
    Delayms(5);
    col=(GPIO_ReadInputData(GPIOE)>>12)&0X0f;
   
    //col=((PBin(1))|((PBin(0))<<1)|(PCin(5)<<2)|(PCin(4)<<3));
    
  }
  temp=col*(row+3);
  switch(temp)
  {
  case 4:return 1;
  case 5:return 3;
  case 7:return 6;
  case 11:return 9;
  case 8:return 2;
  case 10:return 5;
  case 14:return 7;
  case 22:return 10;
  case 16:return 4;
  case 20:return 12;
  case 28:return 13;
  case 44:return 11;
  case 32:return 16;
  case 40:return 8;
  case 56:return 15;
  case 88:return 14;
  default: return 0;
  }
  //  for(uint8_t i=0;i<16;i++)
  //  {
  //    if(temp==key_tab[i])
  //    {
  //      if((i+1)==15)
  //      {
  //	col=col+1;
  //	row=row+1;
  //      }
  //      else if((i+1)==16)
  //      {
  //	col=col+1;
  //	row=row+1;
  //      }
  //      return i+1;
  //    }
  //  }
  
}
/* 
********************************************************************************************************* 
*        �� �� ��: bsp_DetectButton 
*        ����˵��: ���һ��������������״̬�����뱻�����Եĵ��á� 
*        ��    �Σ������ṹ����ָ�� 
*        �� �� ֵ: �� 
********************************************************************************************************* 
*/  
uint8_t first_scan=0;
void Button_Detect(void)  
{  
  uint8_t i=Key_Scan();
  
  if((0<i)&&(i<17))
  {
    s_KEY[i-1].IsKeyDown=1;
    if (s_KEY[i-1].Count < s_KEY[i-1].FilterTime)  
    {  
      s_KEY[i-1].Count = s_KEY[i-1].FilterTime;  
    }  
    else if(s_KEY[i-1].Count < 2 * s_KEY[i-1].FilterTime)  
    {  
      s_KEY[i-1].Count++;  
    }  
    else  
    {  
      if (s_KEY[i-1].State == 0)  
      {  
	s_KEY[i-1].State = 1;  
	
	/* ���Ͱ�ť���µ���Ϣ */  
	if (s_KEY[i-1].KeyCodeDown > 0)  
	{  
	  /* ��ֵ���밴��FIFO */  
	  Pannelkey_Put(i*4);// ��¼�������±�־���ȴ��ͷ�  
	  
	}  
      }  
      
      if ((s_KEY[i-1].LongTime > 0))  
      {  
	if ((s_KEY[i-1].LongCount < s_KEY[i-1].LongTime)&&(s_KEY[i-1].KeyCodeLong==KEY_LONG))  
	{  
	  /* ���Ͱ�ť�������µ���Ϣ */  
	  if (++s_KEY[i-1].LongCount == s_KEY[i-1].LongTime)  
	  {  
	    /* ��ֵ���밴��FIFO */  
	    Pannelkey_Put(i*4+1);          //��¼������־
	    
	  }  
	}  
	else  
	{  
	  if (s_KEY[i-1].RepeatSpeed > 0)  
	  {  
	    if (++s_KEY[i-1].RepeatCount >= s_KEY[i-1].RepeatSpeed)  
	    {  
	      s_KEY[i-1].RepeatCount = 0;  
	      /* ��������ÿ��10ms����1������ */  
	      Pannelkey_Put(i*4+2);          
	      
	    }  
	  }  
	}  
      }  
      lastkey=i;
    }  
  }  
  else if(i==0) 
  {  
    if((0<lastkey)&&(lastkey<17))
    {
      if(s_KEY[lastkey-1].Count > s_KEY[lastkey-1].FilterTime)  
      {  
	s_KEY[lastkey-1].Count = s_KEY[lastkey-1].FilterTime;  
      }  
      else if(s_KEY[lastkey-1].Count != 0)  
      {  
	s_KEY[lastkey-1].Count--;  
      }  
      else  
      {  
	if (s_KEY[lastkey-1].State == 1)  
	{  
	  s_KEY[lastkey-1].State = 0;  
	  
	  /* ���Ͱ�ť�������Ϣ */  
	  if (s_KEY[lastkey-1].KeyCodeUp > 0) /*�����ͷ�*/  
	  {  
	    /* ��ֵ���밴��FIFO */  
	    Pannelkey_Put(i*4+3);          
	    
	  }  
	  s_KEY[lastkey-1].IsKeyDown=0;
	  lastkey=i;
	}  
      }  
      
      s_KEY[lastkey-1].LongCount = 0;  
      s_KEY[lastkey-1].RepeatCount = 0;  
    }
  }  
  
}  
uint8_t bsp_keyRead( void )
{
  uint8_t revalue=0;
  uint8_t i=0 ;
  if(KEY_FIFO.Read < KEY_FIFO.Write)
  {
    revalue = KEY_FIFO.Buf[KEY_FIFO.Read]/4;
    
    KEY_FIFO.Read ++;                   
  }
  else if((KEY_FIFO.Read == KEY_FIFO .Write  ) ||(KEY_FIFO.Read == KEY_FIFO_SIZE))
  {
    revalue = 0;
    
    for(i = 0;i<KEY_FIFO.Read ;i++)
      KEY_FIFO.Buf[i] = 0;
    
    KEY_FIFO.Read = 0;
    KEY_FIFO.Write = 0;
  }
  
  return revalue;
  
}