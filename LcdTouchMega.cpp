/*  last update 7/2/20 */
/*  Avi Hayun */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>
#include "TouchMega.h"
#include "glcdfonth.h"
#include "registers.h"
#include <SPI.h>
#include "Arduino.h"

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

TouchMega::TouchMega(int8_t TypeLCD){
	//pin lcd
	_type_lcd = TypeLCD;

	_wr = 39;  // PG2
	_cs   = 40;  //PG1
	_dc   = 38;  //PD7

	// pin touch
	_Tcs=53;    //PBO  
	_Tirq=44;   //PL5

	_x = 0;
	_y = 0;
	_width    = WIDTH;
	_height   = HEIGHT;
	_fontSize = 1;
	_rotation  = 0;
	_color = BLACK;
	_background = WHITE;
	numbera = 0;
	numberb = 0;
	number1 = 1;
	number2 = 64;
}


//send data 8-bit to spi
void TouchMega::writeData(uint8_t data8){
	digitalWrite(_dc, HIGH);//LCD_RS=1;

	if(_type_lcd==0)
		PORTA =data8;
	else
		PORTC =data8;

	PORTG&= 0xfb;//PG2=wr=0 
	PORTG |=0x04; //PG2=wr=1
}

//write command 8-bit to spi
void TouchMega::writeCommand(uint8_t cmd){	
	digitalWrite(_dc, LOW);

	if(_type_lcd==0)
		PORTA =cmd;
	else
		PORTC =cmd;

	PORTG&= 0xfb;//PG2=wr=0 
	PORTG |=0x04; //PG2=wr=1
}


void TouchMega::pushColor(uint16_t color) {

	digitalWrite(_dc, HIGH);

	if(_type_lcd==0)
		PORTA =color >> 8;
	else
		PORTC =color >> 8;

	PORTG&= 0xfb;//PG2=wr=0 
	PORTG |=0x04; //PG2=wr=1

	if(_type_lcd==0)
		PORTA =color;
	else
		PORTC =color;

	PORTG&= 0xfb;//PG2=wr=0 
	PORTG |=0x04; //PG2=wr=1
}


