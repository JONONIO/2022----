/*


 * FS.c
 *
 *  Created on: 2022Äê6ÔÂ16ÈÕ
 *      Author: Ding
 */

#include "FS.h"

float Fx_Analize(unsigned channel){
	unsigned int fx,fs_n,fs_p;
	unsigned int base = (channel==0)?FS_BASE_0:FS_BASE_1;

	float fs;
	fx = Fx_read(base);
	fs_n = Fs_n_read(base);
	fs_p = Fs_p_read(base);
	fs = (float)fx/((float)(fs_n+fs_p))*(float)FS;
	printf("BASE:%x",base);
	printf("FS_cnt channel:%d:  %d \r\n",channel,fs_p+fs_n);
	printf("Fx_cnt channel:%d:  %d \r\n",channel,fx);
	printf("FS channel:%d:  %u \r\n",channel,Fxy_read(base));
	printf("P channel:%d:  %d \r\n",channel,P_read(base));

	printf("FS channel:%d:  %f \r\n",channel,fs);
	return fs;
}
