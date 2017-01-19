#include "ads1118.h"
#include  <os.h>
void SPI2_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
//  SPI_InitTypeDef  SPI_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOAʱ��
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//ʹ��SPI1ʱ��
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ; //GPIOA9��GPIOA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ù���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //���츴�����
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
  GPIO_SetBits(GPIOD,GPIO_Pin_8);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ; //GPIOA9��GPIOA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ù���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //���츴�����
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
  GPIO_SetBits(GPIOB,GPIO_Pin_12);
  
  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;//PB3~5���ù������	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//PB3~5���ù������	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
 
  //GPIOF9,F10��ʼ������
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//PB3~5���ù������	
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//�������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
//  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
//	
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); //PB3����Ϊ SPI1
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2); //PB4����Ϊ SPI1
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2); //PB5����Ϊ SPI1
//  
//  
//  
//  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
//  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
//  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
//  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
//  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
//  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
//  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
//  SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
//  SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
//  SPI_Cmd(SPI2, ENABLE); //ʹ��SPI���� 
}   

/*******************************************************************************
* @Function Name: ADS1118_WriteByte
* @Description  : Sends a byte through the SPI interface and return the byte received from the SPI bus.
* @param		: u8 data byte to send
* @retval	   	: u8 The value of the received byte
* @date		   	: 2014/10/04
*******************************************************************************/
uint8_t  ADS1118_WriteByte(uint8_t data)

