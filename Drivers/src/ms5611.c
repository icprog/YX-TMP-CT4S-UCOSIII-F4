/*******************************************************************************
 *CopyRight Reserved:	
 *Filename��			MS561101BA.h
 *Description��     	        ��ײ��ѹ�������� 
 *Date:
 *Author:		
*******************************************************************************/

#include "ms5611.h"

#include "os.h"




//��������
#define     MS561101BA_SlaveAddress     0xee   //����������IIC�����еĴӵ�ַ��Ӳ����CSBΪ�ߣ�ȡ��Ϊ��
#define     MS561101BA_RST              0x1E    //���帴λָ��

//D1����ѹ��������λ����
#define     MS561101BA_D1_OSR_256       0x40     
#define     MS561101BA_D1_OSR_512       0x42 
#define     MS561101BA_D1_OSR_1024      0x44 
#define     MS561101BA_D1_OSR_2048      0x46 
#define     MS561101BA_D1_OSR_4096      0x48 

//D2�����¶ȣ�����λ����
#define     MS561101BA_D2_OSR_256       0x50 
#define     MS561101BA_D2_OSR_512       0x52 
#define     MS561101BA_D2_OSR_1024      0x54 
#define     MS561101BA_D2_OSR_2048      0x56 
#define     MS561101BA_D2_OSR_4096      0x58 

//ADC��ȡ��PROM��ȡ
#define     MS561101BA_ADC_RD           0x00     //ADֵ��ȡ
#define     MS561101BA_PROM_RD          0xA0     //PROM��ȡ
#define     MS561101BA_PROM_CRC         0xAE     //CRCУ���ȡ


/****************************** MS561101BA ��������*****************************/

//����ȫ�����ر���
static uint16_t Cal_C[8];	                         //���ڴ��PROM�е�8������
static uint32_t D1_Pres,D2_Temp;	                 //���ѹ�����¶�
static float dT,TEMP;                            
static double OFF_,SENS;
static float Pressure;				 //����ѹ
static float OFF2,SENS2,T2;	         //�¶�У��ֵ

/**
  * @brief  PRESS I/O����
  * @param  ��
  * @retval ��
  */
static void PRESS_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*
********************************************************************************
�������ƣ�void PRESS_I2C_Start(void) 
�������ܣ�I2C��ʼ�ź�
��ڲ�������
���ڲ�������
********************************************************************************
*/
void PRESS_I2C_Start(void) 
{ 
    PRESS_SDA_OUT();        
    PRESS_SET_SDA();
    PRESS_SET_SCL();
    Delay_Nus(1);
    PRESS_CLR_SDA();
    Delay_Nus(1);
    PRESS_CLR_SCL();    
} 

/*
********************************************************************************
�������ƣ�void PRESS_I2C_Stop(void)  
�������ܣ�I2Cֹͣ�ź�
��ڲ�������
���ڲ�������
********************************************************************************
*/
void PRESS_I2C_Stop(void) 
{ 
    PRESS_SDA_OUT(); 
    PRESS_CLR_SDA();
    PRESS_SET_SCL();
    Delay_Nus(1);
    PRESS_SET_SDA();
    Delay_Nus(1);
} 

/*
********************************************************************************
�������ƣ�void PRESS_I2C_SendACK(uint8_t ack) 
�������ܣ�I2C����Ӧ���ź�
��ڲ�����(0:ACK 1:NAK)
���ڲ�������
********************************************************************************
*/
void PRESS_I2C_SendACK(uint8_t ack)  
{ 
    PRESS_SDA_OUT(); 
    if(ack == 1)
    {
        PRESS_SET_SDA(); 
    }
    if(ack == 0)
    {
        PRESS_CLR_SDA(); 
    }
    PRESS_SET_SCL();
    Delay_Nus(1);
    PRESS_CLR_SCL();
    Delay_Nus(1);
} 

/*
********************************************************************************
�������ƣ�uint8_t PRESS_I2C_RecvACK(void)
�������ܣ�I2C����Ӧ���ź�
��ڲ�������
���ڲ���������Ӧ���ź�
********************************************************************************
*/
uint8_t PRESS_I2C_RecvACK(void)
{
    uint16_t rn = 0;
    PRESS_CLR_SCL();                    //����ʱ����
    PRESS_SDA_IN();
    PRESS_SET_SCL();                   //����ʱ����
    Delay_Nus(1);                   //��ʱ
    while(PRESS_GET_SDA())
    {
        rn++;
        if(rn>1000)	  
        {
          return 1; 
        }
    }
    PRESS_CLR_SCL();                   //����ʱ����
    Delay_Nus(1);                  //��ʱ
    return 0;
}
/*
********************************************************************************
�������ƣ�void PRESS_I2C_SendByte(uchar dat)
�������ܣ�I2Cд��1�ֽ�����
��ڲ�����д�������
���ڲ�������
********************************************************************************
*/
void PRESS_I2C_SendByte(uint8_t dat)
{
    uint8_t Rn = 8;
     CPU_SR_ALLOC();


    CPU_CRITICAL_ENTER();	
    PRESS_SDA_OUT(); 
    while(Rn--)
    {
        PRESS_CLR_SCL();
        Delay_Nus(1); 
        if (dat & 0x80)
        {
            PRESS_SET_SDA();
        }
        else
        {
            PRESS_CLR_SDA();
        }
        dat <<= 1;  
        PRESS_SET_SCL();
        Delay_Nus(1);
    }
    PRESS_I2C_RecvACK();
     CPU_CRITICAL_EXIT();
}

