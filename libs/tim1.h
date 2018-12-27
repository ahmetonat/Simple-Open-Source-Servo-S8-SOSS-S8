
#ifndef __TIM1_H__
#define __TIM1_H__

#define ENC_RANGE 10000
#define ENC_ABS_RANGE 50000

//AO!: Prescaler in the encoder mode is not documented well. 
//If the final value is even, the encoder will not work correctly and drift.
//The final prescaler value will be the value below +1. 
//for a good explanation, see:
//https://stackoverflow.com/questions/39450610/stm32f4-encoder-count-is-changing-when-it-should-not
#define ENC_Prescaler 0 //Used in TIM1 prescaler.

void TIM1_setup(void);

#endif
