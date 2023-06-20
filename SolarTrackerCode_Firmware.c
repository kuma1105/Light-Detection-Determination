/*
===========================================
         
                           Header Section                            

===========================================
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f4xx.h"

typedef enum
{
  false = 0, true  = !false
}bool;

char buffer[100];

uint16_t  light_sensor1= 2;
uint16_t  light_sensor2= 2;
uint16_t  light_sensor3= 2;
uint16_t  light_sensor4= 2;
uint32_t  light_sensor5= 2;
/*
===========================================
         
                    Library Function Declaration                    

===========================================
*/

void Init_TIM2(){
    uint16_t PrescalerValue;
    
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   
    NVIC_Init(&NVIC_InitStructure);  
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    SystemCoreClockUpdate();PrescalerValue = 8399;
    TIM_TimeBaseStructure.TIM_Period = 99;


    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue; 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    /* TIM IT enable */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
    /* TIMX enable counter */
    TIM_Cmd(TIM2, ENABLE);
}

void Init_USART1(void){
    
   GPIO_InitTypeDef GPIO_InitStructure; 
   USART_InitTypeDef USART_InitStructure; 
   NVIC_InitTypeDef NVIC_InitStructure; 
   
   // Enable peripheral
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
   
   // Configure USART Interrupt
   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f; 
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f; 
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
   NVIC_Init(&NVIC_InitStructure); 
   
   // GPIO AF config
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
   
   // Configure GPIO(UART TX/RX) 
   GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9; // USART TX Pin Number
   GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   
   GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10; // USART RX Pin Number
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   
   // Configure UART peripheral
   USART_InitStructure.USART_BaudRate   = 115200; //Baudrate
   USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
   USART_InitStructure.USART_StopBits   = USART_StopBits_1; 
   USART_InitStructure.USART_Parity     = USART_Parity_No ; 
   USART_InitStructure.USART_HardwareFlowControl 
      = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx; 
   
   USART_Init(USART1, &USART_InitStructure); 
   
   // Enable USART receive interrupt
   USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
   
   USART_Cmd(USART1, ENABLE); 
}

void Init_ADC1_PA6() // multi channel
{
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // xxx
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  // xxx
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  // xxx
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  // xxx
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // xxx
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; // xxx
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;   
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;   // xxx
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;  // xxx
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // xxx
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);
  ADC_Cmd(ADC1, ENABLE);
  light_sensor1 = ADC_GetConversionValue(ADC1); //light_sensor1 is ADC receive value
}

void Init_ADC2_PA4()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC2, &ADC_InitStructure);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 1, ADC_SampleTime_3Cycles);
  ADC_Cmd(ADC2, ENABLE);
  light_sensor2 = ADC_GetConversionValue(ADC2); //light_sensor2 is ADC receive value
}

void Init_ADC3_PA0()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC3, &ADC_InitStructure);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);
  ADC_Cmd(ADC3, ENABLE);
  light_sensor3 = ADC_GetConversionValue(ADC3); //light_sensor3 is ADC receive value
}

/*
===========================================
         
               User-Defined Function Declaration                 

===========================================
*/

void USART_print(USART_TypeDef* USARTx, char *s){
      while(*s){
        while(!(USARTx->SR & 0x00000040));
            USART_SendData(USARTx,*s);
        *s++;    
      }

      /*
      char buffer[32];
      int print_value = 0;
      sprintf(buffer, "ADC : %d\n", print_value);     
      USART_print(USART1, buffer); 
      */  
}

void GPIO_Config(){
        
}

/*
===========================================
         
                              Main Section                                

===========================================
*/

uint16_t ADCValues[6] = {0,0,0,0,0,0};
int main()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  ADC_InitTypeDef       ADC_InitStruct;
ADC_CommonInitTypeDef ADC_CommonInitStruct;
DMA_InitTypeDef       DMA_InitStruct;
GPIO_InitTypeDef      GPIO_InitStruct;

//Enabling the related buses
RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_DMA2 , ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
  
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//Set GPIO for Analog Input
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
//Initialize the GPIOA with above configurations 
GPIO_Init(GPIOA, &GPIO_InitStruct);
    //We will set the channel as DMA_Channel_1
    DMA_InitStruct.DMA_Channel = DMA_Channel_1;
    //We will set the source as ADC2s' Data and memory ADCValues
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC2->DR;
    DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)&ADCValues;
    //Set the direction as Periphal to Memory
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
    //We will use 6 adc channel. and buffer size is 6
    DMA_InitStruct.DMA_BufferSize = 6;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //We will set data sizes as 16-bit
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    //DMA circular mode enables the dma to start when it is done
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    //Initialize the DMA
    DMA_Init(DMA2_Stream2, &DMA_InitStruct);
    //Enable the DMA
    DMA_Cmd(DMA2_Stream2, ENABLE);
  
    ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStruct);
   
    //First DeInit ADC to reset ADC
    ADC_DeInit();
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_8b;
    //Enable to read multiple channel
    ADC_InitStruct.ADC_ScanConvMode = ENABLE;
    //Enable the continuos adc voltage sampling
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStruct.ADC_ExternalTrigConv = 0;
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    //How many analog voltage values we will read
    ADC_InitStruct.ADC_NbrOfConversion = 6;
    //Initialize the ADC with above configurations.
    ADC_Init(ADC2, &ADC_InitStruct);
  
  
    ADC_RegularChannelConfig(ADC2, ADC_Channel_2, 1, ADC_SampleTime_144Cycles);//PA2
    ADC_RegularChannelConfig(ADC2, ADC_Channel_3, 2, ADC_SampleTime_144Cycles);//PA3
    ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 3, ADC_SampleTime_144Cycles);//PA4
    ADC_RegularChannelConfig(ADC2, ADC_Channel_5, 4, ADC_SampleTime_144Cycles);//PA5
    ADC_RegularChannelConfig(ADC2, ADC_Channel_6, 5, ADC_SampleTime_144Cycles);//PA6
    ADC_RegularChannelConfig(ADC2, ADC_Channel_7, 6, ADC_SampleTime_144Cycles);//PA7
  
      // Enable DMA request after last transfer
    ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);
    //Enable the ADC
    ADC_DMACmd(ADC2, ENABLE);
    ADC_Cmd(ADC2, ENABLE);
    //Start the ADC conversation
    ADC_SoftwareStartConv(ADC2);

RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
//Activate the GPIO configurations
GPIO_Init(GPIOB, &GPIO_InitStructure);
		
Init_USART1();
Init_TIM2();

    while(1) 
    {

    } 
}

/*
===========================================
         
                            Handler Section                             

===========================================
*/

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        
        ADC_SoftwareStartConv(ADC1);        
        if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == SET)    
          {
              //light_sensor1 = ADC_GetConversionValue(ADC1);
            //light_sensor5 = ADC_GetMultiModeConversionValue();
            //light_sensor5= ADC_GetConversionValue(ADC1);
              
          }
        
        
        sprintf(buffer, "$%d %d %d %d\r\n", ADCValues[1], ADCValues[2], ADCValues[3], ADCValues[4]);
            USART_print(USART1, buffer); 

    }
}

void USART1_IRQHandler(void)
{    
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
           //recv_num = USART_ReceiveData(USART1); //recv_num is receive data
           //USART_SendData(USART1, send_num); //send_num is send data
    }
}