void TouchMega::begin(){
	// set ports

	pinMode(_Tcs, OUTPUT);
	pinMode(_Tirq, INPUT_PULLUP);
	digitalWrite(_Tcs,HIGH);

	pinMode(_dc, OUTPUT);
	pinMode(_cs, OUTPUT);
	pinMode(_wr, OUTPUT);

	pinMode(43, OUTPUT);//RD
	pinMode(41, OUTPUT);//RST

	digitalWrite(43, HIGH);
	digitalWrite(_cs, LOW);
	digitalWrite(_wr, HIGH);

	digitalWrite(41, LOW);
	_delay_ms(50);
	digitalWrite(41, HIGH);
	_delay_ms(10);

	DDRA=0xFF;
	DDRC=0xFF;

	SPI.begin(); 

	digitalWrite(_dc, HIGH);

	writeCommand(0xEE);//Set EQ
	writeData(0x02);
	writeData(0x01);
	writeData(0x02);
	writeData(0x01);

	writeCommand(0xED);//Set DIR TIM
	writeData(0x00);
	writeData(0x00);
	writeData(0x9A);
	writeData(0x9A);
	writeData(0x9B);
	writeData(0x9B);
	writeData(0x00);
	writeData(0x00);
	writeData(0x00);
	writeData(0x00);
	writeData(0xAE);
	writeData(0xAE);
	writeData(0x01);
	writeData(0xA2);
	writeData(0x00);

	writeCommand(0xB4);//Set RM, DM
	writeData(0x00);//

	writeCommand(0xC0); //Set Panel Driving
	writeData(0x10); //REV SM GS
	writeData(0x3B); // NL[5:0]
	writeData(0x00); //SCN[6:0]
	writeData(0x02); //NDL 0 PTS[2:0]
	writeData(0x11); //PTG ISC[3:0]

	writeCommand(0xC1);//
	writeData(0x10);//line inversion

	writeCommand(0xC8);//Set Gamma
	writeData(0x00); //KP1,KP0
	writeData(0x46); //KP3,KP2
	writeData(0x12); //KP5,KP4
	writeData(0x20); //RP1,RP0
	writeData(0x0c); //VRP0 01
	writeData(0x00); //VRP1
	writeData(0x56); //KN1,KN0
	writeData(0x12); //KN3,KN2
	writeData(0x67); //KN5,KN4
	writeData(0x02); //RN1,RN0
	writeData(0x00); //VRN0
	writeData(0x0c); //VRN1 01

	writeCommand(0xD0);//Set Power
	writeData(0x44);//DDVDH :5.28
	writeData(0x42); // BT VGH:15.84 VGL:-7.92
	writeData(0x06);//VREG1 4.625V

	writeCommand(0xD1);//Set VCOM
	writeData(0x73); //VCOMH
	writeData(0x16);

	writeCommand(0xD2);
	writeData(0x04);
	writeData(0x22); //12

	writeCommand(0xD3);
	writeData(0x04);
	writeData(0x12);

	writeCommand(0xD4);
	writeData(0x07);
	writeData(0x12);

	writeCommand(0xE9); //Set Panel
	writeData(0x00);

	writeCommand(0xC5); //Set Frame rate
	writeData(0x08); //61.51Hz

	writeCommand(0X36);
	writeData(0X0a);

	writeCommand(0X3A);
	writeData(0X55);

	writeCommand(0X2A);
	writeData(0X00);
	writeData(0X00);
	writeData(0X01);
	writeData(0X3F);

	writeCommand(0X2B);
	writeData(0X00);
	writeData(0X00);
	writeData(0X01);
	writeData(0XE0);
	_delay_ms(20);

	writeCommand(0x11);//Sleep Out
	_delay_ms(120);

	writeCommand(0x35);
	writeData(0x00);//TE ON

	writeCommand(0x29); //Display On
	_delay_ms(5);

	//SetParam();
	setRotation(3);
	fillScreen(BLACK);
	setTextSize(2);
	setTextColor(WHITE, BLACK);
	set(325, 3770, 3890, 320);
}

// all display one color
void TouchMega::fillScreen (uint16_t color){
	int16_t i,j;
	int8_t DH,DL;
	DH=color>>8;
	DL=color;

	_x = 0;
	_y = 0;
	sector(0,0,_width-1,_height-1);
	digitalWrite(_dc, HIGH);

	if(_type_lcd==DB15_8) //0
		for (i=0;i<_height;i++)
			for (j=0;j<_width;j++){
				PORTA =DH;

				PORTG&= 0xfb;//cbi(PG, 2);
				PORTG |=0x04; //sbi(PG, 2);

				PORTA =DL;

				PORTG&= 0xfb;//PG2=wr=0 
				PORTG |=0x04; //PG2=wr=1
			}

	else if(_type_lcd==DB7_0) //1
		for (i=0;i<_height;i++)
			for (j=0;j<_width;j++){

				PORTC =DH;

				PORTG&= 0xfb;//cbi(PG, 2);
				PORTG |=0x04; //sbi(PG, 2);

				PORTC =DL;

				PORTG&= 0xfb;//PG2=wr=0 
				PORTG |=0x04; //PG2=wr=1
			}
}

void TouchMega::sector(int16_t x0, int16_t y0, int16_t x1, int16_t y1){
	writeCommand(CASET); // 0x2A Column addr set
	writeData(x0 >> 8);
	writeData(x0 & 0xFF);     // XSTART
	writeData(x1 >> 8);
	writeData(x1 & 0xFF);     // XEND

	writeCommand(PASET); // 0x2B Row addr set
	writeData(y0>>8);
	writeData(y0);     // YSTART
	writeData(y1>>8);
	writeData(y1);     // YEND

	writeCommand(RAMWR); //0x2C  write to RA
}


