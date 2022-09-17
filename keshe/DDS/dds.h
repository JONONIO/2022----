/*
 * dds902.h
 *
 *  Created on: 2022Äê6ÔÂ15ÈÕ
 *      Author: Ding
 */

#ifndef DDS_DDS_H_
#define DDS_DDS_H_
#include "DDS902.h"
#include "xparameters.h"
#include "xil_io.h"

#define DDS902_Write DDS902_mWriteReg
#define DDS902_BASE XPAR_DDS902_0_S00_AXI_BASEADDR

#define DDS902_ADDR0 XPAR_DDS902_0_S00_AXI_BASEADDR+DDS902_S00_AXI_SLV_REG0_OFFSET // reg 0
#define DDS902_ADDR1 XPAR_DDS902_0_S00_AXI_BASEADDR+DDS902_S00_AXI_SLV_REG1_OFFSET // reg 1

#define X_Write_REG0(DATA) Xil_Out32(DDS902_ADDR0,(unsigned int) DATA)
#define X_Write_REG1(DATA) Xil_Out32(DDS902_ADDR1,(unsigned int) DATA)

#define BIAS 0
#define DUTY 0
#define AMP 5

void DDS902WaveGen(unsigned long int fs , unsigned int style,unsigned int duty ,unsigned int bias,unsigned int amp );

unsigned int CaCualteFS(unsigned long int fs);

#endif /* DDS_DDS_H_ */
