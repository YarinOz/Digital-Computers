#ifndef _BSP_MSP430X2XX_H_
#define _BSP_MSP430X2XX_H_

#include  <msp430g2553.h>          // MSP430x2xx

#define   debounceVal      250
#define   LEDs_SHOW_RATE   0xFFFF  // 62_5ms

// RGB abstraction R->P2.2, G->P2.1, B->P2.0
#define RGBArrPortOut      P2OUT
#define RGBArrPortDir      P2DIR
#define RGBArrPortSEL      P2SEL

// LCDs abstraction 
#define LCD_DATA_WRITE     P1OUT
#define LCD_DATA_DIR       P1DIR
#define LCD_DATA_READ      P1IN
#define LCD_DATA_SEL       P1SEL
#define LCD_CTL_SEL        P2SEL

//   Buzzer abstraction P2.4
#define BuzzPortSel        P2SEL
#define BuzzPortDir        P2DIR
#define BuzzPortOut        P2OUT

// Potentiometer abstraction P1.3
#define PotPort            P1IN 
#define PotPortSel         P1SEL
#define PotPortDir         P1DIR
#define PotPortOut         P1OUT

PushButtons abstraction
#define PBsArrPort         P1IN
#define PBsArrIntPend      P1IFG
#define PBsArrIntEn        P1IE
#define PBsArrIntEdgeSel   P1IES
#define PBsArrPortSel      P1SEL
#define PBsArrPortDir      P1DIR
#define PBsArrPortOut      P1OUT
// #define PB0                0x01   // P1.0
#define PB1                0x02  // P1.1
#define PB2                0x04  // P1.2
// #define PB3                0x10   // P2.0

#define TXLED BIT0
#define RXLED BIT6
#define TXD BIT2
#define RXD BIT1

extern void GPIOconfig(void);
extern void TIMER0_A0_config(void);
extern void TIMER1_A2_config(void);
extern void TIMER1_A1_config(void);

#endif
