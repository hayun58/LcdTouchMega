/*************** Avi Hayun 7/2/20 ***************/

#include "TouchMega.h"


TouchMega LcdTouch(DB7_0); //DB15_8 or DB7_0

uint16_t ColorTab[5] = {BRED, YELLOW, RED, GREEN, BLUE};
String str;
int x, y;

void setup() {
  LcdTouch.begin();
  LcdTouch.print (5, 100, " Avi Hayun  ", 4, YELLOW, BLACK);
  delay(2000);
  Test_Color();
  Test_shape();
  English_Font_test();
  delay(2000);
  LcdTouch.fillScreen(BLACK);
  LcdTouch.print (5, 100, " Press Touch Screen ", 2, YELLOW, BLACK);
}

void loop() {
  if (LcdTouch.touched())
  {
    LcdTouch.readTouch();
    x = LcdTouch.xTouch;
    y = LcdTouch.yTouch;
    {
      str = "x=" + String(x) + " y=" + String(y) + "   ";
      LcdTouch.print (25, 220, str, 2, WHITE, BLACK);
      LcdTouch.fillCircle(x, y, 3, WHITE);

      while (!LcdTouch.touched());
    }
  }
}


void DrawTestPage(byte *str) {
  LcdTouch.fillRect(0, 0, LcdTouch.width(), 20, BLUE);
  LcdTouch.print(0, 2, str, 2, WHITE, BLUE);
  LcdTouch.fillRect(0, 20, LcdTouch.width(), LcdTouch.height() - 20, BLACK);
}

void Test_Color() {
  DrawTestPage("Test Color ");
  LcdTouch.fillRect(0, 20, LcdTouch.width(), LcdTouch.height() - 20, WHITE);
  LcdTouch.print(30, 30, "White", 1, BLUE); delay(500);
  LcdTouch.fillRect(0, 20, LcdTouch.width(), LcdTouch.height() - 20, BLACK);
  LcdTouch.print(30, 30, "Black", 1, YELLOW); delay(500);
  LcdTouch.fillRect(0, 20, LcdTouch.width(), LcdTouch.height() - 20, RED);
  LcdTouch.print(30, 30, "Red", 2, YELLOW); delay(500);
  LcdTouch.fillRect(0, 20, LcdTouch.width(), LcdTouch.height() - 20, GREEN);
  LcdTouch.print(30, 30, "Green", 2, WHITE); delay(500);
  LcdTouch.fillRect(0, 20, LcdTouch.width(), LcdTouch.height() - 20, BLUE);
  LcdTouch.print(30, 30, "Blue", 2, WHITE); delay(500);
}

void Test_shape() {
  int i = 0;
  DrawTestPage("Test_FillRec  ");
  LcdTouch.fillRect(0, 20, LcdTouch.width(), LcdTouch.height() - 20, BLACK);
  for (i = 0; i < 5; i++) {
    LcdTouch.drawRect(LcdTouch.width() / 2 - 80 + (i * 15), LcdTouch.height() / 2 - 80 + (i * 15), 100, 100, ColorTab[i]);
  }
  delay(1000);

  LcdTouch.fillRect(0, 20, LcdTouch.width(), LcdTouch.height() - 20, BLACK);
  for (i = 0; i < 5; i++) {
    LcdTouch.fillRect(LcdTouch.width() / 2 - 80 + (i * 15), LcdTouch.height() / 2 - 80 + (i * 15), 80, 80, ColorTab[i]);
  }
  delay(1000);

  LcdTouch.fillRect(0, 20, LcdTouch.width(), LcdTouch.height() - 20, BLACK);
  for (i = 0; i < 5; i++) {
    LcdTouch.fillRoundRect(LcdTouch.width() / 2 - 80 + (i * 15), LcdTouch.height() / 2 - 80 + (i * 15), 60, 60, 10, ColorTab[i]);
  }
  delay(1000);

  DrawTestPage("Test_Circle");
  LcdTouch.fillRect(0, 20, LcdTouch.width(), LcdTouch.height() - 20, BLACK);

  for (i = 0; i < 5; i++)
    LcdTouch.drawCircle(LcdTouch.width() / 2 - 80 + (i * 25), LcdTouch.height() / 2 - 50 + (i * 25), 30, ColorTab[i]);
  delay(1000);

  LcdTouch.fillRect(0, 20, LcdTouch.width(), LcdTouch.height() - 20, BLACK);
  for (i = 0; i < 5; i++)
    LcdTouch.fillCircle(LcdTouch.width() / 2 - 80 + (i * 25), LcdTouch.height() / 2 - 50 + (i * 25), 30, ColorTab[i]);
  delay(1000);

  DrawTestPage("Test_Triangle");
  LcdTouch.fillRect(0, 20, LcdTouch.width(), LcdTouch.height() - 20, BLACK);
  for (i = 0; i < 5; i++)
    LcdTouch.drawTriangle(50 + i * 10, 50 + i * 10, 150 + i * 10, 80 + i * 10, 100 + i * 10, 180 + i * 10, ColorTab[i]);
  delay(1000);

  LcdTouch.fillRect(0, 20, LcdTouch.width(), LcdTouch.height() - 20, BLACK);
  for (i = 0; i < 5; i++)
    LcdTouch.fillTriangle(50 + i * 10, 50 + i * 10, 150 + i * 10, 80 + i * 10, 100 + i * 10, 180 + i * 10, ColorTab[i]);
}


void English_Font_test()
{
  DrawTestPage("Font_test");

  LcdTouch.print(20, 30, "abcd", 1, WHITE, BLACK);
  LcdTouch.print(20, 60, "Avi Hayun ", 2, WHITE, BLACK);
  LcdTouch.print(20, 90, "3.95' ILI9488 320X480", 3, YELLOW);
  LcdTouch.print(20, 120, "Test 2020-02-07", 3, GREEN);
  LcdTouch.println("");
  LcdTouch.setTextSize(2);
  LcdTouch.setTextColor(WHITE);

  LcdTouch.println("abcdefghijklmnopqrstuvwxyz");
  LcdTouch.println("ABCDEFGHIJKLMNOP0123456789");
  LcdTouch.println("~!@#$%^&*()_+{}:<>?/|-+.");
  LcdTouch.setTextSize(3);
  LcdTouch.setTextColor(CYAN);

  LcdTouch.println("ABCDEFGHIJKLMN01234567");
  LcdTouch.println("~!@#$%^&*()_+{}:<>?/|");
  LcdTouch.println("");
}
