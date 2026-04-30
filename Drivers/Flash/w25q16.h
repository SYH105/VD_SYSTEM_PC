#ifndef __W25Q16_H__
#define __W25Q16_H__

#include "stm32f4xx_hal.h"

// ===== API =====

uint32_t W25Q16_ReadID(void);

void W25Q16_Read(uint32_t addr, uint8_t *buf, uint16_t len);

void W25Q16_Write(uint32_t addr, uint8_t *buf, uint16_t len);

void W25Q16_EraseSector(uint32_t addr);

uint8_t W25Q16_IsBusy(void);

#endif
