#ifndef __ADS1118_H
#define __ADS1118_H
#include "target.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//SPI���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
#define SET_ADS_CS1()    GPIO_SetBits(GPIOD,GPIO_Pin_8)
#define CLR_ADS_CS1()    GPIO_ResetBits(GPIOD,GPIO_Pin_8)

#define SET_ADS_CS2()    GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define CLR_ADS_CS2()    GPIO_ResetBits(GPIOB,GPIO_Pin_12)

#define ADS1118_SCK_H()  GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define ADS1118_SCK_L()  GPIO_ResetBits(GPIOB,GPIO_Pin_13)

#define ADS1118_MOSI_H() GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define ADS1118_MOSI_L() GPIO_ResetBits(GPIOB,GPIO_Pin_15)

#define ADS1118_DO_PIN() GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
typedef enum 
{
  FLOW_ADC,
  CURRENT_ADC
}ADS1118_type;
typedef enum 
{
  ADC_CH1,
  ADC_CH2,
  ADC_CH3,
  ADC_CH4
}ADS1118_CH; 	 				  	    													  
void SPI2_Init(void);			 //��ʼ��SPI��

uint8_t SPI2_ReadWriteByte(uint8_t TxData);//SPI���߶�дһ���ֽ�
uint32_t ADS1118_ReadWord(void);
uint16_t ADS1118_ReadWriteReg(uint16_t reg);
uint16_t ADS118_Convert(ADS1118_type adc,ADS1118_CH adc_ch);
float Get_Avg(ADS1118_type adc,ADS1118_CH adc_ch);
float Filter(ADS1118_type adc,ADS1118_CH ch) ;
#endif