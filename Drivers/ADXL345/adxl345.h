#ifndef __ADXL345_H
#define __ADXL345_H

#include "stm32f4xx_hal.h"

/* =========================
   Address
========================= */
#define ADXL345_I2C_ADDR   (0x53 << 1)

/* =========================
   Sampling control
========================= */
extern volatile uint8_t adxl_sample_flag;

/* =========================
   Low level
========================= */
HAL_StatusTypeDef ADXL345_Write(uint8_t reg, uint8_t data);

/* =========================
   Init
========================= */
uint8_t ADXL345_Init(void);

/* =========================
   Data read
========================= */
HAL_StatusTypeDef ADXL345_ReadXYZ(int16_t *x, int16_t *y, int16_t *z);

#endif
