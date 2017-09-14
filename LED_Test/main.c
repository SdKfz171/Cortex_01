#include "stm32f10x_lib.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_map.h"
#include "stm32f10x_sdio.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_exti.h"
#include "main.h"

void GPIO_Configuration(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;      
 
       // KEY
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
                                   GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_13;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
     GPIO_Init(GPIOC, &GPIO_InitStructure);
   
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
     GPIO_Init(GPIOA, &GPIO_InitStructure);
       // LED 
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4; 
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
     GPIO_Init(GPIOE, &GPIO_InitStructure);        
     
     // Touch Sensor
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ; 
     GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
     GPIO_Init(GPIOD, &GPIO_InitStructure);
     
}

void RCC_Configuration(void)
{   
     ErrorStatus HSEStartUpStatus;
     RCC_DeInit(); /* RCC system reset(for debug purpose) */
     RCC_HSEConfig(RCC_HSE_ON); /* Enable HSE */
     HSEStartUpStatus = RCC_WaitForHSEStartUp(); /* Wait till HSE is ready */
     if(HSEStartUpStatus == SUCCESS)
     {
          /* Enable Prefetch Buffer */
          FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
          FLASH_SetLatency(FLASH_Latency_2);      /* Flash 2 wait state */	
          RCC_HCLKConfig(RCC_SYSCLK_Div1);       /* HCLK = SYSCLK */
          /* PCLK2 = HCLK */
          RCC_PCLK2Config(RCC_HCLK_Div1); 
          RCC_PCLK1Config(RCC_HCLK_Div2);        /* PCLK1 = HCLK/2 */
          RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); /* PLLCLK = 8MHz * 9 = 72 MHz */
          RCC_PLLCmd(ENABLE); /* Enable PLL */ 
          /* Wait till PLL is ready */
          while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){
          }
          /* Select PLL as system clock source */
          RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
          /* Wait till PLL is used as system clock source */
          while(RCC_GetSYSCLKSource() != 0x08){
          }
     }
}

int reset(){
    GPIO_ResetBits(GPIOE, GPIO_Pin_2);
    GPIO_ResetBits(GPIOE, GPIO_Pin_3);
    GPIO_ResetBits(GPIOE, GPIO_Pin_4);
    
    return 0;
}

void BlinkLed(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, int duty){
  
  GPIO_SetBits(GPIOx, GPIO_Pin);
  delay_ms(duty);
  GPIO_ResetBits(GPIOx, GPIO_Pin);
  delay_ms(duty);

}

void LedControl(GPIO_TypeDef* GPIOx, u16 GPIO_Pin){
  if(GPIO_Pin == GPIO_Pin_2){
    GPIO_SetBits(GPIOx, GPIO_Pin_2);
    GPIO_ResetBits(GPIOx, GPIO_Pin_3);
    GPIO_ResetBits(GPIOx, GPIO_Pin_4);
  }else if(GPIO_Pin == GPIO_Pin_3){
    GPIO_ResetBits(GPIOx, GPIO_Pin_2);
    GPIO_SetBits(GPIOx, GPIO_Pin_3);
    GPIO_ResetBits(GPIOx, GPIO_Pin_4);
  }else if(GPIO_Pin == GPIO_Pin_4){
    GPIO_ResetBits(GPIOx, GPIO_Pin_2);
    GPIO_ResetBits(GPIOx, GPIO_Pin_3);
    GPIO_SetBits(GPIOx, GPIO_Pin_4);
  }else if(GPIO_Pin == GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4){
    GPIO_SetBits(GPIOx, GPIO_Pin_2);
    GPIO_SetBits(GPIOx, GPIO_Pin_3);
    GPIO_SetBits(GPIOx, GPIO_Pin_4);
  }
  else {
    GPIO_ReetBits(GPIOx, GPIO_Pin_2);
    GPIO_ResetBits(GPIOx, GPIO_Pin_3);
    GPIO_ResetBits(GPIOx, GPIO_Pin_4);
  }
}

u16 pinList[3] = {GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4};

void special(GPIO_TypeDef* GPIOx, u16 GPIO_Pin){
  reset();
  for(int i = 1; i <= 5;i++){
    BlinkLed(GPIOx, GPIO_Pin_2,100 - i*10);
    BlinkLed(GPIOx, GPIO_Pin_3,100 - i*10);
    BlinkLed(GPIOx, GPIO_Pin_4,100 - i*10);
  }
  for(int i = 1; i <= 5;i++){
    BlinkLed(GPIOx, GPIO_Pin_2,0 + i*10);
    BlinkLed(GPIOx, GPIO_Pin_3,0 + i*10);
    BlinkLed(GPIOx, GPIO_Pin_4,0 + i*10);
  }
  
  BlinkLed(GPIOx, GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_3 ,100);
  BlinkLed(GPIOx, GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_3 ,100);
  BlinkLed(GPIOx, GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_3 ,100);
  BlinkLed(GPIOx, GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_3 ,300);
  BlinkLed(GPIOx, GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_3 ,300);
  BlinkLed(GPIOx, GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_3 ,300);
  BlinkLed(GPIOx, GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_3 ,100);
  BlinkLed(GPIOx, GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_3 ,100);
  BlinkLed(GPIOx, GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_3 ,100);
}

int main(void)
{  
     u16 i, j;
  
     RCC_Configuration();
     
     RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA |  
                             RCC_APB2Periph_GPIOC |
                             RCC_APB2Periph_GPIOE |
                             RCC_APB2Periph_GPIOD , ENABLE);
     
     GPIO_Configuration();
     bool flag = 0; 
     int command = 0;
     for(;;){
       if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)){
          command += 3;
          flag = reset(); 
          LedControl(GPIOE,GPIO_Pin_2);
       }
       else if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)){
          command += 2;
          flag = reset(); 
          LedControl(GPIOE,GPIO_Pin_3);
       }
       else if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)){
          command += 5;
          flag = reset(); 
          LedControl(GPIOE,GPIO_Pin_4);
       }
       else if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3)){
          command += 7;
          flag = reset(); 
          LedControl(GPIOE,GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
       }
       else if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4))
          flag = 1;
       else if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))
          if(command >= 37)
            special(GPIOE,GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
       if(flag == 1)
          BlinkLed(GPIOE,GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4,500);
       /*
       if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)){
            GPIO_ResetBits(GPIOE, GPIO_Pin_2);
            GPIO_ResetBits(GPIOE, GPIO_Pin_3);
            GPIO_ResetBits(GPIOE, GPIO_Pin_4);
        }
        else { 
            GPIO_SetBits(GPIOE, GPIO_Pin_2);
            GPIO_SetBits(GPIOE, GPIO_Pin_3);
            GPIO_SetBits(GPIOE, GPIO_Pin_4);
        }
        */
       /*
        if(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12)){
            GPIO_ResetBits(GPIOE, GPIO_Pin_2);
            GPIO_ResetBits(GPIOE, GPIO_Pin_3);
            GPIO_ResetBits(GPIOE, GPIO_Pin_4);
        }
        else { 
            GPIO_SetBits(GPIOE, GPIO_Pin_2);
            GPIO_SetBits(GPIOE, GPIO_Pin_3);
            GPIO_SetBits(GPIOE, GPIO_Pin_4);
        }*/
        /*
        //delay_ms(1000);
        BlinkLed(GPIOE,GPIO_Pin_2);
        BlinkLed(GPIOE,GPIO_Pin_3);
        BlinkLed(GPIOE,GPIO_Pin_4);
        */
     }
  
}
