#include <Adafruit_LiquidCrystal.h>

// Definición de constantes para los pines
const int PinAnalogica = A0;
const int PIN_INICIO = 3;
const int PIN_PAUSA = 5;

// Configuración de la pantalla LCD
Adafruit_LiquidCrystal lcd_1(0);

// Declarar un puntero a un arreglo de enteros
int* datos;

// Tamaño máximo del arreglo
const int tamanoMaximo = 200;  // Límite de tamaño del arreglo
int tamanoArreglo = 10;  // Tamaño inicial del arreglo
int indice = 0;  // Índice para almacenar los valores

void setup() {
  Serial.begin(9600);  // Inicializar monitor serial
  lcd_1.begin(16, 2);  // Configurar LCD de 16x2
  lcd_1.print("Esperando...");  // Mensaje inicial en la pantalla
  
  // Reservar memoria dinámica para el arreglo
  datos = new int[tamanoArreglo];

  // Configurar los pines de los pulsadores
  pinMode(PIN_INICIO, INPUT_PULLUP);
  pinMode(PIN_PAUSA, INPUT_PULLUP);
}

void loop() 
{
  // Verificar si el pulsador de inicio está presionado
  if (digitalRead(PIN_INICIO) == HIGH)  // Botón de inicio presionado
  {
    lcd_1.clear();
    lcd_1.print("Inicio ");
    bool confirm = true;
    indice = 0;
  
    while (confirm == true) 
    {
      int val = analogRead(PinAnalogica);  // Leer la señal analógica
      Serial.println(val);  // Mostrar el valor en el monitor serial
    
      // Almacenar el valor en el arreglo
      if (indice < tamanoArreglo) {
        datos[indice] = val;
        indice++;
      }

      // Verificar si se ha alcanzado el tamaño máximo del arreglo
      if (indice >= tamanoArreglo && tamanoArreglo < tamanoMaximo) {
        // Aumentar el tamaño del arreglo
        tamanoArreglo *= 2;
        if (tamanoArreglo > tamanoMaximo) {
          tamanoArreglo = tamanoMaximo;  // Limitar el tamaño del arreglo
        }

        // Redimensionar el arreglo
        int* nuevoArreglo = new int[tamanoArreglo];
        for (int i = 0; i < indice; i++) {
          nuevoArreglo[i] = datos[i];
        }

        delete[] datos;  // Liberar la memoria antigua
        datos = nuevoArreglo;  // Apuntar al nuevo arreglo
      }
    
      // Verificar si el pulsador de pausa está presionado
      if (digitalRead(PIN_PAUSA) == HIGH)  // Botón de pausa presionado
      {
        lcd_1.clear();
        lcd_1.print("Pausa ");
        
        // Mostrar resultados en la pantalla LCD
        mostrarResultadosEnLCD();

        delete[] datos;  // Liberar la memoria del arreglo
        datos = NULL;  // Evitar acceder a memoria liberada
        
        lcd_1.clear();
        lcd_1.print("Esperando...");
        const int tamanoMaximo = 200;  // Límite de tamaño del arreglo
		int tamanoArreglo = 10;  // Tamaño inicial del arreglo
		int indice = 0;  // Índice para almacenar los valores
        datos = new int[tamanoArreglo];
        
     

        confirm = false;  // Detener la adquisición de datos
      }
    
      delay(10);  // Dar un poco de tiempo para que el programa pueda procesar otros eventos
    }
  }
}




// Función para mostrar los resultados en la pantalla LCD
void mostrarResultadosEnLCD() {
  // Suponemos una tasa de muestreo de 100 muestras por segundo (100 Hz)
  float tasaMuestreo = 100.0;  

  // Identificamos la forma de la onda primero
  String formaOnda = identificarFormaDeOnda(datos, indice);
  
  float frecuencia;

  // Si es cuadrada, usamos el método basado en transiciones
  if (formaOnda == "Cuadrada") {
    frecuencia = calcularFrecuenciaPorTransiciones(datos, indice, tasaMuestreo);
  } else {
    frecuencia = calcularFrecuencia(datos, indice, tasaMuestreo);  // Método estándar para otras formas
  }
  
  float amplitud = calcularAmplitud(datos, indice);

  // Mostrar la frecuencia
  lcd_1.clear();
  lcd_1.print("Frecuencia:");
  lcd_1.setCursor(0, 1);
  lcd_1.print(frecuencia, 2);  // Mostrar con 2 decimales
  lcd_1.print(" Hz");
  delay(3000);  // Esperar 3 segundos

  // Mostrar la amplitud
  lcd_1.clear();
  lcd_1.print("Amplitud:");
  lcd_1.setCursor(0, 1);
  lcd_1.print(amplitud, 2);  // Mostrar con 2 decimales
  lcd_1.print(" V");
  delay(3000);  // Esperar 3 segundos

  // Mostrar la forma de la señal
  lcd_1.clear();
  lcd_1.print("Forma de onda:");
  lcd_1.setCursor(0, 1);
  lcd_1.print(formaOnda);
  delay(3000);  // Esperar 3 segundos
}


