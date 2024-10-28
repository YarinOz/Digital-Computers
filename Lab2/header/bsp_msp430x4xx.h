#ifndef _BSP_MSP430X4XX_H_
#define _BSP_MSP430X4XX_H_

// #include  <msp430g2553.h>          // MSP430x2xx
#include  <msp430xG46x.h>  // MSP430x4xx
#define _MSP430_HAS_PORT1_
#define _MSP430_HAS_PORT2_

#define   debounceVal      250
#define   LEDs_SHOW_RATE   0xFFFF  // 62_5ms

// Switches abstraction
#define SWsArrPort         P2IN
#define SWsArrPortDir      P2DIR
#define SWsArrPortSel      P2SEL
#define SWsArrIntEn        P2IE
#define SWsArrIntEdgeSel   P2IES
#define SWsArrIntPend      P2IFG
#define SWmask             0x01

// LCDs abstraction
#define LCD_DATA_WRITE     P1OUT
#define LCD_DATA_DIR       P1DIR
#define LCD_DATA_READ      P1IN
#define LCD_DATA_SEL       P1SEL
#define LCD_CTL_SEL        P2SEL

//   Generator abstraction
#define GenPort            P2IN
#define GenPortSel         P2SEL
#define GenPortDir         P2DIR
#define GenPortOut         P2OUT

//   Buzzer abstraction
#define BuzzPortSel        P2SEL
#define BuzzPortDir        P2DIR
#define BuzzPortOut        P2OUT

// PushButtons abstraction
#define PBsArrPort         P1IN
#define PBsArrIntPend      P1IFG
#define PBsArrIntEn        P1IE
#define PBsArrIntEdgeSel   P1IES
#define PBsArrPortSel      P1SEL
#define PBsArrPortDir      P1DIR
#define PBsArrPortOut      P1OUT
#define PB0                0x01   // P1.0
#define PB1                0x02  // P1.1
#define PB2                0x04  // P1.2
#define PB3                0x10   // P2.0


extern void GPIOconfig(void);
extern void TIMER0_A0_config(void);
extern void TIMER1_A2_config(void);
extern void TIMER1_A1_config(void);
extern void ADCconfig(void);

#endif



