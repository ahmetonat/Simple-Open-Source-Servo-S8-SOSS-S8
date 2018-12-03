#include "stm8s.h"

#include "tbase.h"

//void TIM4_Config(void)
void SystickConfig(void)
{
  //TIM4_DeInit();

  TIM4->CR1 = TIM4_CR1_RESET_VALUE;  //Init TIM4 registers to reset values.
  TIM4->IER = TIM4_IER_RESET_VALUE;
  TIM4->CNTR = TIM4_CNTR_RESET_VALUE;
  TIM4->PSCR = TIM4_PSCR_RESET_VALUE;
  TIM4->ARR = TIM4_ARR_RESET_VALUE;
  TIM4->SR1 = TIM4_SR1_RESET_VALUE;
  
  // Time base configuration
  //TIM4_TimeBaseInit(TIM4_PRESCALER_128, INIT_TIM4_ARR);
  //  TIM4->PSCR = (uint8_t)(TIM4_PRESCALER_128); //Set prescaler.
  TIM4->PSCR = (uint8_t)(TIM4_PRESCALER); //Set prescaler.
  TIM4->ARR = (uint8_t)INIT_TIM4_ARR;      // Set the Autoreload value


  //TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  TIM4->SR1 = (uint8_t)(~TIM4_FLAG_UPDATE); //Clear pending update INT
  
  //TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);  // Update Interrupt Enable
  TIM4->IER |= (uint8_t)TIM4_IT_UPDATE;  //Enable INT
  //TIM4->IER &= (uint8_t)(~TIM4_IT_UPDATE);  //This would disable INT.
  
  //TIM4_ARRPreloadConfig(ENABLE);          // ARRPreload Enable
  
  //TIM4_Cmd(ENABLE);                       // Enable TIM4
  TIM4->CR1 |= TIM4_CR1_CEN;
  //TIM4->CR1 &= (uint8_t)(~TIM4_CR1_CEN);  //This would disable TIM4


  intcount=0; //Initialize INT counter...
}

//INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_BRK_IRQHandler, 11)
void TIM4_UPD_OVF_TRG_BRK_IRQHandler(void) __interrupt(TIM4_ISRVECT)
{

  
  //TimerEventFlag = TRUE; // This variable is exported to be used in main.c
  ++intcount;
  if (intcount >=MS_COUNT){
    intcount=0;
    MS_TickEventFlag = TRUE; // This variable is exported to be used in main.c
  }
  
  CTRL_EventFlag=TRUE;  //CTRL event at each ISR.

  if (TimingDelay != 0x00){  // Delayms function counter.
    TimingDelay --;
  }
 
  //TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
  TIM4->SR1 = (uint8_t)(~TIM4_IT_UPDATE); //Clear IT pending bit in software.
}


void Delayms( uint32_t msTime ){
  //Blocking delay timer. 
  //Depends on the TIM4 overflow handler.
  TimingDelay = msTime ;
  while (TimingDelay != 0);   //Burn off time while the timer ISR occurs.
}


