/*******************************************************************************
 *CopyRight Reserved:	
 *Filename��			key.h
 *Description��     	        �����������
 *Date:
 *Author:		
*******************************************************************************/

#ifndef __KEY_H__
#define __KEY_H__
#include "sys.h"
#define BUTTON_FILTER_TIME         5  
#define BUTTON_LONG_TIME         50                /* ����1�룬��Ϊ�����¼� */  
#define  KEY_FIFO_SIZE            20
/* 
        ÿ��������Ӧ1��ȫ�ֵĽṹ������� 
        ���Ա������ʵ���˲��Ͷ��ְ���״̬������� 
*/  
typedef struct  
{  
        /* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */  
        unsigned char  IsKeyDown; /* �������µ��ж�ָ��,1��ʾ���� */  
   
        unsigned char  Count;                        /* �˲��������� */  
        unsigned char  FilterTime;                /* �˲�ʱ��(���255,��ʾ2550ms) */  
        unsigned short LongCount;                /* ���������� */  
        unsigned short LongTime;                /* �������³���ʱ��, 0��ʾ����ⳤ�� */  
        unsigned char   State;                        /* ������ǰ״̬�����»��ǵ��� */  
        unsigned char  KeyCodeUp;                /* ��������ļ�ֵ����, 0��ʾ����ⰴ������ */  
        unsigned char  KeyCodeDown;        /* �������µļ�ֵ����, 0��ʾ����ⰴ������ */  
        unsigned char  KeyCodeLong;        /* ���������ļ�ֵ����, 0��ʾ����ⳤ�� */  
        unsigned char  RepeatSpeed;        /* ������������ */  
        unsigned char  RepeatCount;        /* �������������� */  
}KEY_T;  
typedef struct
{
	unsigned char Buf[KEY_FIFO_SIZE];		               /* ��ֵ������ */
	unsigned char Read;					       /* ��������ָ�� */
	unsigned char  Write;					       /* ������дָ�� */
	
}KEY_FIFO_T;
extern KEY_FIFO_T  KEY_FIFO;
typedef enum  
{  
        KEY_NONE = 0,                        /* 0 ��ʾ�����¼� */  
   
        KEY_DOWN,                        /* ���������� */  
        KEY_UP,                        /* ���������� */  
        KEY_LONG,                        /* ���������� */  
 
}KEY_ENUM;  
extern KEY_T s_KEY[16];
void Key_Init(void);
void TIM7_Init(u16 arr,u16 psc);
uint8_t Key_Scan(void);
uint8_t Get_KeyVaule(void);
uint8_t bsp_keyRead( void );
void Button_Detect(void);
void  PanakeyVar_Init(void) ;
#endif