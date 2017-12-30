
/*
Copyright (c) 2017, aweatherguy
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
Neither the name of the <organization> nor the
names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 This is an extension to the <avr/boot.h> file which defines a macro
 named "spm" which takes a single argument, a value to be written to
 SPMCSR/SPMCR.
*/

#pragma once

#include <avr/boot.h>

#if 0 // how can we tell if address of SPMCSR < 64 ? 

#define __spm_normal( spmcsr_value )		 \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
	"cli\n\t"				 \
        "out %0, %1\n\t"                         \
        "spm\n\t"	                         \
	"sei\n"					 \
        :                                        \
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),        \
          "r" ((uint8_t)( spmcsr_value ))        \
    );                                           \
}))

#else

#define __spm_normal( spmcsr_value )		 \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
	"cli\n\t"				 \
        "sts %0, %1\n\t"                         \
        "spm\n\t"                                \
	"sei\n"					 \
        :                                        \
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),        \
          "r" ((uint8_t)( spmcsr_value ))	 \
    );                                           \
}))

#endif

#define __spm_alternate( spmcsr_value )		 \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
	"cli\n\t"				 \
        "sts %0, %1\n\t"                         \
        "spm\n\t"                                \
        ".word 0xffff\n\t"                       \
        "nop\n\t"                                \
	"sei\n"					 \
        :                                        \
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),        \
          "r" ((uint8_t)( spmcsr_value ))        \
    );                                           \
}))

#if defined(__AVR_ATmega161__) || defined(__AVR_ATmega163__) \
    || defined(__AVR_ATmega323__)

#define spm( SpmcsrValue )	      __spm_alternate( SpmcsrValue )

#elif (FLASHEND > USHRT_MAX)

#define spm( SpmcsrValue )	      __spm_normal( SpmcsrValue )

#else

#define spm( SpmcsrValue )	      __spm_normal( SpmcsrValue )

#endif
