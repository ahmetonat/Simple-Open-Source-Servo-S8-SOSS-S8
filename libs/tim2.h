
#ifndef __TIM2_H__
#define __TIM2_H__


//#define TIM2_ARR 8000 //CPUFREQ=16MHz, PWMFreq=2kHz.
#define TIM2_ARR 2000 //CPUFREQ=16MHz, PWMFreq=8kHz.
//#define TIM2_ARR 1000 //CPUFREQ=16MHz, PWMFreq=16kHz.
// Higher PWM frequency, lower PWM resolution. Find a compromise.
//16kHz PWM would result in no audible noise from the motor, but control
//  quality suffers due to lack of resolution. 8kHz selected.
//See setup of TIM2.

void TIM2_Config(void); //Initialize TIM2 in PWM mode.

#endif
