#include <Adafruit_LiquidCrystal.h>

// Definición de pines
int analogPin = 0;  // Pin de entrada analógica
int val = 0;        // Valor de la señal leída
bool confirm = false;  // Estado del proceso de adquisición

// Configuración de la pantalla LCD
Adafruit_LiquidCrystal lcd_1(0);

void setup() {
  Serial.begin(9600);  // Inicialización del monitor serial
  lcd_1.begin(16, 2);  // Configurar LCD de 16x2
  lcd_1.print("Esperando...");  // Mensaje inicial en la pantalla
}

void loop() {
  lcd_1.setCursor(0, 0);  // Posicionar el cursor en la pantalla LCD
  
  // Verificar si el pulsador de inicio (pin 3) está presionado
  if (digitalRead(3) == HIGH) {
    lcd_1.clear();
    lcd_1.print("Inicio ");  // Mostrar mensaje de inicio en la pantalla
    confirm = true;  // Activar el estado de adquisición
  }
  
  // Ciclo de adquisición mientras el estado es true
  while (confirm == true) {
    val = analogRead(analogPin);  // Leer la señal analógica
    Serial.println(val);  // Mostrar el valor en el monitor serial
    
    // Verificar si el pulsador de pausa (pin 5) está presionado
    if (digitalRead(5) == HIGH) {
      lcd_1.clear();
      lcd_1.print("Pausa ");  // Mostrar mensaje de pausa en la pantalla
      confirm = false;  // Pausar la adquisición
    }
  } 
}