// Draw a character
void TouchMega::putChar(uint8_t c){
	int8_t i,j;
	uint8_t line;

	if ((_x >= _width) || (_y >= _height) || ((_x + 6 * _fontSize - 1) < 0) || ((_y + 8 * _fontSize - 1) < 0))
		return;

	for (i=0; i<6; i++ )
	{
		if (i == 5)
			line = 0x0;
		else
			line =pgm_read_byte(font+(c*5)+i);
		for (j = 0; j<8; j++)
		{
			if (line & 0x1)
			{
				if (_fontSize == 1) // default size
					drawPixel(_x+i, _y+j,_color);
				else
					fillRect(_x+(i*_fontSize), _y+(j*_fontSize), _fontSize, _fontSize,_color);
			}
			else if (_background != _color)
			{
				//switchColors();
				if (_fontSize == 1) // default size
					drawPixel(_x+i, _y+j,_background);//_color);
				else  // big size
					fillRect(_x+i*_fontSize, _y+j*_fontSize, _fontSize, _fontSize,_background);//_color);
				//switchColors();
			}
			line >>= 1;
		}
	}
	_x += 6 * _fontSize;
}


void TouchMega::write (char c){
	putChar(c);
}

void TouchMega::writeln (char c){
	write (c);
	_x = 0;
	_y += 8 * _fontSize +2;
}

void TouchMega::print (int16_t num, uint8_t radix){
	char str[17];
	itoa (num, str, radix);
	print (str);
}

void TouchMega::println (int16_t num, uint8_t radix){
	print (num, radix);
	_x = 0;
	_y += 8 * _fontSize +2;
}

void TouchMega::println (){
	_x = 0;
	_y += 8 * _fontSize +2;
}

void TouchMega::print (uint16_t num, uint8_t radix){
	char str[16];

	itoa (num, str, radix);
	print (str);
}

void TouchMega::println (uint16_t num, uint8_t radix){
	print (num, radix);
	_x = 0;
	_y += 8 * _fontSize +2;
}

void TouchMega::print (uint8_t num, uint8_t radix){
	char str[8];

	itoa (num, str, radix);
	print (str);
}

void TouchMega::println (uint8_t num, uint8_t radix){
	print (num, radix);
	_x = 0;
	_y += 8 * _fontSize +2;
}

void TouchMega::print (int8_t num, uint8_t radix){
	char str[8];

	itoa (num, str, radix);
	print (str);
}

void TouchMega::println (int8_t num, uint8_t radix){
	print (num, radix);
	_x = 0;
	_y += 8 * _fontSize +2;
}


void TouchMega::print (char *str){
	uint8_t i,temp;
	for (i=0;str[i];i++){
		temp=str[i];
		if(temp <0xb0) 	write(temp);
	}
}


void TouchMega::print (String txtMsg){
	int8_t i,temp;

	for(i=0; i<txtMsg.length(); i++){
		temp=txtMsg[i];
		if(temp <0xb0) write(temp);
	}
}


void TouchMega::println (String txtMsg){
	print (txtMsg);
	_x = 0;
	_y += 8 * _fontSize +2;
}


void TouchMega::println (char *str){
	print (str);
	_x = 0;
	_y += 8 * _fontSize +2;
}

void TouchMega::print (double fnum,uint8_t s){
	int16_t num1,num2,e;
	double fnum1;
	if (s>5) s=5;
	e = pow (10,s);
	num1 = trunc(fnum);
	fnum1 = fnum - num1;
	num2 = abs(round(fnum1*e));

	print (num1);
	write ('.');
	print (num2);
}

void TouchMega::println (double fnum, uint8_t s){
	print (fnum,s);
	_x = 0;
	_y += 8 * _fontSize +2;
}


void TouchMega::print (uint16_t num){
	char str[8];

	ltoa (num, str, DEC);
	print (str);
}


void TouchMega::println (uint16_t num){
	print (num);
	_x = 0;
	_y += 8 * _fontSize +2;
}

