
#include "stm8s.h"
#include "gpio.h"

void GPIO_setup(void){

  //GPIO_DeInit(LED_PORT);
  LED_PORT->ODR = GPIO_ODR_RESET_VALUE; /* Reset Output Data Register */
  LED_PORT->DDR = GPIO_DDR_RESET_VALUE; /* Reset Data Direction Register */
  LED_PORT->CR1 = GPIO_CR1_RESET_VALUE; /* Reset Control Register 1 */
  LED_PORT->CR2 = GPIO_CR2_RESET_VALUE; /* Reset Control Register 2 */
  
  //GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_OD_HIZ_FAST);
  // LED_PORT->ODR=0; //All pins off
  // LED_PORT->DDR |=(1<<LED_BIT);
  // LED_PORT->CR1 |=(1<<LED_BIT);
  // LED_PORT->CR2 &=(!(1<<LED_BIT);
  LED_PORT->DDR |=(LED_BIT);
  LED_PORT->CR1 |=(LED_BIT);
  LED_PORT->CR2 &=(LED_NBIT);


    //GPIO_DeInit(LEDOB_PORT);
  LEDOB_PORT->ODR = GPIO_ODR_RESET_VALUE; /* Reset Output Data Register */
  LEDOB_PORT->DDR = GPIO_DDR_RESET_VALUE; /* Reset Data Direction Register */
  LEDOB_PORT->CR1 = GPIO_CR1_RESET_VALUE; /* Reset Control Register 1 */
  LEDOB_PORT->CR2 = GPIO_CR2_RESET_VALUE; /* Reset Control Register 2 */
  
  //GPIO_Init(LEDOB_PORT, LEDOB_PIN, GPIO_MODE_OUT_OD_HIZ_FAST);
  LEDOB_PORT->ODR=0; //All pins off
  // LEDOB_PORT->DDR |=(1<<LEDOB_BIT);
  // LEDOB_PORT->CR1 |=(1<<LEDOB_BIT);
  // LEDOB_PORT->CR2 &=(!(1<<LEDOB_BIT));
  LEDOB_PORT->DDR |=(LEDOB_BIT);
  LEDOB_PORT->CR1 |=(LEDOB_BIT);
  LEDOB_PORT->CR2 &=(LEDOB_NBIT);

}
