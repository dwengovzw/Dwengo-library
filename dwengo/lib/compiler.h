#ifndef COMPILER_H
#define COMPILER_H
#ifdef SDCC_pic16
	#include "compiler/sdcc_pic16.h"
#else
	#include "compiler/mplabc18.h"
#endif
#endif