void TouchMega::print (uint16_t x,uint16_t y,char *str,uint16_t fontSize, uint16_t fColor){
	setCursor(x,y);
	setTextSize(fontSize);
	setTextColor(fColor);
	print (str);
}

void TouchMega::print (uint16_t x,uint16_t y,char *str,uint16_t fontSize, uint16_t fColor,uint16_t bColor){
	setCursor(x,y);
	setTextSize(fontSize);
	setTextColor(fColor,bColor);
	print (str);
}

void TouchMega::print (uint16_t x,uint16_t y,String txtMsg,uint16_t fontSize, uint16_t fColor){
	setCursor(x,y);
	setTextSize(fontSize);
	setTextColor(fColor);
	print (txtMsg);
}

void TouchMega::print (uint16_t x,uint16_t y,String txtMsg,uint16_t fontSize, uint16_t fColor,uint16_t bColor){
	setCursor(x,y);
	setTextSize(fontSize);
	setTextColor(fColor,bColor);
	print (txtMsg);
}


void TouchMega::drawPixel (int16_t x, int16_t y,uint16_t color){
	int8_t DH,DL;
	DH=color>>8;
	DL=color;
	sector(x,y,x,y);
	digitalWrite(_dc, HIGH);

	if(_type_lcd==DB15_8) //0
	{
		PORTA =DH;
		PORTG&= 0xfb;//cbi(PG, 2);
		PORTG |=0x04; //sbi(PG, 2);

		PORTA =DL;
		PORTG&= 0xfb;//cbi(PG, 2);
		PORTG |=0x04; //sbi(PG, 2);
	}

	else if(_type_lcd==DB7_0) //1
	{
		PORTC =DH;
		PORTG&= 0xfb;//cbi(PG, 2);
		PORTG |=0x04; //sbi(PG, 2);

		PORTC =DL;
		PORTG&= 0xfb;//cbi(PG, 2);
		PORTG |=0x04; //sbi(PG, 2);
	}
}

void TouchMega::drawHLine (int16_t x, int16_t y, int16_t w,uint16_t color){
	int8_t DH,DL;
	DH=color>>8;
	DL=color;
	sector(x,y,x+w-1,y);
	digitalWrite(_dc, HIGH);

	if(_type_lcd==DB15_8) //0
		while (w--){

			PORTA =DH;
			PORTG&= 0xfb;//cbi(PG, 2);
			PORTG |=0x04; //sbi(PG, 2);

			PORTA =DL;
			PORTG&= 0xfb;//cbi(PG, 2);
			PORTG |=0x04; //sbi(PG, 2);
		}


	else if(_type_lcd==DB7_0) //1
		while (w--){

			PORTC =DH;
			PORTG&= 0xfb;//cbi(PG, 2);
			PORTG |=0x04; //sbi(PG, 2);

			PORTC =DL;
			PORTG&= 0xfb;//cbi(PG, 2);
			PORTG |=0x04; //sbi(PG, 2);
		}	
}

void TouchMega::drawFastHLine (int16_t x, int16_t y, int16_t w,uint16_t color){
	drawHLine ( x,  y,  w, color);	
}


void TouchMega::drawVLine (int16_t x, int16_t y, int16_t h,uint16_t color){
	int8_t DH,DL;
	DH=color>>8;
	DL=color;
	sector(x,y,x, y+h-1);
	digitalWrite(_dc, HIGH);

	if(_type_lcd==DB15_8) //0
		while (h--){

			PORTA =DH;
			PORTG&= 0xfb;//cbi(PG, 2);
			PORTG |=0x04; //sbi(PG, 2);

			PORTA =DL;
			PORTG&= 0xfb;//cbi(PG, 2);
			PORTG |=0x04; //sbi(PG, 2);
		}	

	else if(_type_lcd==DB7_0) //1
		while (h--){

			PORTC =DH;
			PORTG&= 0xfb;//cbi(PG, 2);
			PORTG |=0x04; //sbi(PG, 2);


			PORTC =DL;
			PORTG&= 0xfb;//cbi(PG, 2);
			PORTG |=0x04; //sbi(PG, 2);
		}	
}


