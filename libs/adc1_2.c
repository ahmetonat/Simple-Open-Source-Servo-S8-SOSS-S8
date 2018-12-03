
#include "stm8s.h"
#include "adc1_2.h"


/////////////////////////////////////////////////////////
void ADC_Config(void)
//static void ADC_Config()
{
  // Init GPIO PC4 connected to ADC1, CH2:
  //GPIO_DeInit(PORTC);
  /*
  //This De-Init conflicts with LED configuration,
  // and has been done in the LED configuration; so not necessary.
  GPIOC->ODR = GPIO_ODR_RESET_VALUE; // Reset Output Data Register 
  GPIOC->DDR = GPIO_DDR_RESET_VALUE; // Reset Data Direction Register 
  GPIOC->CR1 = GPIO_CR1_RESET_VALUE; // Reset Control Register 1 
  GPIOC->CR2 = GPIO_CR2_RESET_VALUE; // Reset Control Register 2 
  */
  
  //GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT); //ADC1, CH2; PC4
#define ADC1_2_PORT GPIOC
#define ADC1_2_BIT 0x10
#define ADC1_2_NBIT 0xEF
#define ADC1_2_PIN GPIO_PIN_4
  //GPIO_MODE_IN_FL_NO_IT=0x00: From stm..gpio.h

  ADC1_2_PORT->DDR &=(ADC1_2_NBIT); //Set as input.
  ADC1_2_PORT->CR1 &=(ADC1_2_NBIT); //Set as float/Open drain, no pull up.
  ADC1_2_PORT->CR2 &=(ADC1_2_NBIT); //No INT, no slope contr.

  
  //ADC1_DeInit();

  ADC1->CSR  = ADC1_CSR_RESET_VALUE;
  ADC1->CR1  = ADC1_CR1_RESET_VALUE;
  ADC1->CR2  = ADC1_CR2_RESET_VALUE;
  ADC1->CR3  = ADC1_CR3_RESET_VALUE;
  ADC1->TDRH = ADC1_TDRH_RESET_VALUE;
  ADC1->TDRL = ADC1_TDRL_RESET_VALUE;
  ADC1->HTRH = ADC1_HTRH_RESET_VALUE;
  ADC1->HTRL = ADC1_HTRL_RESET_VALUE;
  ADC1->LTRH = ADC1_LTRH_RESET_VALUE;
  ADC1->LTRL = ADC1_LTRL_RESET_VALUE;
  ADC1->AWCRH = ADC1_AWCRH_RESET_VALUE;
  ADC1->AWCRL = ADC1_AWCRL_RESET_VALUE;

  
  /* ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_2,     
	    ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_TIM, DISABLE,
	    ADC1_ALIGN_RIGHT,
	    ADC1_SCHMITTTRIG_CHANNEL2, DISABLE);
  //    ADC1_SCHMITTTRIG_CHANNEL2|ADC1_SCHMITTTRIG_CHANNEL3, DISABLE);
  */

  //-----------------CR1 & CSR configuration --------------------
  // Configure the conversion mode and the channel to convert
  //respectively according to ADC1_ConversionMode & ADC1_Channel values  &  ADC1_Align values

  //  ADC1_ConversionConfig(ADC1_ConversionMode, ADC1_Channel, ADC1_Align);
  ADC1->CR2 &= (uint8_t)(~ADC1_CR2_ALIGN);  // Clear the align bit
  ADC1->CR2 |= (uint8_t)(ADC1_ALIGN_RIGHT);  // Configure the data alignment
  
  ADC1->CR1 |= ADC1_CR1_CONT; //Set: ADC1_CONVERSIONMODE_CONTINUOUS

  ADC1->CSR &= (uint8_t)(~ADC1_CSR_CH);  // Clear the ADC1 channels
  ADC1->CSR |= (uint8_t)(ADC1_CHANNEL_1);  // Select the ADC1 channel

    //ADC1_PrescalerConfig(ADC1_PrescalerSelection);
  ADC1->CR1 &= (uint8_t)(~ADC1_CR1_SPSEL);   // Clear the SPSEL bits
  // Select the prescaler division factor according to ADC1_PrescalerSelection values
  ADC1->CR1 |= (uint8_t)(ADC1_PRESSEL_FCPU_D2); //As fCPU/2
  
  //-----------------CR2 configuration --------------------
  // Configure the external trigger state and event respectively
  //according to NewState, ADC1_ExtTrigger
  //ADC1_ExternalTriggerConfig(ADC1_ExtTrigger, ADC1_ExtTriggerState);
  ADC1->CR2 &= (uint8_t)(~ADC1_CR2_EXTSEL);  // Clear the external trigger selection bits
  ADC1->CR2 &= (uint8_t)(~ADC1_CR2_EXTTRIG);   // Disable the selected external trigger
  ADC1->CR2 |= (uint8_t)(ADC1_EXTTRIG_TIM);  // DUMMY! Set the selected external trigger

  //------------------TDR configuration --------------------------
  // Configure the schmitt trigger channel and state respectively
  //according to ADC1_SchmittTriggerChannel & ADC1_SchmittTriggerNewState  values:
  //ADC1_SchmittTriggerConfig(ADC1_SCHMITTTRIG_CHANNEL2, DISABLE);
  ADC1->TDRL |= (uint8_t)((uint8_t)0x01 << (uint8_t)ADC1_SCHMITTTRIG_CHANNEL2);
  
  
  ADC1->CR1 |= ADC1_CR1_ADON;
  

  
  //ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE);  // Enable EOC interrupt.
  // For the first argument, see stm8_adc1.h; ADC1_IT_TypeDef
  ADC1->CSR |= (uint8_t)ADC1_IT_EOCIE;
}


////////////////////  ADC1 ISR  //////////////////////////

void ADC1_EOC_ISR(void) __interrupt(ADC1_INT)
{
  //uint8_t temp=0;

  uint16_t temph = 0;
  uint8_t templ = 0;

  //ADC_RES= ADC1_GetConversionValue(); //Read the value
  templ = ADC1->DRL;  // Read LSB first
  temph = ADC1->DRH;  // Then read MSB
  ADC_RES = (uint16_t)(templ | (uint16_t)(temph << (uint8_t)8));

  
  /* ADC1_ConversionConfig(ADC1_CONVERSIONMODE_CONTINUOUS,
			ADC1_CHANNEL_2, ADC1_ALIGN_RIGHT);*/
  //  ADC1->CR2 &= (uint8_t)(~ADC1_CR2_ALIGN);  // Clear the align bit
  //  ADC1->CR2 |= (uint8_t)(ADC1_ALIGN_RIGHT);  // Configure the data alignment
  //ADC1->CR1 |= ADC1_CR1_CONT;  // Set the continuous conversion mode
  ADC1->CR1 &= ~ADC1_CR1_CONT;  // Set single conversion mode

  ADC1->CSR &= (uint8_t)(~ADC1_CSR_CH);  // Clear the ADC1 channels
  ADC1->CSR |= (uint8_t)(ADC1_CHANNEL_2);  // Select the ADC1 channel

  ADC1->CR1 &= ~ADC1_CR1_ADON; //Stop ADC1 conversion.

  
  //ADC1_ClearITPendingBit(ADC1_IT_EOC);
   ADC1->CSR &= (uint8_t)((uint16_t)~(uint16_t)ADC1_IT_EOC);
}
