#include <ioCC2530.h>
#include "sys_init.h"
#include "uart.h"

void adc_put(void);
float avgTemp;
char output[]="";

/*主函数
-------------------------------------------------------*/
void main(void)
{       
  xtal_init();
  led_init();
  uart0_init(0x00, 0x00);            //初始化串口：无奇偶校验，停止位为1位
  EA = 1;
  IEN2 |= 0x04;
  ADCIE = 1;    //3个中断开启
  
  Uart_Send_String("Hello CC2530 - TempSensor!\r\n");
  
  ADCL &= 0x00;
  ADCH &= 0x00;
  ADCCON3  = 0x3E;                  //选择1.25V为参考电压；12位分辨率；对片内温度传感器采样    
  ADCCON1 |= 0x30;                    //选择ADC的启动模式为手动
  ADCCON1 |= 0x40;                    //启动AD转化
  
  while(1)
  {
    ADCL &= 0x00;
    ADCH &= 0x00;
    ADCCON3  = 0x3E;                  //选择1.25V为参考电压；12位分辨率；对片内温度传感器采样    
    ADCCON1 |= 0x30;                    //选择ADC的启动模式为手动
    ADCCON1 |= 0x40;                    //启动AD转化
    adc_put();
  }
}

/*adc采集函数
-------------------------------------------------------*/
void adc_put(void)
{ 
  output[0] = (unsigned char)(avgTemp)/10 + 48;          //十位
  output[1] = (unsigned char)(avgTemp)%10 + 48;          //个位
  output[2] = '.';                                       //小数点 
  output[3] = (unsigned char)(avgTemp*10)%10+48;         //十分位
  output[4] = (unsigned char)(avgTemp*100)%10+48;        //百分位
  output[5] = '\0';                                      //字符串结束符
  
  Uart_Send_String(output);
  Uart_Send_String("℃\r\n");
}



#pragma vector = ADC_VECTOR
__interrupt void ADC_ISR()
{
    ADCIE = 0;
    
    unsigned int  value;
    value =  ADCL >> 2;
    value |= (ADCH << 6);               //取得最终转化结果，存入value中
    avgTemp = (avgTemp+value)/2;    
    //halWait(250);
    ADCIE = 1;
}

#pragma vector = UTX0_VECTOR
__interrupt void UTX0_ISR()
{
  IEN2 &= 0x00;
  D7 = ~D7;                          //LED熄灭，表示转换结束，
  halWait(25);
  UTX0IF = 0;
  IEN2 |= 0x04;
}