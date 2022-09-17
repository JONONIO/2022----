#include <stdio.h>

#include "math.h"
#include "xparameters.h"
#include "string.h"
#include "../DELAY/delay.h"
#include "../LCD/ILI93xx.h"
#include "../LCD/lcd.h"
#include "../TOUCH/touch.h"
#include "../DDS/dds.h"
#include "platform.h"
#include "xgpio_l.h"
#include "xil_printf.h"
#include "../FS/FS.h"
#include "../AD/ad9325.h"
#include "../FIFO/FIFO.h"
//                      .......................................................DAC_BASE

//                     ........................................................ADC_BASE

#define K1   1
#define K2   1
#define K3   1

//                     ........................................................SWITCH_BASE
#define FIFO_DEPTH 4096
typedef struct
{
	u16 FsA;
	u16 Dut;
	u16 Amp;
	u16 Bia;
	char WaveStyle;
	float Voltage;
	float Freq;
	float THD;
	u32  Data[FIFO_DEPTH];
	char  state;
}Toch_pre;

//#define D5 0xc

//------------------------------------------------------------------------------DAC

//-------------------------------------------------------------------------------LCD
int TouchInBar(u16 x1, u16 y1, u16 x2, u16 y2);
void touchdetect(char *expression);
void display(void);
void LCD_DrawLine2(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	u16 t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向
	else if(delta_x==0)incx=0;//垂直线
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if(delta_y==0)incy=0;//水平线
	else{incy=-1;delta_y=-delta_y;}
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
	else distance=delta_y;
	for(t=0;t<=distance+1;t++ )//画线输出
	{
		LCD_DrawPoint(uRow,uCol,color);//画点
		xerr+=delta_x ;
		yerr+=delta_y ;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}
void Show_Cur_Data(Toch_pre toc_pre){

	LCD_ShowNum(70 , 570,toc_pre.FsA,3,YELLOW,0); //FSA
	LCD_ShowNum(70 , 610,toc_pre.Amp,3,YELLOW,0); //Amp
	LCD_ShowNum(310 , 570,toc_pre.Dut,3,YELLOW,0); //Dut
	LCD_ShowNum(310 , 610,toc_pre.Bia,3,YELLOW,0); //Bia

	switch(toc_pre.state){
			case '-' :{
				LCD_Fill(410-2, 70-2, 410+2, 70+2,RED);
				break;
			}
			case '+':{
				LCD_Fill(410-2, 120-2, 410+2, 120+2,RED);
				break;
			}
		}
		switch(toc_pre.WaveStyle){
			case '0' :{
				LCD_Fill(390-2, 320-2, 390+2, 320+2,YELLOW);
				break;
			}
			case '1':{
				LCD_Fill(390-2, 370-2, 390+2, 370+2,YELLOW);
				break;
			}
			case '2':{
				LCD_Fill(390-2, 420-2, 390+2, 420+2,YELLOW);
				break;
			}
			case '3':{
				LCD_Fill(390-2, 470-2, 390+2, 470+2,YELLOW);
				break;
			}
		}

}
void Clean_WaveFlag(){
	LCD_Fill(390-2, 320-2, 390+2, 320+2,BLACK);
	LCD_Fill(390-2, 370-2, 390+2, 370+2,BLACK);
	LCD_Fill(390-2, 420-2, 390+2, 420+2,BLACK);
	LCD_Fill(390-2, 470-2, 390+2, 470+2,BLACK);
}
void main(){
	char expBuf[90]={'\0'} ;
	char *expression;
	char touch;
	Toch_pre toc_pre={
			1,
			50,
			10,
			0,
		   '0',
			0.,
			0.,
			0.,
			NULL,
			'+'
		};
	// 
	expression = expBuf ;
	LCD_Init();
	init_platform();
	tp_dev.init();
	dispaly();
	// AD9235_CLOCK(CLOCK_2);
	AD9235_INIT(CLOCK_4);
	Show_Cur_Data(toc_pre);
	xil_printf("LCD and TOUCH Init Successfully!\n\r");
//	toc_pre.Data = (u32*)malloc(FIFO_DEPTH*sizeof(u32));
	while(1)
	   {
		touchdetect(&touch);
		if(touch=='-'){
			if (toc_pre.state == '+'){
				LCD_Fill(410-2, 120-2, 410+2, 120+2,BLACK);
			}
			toc_pre.state = '-';
			// LCD_Clear(BLACK);
			// dispaly();
			// DDS902WaveGen(1,1,DUTY,BIAS,AMP);
			// AD9235_Enable();
			// LCD_ShowString(330, 600, "2", WHITE, 0);
		}
		else if(touch=='+'){
			if (toc_pre.state == '-'){
				LCD_Fill(410-2, 70-2, 410+2, 70+2,BLACK);

			}
			toc_pre.state = '+';
		}
		else if(touch=='F'){
			if(toc_pre.state == '+'){
				if (toc_pre.FsA<100){toc_pre.FsA +=1;}
				else {toc_pre.FsA = 100;}
			}
			else if(toc_pre.state == '-'){
				if (toc_pre.FsA>1){toc_pre.FsA -=1;}
				else {toc_pre.FsA = 1;}
			}
			else {
				toc_pre.FsA = 1;
			}
			        // LCD_Clear(BLACK);
					// dispaly();
					// DDS902WaveGen(1,2,DUTY,BIAS,AMP);
					// LCD_ShowString(100, 680, "3", WHITE, 0);
					// LCD_Clear(BLACK);
					// dispaly();
					// DDS902WaveGen(1,3,DUTY,BIAS,AMP);
					// LCD_ShowString(330, 680, "4", WHITE, 0);
		}
		else if(touch=='D'){
			if(toc_pre.state == '+'){
				if (toc_pre.Dut<99){toc_pre.Dut +=1;}
				else {toc_pre.Dut = 50;}
			}
			else if(toc_pre.state == '-'){
				if (toc_pre.Dut>1){toc_pre.Dut -=1;}
				else {toc_pre.Dut == 50;}
			}
			else {
				toc_pre.Dut = 50;
			}
		}
		else if(touch=='A'){
			if(toc_pre.state == '+'){
				if (toc_pre.Amp<12){toc_pre.Amp +=1;}
				else {toc_pre.Amp == 12;}
			}
			else if(toc_pre.state == '-'){
				if (toc_pre.Amp>1){toc_pre.Amp -=1;}
				else {toc_pre.Amp == 1;}
			}
			else {
				toc_pre.Amp = 1;
			}
		}
		else if(touch=='0'){
				if (toc_pre.WaveStyle!=touch){
					Clean_WaveFlag();
				}
				toc_pre.WaveStyle = '0';
			        //  LCD_Clear(BLACK);
			        //  dispaly();
			        //  fs = Fx_Analize(0);
			         
				    //  LCD_ShowString(100, 760, "5", WHITE, 0);
		}
		else if(touch=='1'){
				if (toc_pre.WaveStyle!=touch){
					Clean_WaveFlag();
				}
				toc_pre.WaveStyle = '1';
		}
		else if(touch=='2'){
				if (toc_pre.WaveStyle!=touch){
					Clean_WaveFlag();
				}
				toc_pre.WaveStyle = '2';
		}
		else if(touch=='3'){
				if (toc_pre.WaveStyle!=touch){
					Clean_WaveFlag();
				}
				toc_pre.WaveStyle = '3';
		}
		else if(touch=='S'){
			// LCD_Clear(BLACK);
			// dispaly();
			// DDS902WaveGen(1,0,DUTY,BIAS,AMP);
			// AD9235_Disable();
//			delay_ms(1000);
//			print("3");
//			toc_pre.Freq = Fx_Analize(1);
			printf("111:::%d\r\n",is_FIFO_Full());
//			u32 Da = 0;
//			for(int i = 0;i<FIFO_DEPTH;i++){
//				Da = FIFO_READ_PER();
//				printf("%d-->%u \r\n",i,Da);
//			}
//			u32 name = (u32) toc_pre.Freq;
//			printf("%u \r\n",name);
//			LCD_ShowNum(70, 750,name,10,YELLOW,0);
			// LCD_ShowString(100, 600, "1", WHITE, 0);
		}
		else if(touch=='R'){
			toc_pre.FsA = 1;
			toc_pre.Dut = 50;
			toc_pre.Amp = 10;
			toc_pre.Bia = 0;
			toc_pre.WaveStyle = '0';
			toc_pre.Voltage = 0.;
			toc_pre.Freq = 0.;
			toc_pre.THD = 0.;
			for(int i = 0;i<FIFO_DEPTH;i++){
				toc_pre.Data[i] = 0;
			}
//			toc_pre.Data = (void*)0;
			toc_pre.state ='+';
			LCD_Fill(410-2, 120-2, 410+2, 120+2,BLACK);
			LCD_Fill(410-2, 70-2, 410+2, 70+2,BLACK);
			// delay_ms(1000);
			// toc_pre.Freq = Fx_Analize(1);
			// LCD_ShowNum(70, 750,(int)toc_pre.Freq,10,WRITE,0);
			// LCD_ShowString(100, 600, "1", WHITE, 0);
		}
		else if (touch == 'G'){
			start_FIFO_wirite();
		}
		Show_Cur_Data(toc_pre);
		
	   }



}
int TouchInBar(u16 x1, u16 y1, u16 x2, u16 y2)
{
	if(tp_dev.x[0]>x1 && tp_dev.x[0]<x2 && tp_dev.y[0]>y1 && tp_dev.y[0]<y2) return 1;
	else return 0;
}

void dispaly(){


		LCD_Fill(9,20,11,553,RED);
		LCD_Fill(9,550,470,553,RED);
		// AD (10,50)-> (370,298)
		// FFT  AD (10,302)-> (370,550)

		// 单元格 (w,h)->(60,50)
		//---------------竖向线 ------------------------
		LCD_DrawLine2(70,50,70,550,WHITE);   //1
		LCD_DrawLine2(130,50,130,550,WHITE); //10
		LCD_DrawLine2(190,50,190,550,WHITE); //100
		LCD_DrawLine2(250,50,250,550,WHITE); //1k
		LCD_DrawLine2(310,50,310,550,WHITE); //10k
		LCD_DrawLine2(370,50,370,550,WHITE); //100k
		// LCD_DrawLine2(470,20,470,550,WHITE); //xx
		LCD_Fill(370,20,375,550,LIGHTBLUE);
		//----------------------------------------------

		//---------------横向线 ------------------------
		LCD_Fill(10,45,470,50,LIGHTBLUE);
		LCD_DrawLine2(10,100,370,100,WHITE); //9
		LCD_DrawLine2(10,150,370,150,WHITE); //8
		LCD_DrawLine2(10,200,370,200,WHITE); //7
		LCD_DrawLine2(10,250,370,250,WHITE); //6
		//.........................................
		LCD_Fill(10,300-2,470,300+2,LIGHTBLUE); //5
		// LCD_DrawLine2(10,300,370,300,WHITE);
		// LCD_Fill(10,300,470,300,LIGHTBLUE);
		LCD_DrawLine2(10,350,370,350,WHITE); //4
		LCD_DrawLine2(10,400,370,400,WHITE); //3
		LCD_DrawLine2(10,450,370,450,WHITE); //2
		LCD_DrawLine2(10,500,370,500,WHITE); //1
		//----------------------------------------------

		//---------------AD侧栏-------------------------
		POINT_COLOR = GRAYBLUE;
		LCD_DrawRectangle(375, 51, 475, 99);
		LCD_ShowString(420, 70,"-",WHITE,0);
		
		POINT_COLOR = WHITE;
		POINT_COLOR = GRAYBLUE;
		LCD_DrawRectangle(375, 101, 475, 149);
		LCD_ShowString(420, 120,"+",WHITE,0);
		POINT_COLOR = WHITE;
		POINT_COLOR = GRAYBLUE;
		LCD_DrawRectangle(375, 151, 475, 199);
		LCD_ShowString(400, 170,"FsA",WHITE,0);
		// LCD_ShowString(400, 170,"x",WHITE,0);
		// LCD_ShowNum(430,170,11,2,BROWN,0);
		POINT_COLOR = WHITE;
		POINT_COLOR = GRAYBLUE;
		LCD_DrawRectangle(375, 201,  475, 249);
		LCD_ShowString(400, 220,"Dut",WHITE,0);
		LCD_DrawRectangle(375, 251,  475, 298);
		LCD_ShowString(400, 270,"Amp",WHITE,0);
		POINT_COLOR = WHITE;
		//----------------------------------------------

		//---------------FFT侧栏------------------------
		POINT_COLOR = GRAYBLUE;
		LCD_DrawRectangle(375, 302, 475, 349);
		LCD_ShowString(400, 320,"Sin",WHITE,0);
		POINT_COLOR = WHITE;
		POINT_COLOR = GRAYBLUE;
		LCD_DrawRectangle(375, 351, 475, 399);
		LCD_ShowString(400, 370,"Squ",WHITE,0);
		POINT_COLOR = WHITE;
		POINT_COLOR = GRAYBLUE;
		LCD_DrawRectangle(375, 401, 475, 449);
		LCD_ShowString(400, 420,"Tri",WHITE,0);
		// LCD_ShowString(400, 170,"x",WHITE,0);
		// LCD_ShowNum(430,170,11,2,BROWN,0);
		POINT_COLOR = WHITE;
		POINT_COLOR = GRAYBLUE;
		LCD_DrawRectangle(375, 451,  475, 499);
		LCD_ShowString(400, 470,"Saw",WHITE,0);
		LCD_DrawRectangle(375, 501,  475, 549);
		LCD_ShowString(410, 520,"RS",WHITE,0);
		POINT_COLOR = WHITE;
		//----------------------------------------------

		//----------------------------------------------
        LCD_DrawRectangle(1, 560, 60, 600);
		LCD_DrawRectangle(1, 600, 60, 640);
		LCD_DrawRectangle(60, 560, 240, 600);
		LCD_DrawRectangle(60, 600, 240, 640);
		//----------------------------------------------
		LCD_DrawRectangle(240, 560, 300, 600);
		LCD_DrawRectangle(240, 600, 300, 640);
		LCD_DrawRectangle(300, 560, 479, 600);
		LCD_DrawRectangle(300, 600, 479, 640);
		//----------------------------------------------
		LCD_ShowString(10 , 570, "FsA", WHITE, 0);
		LCD_ShowString(10 , 610, "Amp", WHITE, 0);
	//	LCD_ShowString(100, 490, "5", WHITE, 0);
		LCD_ShowString(250, 570, "Dut", WHITE, 0);
		LCD_ShowString(250 , 610, "Bia", WHITE, 0);
	//	LCD_ShowString(330, 490, "-", WHITE, 0);
	//
        LCD_DrawRectangle(1, 640, 60, 720);
		LCD_DrawRectangle(60, 640, 240, 720);
		LCD_DrawRectangle(240, 640, 300, 720);
		LCD_DrawRectangle(300, 640, 479, 720);
		LCD_ShowString(10, 670, "Vol", WHITE, 0);
	//	LCD_ShowString(100, 610, "8", WHITE, 0);
		LCD_ShowString(250, 670, "THD", WHITE, 0);
	//	LCD_ShowString(330, 610, "*", WHITE, 0);-[=]

        LCD_DrawRectangle(1, 720, 60, 799);
		LCD_DrawRectangle(60, 720, 479, 799);


		LCD_ShowString(10, 750, "Fre", WHITE, 0);

		LCD_Fill(250, 730, 350, 790,BRRED); // Start
		LCD_ShowString(260 , 750, "Start", DARKBLUE, 1);
		LCD_Fill(370, 730, 470, 790,BRRED); // GenWave
		LCD_ShowString(380 , 750, "Gener", DARKBLUE, 1);
		//LCD_ShowString(175, 730, "0", WHITE, 0);
		//LCD_ShowString(240, 730, ")", WHITE, 0);
		//LCD_ShowString(415, 730, "/", WHITE, 0);
}

void touchdetect(char *expression)
{
	char tempLCD;
	while((tp_dev.sta) == '@'||(tp_dev.sta) == 0)
    {
    	tp_dev.scan(0);
    	delay_ms(10);
    }
	if(tp_dev.sta)
	{
		 if(TouchInBar(370, 730, 470, 790))
		{
			tempLCD = 'G';
			*expression = tempLCD;
		}
		else if(TouchInBar(250, 730, 350, 790))
		{
			tempLCD = 'S';
			*expression = tempLCD;
		}
		else if(TouchInBar(375, 51, 475, 99))//RS
		{
			tempLCD = '-';
			*expression = tempLCD;
		}
		else if(TouchInBar(375, 101, 475, 149))//delete
		{
			tempLCD = '+';
			*expression = tempLCD;
		}
		else if(TouchInBar(375, 151, 475, 199))//.
		{
			tempLCD = 'F';
			*expression = tempLCD;
		}
		else if(TouchInBar(375, 201,  475, 249))//.
		{
			tempLCD = 'D';
			*expression = tempLCD;
		}
		else if(TouchInBar(375, 251,  475, 298))//.
		{
			tempLCD = 'A';
			*expression = tempLCD;
		}
		else if(TouchInBar(375, 302, 475, 349))//.
		{
			tempLCD = '0';
			*expression = tempLCD;
		}
		else if(TouchInBar(375, 351, 475, 399))//.
		{
			tempLCD = '1';
			*expression = tempLCD;
		}
		else if(TouchInBar(375, 401, 475, 449))//.
		{
			tempLCD = '2';
			*expression = tempLCD;
		}
		else if(TouchInBar(375, 451,  475, 499))//.
		{
			tempLCD = '3';
			*expression = tempLCD;
		}
		else if(TouchInBar(375, 501,  475, 549))//.
		{
			tempLCD = 'R';
			*expression = tempLCD;
		}
		else
		{
			tempLCD = 0;
			*expression = tempLCD;
		}
	}
	while((tp_dev.sta)!= '@')
	{
	    tp_dev.scan(0);
	    delay_ms(10);
	}
}
