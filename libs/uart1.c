


#include "uart1.h"
const char  HEX_CHARS[]="0123456789ABCDEF";


void UART1_Config(void){

  uint32_t BaudRate_Mantissa = 0, BaudRate_Mantissa100 = 0;

  //UART1_DeInit();

  // Clear the Idle Line Detected bit in the status register by a read
  //to the UART1_SR register followed by a Read to the UART1_DR register
  (void)UART1->SR;
  (void)UART1->DR;
  UART1->BRR2 = UART1_BRR2_RESET_VALUE;  // Set UART1_BRR2 to reset value 0x00 
  UART1->BRR1 = UART1_BRR1_RESET_VALUE;  // Set UART1_BRR1 to reset value 0x00 
  UART1->CR1 = UART1_CR1_RESET_VALUE;  // Set UART1_CR1 to reset value 0x00 
  UART1->CR2 = UART1_CR2_RESET_VALUE;  // Set UART1_CR2 to reset value 0x00 
  UART1->CR3 = UART1_CR3_RESET_VALUE;  // Set UART1_CR3 to reset value 0x00 
  UART1->CR4 = UART1_CR4_RESET_VALUE;  // Set UART1_CR4 to reset value 0x00 
  UART1->CR5 = UART1_CR5_RESET_VALUE;  // Set UART1_CR5 to reset value 0x00 
  UART1->GTR = UART1_GTR_RESET_VALUE;
  UART1->PSCR = UART1_PSCR_RESET_VALUE;
  
  
  // UART1 configuration: 115200 81N
  // Receive and transmit enabled
  //  UART1 Synch Clock disabled

  /*  UART1_Init((uint32_t)115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1,
	     UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE,
	     UART1_MODE_TXRX_ENABLE);

  */
  
  UART1->CR1 &= (uint8_t)(~UART1_CR1_M);    // Clear the word length bit   
  UART1->CR1 |= (uint8_t)UART1_WORDLENGTH_8D;  // Set the word length as desired.
  

  UART1->CR3 &= (uint8_t)(~UART1_CR3_STOP);    // Clear the STOP bits 
  UART1->CR3 |= (uint8_t)UART1_STOPBITS_1;    // Set the STOP bits number as desired.
 
  UART1->CR1 &= (uint8_t)(~(UART1_CR1_PCEN | UART1_CR1_PS  ));//Clear Parity Control bit 
  UART1->CR1 |= (uint8_t)UART1_PARITY_NO; // Set the Parity Control bit to UART1_Parity value 

  
  UART1->BRR1 &= (uint8_t)(~UART1_BRR1_DIVM);    //Clear the LSB mantissa of UART1DIV  
  UART1->BRR2 &= (uint8_t)(~UART1_BRR2_DIVM);    // Clear the MSB mantissa of UART1DIV  
  UART1->BRR2 &= (uint8_t)(~UART1_BRR2_DIVF);    // Clear the Fraction bits of UART1DIV 

  // Set the UART1 BaudRates in BRR1 and BRR2 registers according to UART1_BaudRate value 
  BaudRate_Mantissa    = ((uint32_t)CLK_GetClockFreq() / (BAUD_SETT << 4));
  BaudRate_Mantissa100 = (((uint32_t)CLK_GetClockFreq() * 100) / (BAUD_SETT << 4));
  // Set the fraction of UART1DIV  
  UART1->BRR2 |= (uint8_t)((uint8_t)(((BaudRate_Mantissa100 -
				       (BaudRate_Mantissa * 100)) << 4) / 100) & (uint8_t)0x0F); 
  UART1->BRR2 |= (uint8_t)((BaudRate_Mantissa >> 4) & (uint8_t)0xF0);   // Set the MSB mantissa of UART1DIV  
  UART1->BRR1 |= (uint8_t)BaudRate_Mantissa;  // Set the LSB mantissa of UART1DIV  

  // Disable the Transmitter and Receiver before setting the LBCL, CPOL and CPHA bits 
  UART1->CR2 &= (uint8_t)~(UART1_CR2_TEN | UART1_CR2_REN); 
  // Clear the Clock Polarity, lock Phase, Last Bit Clock pulse 
  UART1->CR3 &= (uint8_t)~(UART1_CR3_CPOL | UART1_CR3_CPHA | UART1_CR3_LBCL); 
  // Set the Clock Polarity, lock Phase, Last Bit Clock pulse 
  UART1->CR3 |= (uint8_t)((uint8_t)UART1_SYNCMODE_CLOCK_DISABLE &
			  (uint8_t)(UART1_CR3_CPOL |
				    UART1_CR3_CPHA |
				    UART1_CR3_LBCL));  
  
  UART1->CR2 |= (uint8_t)UART1_CR2_TEN;      // Set the Transmitter Enable bit 
  //UART1->CR2 &= (uint8_t)(~UART1_CR2_TEN);   //TX disable.
  
  UART1->CR2 |= (uint8_t)UART1_CR2_REN;      // Set the Receiver Enable bit 
  //UART1->CR2 &= (uint8_t)(~UART1_CR2_REN);  //RX disable.
  
  // Set the Clock Enable bit, lock Polarity, lock Phase and Last Bit Clock 
  //pulse bits according to UART1_Mode value 
  //Syncmode Clock Disable:
  UART1->CR3 &= (uint8_t)(~UART1_CR3_CKEN);     // Clear the Clock Enable bit 
}

/////////////////////////////////////////////////////////

void putstr (char* c){ //Blocking put string.
  uint8_t i;
  i=0;
  while (c[i]){  //until null termination is reached...
    putchar(c[i++]);
  }
  //return 1;
}

/////////////////////////////////////////////////////////

void SendBufHex(char *buf, uint8_t bufsize) { //Send buffer consisting of 8 bit vars.
    uint8_t i;
    char ch;
    for (i = 0; i < bufsize; i++) {
        ch = *buf++;
	putchar(HEX_CHARS[(ch >> 4) &0x0F]);
	putchar(HEX_CHARS[ch & 0x0F]);
    }
}
 
/////////////////////////////////////////////////////////
 
void putchar (char c) //Send one character and return.
{
  UART1->DR = c;  // Write char to transmit data register.
  
  // Loop until the end of transmission:
  while ((UART1->SR & (uint8_t)UART1_FLAG_TXE) == (uint8_t)0x00);
  //return (c); //TODO: No need to send back the character?
}

/////////////////////////////////////////////////////////

char getchar (void) //Busy wait get one character.
{
  char c;
  // Loop until the Read data register flag is SET 
  while ((UART1->SR & (uint8_t)UART1_FLAG_RXNE) == (uint8_t)0x00);
  c = (uint8_t)UART1->DR;
  return (c);
}


char getchar_nb (void) //Non blocking get one character.
{
  char c;
  // If the read data register flag is SET ,
  if ((UART1->SR & (uint8_t)UART1_FLAG_RXNE) != (uint8_t)0x00){
    c = (uint8_t)UART1->DR; //Read the data:
    return (c);
  }else{
    return ('\0'); //Else return a null.
  }
}
