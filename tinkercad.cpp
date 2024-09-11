// C++ code
//
#include <Adafruit_LiquidCrystal.h>
int analogPin = 0;
int val = 0;
bool confirm = false; 



Adafruit_LiquidCrystal lcd_1(0);

void setup()
{
  Serial.begin(9600);
  lcd_1.begin(16, 2);  // Configurar LCD de 16x2
  lcd_1.print("Esperando...");

}

void loop()
{
  
  //lcd_1.clear();
  lcd_1.setCursor(0, 0);
  
  
  if (digitalRead(3)== HIGH) {
    lcd_1.clear();
    lcd_1.print("Inicio ");
    confirm = true;
    }
  while (confirm == true) {
  val = analogRead(analogPin);
      Serial.println(val);
    
    if (digitalRead(5) == HIGH) {
    lcd_1.clear();
    lcd_1.print("Pausa ");
    confirm = false;
    }
  }
  
  
  
  
}