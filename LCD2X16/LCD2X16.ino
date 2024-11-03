
// include the library code:
#include <LiquidCrystal.h>

// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12; // قرار دادن پین ها در متغیر
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // پیکربندی ال سی دی

void setup() 
{
lcd.begin(16,2);

lcd.clear();
}

void loop() 
{
    // Print a message to the LCD.
    lcd.setCursor(0, 0);
    lcd.print(" Hello world!");
 
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 1);
    // Print a message to the LCD.
    lcd.print(" LCD Tutorial");
}