/*
********************************************************************************
�������ƣ�uint8_t PRESS_I2C_RecvByte (void)
�������ܣ�I2C�����ֽ�����
��ڲ�������
���ڲ��������ؽ��յ�����
********************************************************************************
*/
uint8_t PRESS_I2C_RecvByte(void)
{
    uint8_t Rn=8,dat=0;
     CPU_SR_ALLOC();


    CPU_CRITICAL_ENTER();	
    PRESS_SET_SDA();
    PRESS_SDA_IN();
    while(Rn--)
    {
        dat <<= 1;
        PRESS_CLR_SCL(); 
        Delay_Nus(1);
        PRESS_SET_SCL();
        Delay_Nus(1);
        if(PRESS_GET_SDA())
        {
            dat++;
        }
    }
    PRESS_CLR_SCL(); 
    CPU_CRITICAL_EXIT();
    return dat;
}

/*
********************************************************************************
�������ƣ�void MS561101BA_RESET(void)
�������ܣ�MS561101BA��ѹоƬ��λ����
��ڲ�������
���ڲ�������
********************************************************************************
*/
void MS561101BA_RESET(void)
{	
    PRESS_I2C_Start();
    PRESS_I2C_SendByte(MS561101BA_SlaveAddress);
    PRESS_I2C_SendByte(MS561101BA_RST);
    PRESS_I2C_Stop();
}

/*
********************************************************************************
�������ƣ�void MS561101BA_PROM_READ(void)
�������ܣ�MS561101BA��ѹоƬPROM��ȡ
��ڲ�������
���ڲ�������
********************************************************************************
*/
void MS561101BA_PROM_READ(void)
{
    uint8_t d1 = 0,d2 = 0,i = 0;                                      
    for(i = 0;i < 8; i++)
    {
        PRESS_I2C_Start();
        PRESS_I2C_SendByte(MS561101BA_SlaveAddress);
        PRESS_I2C_SendByte((MS561101BA_PROM_RD+i*2));             //��ȡ��7�����ݣ�У��δ����
    
        PRESS_I2C_Start();
        PRESS_I2C_SendByte(MS561101BA_SlaveAddress+1);
        d1=PRESS_I2C_RecvByte();
        PRESS_I2C_SendACK(0);
        d2=PRESS_I2C_RecvByte();
        PRESS_I2C_SendACK(1);
        PRESS_I2C_Stop();
        Delayms(5);
        
        Cal_C[i]=((uint16_t)d1 << 8) | d2;                    
    }
}

/*
********************************************************************************
�������ƣ�int64u MS561101BA_DO_CONVERSION(uint8_t command)
�������ܣ�MS561101BA��ѹоƬ��ѹ���¶ȵ�24λֵ�Ķ�ȡ
��ڲ�����command����ȡ������λ���趨��������ת��
���ڲ���������ADC��ת�����
********************************************************************************
*/
uint32_t MS561101BA_DO_CONVERSION(uint8_t command)
{
    uint32_t conversion=0;
    uint8_t conv1,conv2,conv3; 
    
//    CPU_SR_ALLOC();
//    
//    
//    CPU_CRITICAL_ENTER();
//    OSIntEnter();                                               /* Tell uC/OS-III that we are starting an ISR           */
//    CPU_CRITICAL_EXIT();//�����ٽ�״̬
    							//�����ж�
    //�趨ת��������λ����ת��
    PRESS_I2C_Start();
    PRESS_I2C_SendByte(MS561101BA_SlaveAddress);
    PRESS_I2C_SendByte(command);
    PRESS_I2C_Stop();
//     OSIntExit(); //�˳��ٽ�״̬
    							//�����ж�
    
    Delayms(10);
    
//    CPU_CRITICAL_ENTER();
//    OSIntEnter();                                               /* Tell uC/OS-III that we are starting an ISR           */
//    CPU_CRITICAL_EXIT();//�����ٽ�״̬//�����ٽ�״̬
//    							//�����ж�
    //ADCֵ��ȡ
    PRESS_I2C_Start();
    PRESS_I2C_SendByte(MS561101BA_SlaveAddress);
    PRESS_I2C_SendByte(0);
    
    //I2C��MS5611-01BA��ȡ����
    PRESS_I2C_Start();
    PRESS_I2C_SendByte(MS561101BA_SlaveAddress+1);
    conv1=PRESS_I2C_RecvByte();
    PRESS_I2C_SendACK(0);
    conv2=PRESS_I2C_RecvByte();
    PRESS_I2C_SendACK(0);
    conv3=PRESS_I2C_RecvByte();
    PRESS_I2C_SendACK(1);
    PRESS_I2C_Stop();
     
   						//�����ж�
    
    conversion=conv1*65535+conv2*256+conv3;
//    OSIntExit(); //�˳��ٽ�״̬
    
    return conversion;
}

