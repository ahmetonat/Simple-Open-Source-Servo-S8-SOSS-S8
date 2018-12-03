
#ifndef _ADC1_2_H_
#define _ADC1_2_H_


#define ADC1_INT 22

extern __IO uint16_t ADC_RES;


void ADC_Config(void);

void ADC1_EOC_ISR(void) __interrupt(ADC1_INT);






#endif
