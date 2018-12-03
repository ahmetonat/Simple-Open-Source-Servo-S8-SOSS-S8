
#include "stm8s.h"
#include "clock.h"


void clock_setup(void){
  
  /*
    CLK_DeInit();
  CLK_HSECmd(DISABLE);
  CLK_LSICmd(DISABLE);
  CLK_HSICmd(ENABLE);
  
  while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == FALSE);
  CLK_ClockSwitchCmd(ENABLE);
  */
  
  //CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV4);
  //CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

  // Clear High speed internal clock prescaler
  CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV);
  // Set High speed internal clock prescaler
  CLK->CKDIVR |= (uint8_t)CLK_PRESCALER_HSIDIV1;

  
  /*
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
  
  CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE,
			CLK_CURRENTCLOCKSTATE_ENABLE);
  */

  //PCKENR1:
  //TIM1-1, TIM3-0, TIM2-1, TIM4-1, UART-1, UART-0, SPI-0, I2C-0
  //10111000: 0xB8
  CLK->PCKENR1=0xB8;

  //PCKENR2:
  // CAN-0, RES-0, RES-0, RES-0, ADC-1, AWU-0, RES-0, RES-0
  // 00001000: 0x08
  CLK->PCKENR2=0x08;
  
  /*
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, ENABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);

  CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, DISABLE);
  */
  
}


//copied over from library:
uint32_t CLK_GetClockFreq(void)
{

  //Holds the different HSI Divider factors:
  const uint8_t HSIDivFactor[4] = {1, 2, 4, 8};

  //Holds the different CLK prescaler values.
  //const uint8_t CLKPrescTable[8] = {1, 2, 4, 8, 10, 16, 20, 40}; 

  uint32_t clockfrequency = 0;
  CLK_Source_TypeDef clocksource = CLK_SOURCE_HSI;
  uint8_t tmp = 0, presc = 0;
  
  /* Get CLK source. */
  clocksource = (CLK_Source_TypeDef)CLK->CMSR;
  
  if (clocksource == CLK_SOURCE_HSI)
  {
    tmp = (uint8_t)(CLK->CKDIVR & CLK_CKDIVR_HSIDIV);
    tmp = (uint8_t)(tmp >> 3);
    presc = HSIDivFactor[tmp];
    clockfrequency = HSI_VALUE / presc;
  } else if ( clocksource == CLK_SOURCE_LSI) {
    clockfrequency = LSI_VALUE;
  } else {
    clockfrequency = HSE_VALUE;
  }
  
  return((uint32_t)clockfrequency);
}
