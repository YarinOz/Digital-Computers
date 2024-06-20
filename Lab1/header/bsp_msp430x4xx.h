#ifndef _BSP_MSP430X4XX_H_
#define _BSP_MSP430X4XX_H_

// #include  <msp430g2553.h>          // MSP430x2xx
#include  <msp430xG46x.h>  // MSP430x4xx
#define _MSP430_HAS_PORT1_
#define _MSP430_HAS_PORT2_

#define   debounceVal      250
#define   LEDs_SHOW_RATE   0xFFFF  // 62_5ms

// LEDs abstraction
#define LEDsArrPort        P9OUT
#define LEDsArrPortDir     P9DIR
#define LEDsArrPortSel     P9SEL

// Switches abstraction
#define SWsArrPort         P1IN
#define SWsArrPortDir      P1DIR
#define SWsArrPortSel      P1SEL
#define SWmask             0x7F

// PushButtons abstraction
#define PBsArrPort	       P1IN 
#define PBsArrPortOut	   P1OUT
#define PBsArrIntPend	   P1IFG 
#define PBsArrIntEn	       P1IE
#define PBsArrIntEdgeSel   P1IES
#define PBsArrPortSel      P1SEL 
#define PBsArrPortDir      P1DIR 
#define PB0                0x01
#define PB1                0x02
#define PB2                0x04
#define PB3                0x08


extern void GPIOconfig(void);
extern void TIMERconfig(void);
extern void ADCconfig(void);

#endif



