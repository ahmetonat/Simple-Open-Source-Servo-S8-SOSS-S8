#include "stm8s.h"
#include "HT1621_NoLib.h"
#include "tbase.h"
#include "tim1.h"
#include "tim2.h"
#include "gpio.h"
#include "clock.h"
#include "adc1_2.h"
#include "uart1.h"

#define USELCD  //LCD will be used.

//If next line is commented out, square wave reference will be used:
#define ANALOG_REFERENCE //Pot will be used for reference generation.

#define U_INT_T int32_t  //Define control signal type as such.

//CAUTION: Ts is no longer used. Sub ms sampling times now...
//#define Ts 1 //Control sampling time in 'ms'
//#define Ts 20
#define REF_PERIOD 1000 //Reference changes every 1000ms (1s).
#define REF_VAL_1 00    //Reference angle 1.
#define REF_VAL_2 300   //Reference angle 2. (in encoder counts)
#define REF_VALS_MAX 2  //This many reference values.

#define PRINT_PERIOD  2 //Printout every 2ms to serial port.

//PIN Connections:
// Encoder inputs:
// TIM1 CH1: PC6 (After remapping of AFR0)
// TIM1 CH2: PC7 (After remapping of AFR0)
//
// PWM outputs:
// TIM2 CH1: PC5 (After remapping of AFR0)
// TIM2 CH2: PD3
// LED: PC3
//
// LCD:
// CS: PA1
// WR: PA2
// DAT: PA3
//
// PD5: UART1 TX
// PD6: UART1 RX
//
// PC4: AIN2 (Reference is captured from here.


__IO uint8_t MS_TickEventFlag; //External variable modified in SysTick timer ISR
__IO uint8_t CTRL_EventFlag; //time to execute control.

typedef struct {

  int16_t Kp, Ki; 
  int16_t Kd;
  int8_t Ki_div, Kd_div;
  U_INT_T u_int; //It might be necessary to change its type. See above typedef.
  int16_t ref, error, sum_error, dif_error, prev_error;
  int16_t ref_filt; //Filter running sum  for analog reference input voltage.
}ControlValues_t;

void apply_control (ControlValues_t *ctrl);


typedef struct {
    uint16_t reading_curr, reading_prev;
  int16_t value;
__IO  uint8_t tim1_msb, tim1_lsb, tim1_tmp;
}EncoderValues_t;

void read_enc(EncoderValues_t *enc);

uint8_t  cpstr (char * input, char* buffer, uint8_t location);

__IO uint16_t ADC_RES; //Required for shared lib with ADC ISR.


