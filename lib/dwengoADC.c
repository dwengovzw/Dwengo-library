/*
 * Sensors
 * part of Dwengo library
 *
 * Analog/Digital conversion, AN0-4
 *
 * Version: 1.0.$Revision: 1933 $
 * Date: $Date: 2010-03-14 21:43:25 +0100 (Sun, 14 Mar 2010) $
 * (c) Dwengo vzw - http://www.dwengo.org
 */

#include <adc.h>
#include "dwengoADC.h"
#include "dwengoDelay.h"

void initADC() {
  LATA = 0xFF;   // set all A to inputs

  /*
    ADC_FOSC_64: conversion clock, table 21-1, => 1TAD = 1,33 us (has to be between 0,7 us and 25 us)
    ADC_RIGHT_JUST: Least significant bits
    ADC_6_TAD: acquisition time, 6 TAD's used for good conversion, minimal 1,4 us required
    ADC_INT_OFF: no interrupts
    ADC_VREFPLUS_VDD and ADC_VREFMINUS_VSS: use PIC ground and source as voltage reference
    0b1010: pins AN0-AN4 configured as analog
  */
#ifdef SDCC_pic16
  adc_open(ADC_CHN_0, ADC_FOSC_64 | ADC_ACQT_6, ADC_CFG_5A, ADC_FRM_RJUST | ADC_INT_OFF | ADC_VCFG_VDD_VSS);
#else
  OpenADC(ADC_FOSC_64 & ADC_RIGHT_JUST & ADC_6_TAD, ADC_CH0 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 0b1010);   // Configuring ADC
#endif
}

int readADC(BYTE address) {
  int data;

  // Choosing ADC channel (override ADC_CH0)
  switch(address) {
    case 0: SetChanADC(ADCCHAN(0)); break;
    case 1: SetChanADC(ADCCHAN(1)); break;
    case 2: SetChanADC(ADCCHAN(2)); break;
    case 3: SetChanADC(ADCCHAN(3)); break;
    case 4: SetChanADC(ADCCHAN(4)); break;
    default: return -1;  // non existing channel
  }

  delay_us(125);       // wait until channel selection is stable
  ConvertADC();        // Start conversion
  while (BusyADC());   // Wait for ADC conversion
  data = ReadADC();    // Read result

  return data;
}
