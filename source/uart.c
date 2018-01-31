#include <ioCC2530.h>             //引入CC2530所对应的头文件（包含各SFR的定义）
#include "uart.h"

/*发送字节
-------------------------------------------------------*/
void Uart_Send_char(char ch)
{
  U0DBUF = ch;
}

/*发送字符串
-------------------------------------------------------*/
void Uart_Send_String(char *Data)
{  
  while (*Data != '\0')
  {
    Uart_Send_char(*Data++);
  }
}

/*接收字节
-------------------------------------------------------*/
int Uart_Recv_char(void)
{
  int ch;
    
  while (URX0IF == 0);
  ch = U0DBUF;
  URX0IF = 0;
  return ch;
}