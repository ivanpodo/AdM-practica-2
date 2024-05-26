/*
 * asm_func.h
 *
 *  Created on: May 15, 2024
 *      Author: denis
 */

#ifndef INC_ASM_FUNC_H_
#define INC_ASM_FUNC_H_

void pack32to16 (int32_t * vectorIn, int16_t *vectorOut, uint32_t longitud);

uint32_t max (int32_t * vectorIn, uint32_t longitud);

void downSample (int32_t * vectorIn, int32_t * vectorOut, uint32_t longitud, uint32_t N);

void invertir (uint16_t * vector, uint32_t longitud);

#endif /* INC_ASM_FUNC_H_ */