void main(){     
  
  uint8_t PRINT_count;  //For timing serial print events from ms ISR.
  uint16_t REF_count;   //For generating step reference timing from ms ISR.
  uint8_t ref_seq_no;   // Which reference to generate next.

  int16_t references[REF_VALS_MAX]; //Array holding step reference levels.
                                    //Filled up later, below.

  EncoderValues_t enc;  // Encoder variables structure
  ControlValues_t ctrl; // Control variables structure
  
  uint8_t printing;     //Print buffer active flag.
  uint8_t printbuf[40];
  //TODO: Make this into a circular buffer!:(Abandoned to avoid overhead)
  uint8_t print_bufloc;
  uint8_t print_buflen; //Current print buffer contents are this long.

  //Define some messages:
  const char STR_ready[]="\n\n\n\rReady for transmissions.\n\r";
  const char STR_info[]="Simple Open Servo S8\n\rVersion 09\n\n\n\r";
  const char STR_adcdesc[]="ADC1: ";
  const char STR_hexheader[]="0x";
  //For printing in HEX:
  const char  HEX_CHARS[]="0123456789ABCDEF";

#ifdef USELCD
  HT1621Values Seg_LCD; 
#endif
  
  references[0]=REF_VAL_1;
  references[1]=REF_VAL_2;

  PRINT_count=0;  //Counts ms until information printout.
  REF_count=0;
  ref_seq_no=0;
  
  //Could be done in a "ctrl_init" function, but this is simpler.
  ctrl.ref=references[REF_count];
  //At 12V supply voltage, Ts=20ms; Kp=28, Ki=4, Kd=9 is good.
  //12V, Ts=1ms; Kp=100, Ki=19, Kd=5000, Ki>>5, Kd>>5 is good.
  //12V, Ts=1ms; Kp=100, Ki=35, Kd=31000, Ki>>5, Kd>>5 is good.
  //12V, Ts=.5ms; Kp=100, Ki=35, Kd=20000, Ki>>5, Kd>>4 is good.
  //     Control saturation only occurs during ref transient.
  ctrl.Kp=100;     //
  ctrl.Ki=25;      //
  ctrl.Ki_div=5;  //Shift by:5  >>2:1/4, >>3:1/8 >>4:1/16 >>5:1/32
  ctrl.Kd=20000;  //
  ctrl.Kd_div=4;  //Shift by:5  >>2:1/4, >>3:1/8 >>4:1/16 >>5:1/32
  ctrl.sum_error=0;
  ctrl.dif_error=0;
  ctrl.prev_error=0;
  
  ctrl.ref_filt=0; //Init. running sum for anl. ref voltage reading.
  
  enc.reading_curr=0;
  enc.reading_prev=1;
    
  clock_setup();   // Be sure to turn on periph clocks in clock.c !!!
  GPIO_setup();    // LEDs
  TIM1_setup();    // For Encoder read HW.
  TIM2_Config();   // For PWM generation.

  ADC_Config();
  UART1_Config();
  
  SystickConfig(); // Time base, 1kHz.
  enableInterrupts();


  print_bufloc=0;
  print_buflen=0;
  printing=FALSE;
  
  putstr(STR_ready);
  putstr(STR_info);

  
#ifdef USELCD
  HT1621_PortInit();   //Initialize port pins for the display.
  HT1621_Init();       //Initialize the display itself.
  
  HT1621_ValuesConstructor(&Seg_LCD); //Initialize character map.
  
  LED_ON();

  HT1621_AllOn(16); //Turn on all segments to test display.
  //TODO: Display FW version initially.
  Delayms(500);
  HT1621_AllOff(16); //Turn off all segments.
#endif

  LED_OFF();
  LEDOB_OFF();
  
  while(1){
    
    if (MS_TickEventFlag==TRUE){ //TODO: Increment within timer ISR!
      ++REF_count; //Increment these counters every ms.
      ++PRINT_count;
      MS_TickEventFlag=FALSE; //Flag was raised within systick ISR.
    }
    
    
    if (CTRL_EventFlag==TRUE){ //At every sampling time, do a control loop:
      CTRL_EventFlag=FALSE; // Flag was set within systick ISR
      
      read_enc(&enc);
      
      if(enc.reading_curr != enc.reading_prev){
#ifdef USELCD  //Display values on 7 segment LCD.
	//HT1621_Convert(enc.reading_curr, &Seg_LCD);
	HT1621_Convert(enc.value, &Seg_LCD);
	HT1621_Blanking(&Seg_LCD);
	HT1621_Refresh(&Seg_LCD);
#endif
	enc.reading_prev = enc.reading_curr; 
      }
      
      //TODO: Convert position info to a meaningful position value
      // (Abandoned to avoid proc. overhead for now. Perhaps later.)

#ifdef ANALOG_REFERENCE

      //Smooth input voltage reference using 1st order digital LPF:
      ctrl.ref_filt= ((ctrl.ref_filt<<3)-ctrl.ref_filt+ADC_RES)>>3;
      //<<2: ref_filt=0.75ref_filt+0.25current ref.
      //<<3: ref_filt=0.875ref_filt+0.125current ref.
      ctrl.ref=ctrl.ref_filt;
      //ctrl.ref=ADC_RES; //Was: reduce precision of ref reading st.
                          // noise is also removed. Not a good approach.
#endif

      ctrl.error=(ctrl.ref-enc.value);
      
      apply_control(&ctrl); //Calculate and apply control signal..

      ADC1->CR1 |= ADC1_CR1_ADON;//Start ADC1 conversion for next ref sample.
    } //End: CTRL_EvenFlag.


    if(REF_count>=REF_PERIOD){ //If the time to change reference val has come,
      REF_count=0;             //Reset the timer.
#ifndef ANALOG_REFERENCE
      ctrl.ref=references[ref_seq_no]; //Assign the current reference.
      ++ref_seq_no;            //Select the next reference.
      if (ref_seq_no>=REF_VALS_MAX){ //Check for overflow on reference sequence.
	ref_seq_no=0;
      }
#endif
    } //END: ref_count match.
    
    
    if (PRINT_count>=PRINT_PERIOD){ //When time comes, generate print string.
      PRINT_count=0;
      
      // String will be printed in a non blocking fashion later,
      // because there is no time within the control loop.
      print_bufloc=0; //Initialize print buffer pointer.
      printbuf[print_bufloc++]=(uint8_t)0xAA; //Header is AA55
      printbuf[print_bufloc++]=(uint8_t)0x55;
      //Print current position: (int16_t, big endian)
      printbuf[print_bufloc++]=(uint8_t)((enc.value>>8)&0xFF); //var #1
      printbuf[print_bufloc++]=(uint8_t)((enc.value)&0xFF);
      //Print current reference: (int16_t, big endian)
      printbuf[print_bufloc++]=(uint8_t)(((ctrl.ref)>>8)&0xFF); //var #2
      printbuf[print_bufloc++]=(uint8_t)( (ctrl.ref)    &0xFF);
      //Print current control signal: (int16_t, big endian)
      printbuf[print_bufloc++]=(uint8_t)((((int16_t)ctrl.u_int)>>8)&0xFF);// #3
      printbuf[print_bufloc++]=(uint8_t)( ((int16_t)ctrl.u_int)    &0xFF);

      print_buflen=print_bufloc; //Set the number of characters to send.
      print_bufloc=0; //Initialize the print buffer location
      printing=TRUE;  // and flag for printing.
      // A circular print buffer is not used to reduce overhead.
      //LEDOB_ON();   // This will signal if printing can be completed
      // within the specified print period. Check with an oscilloscope.
      // See the matching LEDOB_OFF() at the end of print sequence.
      
    }//End: PRINT_count>=PRINT_PERIOD

    if(printing==TRUE){ //If a print operation has been initiated,
      //Print one character at a time whenever the TX register is emptied.
      //This is a non blocking putstr.
      //Not implemented as a function to reduce overhead.
      if ((UART1->SR & (uint8_t)UART1_FLAG_TXE)!=(uint8_t)0x00){//TX reg idle.
	UART1->DR=printbuf[print_bufloc++];
	if(print_bufloc>=print_buflen){ //End of print queue reached:
	  printing=FALSE; // Lower print buffer active flag.
	  print_bufloc=0; // Initialize buffer location.
	  print_buflen=0; // Initialize buffer contents.
	  //LEDOB_OFF();  
	}//End of printing
      }//Print one byte
    }//printing==TRUE

  } //End: While 1.
} //End: main.




