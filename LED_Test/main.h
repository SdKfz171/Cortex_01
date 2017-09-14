#include "stm32f10x_type.h"

void USART1_printf(USART_TypeDef* USARTx, u8 *Data,...);


void delay_us (u32 usec);
void Delay_ms (u32 nCount);

void delay_us(u32 nCount)
{
     int i,j;
     for(; nCount != 0; nCount--){
         for(i=0; i<10; i++)asm("NOP");
     }
} 

void delay_ms(u32 nCount)
{
     for(; nCount !=0; nCount--)delay_us(1000);
}