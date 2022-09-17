/*
 * ad9325.h
 *
 *  Created on: 2022Äê6ÔÂ17ÈÕ
 *      Author: Ding
 */

#ifndef AD_AD9325_H_
#define AD_AD9325_H_
#include "xparameters.h"
#include "xil_io.h"
#include "AD9235.h"
#define AD9235_BASE XPAR_AD9235_0_S00_AXI_BASEADDR

#define DATA_ADD AD9235_BASE+AD9235_S00_AXI_SLV_REG1_OFFSET
#define EN_ADD AD9235_BASE+AD9235_S00_AXI_SLV_REG0_OFFSET

#define CLOCK_2 0
#define CLOCK_4 1
#define CLOCK_6 2

#define EN_AD9235 0x1
#define AD9235_CLOCK(DATA) Xil_Out32((DATA_ADD),DATA)
#define AD9235_EN(EN) Xil_Out32((EN_ADD),EN)

void AD9235_INIT(u32 clock_dev );

void AD9235_Enable();
void AD9235_Disable();
#endif /* AD_AD9325_H_ */
