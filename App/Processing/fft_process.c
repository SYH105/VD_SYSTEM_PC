#include "fft_process.h"
#include <math.h>

// =========================
// 내부 변수
// =========================
static arm_rfft_fast_instance_f32 fft;

static float fft_input[FFT_SIZE];
static float fft_output[FFT_SIZE];
static float mag_fft[FFT_SIZE/2];

static uint16_t sample_index = 0;
static uint8_t fft_ready = 0;

static float peak_freq = 0;
static int peak_index = 0;
static float peak_value = 0;
// =========================
// Init
// =========================
void FFT_Process_Init(void)
{
    arm_rfft_fast_init_f32(&fft, FFT_SIZE);
}

// =========================
// Input
// =========================
void FFT_Process_Input(float sample)
{
    fft_input[sample_index++] = sample;

    if(sample_index >= FFT_SIZE)
    {
        sample_index = 0;
        fft_ready = 1;
    }
}

// =========================
// Ready check
// =========================
uint8_t FFT_Process_IsReady(void)
{
    return fft_ready;
}

// =========================
// Run FFT
// =========================
void FFT_Process_Run(void)
{
    fft_ready = 0;

    // =========================
    // DC 제거
    // =========================
    float mean = 0.0f;

    for(int i = 0; i < FFT_SIZE; i++)
    {
        mean += fft_input[i];
    }
    mean /= FFT_SIZE;

    for(int i = 0; i < FFT_SIZE; i++)
    {
        fft_input[i] -= mean;
    }

    // =========================
    // FFT 실행
    // =========================
    arm_rfft_fast_f32(&fft, fft_input, fft_output, 0);

    // =========================
    // magnitude 계산
    // =========================
    for(int i = 0; i < FFT_SIZE/2; i++)
    {
        float real = fft_output[2*i];
        float imag = fft_output[2*i + 1];

        mag_fft[i] = sqrtf(real*real + imag*imag);
    }

    // =========================
    // peak 찾기
    // =========================
    peak_value = 0.0f;
    peak_index = 0;

    for(int i = 1; i < FFT_SIZE/2; i++)
    {
        if(mag_fft[i] > peak_value)
        {
            peak_value = mag_fft[i];
            peak_index = i;
        }
    }

    if(peak_value > 80.0f)
    {
        peak_freq = (float)peak_index * SAMPLE_FREQ / FFT_SIZE;
    }
    else
    {
        peak_freq = 0.0f;
        peak_index = 0;
        peak_value = 0.0f;
    }
}

// =========================
// Getters
// =========================
float* FFT_Process_GetMagnitude(void)
{
    return mag_fft;
}

float FFT_Process_GetPeakFreq(void)
{
    return peak_freq;
}
int FFT_Process_GetPeakIndex(void)
{
    return peak_index;
}

float FFT_Process_GetPeakValue(void)
{
    return peak_value;
}