//Calculate the control signal u.
// u is calculated as an int, to reduce overhead of converting
// to physical units (rad), and then back to
// CPU units (pulses of encoder, or PWM.)

void apply_control (ControlValues_t *ctrl){

  uint16_t PWM_CH1, PWM_CH2;
  
  LEDOB_ON(); //To see CPU load.
  ctrl->sum_error+=ctrl->error;
  ctrl->dif_error=ctrl->error-ctrl->prev_error;

  //PID control:
  //TODO: This is vanilla. The following will be added:
  // 1. ARMA model
  // 2. Astrom implementation.
  // 3. Cascade implementation.
  //TODO: Calculate errors separately for antiwindup.
  //      Remove only integral component from error.
  ctrl->u_int=(U_INT_T)			\
    (
     ctrl->Kp*(U_INT_T)ctrl->error+
     ((ctrl->Ki*(U_INT_T)ctrl->sum_error)>>ctrl->Ki_div)+
     ((ctrl->Kd*(U_INT_T)ctrl->dif_error)>>ctrl->Kd_div)
     );
  

  //PI control:
  //ctrl->u_int=(U_INT_T)(Kp*(U_INT_T)ctrl->error+Ki*(U_INT_T)ctrl->sum_error);
  //P control:
  //ctrl->u_int=(U_INT_T)(ctrl->Kp*(U_INT_T)ctrl->error);

  ctrl->prev_error=ctrl->error;

  /*  if (ctrl->error<0){ //This will make it possible to measure
    LEDOB_ON();       // the oscillation period for calibration.
  }else{
    LEDOB_OFF();
  }
  */
  //ctrl->u_int=-3500; //For test.

  //Limit u_int to +-TIM2_ARR
  if (ctrl->u_int>TIM2_ARR){
    ctrl->u_int=TIM2_ARR;
    //Anti windup if control signal saturates.
    //TODO: Check! This seems wrong:
    ctrl->sum_error=ctrl->sum_error-ctrl->error;//remove error in integral, or
    //ctrl->sum_error=0; //When max effort is reached, no need to keep error...
    LED_ON();  // Signals saturated control
  }else if(ctrl->u_int<-TIM2_ARR){
    ctrl->u_int=-TIM2_ARR;
    //Anti windup if control signal saturates.
    ctrl->sum_error=ctrl->sum_error-ctrl->error;//remove error in integral, or
    //ctrl->sum_error=0; //When max effort is reached, no need to keep error...
    //Anti windup if control signal saturates.
    LED_ON();  // Signals saturated control
  }else{
    LED_OFF();  // Signals that control is not saturated.
  }
  
  if (ctrl->u_int>=0){     // If u>=0: CH1_PWM=PWM value, CH2_PWM=0
    PWM_CH1=ctrl->u_int;
    PWM_CH2=0;
  }else{              // If u<0: CH1_PWM=0, CH2=PWM value
    PWM_CH1=0;
    PWM_CH2=(uint16_t)(-ctrl->u_int);
  }

  
  //Apply the PWM:
  TIM2->CCR1H = (uint8_t)(PWM_CH1 >> 8);
  TIM2->CCR1L = (uint8_t)(PWM_CH1);
  TIM2->CCR2H = (uint8_t)(PWM_CH2 >> 8);
  TIM2->CCR2L = (uint8_t)(PWM_CH2);

  LEDOB_OFF(); //To see CPU load.
}



void read_enc(EncoderValues_t *enc){

     //enc_reading_curr = TIM1_GetCounter();
      //Read encoder position:
      do{//Make sure that there is no overflow in counter.
	//TODO: This can also check for changes in MSB. Check.
	enc->tim1_tmp= TIM1->CNTRL;
	enc->tim1_msb= TIM1->CNTRH;
	enc->tim1_lsb= TIM1->CNTRL;
      }while (enc->tim1_tmp!=enc->tim1_lsb);
      
      enc->reading_curr= (uint16_t)
	((uint16_t)(enc->tim1_msb<<8)|(uint16_t)enc->tim1_lsb);


      if (enc->reading_curr>ENC_ABS_RANGE){
	enc->value=enc->reading_curr-ENC_RANGE;
      }else{
	enc->value=enc->reading_curr;
      }

}


uint8_t  cpstr (char * input, char* buffer, uint8_t location)
//Copies from existing location until '\0' termination.
//Does not copy termination.
{
  uint8_t i=0;
  while (input[i]){
    buffer[location++]=input[i++];
    
  }
  return location;


}
