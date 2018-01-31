#include <ioCC2530.h>
#include "sys_init.h"
#include "uart.h"

void adc_put(void);
float avgTemp;
char output[]="";

/*������
-------------------------------------------------------*/
void main(void)
{       
  xtal_init();
  led_init();
  uart0_init(0x00, 0x00);            //��ʼ�����ڣ�����żУ�飬ֹͣλΪ1λ
  EA = 1;
  IEN2 |= 0x04;
  ADCIE = 1;    //3���жϿ���
  
  Uart_Send_String("Hello CC2530 - TempSensor!\r\n");
  
  ADCL &= 0x00;
  ADCH &= 0x00;
  ADCCON3  = 0x3E;                  //ѡ��1.25VΪ�ο���ѹ��12λ�ֱ��ʣ���Ƭ���¶ȴ���������    
  ADCCON1 |= 0x30;                    //ѡ��ADC������ģʽΪ�ֶ�
  ADCCON1 |= 0x40;                    //����ADת��
  
  while(1)
  {
    ADCL &= 0x00;
    ADCH &= 0x00;
    ADCCON3  = 0x3E;                  //ѡ��1.25VΪ�ο���ѹ��12λ�ֱ��ʣ���Ƭ���¶ȴ���������    
    ADCCON1 |= 0x30;                    //ѡ��ADC������ģʽΪ�ֶ�
    ADCCON1 |= 0x40;                    //����ADת��
    adc_put();
  }
}

/*adc�ɼ�����
-------------------------------------------------------*/
void adc_put(void)
{ 
  output[0] = (unsigned char)(avgTemp)/10 + 48;          //ʮλ
  output[1] = (unsigned char)(avgTemp)%10 + 48;          //��λ
  output[2] = '.';                                       //С���� 
  output[3] = (unsigned char)(avgTemp*10)%10+48;         //ʮ��λ
  output[4] = (unsigned char)(avgTemp*100)%10+48;        //�ٷ�λ
  output[5] = '\0';                                      //�ַ���������
  
  Uart_Send_String(output);
  Uart_Send_String("��\r\n");
}



#pragma vector = ADC_VECTOR
__interrupt void ADC_ISR()
{
    ADCIE = 0;
    
    unsigned int  value;
    value =  ADCL >> 2;
    value |= (ADCH << 6);               //ȡ������ת�����������value��
    avgTemp = (avgTemp+value)/2;    
    //halWait(250);
    ADCIE = 1;
}

#pragma vector = UTX0_VECTOR
__interrupt void UTX0_ISR()
{
  IEN2 &= 0x00;
  D7 = ~D7;                          //LEDϨ�𣬱�ʾת��������
  halWait(25);
  UTX0IF = 0;
  IEN2 |= 0x04;
}