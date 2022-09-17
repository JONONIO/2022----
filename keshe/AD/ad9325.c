/*
 * ad9325.c
 *
 *  Created on: 2022��6��17��
 *      Author: Ding
 */
#include "ad9325.h"

void AD9235_INIT(u32 clock_dev ){

	AD9235_EN(EN_AD9235);
	AD9235_CLOCK(clock_dev);
}


void AD9235_Enable(){
	AD9235_EN(EN_AD9235);
}
void AD9235_Disable(){
	AD9235_EN(0);
}
