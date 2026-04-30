#include "adxl345.h"

#define ADXL_ADDR (0x53<<1)

extern I2C_HandleTypeDef hi2c1;

/* =========================
   Sampling control
========================= */
volatile uint8_t adxl_sample_flag = 0;
//TIM ISR에서 언제든지 바뀔 수 있는 샘플링 타이밍을 제어하는 플래그 변수
//main loop → flag 확인 → 센서 읽기 → flag = 0

/* =========================
   Low level write
========================= */
HAL_StatusTypeDef ADXL345_Write(uint8_t reg, uint8_t data)
{
    uint8_t buf[2]; //I2C로 보낼 데이터를 담는 2바이트 버퍼
    buf[0] = reg;
    buf[1] = data;

    return HAL_I2C_Master_Transmit(&hi2c1, ADXL_ADDR, buf, 2, 100);
    /*
    &hi2c1,     // I2C 핸들
    ADXL_ADDR,  // 슬레이브 주소
    buf,        // 전송 데이터
    2,          // 길이
    100         // timeout
    */
}
/*HAL_StatusTypeDef : 이건 HAL에서 정의된 enum,값은 보통 4개
HAL_OK(0)
HAL_ERROR(1)
HAL_BUSY(2)
HAL_TIMEOUT(3)*/
/* =========================
   Init
========================= */
uint8_t ADXL345_Init(void)
{
    uint8_t id; //ADXL345의 ID 값(0xE5)을 저장할 변수

    if(HAL_I2C_Mem_Read(&hi2c1, ADXL_ADDR, 0x00, 1, &id, 1, 100) != HAL_OK)
        return 0;
/*HAL_I2C_Mem_Read(
    &hi2c1,     // I2C 핸들
    ADXL_ADDR,  // 장치 주소
    0x00,       // 읽을 레지스터
    1,          // 주소 크기 (1 byte)
    &id,        // 저장할 변수
    1,          // 읽을 길이
    100         // timeout
) */
    if(id != 0xE5)
        return 0;

    ADXL345_Write(0x31, 0x09); // FULL_RES ±4g
    ADXL345_Write(0x2C, 0x0C); // 400Hz
    ADXL345_Write(0x2D, 0x08); // Measure mode

    return 1;
}

/* =========================
   Read XYZ
========================= */
HAL_StatusTypeDef ADXL345_ReadXYZ(int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t data[6];
/*0x32 → X_L
0x33 → X_H
0x34 → Y_L
0x35 → Y_H
0x36 → Z_L
0x37 → Z_H*/

    HAL_StatusTypeDef ret;

    ret = HAL_I2C_Mem_Read(&hi2c1, ADXL_ADDR, 0x32, 1, data, 6, 100);
    if(ret != HAL_OK)
        return ret;

    *x = (int16_t)((data[1] << 8) | data[0]);
    *y = (int16_t)((data[3] << 8) | data[2]);
    *z = (int16_t)((data[5] << 8) | data[4]);

    return HAL_OK;
}

/*센서가 현재 이런 상태인 경우
X = +1000
Y = -500
Z = +200
센서 내부 상태
0x32 (X_L) = 0xE8
0x33 (X_H) = 0x03     → 0x03E8 = 1000

0x34 (Y_L) = 0x0C
0x35 (Y_H) = 0xFE     → 0xFE0C = -500

0x36 (Z_L) = 0xC8
0x37 (Z_H) = 0x00     → 0x00C8 = 200
 */
