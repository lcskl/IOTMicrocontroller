/*
 * tea.h
 *
 *  Created on: 23 de out de 2019
 *      Author: adahil
 */

#ifndef INCLUDE_TEA_H_
#define INCLUDE_TEA_H_

#include <stdio.h>

#define TEA_BASE_ADDR  0x44A00000

typedef volatile struct teaBase{
    uint32_t data_pt1;
    uint32_t data_pt2;
    uint32_t key_pt1;
    uint32_t key_pt2;
    uint32_t key_pt3;
    uint32_t key_pt4;
    uint32_t control;
    uint32_t result_pt1;
    uint32_t result_pt2;
}teaBase_t;

#define teaREG1 ((teaBase_t *)TEA_BASE_ADDR)

void init(teaBase_t * tea, uint32_t * keys);
void encrypt(teaBase_t * tea, uint32_t * word, uint32_t * result);
void decrypt(teaBase_t * tea, uint32_t * word, uint32_t * result);


#endif /* INCLUDE_TEA_H_ */
