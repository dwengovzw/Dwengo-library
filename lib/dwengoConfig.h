/*
 * Configuration
 * part of Dwengo library
 *
 * Sets all pragmas properly
 *
 * Version: 1.0.$Revision: 2117 $
 * Date: $Date: 2010-05-01 18:43:32 +0200 (Sat, 01 May 2010) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */
/**
 * \file
 * \brief Configuration
 *
 * Sets all pragmas properly
 */
 
// CPU Clock: 48MHz
#ifdef SDCC_pic16
#include <pic18fregs.h>

__CONFIG(__CONFIG1L, _USBPLL_CLOCK_SRC_FROM_96MHZ_PLL_2_1L &
                     _PLLDIV_DIVIDE_BY_5__20MHZ_INPUT__1L &
                     _CPUDIV__OSC1_OSC2_SRC___1__96MHZ_PLL_SRC___2__1L);
__CONFIG(__CONFIG1H, _OSC_HS__HS_PLL__USB_HS_1H &
                     _FCMEN_OFF_1H &
                     _IESO_OFF_1H);
__CONFIG(__CONFIG2L, _VREGEN_ON_2L &
                     _PUT_OFF_2L &
                     _BODEN_OFF_2L &
                     _BODENV_4_2V_2L);
__CONFIG(__CONFIG2H, _WDT_DISABLED_CONTROLLED_2H &
                     _WDTPS_1_32768_2H);
__CONFIG(__CONFIG3H, _CCP2MUX_RC1_3H &
                     _PBADEN_PORTB_4_0__CONFIGURED_AS_DIGITAL_I_O_ON_RESET_3H &
                     _LPT1OSC_OFF_3H &
                     _MCLRE_MCLR_ON_RE3_OFF_3H);
__CONFIG(__CONFIG4L, _STVR_ON_4L &
                     _LVP_OFF_4L &
                     _ENHCPU_OFF_4L);
__CONFIG(__CONFIG5L, _CP_0_OFF_5L);
__CONFIG(__CONFIG5H, _CPD_OFF_5H);
__CONFIG(__CONFIG6L, _WRT_0_OFF_6L);
__CONFIG(__CONFIG6H, _WRTD_OFF_6H);
__CONFIG(__CONFIG7L, _EBTR_0_OFF_7L);
__CONFIG(__CONFIG7H, _EBTRB_OFF_7H);

#else
#pragma config   PLLDIV    =   5           // Divide by 5 (20 MHz oscillator input)
#pragma config   FOSC      =   HSPLL_HS    // HS oscillator, PLL enabled, HS used by USB
#pragma config   CPUDIV    =   OSC1_PLL2   // System Clock Postscaler Selection bits

// USB: enabled
#pragma config   USBDIV    =   2           // USB Clock: 48 MHz
#pragma config   VREGEN    =   ON          // USB Voltage Regulator: enabled

// Brown-out Reset: disabled
#pragma config   BOR       =   OFF         // Brown-out Reset enabled in hardware only (SBOREN is disabled)
#pragma config   BORV      =   3           // Brown-out Reset Voltage bits

// Watchdog Timer: disabled
#pragma config   WDT       =   OFF         // Watchdog Timer Enable bit
#pragma config   WDTPS     =   32768       // Watchdog Timer Postscale Select bits

// Miscellaneous
#pragma config   IESO      =   OFF         // Oscillator Switchover mode: disabled
#pragma config   FCMEN     =   OFF         // Fail-Safe Clock Monitor: disabled
#pragma config   PWRT      =   OFF         // Power-up Timer: disabled
#pragma config   MCLRE     =   ON          // MCLR pin: enabled; RE3 input pin: disabled
#pragma config   LPT1OSC   =   OFF         // Low-Power Timer1 Oscillator: disabled
#pragma config   PBADEN    =   OFF         // PORB digital IO on powerup
#pragma config   CCP2MX    =   ON          // CCP2 is multiplexed to RC1 (not RB3)
#pragma config   XINST     =   OFF         // Extended Instruction Set: disabled
#pragma config   LVP       =   OFF         // Low-voltage programming: disabled
#pragma config   STVREN    =   ON          // Stack Full/Underflow Reset: enabled

// Code Protection: disabled
#pragma config   CPB       =   OFF         // Boot Block Code Protection bit
#pragma config   CPD       =   OFF         // Data EEPROM Code Protection bit
#pragma config   CP0       =   OFF         // Code Protection bit Block 0
#pragma config   CP1       =   OFF         // Code Protection bit Block 1
#pragma config   CP2       =   OFF         // Code Protection bit Block 2
#pragma config   CP3       =   OFF         // Code Protection bit Block 3

// Write Protection: disabled
#pragma config   WRTC      =   OFF         // Configuration Register Write Protection bit
#pragma config   WRTB      =   OFF         // Boot Block Write Protection bit
#pragma config   WRTD      =   OFF         // Data EEPROM Write Protection bit
#pragma config   WRT0      =   OFF         // Write Protection bit Block 0
#pragma config   WRT1      =   OFF         // Write Protection bit Block 1
#pragma config   WRT2      =   OFF         // Write Protection bit Block 2
#pragma config   WRT3      =   OFF         // Write Protection bit Block 3

// Table Read Protection: disabled
#pragma config   EBTRB     =   OFF         // Boot Block Table Read Protection bit
#pragma config   EBTR0     =   OFF         // Table Read Protection bit Block 0
#pragma config   EBTR1     =   OFF         // Table Read Protection bit Block 1
#pragma config   EBTR2     =   OFF         // Table Read Protection bit Block 2
#pragma config   EBTR3     =   OFF         // Table Read Protection bit Block 3

#endif