// Función para calcular la frecuencia
float calcularFrecuencia(int* datos, int tamano, float tasaMuestreo) {
  int contadorPicos = 0;

  // Contar los picos
  for (int i = 1; i < tamano - 1; i++) {
    if (datos[i - 1] < datos[i] && datos[i] > datos[i + 1]) {
      contadorPicos++;
    }
  }

  // Calcular frecuencia: número de picos por segundo
  // Frecuencia = picos / (tiempo_total)
  float tiempoTotalSegundos = tamano / tasaMuestreo;  // tiempo en segundos
  if (tiempoTotalSegundos > 0) {
    return contadorPicos / tiempoTotalSegundos;  // Frecuencia en Hz
  }

  return 0.0;  // Retornar 0 si no se puede calcular
}

float calcularFrecuenciaPorTransiciones(int* datos, int tamano, float tasaMuestreo) {
  int transiciones = 0;

  // Recorrer los datos para detectar transiciones
  for (int i = 1; i < tamano; i++) {
    // Detectar una transición (cambio de nivel alto a bajo o de bajo a alto)
    if ((datos[i - 1] < 512 && datos[i] >= 512) || (datos[i - 1] >= 512 && datos[i] < 512)) {
      transiciones++;
    }
  }

  // Cada ciclo completo tiene 2 transiciones, por lo que la frecuencia es:
  float ciclosCompletos = transiciones / 2.0;

  // Calcular el tiempo total en segundos usando la tasa de muestreo
  float tiempoTotalSegundos = tamano / tasaMuestreo;

  // Si tenemos un tiempo válido, calcular la frecuencia
  if (tiempoTotalSegundos > 0) {
    return ciclosCompletos / tiempoTotalSegundos;  // Frecuencia en Hz
  } else {
    return 0.0;  // Retornar 0 si el tiempo es inválido
  }
}


// Función para calcular la amplitud
float calcularAmplitud(int* datos, int tamano) 
{
  int valorMaximo = datos[0];
  int valorMinimo = datos[0];

  for (int i = 1; i < tamano; i++) 
  {
    if (datos[i] > valorMaximo) {
      valorMaximo = datos[i];
    }
    if (datos[i] < valorMinimo) {
      valorMinimo = datos[i];
    }
  }

  // La amplitud es la diferencia entre el máximo y el mínimo
  return (valorMaximo - valorMinimo)*(5.0 / 1023.0);  // Convertir a voltios
}

//Funcion para facilitar la deduccion de la forma de onda
int contarValoresDistintos(int* datos, int tamano) {
  int maxValoresUnicos = 10;  // Máximo número de valores únicos que esperamos
  int valoresUnicos[maxValoresUnicos];  // Arreglo para almacenar los valores únicos
  int contadorValoresUnicos = 0;  // Número de valores únicos encontrados

  // Recorrer los datos
  for (int i = 0; i < tamano; i++) {
    bool encontrado = false;

    // Comprobar si el valor ya está en el arreglo de valores únicos
    for (int j = 0; j < contadorValoresUnicos; j++) {
      if (datos[i] == valoresUnicos[j]) {
        encontrado = true;  // El valor ya está en el arreglo
        break;
      }
    }

    // Si no se ha encontrado el valor, lo añadimos al arreglo
    if (!encontrado) {
      if (contadorValoresUnicos < maxValoresUnicos) {
        valoresUnicos[contadorValoresUnicos] = datos[i];  // Agregar nuevo valor único
        contadorValoresUnicos++;
      } else {
        // Si ya alcanzamos el máximo número de valores únicos
        break;
      }
    }
  }

  return contadorValoresUnicos;  // Retornar la cantidad de valores únicos
}



// Función para identificar la forma de la señal
String identificarFormaDeOnda(int* datos, int tamano) {
  int valoresDiferentes = contarValoresDistintos(datos, tamano);

  // Basar la identificación en el número de valores únicos encontrados
  if (valoresDiferentes == 2) 
  	{
    return "Cuadrada";  // Dos valores únicos indica una onda cuadrada
    } 
  else if (valoresDiferentes > 2 && valoresDiferentes <= 5) 
  {
    return "Triangular";  // Más de dos valores pero menos de cinco podría ser triangular
  } 
  else if (valoresDiferentes > 5) 
  {
    return "Senoidal";  // Muchos valores distintos indican una señal senoidal
  } else 
  {
    return "Desconocida";  // Si no encaja en los patrones, es desconocida
  }
}
