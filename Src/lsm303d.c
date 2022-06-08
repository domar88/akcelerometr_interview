/*
 * lsm303d.c
 *
 *  Created on: 21 mar 2020
 *      Author: USER
 */

#include "lsm303d.h"
#include "i2c.h"

I2C_HandleTypeDef i2c;

uint8_t lsm_read_reg(uint8_t reg)
  {
  uint8_t value = 0;
  HAL_I2C_Mem_Read(&hi2c1, 0x3a, reg, 1, &value, sizeof(value), HAL_MAX_DELAY);
  return value;
  }

void lsm_write_reg(uint8_t reg, uint8_t value)
    {
    HAL_I2C_Mem_Write(&hi2c1, 0x3a, reg, 1, &value, sizeof(value), HAL_MAX_DELAY);
    }

int16_t lsm_read_value(uint8_t reg)
  {
	  int16_t value = 0;
	  HAL_I2C_Mem_Read(&hi2c1, 0x3a, reg | 0x80, 1, (uint8_t*)&value, sizeof(value), HAL_MAX_DELAY);
	  return value;
  }
