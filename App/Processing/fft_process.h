#ifndef __FFT_PROCESS_H__
#define __FFT_PROCESS_H__

#include "arm_math.h"
#include <stdint.h>

#define FFT_SIZE 256
#define SAMPLE_FREQ 400.0f


void FFT_Process_Init(void);
void FFT_Process_Input(float sample);
uint8_t FFT_Process_IsReady(void);

void FFT_Process_Run(void);

float* FFT_Process_GetMagnitude(void);
float FFT_Process_GetPeakFreq(void);
int FFT_Process_GetPeakIndex(void);
float FFT_Process_GetPeakValue(void);

#endif