void TouchMega::drawFastVLine (int16_t x, int16_t y, int16_t h,uint16_t color){
	drawVLine ( x,  y,  h,color);	
}

void TouchMega::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color) {
	int16_t steep = ABS(y1 - y0) > ABS(x1 - x0);
	if (steep) {
		SWAP(x0, y0);
		SWAP(x1, y1);
	}

	if (x0 > x1) {
		SWAP(x0, x1);
		SWAP(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = ABS(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for (; x0<=x1; x0++) {
		if (steep) {
			drawPixel(y0, x0, color);
		} else {
			drawPixel(x0, y0,color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void TouchMega::drawLinePolar (int16_t x0, int16_t y0, int16_t r, int16_t angle,uint16_t color){
	float x,y;
	uint16_t x1;
	uint16_t y1;

	x = x0 + r*cos (angle*M_PI/180);
	y = y0 + r*sin (angle*M_PI/180);

	x1 = round (x);
	y1 = round (y);
	drawLine (x0,y0,x1,y1,color);
}

void TouchMega::drawRect ( int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color) {
	drawHLine(x,y,w,color);
	drawHLine(x,y+h,w,color);
	drawVLine(x,y,h,color);
	drawVLine(x+w,y,h,color);
}

// Draw a triangle
void TouchMega::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2,uint16_t color) {
	drawLine(x0, y0, x1, y1,color);
	drawLine(x1, y1, x2, y2,color);
	drawLine(x2, y2, x0, y0,color);
}

void TouchMega::drawCircle(int16_t x0, int16_t y0, int16_t r,uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	drawPixel(x0  , y0+r,color);
	drawPixel(x0  , y0-r,color);
	drawPixel(x0+r, y0,color);
	drawPixel(x0-r, y0,color);

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		drawPixel(x0 + x, y0 + y,color);
		drawPixel(x0 - x, y0 + y,color);
		drawPixel(x0 + x, y0 - y,color);
		drawPixel(x0 - x, y0 - y,color);
		drawPixel(x0 + y, y0 + x,color);
		drawPixel(x0 - y, y0 + x,color);
		drawPixel(x0 + y, y0 - x,color);
		drawPixel(x0 - y, y0 - x,color);
	}
}

void TouchMega::drawCircleHelper( int16_t x0, int16_t y0, int16_t r, int8_t cornername,uint16_t color) {
	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;
		if (cornername & 0x4) {
			drawPixel(x0 + x, y0 + y,color);
			drawPixel(x0 + y, y0 + x,color);
		}
		if (cornername & 0x2) {
			drawPixel(x0 + x, y0 - y,color);
			drawPixel(x0 + y, y0 - x,color);
		}
		if (cornername & 0x8) {
			drawPixel(x0 - y, y0 + x,color);
			drawPixel(x0 - x, y0 + y,color);
		}
		if (cornername & 0x1) {
			drawPixel(x0 - y, y0 - x,color);
			drawPixel(x0 - x, y0 - y,color);
		}
	}
}

// Draw a rounded rectangle
void TouchMega::drawRoundRect(int16_t x, int16_t y, int16_t w,int16_t h, int16_t r,uint16_t color) {
	// smarter version
	drawHLine(x+r  , y    , w-2*r,color); // Top
	drawHLine(x+r  , y+h-1, w-2*r,color); // Bottom
	drawVLine(x    , y+r  , h-2*r,color); // Left
	drawVLine(x+w-1, y+r  , h-2*r,color); // Right
	// draw four corners
	drawCircleHelper(x+r    , y+r    , r, 1,color);
	drawCircleHelper(x+w-r-1, y+r    , r, 2,color);
	drawCircleHelper(x+w-r-1, y+h-r-1, r, 4,color);
	drawCircleHelper(x+r    , y+h-r-1, r, 8,color);
}

void TouchMega::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color)
{
	int8_t DH,DL;
	sector(x,y,x+w-1,y+h-1);
	digitalWrite(_dc, HIGH);
	DH=color>>8;
	DL=color;

	if(_type_lcd==DB15_8) //0
		for(y=h; y>0; y--)
			for(x=w; x>0; x--){


				PORTA =DH;
				PORTG&= 0xfb;//cbi(PG, 2);
				PORTG |=0x04; //sbi(PG, 2);


				PORTA =DL;
				PORTG&= 0xfb;//cbi(PG, 2);
				PORTG |=0x04; //sbi(PG, 2);
			} 


	else if(_type_lcd==DB7_0) //1
		for(y=h; y>0; y--)
			for(x=w; x>0; x--){

				PORTC =DH;
				PORTG&= 0xfb;//cbi(PG, 2);
				PORTG |=0x04; //sbi(PG, 2);


				PORTC =DL;
				PORTG&= 0xfb;//cbi(PG, 2);
				PORTG |=0x04; //sbi(PG, 2);
			}   
}


// Fill a triangle
void TouchMega::fillTriangle ( int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2,uint16_t color){
	int16_t a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1) {
		SWAP(y0, y1); SWAP(x0, x1);
	}
	if (y1 > y2) {
		SWAP(y2, y1); SWAP(x2, x1);
	}
	if (y0 > y1) {
		SWAP(y0, y1); SWAP(x0, x1);
	}

	if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
		a = b = x0;
		if(x1 < a)      a = x1;
		else if(x1 > b) b = x1;
		if(x2 < a)      a = x2;
		else if(x2 > b) b = x2;
		drawHLine(a, y0, b-a+1,color);
		return;
	}

	int16_t
		dx01 = x1 - x0,
		dy01 = y1 - y0,
		dx02 = x2 - x0,
		dy02 = y2 - y0,
		dx12 = x2 - x1,
		dy12 = y2 - y1,
		sa   = 0,
		sb   = 0;


	if(y1 == y2) last = y1;   // Include y1 scanline
	else         last = y1-1; // Skip it

	for(y=y0; y<=last; y++) {
		a   = x0 + sa / dy01;
		b   = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;

		if(a > b) SWAP(a,b);
		drawHLine(a, y, b-a+1,color);
	}

	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) {
		a   = x1 + sa / dy12;
		b   = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;

		if(a > b) SWAP(a,b);
		drawHLine(a, y, b-a+1,color);
	}
}

