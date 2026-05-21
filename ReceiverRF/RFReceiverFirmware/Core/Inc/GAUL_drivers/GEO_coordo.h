/*
 * GEO_coordo.h
 *
 *  Created on: 20 May 2026
 *      Author: Samuel
 */

#ifndef INC_GAUL_DRIVERS_GEO_COORDO_H_
#define INC_GAUL_DRIVERS_GEO_COORDO_H_
#include <stdint.h>

union {
	float coordo[3];
	uint8_t I2C_REGISTERS[12];
} data_coordo;

extern data_coordo GPS_coordo;
#endif /* INC_GAUL_DRIVERS_GEO_COORDO_H_ */
