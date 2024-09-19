#include <LiquidCrystal.h>

// Pines para el LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Pin de entrada para la señal
const int signalPin = A0;

// Pines para los botones
const int button1Pin = 7;  // Botón 1
const int button2Pin = 8;  // Botón 2

// Variables para controlar los botones
bool acquisitionStarted = false;
bool displayWaveform = false;

void setup() {
    // Inicialización del LCD
    lcd.begin(16, 2); // Pantalla de 16x2
    lcd.print("Frecuencia:"); // Título de frecuencia

    // Configuración del puerto serie (opcional para depuración)
    Serial.begin(9600);

    // Configuración de los botones como entradas
    pinMode(button1Pin, INPUT);
    pinMode(button2Pin, INPUT);
}

void loop() {
    // Comprobar el estado de los botones
    if (digitalRead(button1Pin) == HIGH) {
        acquisitionStarted = !acquisitionStarted;  // Alternar adquisición
        delay(300);  // Para evitar rebotes en el botón
    }

    if (digitalRead(button2Pin) == HIGH) {
        displayWaveform = !displayWaveform;  // Alternar visualización de forma de onda
        delay(300);  // Para evitar rebotes en el botón
    }

    // Si la adquisición está activa
    if (acquisitionStarted) {
        // Leer la señal analógica
        int valorAnalogico = analogRead(signalPin);

        // Convertir el valor analógico a voltaje (asumiendo 5V y 10 bits de resolución)
        float voltaje = (valorAnalogico * 5.0) / 1023.0;

        // Calcular la amplitud (máxima) de la señal
        float amplitud = voltaje;

        // Calcular la frecuencia
        int frecuencia = medirFrecuencia();

        // Mostrar resultados en el LCD
        lcd.setCursor(0, 1); // Mover a la segunda línea
        lcd.print("Amp:");
        lcd.print(amplitud, 2); // Mostrar la amplitud con dos decimales
        lcd.print("V Freq:");
        lcd.print(frecuencia); // Mostrar la frecuencia

        // Imprimir resultados en el monitor serie para depuración
        Serial.print("Amplitud: ");
        Serial.print(amplitud);
        Serial.println(" V");

        Serial.print("Frecuencia: ");
        Serial.print(frecuencia);
        Serial.println(" Hz");

        // Si el botón 2 está activado, mostrar la forma de onda
        if (displayWaveform) {
            mostrarFormaOnda();
        }

        delay(1000); // Esperar un segundo antes de actualizar
    }
}

// Función para medir la frecuencia de la señal
int medirFrecuencia() {
    long duracion = pulseIn(signalPin, HIGH);

    if (duracion == 0) return 0;  // Evitar división por cero

    int frecuencia = 1000000 / (2 * duracion);  // Convertir duración a frecuencia (Hz)
    return frecuencia;
}

// Función para mostrar la forma de onda
void mostrarFormaOnda() {
    for (int i = 0; i < 100; i++) {
        int muestra = analogRead(signalPin);
        Serial.println(muestra);
        delay(10);
    }
}
