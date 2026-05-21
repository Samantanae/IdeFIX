/*
 *  I2C MLX90393 Library
 */


#include "GAUL_drivers/MLX90393.h"


HAL_StatusTypeDef I2C_MLX90393_Init(I2C_MLX90393_HandleTypeDef *hi2cd) {
  HAL_StatusTypeDef res;
  uint8_t mlx_status;

  res = I2C_MLX90393_Reset(hi2cd, &mlx_status);

  return res;
}