void TouchMega::fillCircle(int16_t x0, int16_t y0, int16_t r,uint16_t color){
	drawVLine(x0, y0-r, 2*r+1,color);
	fillCircleHelper(x0, y0, r, 3, 0,color);
}


void TouchMega::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,uint8_t cornername, int16_t delta,uint16_t color){
	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;

		if (cornername & 0x1) {
			drawVLine(x0+x, y0-y, 2*y+1+delta,color);
			drawVLine(x0+y, y0-x, 2*x+1+delta,color);
		}
		if (cornername & 0x2) {
			drawVLine(x0-x, y0-y, 2*y+1+delta,color);
			drawVLine(x0-y, y0-x, 2*x+1+delta,color);
		}
	}
}

// Fill a rounded rectangle
void TouchMega::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r,uint16_t color) {
	// smarter version
	fillRect(x+r, y, w-2*r, h,color);

	// draw four corners
	fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1,color);
	fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1,color);
}

void TouchMega::invertDisplay(uint8_t i) {
	writeCommand(i ? INVON : INVOFF);
}


void TouchMega::setRotation(uint8_t m) {

	_rotation = m % 4; // can't be higher than 3
	switch (_rotation) {
		case 0:
			writeCommand(0x36);
			writeData(MADCTL_MX | MADCTL_BGR);
			_width  = 320;
			_height = 480;
			break;
		case 1:
			writeCommand(0x36);
			writeData(MADCTL_MV | MADCTL_BGR);
			_width  = 480;
			_height = 320;
			break;
		case 2:
			writeCommand(0x36);
			writeData(MADCTL_MY | MADCTL_BGR);
			_width  = 320;
			_height = 480;
			break;
		case 3:
			writeCommand(0x36);
			writeData(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
			_width  = 480;
			_height = 320;
			break;
	}
}



// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t TouchMega::color565(uint8_t r, uint8_t g, uint8_t b) {
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}


