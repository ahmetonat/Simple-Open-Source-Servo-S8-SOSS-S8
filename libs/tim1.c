
#include "stm8s.h"

#include "tim1.h"


void TIM1_setup(void){
  
  // TIM1_DeInit();
  // The following is from STM8 Std Periph Libs:
  TIM1->CR1  = TIM1_CR1_RESET_VALUE;
  TIM1->CR2  = TIM1_CR2_RESET_VALUE;
  TIM1->SMCR = TIM1_SMCR_RESET_VALUE;
  TIM1->ETR  = TIM1_ETR_RESET_VALUE;
  TIM1->IER  = TIM1_IER_RESET_VALUE;
  TIM1->SR2  = TIM1_SR2_RESET_VALUE;
  // Disable channels
  TIM1->CCER1 = TIM1_CCER1_RESET_VALUE;
  TIM1->CCER2 = TIM1_CCER2_RESET_VALUE;
  // Configure channels as inputs: 
  //  it is necessary if lock level is equal to 2 or 3
  TIM1->CCMR1 = 0x01;
  TIM1->CCMR2 = 0x01;
  TIM1->CCMR3 = 0x01;
  TIM1->CCMR4 = 0x01;
  // Then reset channel registers:
  // it also works if lock level is equal to 2 or 3
  TIM1->CCER1 = TIM1_CCER1_RESET_VALUE;
  TIM1->CCER2 = TIM1_CCER2_RESET_VALUE;
  TIM1->CCMR1 = TIM1_CCMR1_RESET_VALUE;
  TIM1->CCMR2 = TIM1_CCMR2_RESET_VALUE;
  TIM1->CCMR3 = TIM1_CCMR3_RESET_VALUE;
  TIM1->CCMR4 = TIM1_CCMR4_RESET_VALUE;
  TIM1->CNTRH = TIM1_CNTRH_RESET_VALUE;
  TIM1->CNTRL = TIM1_CNTRL_RESET_VALUE;
  TIM1->PSCRH = TIM1_PSCRH_RESET_VALUE;
  TIM1->PSCRL = TIM1_PSCRL_RESET_VALUE;
  TIM1->ARRH  = TIM1_ARRH_RESET_VALUE;
  TIM1->ARRL  = TIM1_ARRL_RESET_VALUE;
  TIM1->CCR1H = TIM1_CCR1H_RESET_VALUE;
  TIM1->CCR1L = TIM1_CCR1L_RESET_VALUE;
  TIM1->CCR2H = TIM1_CCR2H_RESET_VALUE;
  TIM1->CCR2L = TIM1_CCR2L_RESET_VALUE;
  TIM1->CCR3H = TIM1_CCR3H_RESET_VALUE;
  TIM1->CCR3L = TIM1_CCR3L_RESET_VALUE;
  TIM1->CCR4H = TIM1_CCR4H_RESET_VALUE;
  TIM1->CCR4L = TIM1_CCR4L_RESET_VALUE;
  TIM1->OISR  = TIM1_OISR_RESET_VALUE;
  TIM1->EGR   = 0x01; // TIM1_EGR_UG
  TIM1->DTR   = TIM1_DTR_RESET_VALUE;
  TIM1->BKR   = TIM1_BKR_RESET_VALUE;
  TIM1->RCR   = TIM1_RCR_RESET_VALUE;
  TIM1->SR1   = TIM1_SR1_RESET_VALUE;
  
  //TIM1_TimeBaseInit(10, TIM1_COUNTERMODE_UP,ENC_RANGE,1); //Upper limit 1000.

  // Set the Autoreload value
  TIM1->ARRH = (uint8_t)(ENC_RANGE >> 8);
  TIM1->ARRL = (uint8_t)(ENC_RANGE);
  
  // Set the Prescaler value 
  TIM1->PSCRH = (uint8_t)(ENC_Prescaler >> 8);
  TIM1->PSCRL = (uint8_t)(ENC_Prescaler);
  
  // Select the Counter Mode
  TIM1->CR1 = (uint8_t)((uint8_t)(TIM1->CR1&(uint8_t)
				  (~(TIM1_CR1_CMS | TIM1_CR1_DIR)))
			|(uint8_t)(TIM1_COUNTERMODE_UP));
  
  // Set the Repetition Counter value
  TIM1->RCR = 1;
  
  //TIM1_EncoderInterfaceConfig(TIM1_ENCODERMODE_TI12,
  //			      TIM1_ICPOLARITY_FALLING,
  //			      TIM1_ICPOLARITY_FALLING);

  //Set input line polarity:
  TIM1->CCER1 |= TIM1_CCER1_CC1P;  //IC Polarity falling on CC1P
  TIM1->CCER1 |= TIM1_CCER1_CC2P;  //IC Polarity falling on CC2P

  // Set the encoder Mode:
  TIM1->SMCR = (uint8_t)((uint8_t)(TIM1->SMCR & (uint8_t)
				   (TIM1_SMCR_MSM | TIM1_SMCR_TS))
			 |(uint8_t) TIM1_ENCODERMODE_TI12);
  
  // Select the Capture Compare 1 and the Capture Compare 2 as input
  TIM1->CCMR1 = (uint8_t)((uint8_t)(TIM1->CCMR1 & (uint8_t)(~TIM1_CCMR_CCxS)) 
                          |(uint8_t) CCMR_TIxDirect_Set);
  TIM1->CCMR2 = (uint8_t)((uint8_t)(TIM1->CCMR2 & (uint8_t)(~TIM1_CCMR_CCxS))
                          |(uint8_t) CCMR_TIxDirect_Set);

  //TIM1_Cmd(ENABLE);
  TIM1->CR1 |= TIM1_CR1_CEN;  //Enable timer.
}
