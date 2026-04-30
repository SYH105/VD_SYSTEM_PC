#include "settings.h"
#include "w25q16.h"

// =========================
// Flash 영역 설정
// Slot 10개 다음 1개 섹터를 Settings로 사용
// =========================
#define SAVE_SLOT_COUNT   10
#define FLASH_SLOT_SIZE   4096
#define FLASH_BASE_ADDR   0x000000
#define SETTINGS_ADDR     (FLASH_BASE_ADDR + SAVE_SLOT_COUNT * FLASH_SLOT_SIZE)

// =========================
// 전역 설정 변수
// =========================
SystemSettings g_settings;

// =========================
// 기본값 설정
// =========================
void Settings_SetDefault(void)
{
    g_settings.magic = SETTINGS_MAGIC;
    g_settings.fft_scale = 3000.0f;
    g_settings.avg_count = 30;
    g_settings.buzzer_enable = 0;

    g_settings.reserved[0] = 0;
    g_settings.reserved[1] = 0;
}

// =========================
// 설정값 Load
// =========================
void Settings_Load(void)
{
    W25Q16_Read(SETTINGS_ADDR, (uint8_t*)&g_settings, sizeof(SystemSettings));

    // magic 값 확인
    if(g_settings.magic != SETTINGS_MAGIC)
    {
        Settings_SetDefault();
        Settings_Save();
        return;
    }

    // 유효 범위 보정
    if(g_settings.fft_scale < 100.0f || g_settings.fft_scale > 20000.0f)
    {
        g_settings.fft_scale = 3000.0f;
    }

    if(g_settings.avg_count < 1 || g_settings.avg_count > 100)
    {
        g_settings.avg_count = 30;
    }

    if(g_settings.buzzer_enable > 1)
    {
        g_settings.buzzer_enable = 1;
    }
}

// =========================
// 설정값 Save
// =========================
void Settings_Save(void)
{
    g_settings.magic = SETTINGS_MAGIC;

    W25Q16_EraseSector(SETTINGS_ADDR);
    W25Q16_Write(SETTINGS_ADDR, (uint8_t*)&g_settings, sizeof(SystemSettings));
}
