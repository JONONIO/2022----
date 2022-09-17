/*
 * FIFO.h
 *
 *  Created on: 2022Äê6ÔÂ19ÈÕ
 *      Author: Ding
 */

#ifndef FIFO_FIFO_H_
#define FIFO_FIFO_H_
#include "xparameters.h"
#include "Write_FIFO_CNT.h"
#include "AXI_LITE_FIFO_READ.h"
#include "xil_io.h"
#define FIFO_READ_BASE XPAR_AXI_LITE_FIFO_READ_0_S00_AXI_BASEADDR
#define FIFO_WRITE_BASE XPAR_WRITE_FIFO_CNT_0_S00_AXI_BASEADDR


#define start_FIFO_wirite()  Xil_Out32(FIFO_WRITE_BASE+4,0x1)
#define is_FIFO_Full() Xil_In32(FIFO_WRITE_BASE)


#define start_FIFO_read()  Xil_Out32(FIFO_READ_BASE+4,0x0)


#define READ_FIFO_DATA() Xil_In32(FIFO_READ_BASE)

u32 FIFO_READ_PER();

#endif /* FIFO_FIFO_H_ */