void TouchMega::putRgbColorPixel(uint8_t r, uint8_t g, uint8_t b){
	uint16_t colorPixel;
	colorPixel = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
	writeData(colorPixel>>8);
	writeData(colorPixel);
}

void TouchMega::startScrollingRolling(){
	writeCommand(0x33);
	writeData(0x00);
	writeData(0x00);
	writeData(0x01);
	writeData(0x2c);
	writeData(0x00);
	writeData(0x14);
}

void TouchMega::endScrollingRolling (uint8_t Direction){
	if( Direction == 2)
	{
		writeCommand(0x37);
		writeData(numbera);
		writeData(numberb);

		if (numbera == 0)
		{
			if (numberb < 255 )
				numberb++;
			else
			{
				numbera = 1;
				numberb = 0;
			}
		}
		else
		{
			if	(numberb < 64)
				numberb++;
			else
			{
				numberb=0;
				numbera=0;
			}
		}
	}

	if( Direction == 1)
	{
		writeCommand(0x37);
		writeData(number1);
		writeData(number2);

		if(number1 == 1)
		{
			if(number2 > 0)
				number2--;
			else
			{
				number2 = 255;
				number1 = 0;
			}
		}

		else if (number2> 0)
			number2--;
		else
		{
			number2 = 64 ;
			number1 = 1;
		}
	}
}


/*  touch   */

bool TouchMega::touched(){
	if(!(PINL&0x20))
	{
		return true;
	}
	else return false;
}


uint16_t TouchMega::ADS_Read_AD(uint16_t CMD){
	uint8_t data1, data2;
	uint16_t dx;
	SPI.setClockDivider(SPI_CLOCK_DIV16);
	digitalWrite(_Tcs,LOW);//T_SS = 0;
	SPI.transfer(CMD);////
	data1 = SPI.transfer(0x00);
	data2 = SPI.transfer(0x00);
	dx = (((data1<<8)+data2)>>3) &0xFFF;
	digitalWrite(_Tcs,HIGH);//T_SS = 1;
	return (dx);
}


uint16_t TouchMega::getToucX(){

	return(ADS_Read_AD(0xD0));
}


uint16_t TouchMega::getToucY(){

	return (ADS_Read_AD(0x90));
}


void TouchMega::readTouch(){
	int temp = _x;

	if (_rotation == 1){
		_x= map(getToucY(), y1, y2, 10, 470);
		_y=320-map(getToucX(), x1, x2, 10, 310);
	}
	else if (_rotation == 2){
		_x=320-map(getToucX(), x1, x2, 10, 310);
		_y=480-map(getToucY(), y1, y2, 10, 470);
	}
	else if (_rotation == 0){

		_x= map(getToucX(), x1, x2, 10, 310);
		_y=map(getToucY(), y1, y2, 10, 470);
	}

	else if (_rotation == 3){
		_x=480-map(getToucY(), y1, y2, 10, 470);
		_y= map(getToucX(), x1, x2, 10, 310);
	}

	if ((_x < _width) &&   (_x > 0))
		xTouch=_x;
	else
		xTouch=-1;

	if ((_y < _height) && (_y > 0))
		yTouch=_y;
	else
		yTouch=-1;	
}


void TouchMega::set(uint16_t xs1, uint16_t xs2, uint16_t ys1, uint16_t ys2){
	x1 = xs1; x2 = xs2; y1 = ys1; y2 = ys2;
}



