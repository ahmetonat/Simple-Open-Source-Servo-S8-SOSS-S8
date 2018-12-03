

#include "stm8s.h"
#include "tim2.h"

//static void TIM2_Config(void){ //TIM2 in PWM mode.
void TIM2_Config(void){ //TIM2 in PWM mode.
  //Processor must be initialized to 16MHz (CPUDIV=1)
  // Time base configuration for 2kHZ PWM: (16MHZ/(7999+1))=2kHz
  //TIM2_TimeBaseInit(TIM2_PRESCALER_1, TIM2_ARR-1); //7999...  

  TIM2->PSCR = (uint8_t)(TIM2_PRESCALER_1);

  TIM2->ARRH = (uint8_t)((TIM2_ARR-1) >> 8);
  TIM2->ARRL = (uint8_t)(TIM2_ARR-1);

  
  //TIM2_OC1Init(TIM2_OCMODE_PWM1, // PWM1 Mode configuration: Channel1 PD4
  //	       TIM2_OUTPUTSTATE_ENABLE,0, TIM2_OCPOLARITY_HIGH);


  TIM2->CCER1 &= (uint8_t)(~( TIM2_CCER1_CC1E | TIM2_CCER1_CC1P));
  // Set Output State & Output Polarity
  TIM2->CCER1 |= (uint8_t)((uint8_t)(TIM2_OUTPUTSTATE_ENABLE&TIM2_CCER1_CC1E )|
                           (uint8_t)(TIM2_OCPOLARITY_HIGH & TIM2_CCER1_CC1P));
  
  // Reset the Output Compare Bits  & Set the Ouput Compare Mode
  TIM2->CCMR1 = (uint8_t)((uint8_t)(TIM2->CCMR1 & (uint8_t)(~TIM2_CCMR_OCM)) |
                          (uint8_t)TIM2_OCMODE_PWM1);
  
  //Set the Pulse value 
  TIM2->CCR1H = (uint8_t)(0); //TIM2_Pulse replaced with 0.
  TIM2->CCR1L = (uint8_t)(0);
  //-------------------------

  
  //TIM2_OC1PreloadConfig(ENABLE);
  TIM2->CCMR1 |= (uint8_t)TIM2_CCMR_OCxPE; // Init to 0% PWM.
  //-------------------------
  

  //TIM2_OC2Init(TIM2_OCMODE_PWM1, // PWM1 Mode configuration: Channel2 PD3
  //	       TIM2_OUTPUTSTATE_ENABLE,0, TIM2_OCPOLARITY_HIGH);

  // Disable Ch1: Reset the CCE Bit, Set the Output State, the Output Polarity
  TIM2->CCER1 &= (uint8_t)(~( TIM2_CCER1_CC2E |  TIM2_CCER1_CC2P ));
  // Set the Output State & Set the Output Polarity
  TIM2->CCER1 |= (uint8_t)((uint8_t)(TIM2_OUTPUTSTATE_ENABLE&TIM2_CCER1_CC2E)|
                           (uint8_t)(TIM2_OCPOLARITY_HIGH& TIM2_CCER1_CC2P));
  
  
  // Reset Output Compare Bits & Set Output Compare Mode
  TIM2->CCMR2 = (uint8_t)((uint8_t)(TIM2->CCMR2 & (uint8_t)(~TIM2_CCMR_OCM)) | 
                          (uint8_t)TIM2_OCMODE_PWM1);
  
  // Set the Pulse value
  TIM2->CCR2H = (uint8_t)(0); //TIM2_Pulse replaced with 0.
  TIM2->CCR2L = (uint8_t)(0);
  //-------------------------
  
  //TIM2_OC2PreloadConfig(ENABLE);
  TIM2->CCMR2 |= (uint8_t)TIM2_CCMR_OCxPE; // Init to 0% PWM.
  //-------------------------

  //TIM2_ARRPreloadConfig(ENABLE);
  TIM2->CR1 |= (uint8_t)TIM2_CR1_ARPE;
  //-------------------------
  
  //TIM2_Cmd(ENABLE);                  // TIM2 enable counter
  TIM2->CR1 |= (uint8_t)TIM2_CR1_CEN;  // TIM2 enable counter

}
