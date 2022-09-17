# include "Fsmoniter.h"
# include "xparameters.h"
# include "xil_io.h"
#define FS_BASE_0 XPAR_FSMONITER_0_S00_AXI_BASEADDR
#define FS_BASE_1 XPAR_FSMONITER_1_S00_AXI_BASEADDR
//#define FS_read_fs_n

#define FS_N_ADDR 	FSMONITER_S00_AXI_SLV_REG0_OFFSET
#define FS_P_ADDR 	FSMONITER_S00_AXI_SLV_REG1_OFFSET
#define Fx_ADDR 	FSMONITER_S00_AXI_SLV_REG2_OFFSET
#define Fxy_ADDR 	FSMONITER_S00_AXI_SLV_REG3_OFFSET
#define P_ADDR		FSMONITER_S00_AXI_SLV_REG4_OFFSET

#define Fs_n_read(BASE)  Xil_In32(BASE+FS_N_ADDR)
#define Fs_p_read(BASE)  Xil_In32(BASE+FS_P_ADDR)
#define Fx_read(BASE) 	 Xil_In32(BASE+Fx_ADDR)
#define Fxy_read(BASE) 	 Xil_In32(BASE+Fxy_ADDR)
#define P_read(BASE) 	 Xil_In32(BASE+P_ADDR)

#define FS  100000000


float Fx_Analize();