/*
********************************************************************************
�������ƣ�void MS561101BA_getTemperature(uint8_t OSR_Temp)
�������ܣ�MS561101BA�¶�ֵת��
��ڲ�����OSR_Temp���¶�λ��ָ��
���ڲ�������
********************************************************************************
*/
float MS561101BA_getTemperature(uint8_t OSR_Temp)
{
    D2_Temp = MS561101BA_DO_CONVERSION(OSR_Temp);
    dT = (float)(int32_t)(D2_Temp - (((uint32_t)Cal_C[5]) << 8));
    TEMP = 2000+dT*((uint32_t)Cal_C[6])/8388608;
    return TEMP;

}

/*
********************************************************************************
�������ƣ�float MS561101BA_getPressure(uint8_t OSR_Pres)
�������ܣ�MS561101BA��ѹֵת��
��ڲ�����OSR_Pres����ѹת��λ��ָ��
���ڲ�����������ѹֵ
********************************************************************************
*/
float MS561101BA_getPressure(uint8_t OSR_Pres)
{
    D1_Pres= MS561101BA_DO_CONVERSION(OSR_Pres);
    OFF_ = (uint32_t)Cal_C[2]*65536+((uint32_t)Cal_C[4]*dT)/128;
    SENS = (uint32_t)Cal_C[1]*32768+((uint32_t)Cal_C[3]*dT)/256;
    
    if(TEMP < 2000)
    {
        // ��20���¶����¶Ȳ���
        T2 = (dT*dT) / 0x80000000;
        OFF2 = 2.5*(TEMP - 2000)*(TEMP - 2000);
        SENS2 = 1.25*(TEMP - 2000)*(TEMP - 2000);
        if(TEMP < (-1500))
        {
            OFF2 = OFF2 + 7*(TEMP+1500)*(TEMP+1500);
            SENS2 = SENS2 + 11*(TEMP+1500)*(TEMP+1500)/2;
        }
        TEMP = TEMP - T2;
        OFF_ = OFF_ - OFF2;
        SENS = SENS - SENS2;	
    }
    
    Pressure=(D1_Pres*SENS/2097152-OFF_)/32768;
    return Pressure;                                        //�õ�barֵ
}
/*
********************************************************************************
�������ƣ�void MS561101BA_Init(void)
�������ܣ�MS561101BA��ʼ����ʵ�ָ�λ��prom��ȡ
��ڲ�������
���ڲ�������
********************************************************************************
*/
void MS561101BA_Init(void)
{
PRESS_GPIO_Config();  
    MS561101BA_RESET();                                //оƬ��λ
    MS561101BA_PROM_READ();                            //��ȡPROMֵ
}

/*
********************************************************************************
�������ƣ�void MS561101BA_Pressure_Read(void)
�������ܣ�MS561101BA��ѹֵ��ȡ
��ڲ�������
���ڲ�����Ϊkpa��ѹֵ
********************************************************************************
*/
float MS561101BA_Pressure_Read(void)
{
    float Pre = 0;
    //MS561101BA_getTemperature(MS561101BA_D2_OSR_4096);       //����Ҫ�����¶Ⱥ������ܲ�����ѹ
    Pre = MS561101BA_getPressure(MS561101BA_D1_OSR_4096);
    Pre = Pre / 1000;                                        //���kbar
    return Pre;
}

/*
********************************************************************************
�������ƣ�float MS561101BA_Temperature_Read(void)
�������ܣ�MS561101BA�¶�ֵ��ȡ
��ڲ�������
���ڲ��������¶�ֵ
********************************************************************************
*/
float MS561101BA_Temperature_Read(void)
{
    float temp = 0;
    temp = MS561101BA_getTemperature(MS561101BA_D2_OSR_4096);
    temp = temp / 100;                                        //����¶�ֵ
    return temp;
}
