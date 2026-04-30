#include "app.h"
#include "button.h"
#include "lcd.h"
#include "ui.h"
#include "fft_process.h"
#include "w25q16.h"
#include "settings.h"
#include "buzzer.h"
#include <stdio.h>


// =========================
// 시스템 상태 머신 정의
// - UI 화면 단위 상태
// =========================
typedef enum {
    STATE_MAIN_MENU = 0,   // 메인 메뉴
    STATE_MEASURE,         // 실시간 측정 (FFT)
    STATE_SAVE,            // 측정 후 저장
    STATE_LOAD,            // 저장 데이터 조회
    STATE_SETTINGS,        // 설정값
	STATE_PC_MODE          // PC MODE전환
} SystemState;

// =========================
// 상태 관리 변수 - MAIN
// =========================
static SystemState state = STATE_MAIN_MENU;
static uint8_t menu_index = 0;

// =========================
// 상태 관리 변수 - MEASURE
// =========================
static int error_count = 0;
// =========================

// =========================
// 상태 관리 변수 - SETTING
// =========================
static uint8_t settings_index = 0;
extern UART_HandleTypeDef huart1;

// =========================
// 상태 관리 변수 - PC MODE
// =========================
static uint8_t pc_state = 0;
// UART 명령 버퍼
char cmd_buf[50];
uint8_t cmd_ready;

// 상태 진입 시간 / 단계 관리
// =========================
static uint32_t state_start_time = 0;

static uint8_t measure_phase = 0;   // 측정 단계
static uint8_t save_phase = 0;      // 저장 단계
static uint8_t load_phase = 0;      // 로드 단계

// =========================
// 슬롯 관련
// =========================
#define SAVE_SLOT_COUNT 10

static uint8_t selected_slot = 0;
static uint8_t slot_used[10];
static uint8_t prev_selected = 0xFF;  // redraw 최적화용

// =========================
// FFT 처리 관련
// =========================
static int fft_skip_count;
static int fft_done_count;

// =========================
// FFT 저장 버퍼
// =========================
static float save_fft_sum[FFT_SIZE / 2];
static float save_fft_avg[FFT_SIZE / 2];
static float load_fft_data[FFT_SIZE / 2];

static float live_fft_sum[FFT_SIZE / 2];
static float live_fft_avg[FFT_SIZE / 2];
static int live_fft_count = 0;
// =========================
// 동작 파라미터
// =========================
#define COUNTDOWN_TIME 3000       // 3초 카운트다운
// =========================
// Flash 저장/로드 내부 함수
// =========================
static void Save_LoadSlotStatus(void);     // 슬롯 사용 여부 읽기
static void Save_WriteSlot(uint8_t slot, float *data);  // 슬롯 저장
static void Load_ReadSlot(uint8_t slot, float *data);   // 슬롯 읽기
static float Load_CalcPeak(float *data);   // peak 계산
static void Load_DeleteSlot(uint8_t slot); // 슬롯 삭제

// =========================
// Measure 비교결과 도출함수
// =========================
static int CompareFFT(float *base, float *live, int size);
// =========================
// 초기화
// - UI 첫 화면 구성
// =========================
void App_Init(void)
{
    Settings_Load(); //설정값 Load

    ILI9341_FillScreen(BLACK);
    UI_DrawMainMenu(menu_index);
}

