
#ifndef _TIM4_TBASE_H_
#define _TIM4_TBASE_H_ //To prevent recursive inclusion.

//TIMER ISR Timebase section start:

#include "stm8s_tim4.h"

//To get 2kHz: 16,000,000/32=500,000 ARR=250: 2000
//To get ~3kHz: 16,000,000/32=500,000 ARR=167:2994
//To get 4kHz: 16,000,000/32=500,000 ARR=125: 4000

//For 1ms Ts:
//#define TIM4_PRESCALER TIM4_PRESCALER_128
//#define INIT_TIM4_ARR  125 // 1000  (125kHz/125=1kHz-> 1ms)
//#define MS_COUNT 1 //This many ticks of the TIM4 INT in 1ms

//For 500us Ts:
#define TIM4_PRESCALER TIM4_PRESCALER_32
#define INIT_TIM4_ARR  250 // 1000  (125kHz/125=1kHz-> 1ms)
#define MS_COUNT 2 //This many ticks of the TIM4 INT in 1ms

#define TIM4_ISRVECT 23

#define TRUE 1
#define FALSE 0

//Timer ISR sets this flag to communicate with the main program.
//Since the variable is used in two files, the header is included in both:
//extern __IO uint8_t TimerEventFlag;
extern __IO uint8_t MS_TickEventFlag;
// Furthermore, the variable must be defined in one of the two files as:
//__IO uint8_t TimerEventFlag;
//__IO uint8_t MS_TickEventFlag;

extern __IO uint8_t CTRL_EventFlag;


static __IO uint32_t TimingDelay;
static __IO uint8_t intcount;
void Delayms( uint32_t msTime );

//void TIM4_Config(void);
void SystickConfig(void); //To give a name not related with the timer number.

void TIM4_UPD_OVF_TRG_BRK_IRQHandler(void) __interrupt(TIM4_ISRVECT);
//TIMER ISR Timebase section end.


#endif
