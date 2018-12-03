# Simple-Open-Source-Servo-S8: SOSS-S8
Feedback position control servo for STM8 and SDCC.

**SOSS-S8** is a feedback position controller for DC motors using an incremental encoder for feedback. Its main puropse is education, by allowing anyone to implement a position servo using very low cost components and open source software tools. You can learn many things with **SOSS-S8**:

- Basic concepts in feedback control systems implementation (PID etc),
- Convert continuous time designs into discrete time (digital control),
- High precision fixed point math implementation in a tight control loop timing,
- Writing code for real-time execution and using simple tools to check (LED, cheap logic analyser etc.),
- Using sdcc, GNU make and standard peripheral library to write a software project for STM8S processor,
- Simple digital filter implementation
- And others...

It is very satisfactory see the motor follow reference signal, observe overshoot, steady state error, instabilities etc. or tune the control loop using textbook methods like Ziegler Nichols to achieve fast response and zero steady stete error. But the real value comes when you connect the **SOSS-S8** to your computer and see the control signals on your computer, in real-time, using the excellent [SerialPlot software](https://bitbucket.org/hyOzd/serialplot), which is also open source. The development was done in Linux, but it is perfectly valid for Windows or MacOS-X also.

## Specifications:

- Control sampling frequency: 2kHz (can be an order of magnitude higher)
- PWM frequency: 8kHz which is barely audible. 
- PWM resolution at 8kHz: 2000 counts
- 16bit PID coefficients with 1/32 precision (scaling implemented for speed)
- Hardware quadrature encoder reading suitable for high resolution encoders (500ppr, 1000ppr etc.)
- 16 bit control parameters sent in real-time at 500 samples/second for graphing
- Hardware PWM and timebase generation.
- Generate its own time based step reference signals, or read from a potentiometer using ADC.

## Hardware:
**SOSS-S8** uses very low cost off the shelf components:

- STM8S103F3P6 processor (STM8 or S8), ~$1~2 :[See this](https://www.banggood.com/STM8S103F3P6-System-Board-STM8S-STM8-Development-Board-Minimum-Core-Board-p-1195874.html?cur_warehouse=CN)
- DC motor with optical incremental encoder. Either salvaged from an old ink jet printer, or [China ~$3.5](https://www.aliexpress.com/item/Great-Johnson-2pcs-20MA-Standard-130-motor-Green-Micro-DC-motor-with-encoder-32-lines-of/1958806724.html)
- Power amplifier (H-bridge); [ubiquitious L298 is perfectly fine; ~$2.5](https://www.banggood.com/Wholesale-Dual-H-Bridge-DC-Stepper-Motor-Drive-Controller-Board-Module-Arduino-L298N-p-42826.html?rmmds=search&cur_warehouse=CN)
- 7 segment LCD display (not necessary, but very informative) such as a [HT1621 module from DX, ~$3](https://www.dx.com/p/2-4-6-digit-7-segment-lcd-display-module-white-backlit-for-arduino-2071629)
- USB to serial converter such as [CH340 ~$1.75](https://www.banggood.com/CH340-3_3V5_5V-USB-To-TTL-Converter-Module-CH340G-STC-SCM-Download-Module-Upgrade-Brush-Board-p-1227701.html?rmmds=search&cur_warehouse=CN)
- 12V power supply. If you don't have one lying around, try a [generic one, ~$4.5 at 2A](https://www.banggood.com/AC-100-240V-Converter-Adapter-12V-2A-24W-Power-Supply-For-LED-Strip-p-1006925.html?rmmds=search&ID=43101&cur_warehouse=CN)
- Some protoboard, wires LEDs etc.
Most of the components are already laying around in a typical workshop, but even if you buy them all, the maximum cost is less than $20. The connections are very simple and can be done on a breadboard using the common "DuPont" patch cables in a few minutes.

So, go ahead and build yourself a **SOSS-S8** and try your hand at closed loop control implementation!
