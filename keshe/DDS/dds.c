/*
 * dds.c

 *
 *  Created on: 2022Äê6ÔÂ15ÈÕ
 *      Author: Ding
 */

#include "dds.h"
#include "xil_printf.h"
unsigned int CaCualteFS(unsigned long int fs){
	unsigned int K = 0;
	if (fs > 100000000){

		K = 1<<24;
	}
	else {

		K = (unsigned int)(((unsigned long long int)fs<<32)/100000000);
	}
	xil_printf("%d \n\r",2^31);
	printf("%lu \n\r",((unsigned long long int)1)<<33);
	xil_printf("%d \n\r",fs);
	printf("K11: %u\r\n",K);
	return K ;
}

void DDS902WaveGen(unsigned long int fs , unsigned int style,unsigned int duty ,unsigned int bias,unsigned int amp ){
	unsigned int K = CaCualteFS(fs);
	unsigned int REG0 =  0,REG1 =  0;
	unsigned int LOW = 0,HIGH=0;
	LOW = K&0x3f;

	HIGH = (K&0xffffffc0)>>6;
	printf("LOW:  %d",LOW);
	REG0 |=  (style& 0x3);
//	xil_printf("%d\r\n",REG0);
	REG0 |=  (((bias& 0xfff)<<2));
//	xil_printf("%d\r\n",REG0);
	REG0 |=  (((duty& 0x7f)<<14));
//	xil_printf("%d\r\n",REG0);
	REG0 |=  (((amp& 0x1f)<<21));
//	xil_printf("%d\r\n",REG0);
	REG0 |=  (((LOW& 0x3f)<<26));
	printf("RG0: %d",REG0);
	REG1 |=  (HIGH)& 0x3ffffff;
//	xil_printf("HHHHHHHHHHHHHHHHHHHHHH\r\n");
//	xil_printf("%d",REG0);

	X_Write_REG0(REG0);
	X_Write_REG1(REG1);


}
