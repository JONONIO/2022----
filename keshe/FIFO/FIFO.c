/*
 * FIFO.c
 *
 *  Created on: 2022��6��19��
 *      Author: Ding
 */

#include "FIFO.h"
u32 FIFO_READ_PER(){
	u32 Data;
	start_FIFO_read();
	Data = READ_FIFO_DATA();
	return Data;
}