{
  uint8_t bit_ctr;
  uint8_t temp = 0;
  CPU_SR_ALLOC();
  
  
  CPU_CRITICAL_ENTER();
  OSIntEnter();                                               /* Tell uC/OS-III that we are starting an ISR           */
  CPU_CRITICAL_EXIT();//�����ٽ�״̬
  //�����ж�
  for(bit_ctr=0;bit_ctr<8;bit_ctr++) // output 8-bit
  {
    
    ADS1118_SCK_L();
    if(data & 0x80)
      ADS1118_MOSI_H();  // MOSI =1;       
    else 
      ADS1118_MOSI_L();  //MOSI=0;
     Delay_Nus(20);
    data = (data << 1);                  // shift next bit into MSB..
    
    ADS1118_SCK_H();    //SCK = 1;                      // Set SCK high..
    
    temp = temp<<1;
    if(ADS1118_DO_PIN())
      temp++;
     Delay_Nus(20);
    //data |= ADS1118_DO_PIN() ;       		  // capture current MISO bit	
    ADS1118_SCK_L();     //SCK = 0;            		  // ..then set SCK low again
  }
  OSIntExit(); //�˳��ٽ�״̬
  return(temp);         
  
}

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{		
//  u8 retry=0;	
 
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
  {
//    retry++;
//    if(retry>200)return 0;
  }			  
  SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
//  retry=0;
  
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
  {
//    retry++;
//    if(retry>200)return 0;
  }	  
  
  return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����					    
}
uint16_t ADS1118_ReadWriteReg(uint16_t reg)
{
  uint16_t buf=0;
//  CPU_SR_ALLOC();
//   CPU_CRITICAL_ENTER();
   
  buf= ADS1118_WriteByte(reg>>8);
   buf=(buf<<8)| ADS1118_WriteByte(reg);
//  buf=SPI2_ReadWriteByte(reg>>8);
//  buf=(buf<<8)|(SPI2_ReadWriteByte((uint8_t)reg));
//  	CPU_CRITICAL_EXIT();	//�˳��ٽ���
  return buf;
}
void ADS1118_WriteReg(uint16_t reg)
{
  CLR_ADS_CS1();
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
  {
  }			  
  SPI_I2S_SendData(SPI2, reg>>8); //ͨ������SPIx����һ������
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
  {
  }			  
  SPI_I2S_SendData(SPI2, ((uint8_t)reg)); //ͨ������SPIx����һ������
  SET_ADS_CS1();
}
uint32_t ADS1118_ReadWord(void)
{
  uint32_t buf=0;
  CLR_ADS_CS1();
  //while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
  buf=SPI_I2S_ReceiveData(SPI2);
  buf=(buf<<8)|SPI_I2S_ReceiveData(SPI2);
  buf=(buf<<8)|SPI_I2S_ReceiveData(SPI2);
  buf=(buf<<8)|SPI_I2S_ReceiveData(SPI2);
  SET_ADS_CS1();
  return buf;
}
uint16_t ADS1118_ReadHalfWord(void)
{
  uint16_t buf=0;
  CLR_ADS_CS1();
  while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14))
  {
  }
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
  {
  }
  buf=SPI_I2S_ReceiveData(SPI2);
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
  {
  }
  buf=(buf<<8)|SPI_I2S_ReceiveData(SPI2);
  SET_ADS_CS1();
  return buf;
}
uint16_t ADS118_Convert(ADS1118_type adc,ADS1118_CH adc_ch)
{
  uint16_t vaule;
    if(adc==FLOW_ADC)
    {
      CLR_ADS_CS1();
      Delay_Nus(10);
     
      vaule=ADS1118_ReadWriteReg(0xC1e3|(adc_ch<<12));
//      
//      //vaule=ADS1118_ReadHalfWord();
       
      Delay_Nus(10);
      SET_ADS_CS1();
       Delay_Nus(10);
    }
    else if(adc==CURRENT_ADC)
    {
      CLR_ADS_CS2();
      Delay_100ns();
      vaule=ADS1118_ReadWriteReg(0xC1e3|(adc_ch<<12));
      Delay_100ns();
      SET_ADS_CS2();
    }
    return vaule;
}
uint16_t value_buf[4][12]={0};
uint8_t FilterI[4]={0},first_num=0;
uint16_t vaule[30]={0};
float Filter(ADS1118_type adc,ADS1118_CH ch) 
{
  uint32_t sum=0;
  uint32_t Average_data=0;
  uint16_t temp=0;
  ADS118_Convert(adc,ch);
  ADS118_Convert(adc,ch);
       for(uint8_t i=0;i<30;i++)
       {
       vaule[i]=ADS118_Convert(adc,ch);
      
       }
       for (uint8_t i = 0; i < 30; i++)
      {
        for (uint8_t j = i; j < 30; j++)
        {
          if (vaule[i] > vaule[j])
          {
            temp = vaule[i];
            vaule[i] = vaule[j];
            vaule[j] = temp;
          }
        }
      }
      for(uint8_t i=0;i<10;i++)
      {
        Average_data+=vaule[10+i];
      }
  value_buf[ch][FilterI[ch]++]=Average_data/10;
  
 // return Average_data/10.0/32768.0*6.144;
  first_num+=1;
  if(first_num>=12)
  {
    first_num=12;
  if(FilterI[ch]==12)
    FilterI[ch]=0; //�Ƚ��ȳ�������ƽ��ֵ
  for(uint8_t count=0;count<12;count++)
    sum+=value_buf[ch][count];
  
  }
  else
  {
    for(uint8_t count=0;count<first_num;count++)
    sum+=value_buf[ch][count];
    
  }
  return (float)(sum/(first_num*1.0)/32768.0*6.144);
}
//float Get_Avg(ADS1118_type adc,ADS1118_CH adc_ch)
//{
//  uint32_t Average_data=0;
//  uint16_t vaule[31]={0},temp=0;
//  float adc_vaule=0;
//       for(uint8_t i=0;i<31;i++)
//       {
//       vaule[i]=ADS118_Convert(adc,adc_ch);
//       }
//       for (uint8_t i = 1; i < 31; i++)
//      {
//        for (uint8_t j = i; j < 30; j++)
//        {
//          if (vaule[i] > vaule[j])
//          {
//            temp = vaule[i];
//            vaule[i] = vaule[j];
//            vaule[j] = temp;
//          }
//        }
//      }
//      for(uint8_t i=0;i<10;i++)
//      {
//        Average_data+=vaule[11+i];
//      }
//      adc_vaule=Average_data/10.0/32768.0*6.144;
//      return adc_vaule;
//}