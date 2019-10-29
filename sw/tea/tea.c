/*
 * tea.c
 *
 *  Created on: 24 de out de 2019
 *      Author: adahil
 */

#include "tea.h"

void init(teaBase_t * tea, uint32_t * keys){
	tea->key_pt1 = *(keys+0x0);
	tea->key_pt2 = *(keys+0x1);
	tea->key_pt3 = *(keys+0x2);
	tea->key_pt4 = *(keys+0x3);
}

void encrypt(teaBase_t * tea, uint32_t * word, uint32_t * result){

	tea->data_pt1 = *(word+0x0);
	tea->data_pt2 = *(word+0x1);

	tea->control = 0x1;

	*(result+0x0) = tea->result_pt1;
	*(result+0x1) = tea->result_pt2;
}

void decrypt(teaBase_t * tea, uint32_t * word, uint32_t * result){

	tea->data_pt1 = *(word+0x0);
	tea->data_pt2 = *(word+0x1);

	tea->control = 0x2;

	*(result+0x0) = tea->result_pt1;
	*(result+0x1) = tea->result_pt2;
}
