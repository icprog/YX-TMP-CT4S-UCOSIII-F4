#ifndef __DS18B20_H
#define __DS18B20_H 
#include "target.h"
//IO��������

#define DS18B20_IO_IN()  {GPIOA->MODER&=~(3<<(8*2));GPIOA->MODER|=0<<8*2;}	
#define DS18B20_IO_OUT()  {GPIOA->MODER&=~(3<<(8*2));GPIOA->MODER|=1<<8*2;} 
////IO��������											   
#define	DS18B20_DQ_OUT PAout(8) //���ݶ˿�	PA0 
#define	DS18B20_DQ_IN  PAin(8)  //���ݶ˿�	PA0 
   	
uint8_t DS18B20_Init(void);//��ʼ��DS18B20
float DS18B20_Get_Temp(void);//��ȡ�¶�
void DS18B20_Start(void);//��ʼ�¶�ת��
void DS18B20_Write_Byte(uint8_t dat);//д��һ���ֽ�
uint8_t DS18B20_Read_Byte(void);//����һ���ֽ�
uint8_t DS18B20_Read_Bit(void);//����һ��λ
uint8_t DS18B20_Check(void);//����Ƿ����DS18B20
void DS18B20_Rst(void);//��λDS18B20    
#endif