// =========================
// 메인 상태 머신 (핵심 로직)
// - 버튼 이벤트 기반 UI 제어
// =========================
void App_Update(int16_t x, int16_t y, int16_t z)
{
    InputEvent event = Button_GetEvent();   // 버튼 이벤트 1회성 획득

    static SystemState prev_state = -1;     // 상태 변경 감지용

    // =========================
    // 상태 변경 감지
    // - 상태 진입 시 1회 실행되는 초기화 영역
    // =========================
    if(prev_state != state)
    {
        prev_state = state;

        switch(state)
        {
            case STATE_MAIN_MENU:
                UI_DrawMainMenu(menu_index);
                save_phase = 0;
                measure_phase = 0;
                Buzzer_Off();
                break;

            case STATE_MEASURE:
                measure_phase = 0;
                fft_skip_count = 2;
                fft_done_count = 0;
                error_count = 0;

                live_fft_count = 0;

                for(int i = 0; i < FFT_SIZE/2; i++)
                {
                    live_fft_sum[i] = 0.0f;
                    live_fft_avg[i] = 0.0f;
                }


                Save_LoadSlotStatus();   // 슬롯 상태 읽기
                selected_slot = 0;
                prev_selected = 0xFF;
                UI_DrawMeasureSlots(selected_slot, slot_used);
                LCD_ResetFFTProgress();

                break;
            case STATE_SAVE:
                state_start_time = HAL_GetTick();
                fft_skip_count = 2;
                fft_done_count = 0;
                save_phase = 0;
                LCD_DrawUIFrame();
                LCD_ResetFFTProgress();

                // FFT 누적 배열 초기화
                for(int i = 0; i < FFT_SIZE/2; i++)
                {
                    save_fft_sum[i] = 0.0f;
                }
                break;
            case STATE_LOAD:
            	load_phase = 0;
                Save_LoadSlotStatus();
                selected_slot = 0;
                prev_selected = 0xFF;
                UI_DrawLoadSlots(selected_slot, slot_used);
                break;
            case STATE_SETTINGS:
                settings_index = 0;
                UI_DrawSettingsMenu(settings_index);
                break;
            case STATE_PC_MODE:
                pc_state = 0; // PC_WAIT
                UI_DrawPCMode();
                break;
        }
    }

    // =========================
    // ★MAIN MENU★
    // =========================
    if(state == STATE_MAIN_MENU)
    {
        if(event == EVENT_UP)
        {
            if(menu_index > 0) menu_index--;
            UI_DrawMainMenu(menu_index);
        }
        else if(event == EVENT_DOWN)
        {
            if(menu_index < 4) menu_index++;
            UI_DrawMainMenu(menu_index);
        }
        else if(event == EVENT_SELECT)
        {
            switch(menu_index)
            {
                case 0: state = STATE_MEASURE; break;
                case 1: state = STATE_SAVE; break;
                case 2: state = STATE_LOAD; break;
                case 3: state = STATE_SETTINGS; break;
                case 4: state = STATE_PC_MODE; break;
            }
        }
    }

    // =========================
    // ★MEASURE★
    // =========================
    else if(state == STATE_MEASURE)
    {
        uint32_t elapsed = HAL_GetTick() - state_start_time;

        // =========================
        // 0. 슬롯 선택 단계
        // =========================
        if(measure_phase == 0)
        {
            if(event == EVENT_UP)
            {
                if(selected_slot > 0) selected_slot--;
            }
            else if(event == EVENT_DOWN)
            {
                if(selected_slot < 9) selected_slot++;
            }

            if(prev_selected != selected_slot)
            {
            	UI_DrawMeasureSlots(selected_slot, slot_used);
                prev_selected = selected_slot;
            }

            if(event == EVENT_SELECT)
            {
                if(slot_used[selected_slot])
                {
                    measure_phase = 1;

                    state_start_time = HAL_GetTick();

                    Load_ReadSlot(selected_slot, load_fft_data);

                    LCD_ClearScreen();
                    UI_DrawLegend();
                    LCD_DrawFFT_Area(load_fft_data, FFT_SIZE/2, GRAPH_TOP_Y, BLUE);
                }
            }

            if(event == EVENT_MENU)
            {
                state = STATE_MAIN_MENU;
            }
        }

        // =========================
        // 1. 카운트다운
        // =========================
        else if(measure_phase == 1)
        {
            int count = 3 - (elapsed / 1000);
            if(count < 1) count = 1;

            LCD_UpdateCount(count);

            if(elapsed >= COUNTDOWN_TIME)
            {
                measure_phase = 2;
                state_start_time = HAL_GetTick();
                LCD_ClearTopBar();
            }
        }

        // =========================
        // 2. FFT 측정
        // =========================
        else if(measure_phase == 2)
        {
            if(FFT_Process_IsReady())
            {
                FFT_Process_Run();

                if(fft_skip_count > 0)
                {
                    fft_skip_count--;
                }
                else
                {
                    fft_done_count++;
                    LCD_DrawFFTProgress(fft_done_count);

                    float *live_data = FFT_Process_GetMagnitude();
                    float peak = FFT_Process_GetPeakFreq();

                    // =========================
                    // 1. DISPLAY는 실시간 FFT
                    // =========================
                    LCD_DrawFFT_Area(live_data, FFT_SIZE/2, GRAPH_BOTTOM_Y, GREEN);

                    // =========================
                    // 2. 평균 누적
                    // =========================
                    for(int i = 0; i < FFT_SIZE/2; i++)
                    {
                        live_fft_sum[i] += live_data[i];
                    }

                    live_fft_count++;

                    // =========================
                    // 3. 평균 계산
                    // =========================
                    for(int i = 0; i < FFT_SIZE/2; i++)
                    {
                        live_fft_avg[i] = live_fft_sum[i] / live_fft_count;
                    }

                    // =========================
                    // 4. 상태 표시
                    // =========================
                    LCD_UpdateStatus(x, y, z, peak);

                    // =========================
                    // 5. 비교는 평균 FFT
                    // =========================
                    int status = CompareFFT(load_fft_data, live_fft_avg, FFT_SIZE/2);
                    LCD_DrawResult(status);

                    // =========================
                    // BUZZER 제어 (여기 추가)
                    // =========================
                    if(g_settings.buzzer_enable)
                    {
                        if(status == STATUS_ERROR)
                        {
                            Buzzer_On();
                        }
                        else
                        {
                            Buzzer_Off();
                        }
                    }

                    if(fft_done_count >= g_settings.avg_count)
                    {
                        fft_done_count = 0;
                        state = STATE_MAIN_MENU;
                    }
                }
            }
        }

        if(event == EVENT_MENU)
        {
            measure_phase = 0;
            state = STATE_MAIN_MENU;
        }
    }
    // =========================
    // ★SAVE★
    // =========================
    else if(state == STATE_SAVE)
    {
        uint32_t elapsed = HAL_GetTick() - state_start_time;

        // =========================
        // 0. 카운트다운
        // =========================
        if(save_phase == 0)
        {
            int count = 3 - (elapsed / 1000);
            if(count < 1) count = 1;

            LCD_UpdateCount(count);

            if(elapsed >= COUNTDOWN_TIME)
            {
                save_phase = 1;
                state_start_time = HAL_GetTick();

                LCD_ClearTopBar();
            }
        }

        // =========================
        // 1. FFT 측정
        // =========================
        else if(save_phase == 1)
        {
            if(FFT_Process_IsReady())
            {
                FFT_Process_Run();

                if(fft_skip_count > 0)
                {
                    fft_skip_count--;
                }
                else
                {
                    fft_done_count++;
                    LCD_DrawFFTProgress(fft_done_count);

                    float *data = FFT_Process_GetMagnitude();
                    float peak = FFT_Process_GetPeakFreq();

                    LCD_DrawFFT(data, FFT_SIZE/2);
                    LCD_UpdateStatus(x, y, z, peak);

                    // ✅ FFT 누적
                    for(int i = 0; i < FFT_SIZE/2; i++)
                    {
                        save_fft_sum[i] += data[i];
                    }

                    if(fft_done_count >= g_settings.avg_count)
                    {
                    	// 평균 계산
                    	for(int i = 0; i < FFT_SIZE/2; i++)
                    	{
                    		save_fft_avg[i] = save_fft_sum[i] / g_settings.avg_count;
                    	}
                        // 슬롯 선택으로 이동
                        save_phase = 2;

                        Save_LoadSlotStatus();
                        selected_slot = 0;

                        UI_DrawSaveSlots(selected_slot, slot_used);
                    }
                }
            }
        }

        // =========================
        // 2. 슬롯 선택
        // =========================
        else if(save_phase == 2)
        {
            if(event == EVENT_UP)
            {
                if(selected_slot > 0) selected_slot--;
            }
            else if(event == EVENT_DOWN)
            {
                if(selected_slot < 9) selected_slot++;
            }
            // 선택 변경 시만 redraw
            if(prev_selected != selected_slot)
            {
                UI_DrawSaveSlots(selected_slot, slot_used);
                prev_selected = selected_slot;
            }
            if(event == EVENT_SELECT)
            {
            	Save_WriteSlot(selected_slot, save_fft_avg);
                state = STATE_MAIN_MENU;
            }
        }

        // =========================
        // 취소
        // =========================
        if(event == EVENT_MENU)
        {
            save_phase = 0;
            state = STATE_MAIN_MENU;
        }
    }
    // =========================
    // ★LOAD★
    // =========================
    else if(state == STATE_LOAD)
    {
        // =========================
        // 0. 슬롯 리스트 상태
        // =========================
        if(load_phase == 0)
        {
            if(event == EVENT_UP)
            {
                if(selected_slot > 0) selected_slot--;
            }
            else if(event == EVENT_DOWN)
            {
                if(selected_slot < 9) selected_slot++;
            }

            // 선택 변경 시 redraw
            if(prev_selected != selected_slot)
            {
                UI_DrawLoadSlots(selected_slot, slot_used);
                prev_selected = selected_slot;
            }

            if(event == EVENT_SELECT)
            {
                if(slot_used[selected_slot])
                {
                    // 👉 VIEW 상태 진입
                    load_phase = 1;

                    // 1. Flash 읽기
                    Load_ReadSlot(selected_slot, load_fft_data);

                    // 2. peak 계산
                    float peak = Load_CalcPeak(load_fft_data);

                    // 3. UI 초기화
                    LCD_DrawUIFrame();

                    // 4. 그래프
                    LCD_DrawFFT(load_fft_data, FFT_SIZE/2);

                    // 5. 상단 표시
                    LCD_UpdateLoadStatus(selected_slot, peak);
                }
            }

            if(event == EVENT_SELECT_LONG)
            {
                if(slot_used[selected_slot])
                {
                    load_phase = 2;  // DELETE 모드 진입
                    UI_DrawDeletePopup(selected_slot);
                }
            }

            // 리스트 상태에서 MENU → MAIN
            if(event == EVENT_MENU)
            {
                state = STATE_MAIN_MENU;
            }
        }

        // =========================
        // 1. 그래프 보기 상태
        // =========================
        else if(load_phase == 1)
        {
            // 그래프 상태에서 MENU → 리스트로 복귀
            if(event == EVENT_MENU)
            {
                load_phase = 0;
                prev_selected = 0xFF;

                UI_DrawLoadSlots(selected_slot, slot_used);
            }
        }
        // =========================
        // 2. DELETE (팝업 상태)
        // =========================
        else if(load_phase == 2)
        {
            if(event == EVENT_SELECT)
            {
                //삭제 실행
                Load_DeleteSlot(selected_slot);

                //리스트로 복귀
                load_phase = 0;
                prev_selected = 0xFF;

                UI_DrawLoadSlots(selected_slot, slot_used);
            }

            if(event == EVENT_MENU)
            {
                //취소 → 리스트 복귀
                load_phase = 0;
                prev_selected = 0xFF;

                UI_DrawLoadSlots(selected_slot, slot_used);
            }
        }
    }
    // =========================
    // ★SETTING★
    // =========================
    else if(state == STATE_SETTINGS)
    {
        if(event == EVENT_UP)
        {
            if(settings_index > 0) settings_index--;
            UI_DrawSettingsMenu(settings_index);
        }
        else if(event == EVENT_DOWN)
        {
            if(settings_index < 2) settings_index++;
            UI_DrawSettingsMenu(settings_index);
        }
        else if(event == EVENT_SELECT)
        {
            switch(settings_index)
            {
                case 0:// FFT SCALE
                    g_settings.fft_scale += 1000.0f;

                    if(g_settings.fft_scale > 10000.0f)
                        g_settings.fft_scale = 1000.0f;

                    Settings_Save();
                    break;

                case 1:// AVG COUNT
                    g_settings.avg_count += 10;
                    if(g_settings.avg_count > 100)
                        g_settings.avg_count = 10;
                    Settings_Save();
                    break;

                case 2: // BUZZER
                    g_settings.buzzer_enable = !g_settings.buzzer_enable;
                    Settings_Save();
                    break;
            }
            UI_DrawSettingsMenu(settings_index);
        }
        else if(event == EVENT_MENU)
        {
            state = STATE_MAIN_MENU;
        }
    }
    // =========================
    // ★PC_MODE★
    // =========================
    else if(state == STATE_PC_MODE)
    {
        // 애니메이션
        if(pc_state == PC_WAIT || pc_state == PC_MEASURING)
        {
            UI_UpdatePCAnimation(pc_state);
        }

        // =========================
        // UART 명령 처리
        // =========================
        if(cmd_ready)
        {
            cmd_ready = 0;

            if(strcmp(cmd_buf, "HELLO") == 0)
            {
                pc_state = PC_CONNECTED;
                UI_UpdatePCStatus(PC_CONNECTED);
            }
            else if(strcmp(cmd_buf, "START") == 0)
            {
                pc_state = PC_MEASURING;
                UI_UpdatePCStatus(PC_MEASURING);
            }
            else if(strcmp(cmd_buf, "STOP") == 0)
            {
                pc_state = PC_WAIT;
                UI_UpdatePCStatus(PC_WAIT);
            }
        }

        // =========================
        // ⭐ FFT 결과 UART 전송
        // =========================
        if(pc_state == PC_MEASURING)
        {
            if(FFT_Process_IsReady())
            {
                FFT_Process_Run();

                float *data = FFT_Process_GetMagnitude();

                // ===== UART 전송 =====
                char buf[1024];
                int len = 0;

                // 시작 헤더 (Qt에서 구분용)
                len += sprintf(buf + len, "FFT:");

                for(int i = 0; i < FFT_SIZE/2; i++)
                {
                    // float → int 변환 (속도 + 용량 절약)
                    int val = (int)data[i];

                    len += sprintf(buf + len, "%d,", val);
                }

                // 줄 끝
                len += sprintf(buf + len, "\r\n");

                HAL_UART_Transmit(&huart1, (uint8_t*)buf, len, 100);
            }
        }

        if(event == EVENT_MENU)
        {
            state = STATE_MAIN_MENU;
        }
    }
}

