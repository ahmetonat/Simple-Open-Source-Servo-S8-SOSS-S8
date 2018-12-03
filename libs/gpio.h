
#ifndef __GPIO_H__
#define __GPIO_H__

//External LED on PC3
#define LED_PORT GPIOC
#define LED_PIN  GPIO_PIN_3
#define LED_BIT  0x08 //3
#define LED_NBIT  0xF7 //!3

#define LED_ON()  LED_PORT->ODR|=(LED_BIT);
#define LED_OFF() LED_PORT->ODR&=(LED_NBIT);
#define LED_REV() LED_PORT->ODR^=(LED_BIT);

//On board LED config:
#define LEDOB_PORT GPIOB
#define LEDOB_PIN  GPIO_PIN_5 //AO!
#define LEDOB_BIT  0x20 //5 //AO!
#define LEDOB_NBIT  0xDF //5 //AO!

#define LEDOB_OFF()  LEDOB_PORT->ODR|=(LEDOB_BIT); //Connected to other side.
#define LEDOB_ON() LEDOB_PORT->ODR&=(LEDOB_NBIT);
#define LEDOB_REV() LEDOB_PORT->ODR^=(LEDOB_BIT);


void GPIO_setup(void);

#endif
