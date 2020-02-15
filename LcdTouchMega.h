/*  last update 7/2/20 */
/*  Avi Hayun */

#ifndef _TouchMega_H_
#define _TouchMega_H_

#include <avr/pgmspace.h>
#include "registers.h"
#include <SPI.h>
#include "Arduino.h"

#define DB15_8  0
#define DB7_0   1

#define SWAP(a, b) { int16_t t = a; a = b; b = t; }
#define ABS(x) ((x)>0?(x):-(x))
#define SD_CS 4

#ifndef DEC
#define DEC	10
#define HEX	16
#define OCT	8
#define BIN	2
#endif

#define WIDTH			480
#define HEIGHT			320

#define	BLACK			 0x0000
#define	BLUE			 0x001F
#define BRED             0XF81F
#define BROWN            0XBC40
#define BLUE2            0x051F
#define	CYAN			 0x07FF
#define DARKGREEN        0x03E0      
#define DARKCYAN         0x03EF  
#define DARKGREY         0x7BEF    
#define	GRAY			 0x8410
#define	GREEN			 0x07E0
#define GREENYELLOW      0xAFE5
#define LIGHTGREY        0xC618    
#define MAROON           0x7800   
#define MAGENTA          0xF81F   
#define NAVY             0x000F  
#define OLIVE            0x7BE0
#define ORANGE           0xFD20  
#define PURPLE           0x780F
#define PINK             0xF81F
#define	RED				 0xF800
#define	WHITE			 0xFFFF
#define	YELLOW			 0xFFE0

#define up2down		0x08
#define down2up		0xc8
#define right2left		0x68
#define left2right		0xa8

#define MADCTL  0x36

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

#define INVOFF  0x20
#define INVON   0x21

#define scrollup		2
#define scrolldown	1

#define CASET   0x2A
#define PASET   0x2B
#define RAMWR   0x2C

class TouchMega {
private:

	int8_t  _Tcs, _Tirq;// touch
	uint16_t x1, x2, y1, y2;
	uint16_t getToucX();
	uint16_t getToucY();


	int8_t  _cs, _dc , _wr  ,_type_lcd=0; // lcd
	uint16_t  _color, _background;
	int16_t _x, _y,_width, _height;
	uint8_t _rotation, _fontSize;
	int16_t numbera ,numberb ,number1, number2;

	void writeData(uint8_t data8);
	void writeCommand(uint8_t cmd);
	void putChar(uint8_t c);
	uint16_t ADS_Read_AD(uint16_t CMD);

public:
	void pushColor(uint16_t color);
	TouchMega(int8_t TypeLCD);
	// touch
	int16_t xTouch, yTouch;

	bool touched();
	void readTouch();

	void cal();
	void set(uint16_t xs1, uint16_t xs2, uint16_t ys1, uint16_t ys2);

	inline void setCursor (uint16_t x,uint16_t y) { _x = x; _y = y;}
	inline int16_t getX() {return (_x);}
	inline int16_t getY() {return (_y);}
	inline uint8_t getFont () { return (_fontSize); }
	inline void setTextSize(uint8_t fontSize) { _fontSize = fontSize; }
	inline uint16_t getColor () { return (_color); }
	inline void setTextColor(uint16_t color) { _color = color; _background = color; }
	inline void setTextColor(uint16_t color, uint16_t background) {_color=color; _background=background;};
	inline uint16_t getBackground () {	return (_background); }
	inline void setBackground (uint16_t background) { _background = background; }
	inline uint16_t height() { return (_height); }
	inline uint16_t width() { return (_width); }
	inline uint8_t	 getRotation() { return (_rotation); }
	void setRotation(uint8_t m);
	inline void switchColors () { SWAP (_color, _background); }
	void sector(int16_t x0,int16_t y0,int16_t x1,int16_t y1);

	void begin();
	void fillScreen(uint16_t color); 
	void write (char c);
	void print (int16_t num) {print(num,DEC);};
	void print (uint16_t num);
	void print (int8_t num) {print(num,DEC);};
	void print (uint8_t num) {print(num,DEC);};
	void print (int16_t num, uint8_t radix);
	void print (uint16_t num, uint8_t radix);
	void print (uint8_t num, uint8_t radix);
	void print (int8_t num, uint8_t radix);
	void print (char *str);
	void print (double fnum, uint8_t s);
	void print (double fnum) {print (fnum,2);};

	void print(String txtMsg);
	void println(String txtMsg);
	void writeln (char c);
	void println ();
	void println (int16_t num) {println(num,DEC);};
	void println (uint16_t num);
	void println (int8_t num) {println(num,DEC);};
	void println (uint8_t num) {println(num,DEC);};
	void println (int16_t num, uint8_t radix);
	void println (uint16_t num, uint8_t radix);
	void println (uint8_t num, uint8_t radix);
	void println (int8_t num, uint8_t radix);
	void println (char *str);
	void println (double fnum, uint8_t s);
	void println (double fnum) {print (fnum,2);};

	void print(uint16_t x, uint16_t y, char *str, uint16_t fontSize, uint16_t fColor);
	void print(uint16_t x, uint16_t y, char *str, uint16_t fontSize, uint16_t fColor, uint16_t bColor);
	void print (uint16_t x,uint16_t y,String txtMsg,uint16_t fontSize, uint16_t fColor);
	void print (uint16_t x,uint16_t y,String txtMsg,uint16_t fontSize, uint16_t fColor,uint16_t bColor);

	void drawPixel (int16_t x,int16_t y, uint16_t color);
	void drawHLine (int16_t x,int16_t y, int16_t w, uint16_t color);
	void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
	void drawVLine (int16_t x,int16_t y, int16_t h, uint16_t color);
	void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
	void drawLinePolar (int16_t x0, int16_t y0, int16_t r, int16_t angle, uint16_t color);

	void drawRect (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
	void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, int8_t cornername, uint16_t color);
	void drawRoundRect(int16_t x, int16_t y, int16_t w,int16_t h, int16_t r, uint16_t color);
	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	void fillTriangle ( int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
	void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r,uint8_t cornername, int16_t delta, uint16_t color);
	void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);

	void invertDisplay(uint8_t i);
	uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
	uint16_t rgbColor(uint8_t r, uint8_t g, uint8_t b);
	void putRgbColorPixel(uint8_t r, uint8_t g, uint8_t b);

	void startScrollingRolling();
	void endScrollingRolling (uint8_t Direction);
};

#endif