// =========================
// Flash 슬롯 구조
// [0x000000 + n * 4KB]
//  - 0~3   : MAGIC (유효 데이터 여부)
//  - 4~    : FFT 데이터 (float 배열)
// =========================
#define SLOT_MAGIC 0xDEADBEEF
#define FLASH_SLOT_SIZE 4096
#define FLASH_BASE_ADDR 0x000000
#define SLOT_MAGIC 0xDEADBEEF
#define FLASH_SLOT_SIZE 4096
#define FLASH_BASE_ADDR 0x000000

static void Save_LoadSlotStatus(void)
{
    uint32_t addr;
    uint32_t magic;

    for(int i = 0; i < SAVE_SLOT_COUNT; i++)
    {
        addr = FLASH_BASE_ADDR + i * FLASH_SLOT_SIZE;

        W25Q16_Read(addr, (uint8_t*)&magic, 4);

        if(magic == SLOT_MAGIC)
            slot_used[i] = 1;
        else
            slot_used[i] = 0;
    }
}
static void Save_WriteSlot(uint8_t slot, float *data)
{
    uint32_t addr = FLASH_BASE_ADDR + slot * FLASH_SLOT_SIZE;

    W25Q16_EraseSector(addr);

    uint32_t magic = SLOT_MAGIC;
    W25Q16_Write(addr, (uint8_t*)&magic, 4);

    W25Q16_Write(addr + 4, (uint8_t*)data, 128 * sizeof(float));
}

