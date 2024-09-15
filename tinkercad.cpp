#include <Adafruit_LiquidCrystal.h>

// Definición de pines
int analogPin = 0;  // Pin de entrada analógica
int val = 0;        // Valor de la señal leída
bool confirm = false;  // Estado del proceso de adquisición
int indice = 0;
int capacidad = 10;
int* arreglo = new int[capacidad];

// Configuración de la pantalla LCD
Adafruit_LiquidCrystal lcd_1(0);

// Función para redimensionar el arreglo dinámico
void redimensionarArreglo(int*& arreglo, int& capacidad) {
    int nueva_capacidad = capacidad * 2;
    int* nuevo_arreglo = new int[nueva_capacidad];

    // Copiar los elementos del arreglo antiguo al nuevo
    for (int i = 0; i < capacidad; i++) {
        nuevo_arreglo[i] = arreglo[i];
    }

    // Liberar la memoria del arreglo antiguo
    delete[] arreglo;

    // Actualizar el puntero y la capacidad
    arreglo = nuevo_arreglo;
    capacidad = nueva_capacidad;
}

void setup() {
    Serial.begin(9600);  // Inicialización del monitor serial
    lcd_1.begin(16, 2);  // Configurar LCD de 16x2
    lcd_1.print("Esperando...");  // Mensaje inicial en la pantalla

    pinMode(3, INPUT);  // Configurar pulsador de inicio como entrada
    pinMode(5, INPUT);  // Configurar pulsador de pausa como entrada
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
    while (confirm) {
        val = analogRead(analogPin);  // Leer la señal analógica
        Serial.println(val);  // Mostrar el valor en el monitor serial

        arreglo[indice] = val;  // Guardar el valor en el arreglo
        indice++;

        // Redimensionar el arreglo si es necesario
        if (indice == capacidad) {
            redimensionarArreglo(arreglo, capacidad);
            Serial.println("Arreglo redimensionado");
        }

        // Verificar si el pulsador de pausa (pin 5) está presionado
        if (digitalRead(5) == HIGH) {
            lcd_1.clear();
            lcd_1.print("Pausa ");  // Mostrar mensaje de pausa en la pantalla

            // Liberar el arreglo dinámico y detener la adquisición
            delete[] arreglo;
            confirm = false;  // Pausar la adquisición

            // Detener el ciclo de adquisición
            break;
        }
    }
}
