#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "stm32f4xx_hal.h"

#define SETTINGS_MAGIC 0xA5A55A5A

typedef struct
{
    uint32_t magic;          // 유효한 설정 데이터인지 확인
    float fft_scale;         // 그래프 스케일
    uint8_t avg_count;       // FFT 평균 횟수
    uint8_t buzzer_enable;   // 0: OFF, 1: ON
    uint8_t reserved[2];     // 정렬/확장용 여유
} SystemSettings;

// 전역 설정값
extern SystemSettings g_settings;

// 기본값 설정
void Settings_SetDefault(void);

// Flash에서 불러오기
void Settings_Load(void);

// Flash에 저장하기
void Settings_Save(void);

#endif