static void Load_ReadSlot(uint8_t slot, float *data)
{
    uint32_t addr = FLASH_BASE_ADDR + slot * FLASH_SLOT_SIZE;

    // magic 4바이트 건너뛰고 데이터 읽기
    W25Q16_Read(addr + 4, (uint8_t*)data, (FFT_SIZE/2) * sizeof(float));
}

static float Load_CalcPeak(float *data)
{
    float max = 0;
    int index = 0;

    for(int i = 1; i < FFT_SIZE/2; i++)
    {
        if(data[i] > max)
        {
            max = data[i];
            index = i;
        }
    }

    if(max > 80)
    {
        return (float)index * SAMPLE_FREQ / FFT_SIZE;
    }
    else
    {
        return 0;
    }
}

static void Load_DeleteSlot(uint8_t slot)
{
    uint32_t addr = FLASH_BASE_ADDR + slot * FLASH_SLOT_SIZE;

    // Flash 섹터 삭제
    W25Q16_EraseSector(addr);

    // 상태 갱신
    slot_used[slot] = 0;
}
static int CompareFFT(float *base, float *live, int size)
{
    // =========================
    // 1. base peak 찾기
    // =========================
    int base_peak_idx = 1;
    float base_peak_val = 0.0f;

    for(int i = 1; i < size; i++)
    {
        if(base[i] > base_peak_val)
        {
            base_peak_val = base[i];
            base_peak_idx = i;
        }
    }

    // =========================
    // 2. live peak
    // =========================
    int live_peak_idx = 1;
    float live_peak_val = 0.0f;

    for(int i = 1; i < size; i++)
    {
        if(live[i] > live_peak_val)
        {
            live_peak_val = live[i];
            live_peak_idx = i;
        }
    }
    // =========================
    // 3. base 신호가 너무 작으면 비교 무의미
    // =========================
    if(base_peak_val < 80.0f)
    {
        error_count = 0;   // ⭐ 초기화 중요
        return STATUS_GOOD;
    }

    // =========================
    // 4. 주파수 차이 계산
    // =========================
    float freq_diff = fabsf((base_peak_idx - live_peak_idx) * SAMPLE_FREQ / FFT_SIZE);

    // =========================
    // 5. peak 크기 비율
    // =========================
    float amp_ratio = live_peak_val / base_peak_val;

    if(amp_ratio < 1.0f)
        amp_ratio = 1.0f / amp_ratio;

    // =========================
    // 6. band energy 비교 (±3 bin)
    // =========================
    float band_base = 0.0f;
    float band_live = 0.0f;

    int start = base_peak_idx - 3;
    int end   = base_peak_idx + 3;

    if(start < 1) start = 1;
    if(end >= size) end = size - 1;

    for(int i = start; i <= end; i++)
    {
        band_base += base[i];
        band_live += live[i];
    }

    if(band_base < 1.0f) band_base = 1.0f;
    if(band_live < 1.0f) band_live = 1.0f;

    float band_ratio = band_live / band_base;

    if(band_ratio < 1.0f)
        band_ratio = 1.0f / band_ratio;

    // =========================
    // 7. 기존 판정 (instant)
    // =========================
    int instant_status;

    if(freq_diff < 5.0f && amp_ratio < 1.5f && band_ratio < 1.5f)
    {
        instant_status = STATUS_GOOD;
    }
    else if(freq_diff < 12.0f && amp_ratio < 2.5f && band_ratio < 2.5f)
    {
        instant_status = STATUS_CAUTION;
    }
    else
    {
        instant_status = STATUS_ERROR;
    }

    // =========================
    // ⭐ 8. 연속 ERROR 판정
    // =========================
    if(instant_status == STATUS_ERROR)
    {
        error_count++;
    }
    else
    {
        error_count = 0;
    }

    if(error_count >= 3)
    {
        return STATUS_ERROR;
    }

    // =========================
    // 9. 그 외는 기존 상태 유지
    // =========================
    return instant_status;
}
