# Configuración del Sistema Smart Bin Semi-Inteligente

## Parámetros de Configuración Semi-Inteligente

### Configuración General
```cpp
#define DEBUG true              // Habilitar logs de debug
#define THRESHOLD 0.35         // Umbral de clasificación de color
#define MUESTRAS 10            // Número de muestras para promedio
```

### Configuración de Pines
```cpp
#define TRIG 9                 // Pin TRIG del sensor ultrasónico
#define ECHO 10                // Pin ECHO del sensor ultrasónico
#define S0 4                   // Pin S0 del sensor de color
#define S1 5                   // Pin S1 del sensor de color
#define S2 6                   // Pin S2 del sensor de color
#define S3 7                   // Pin S3 del sensor de color
#define sensorOut 8            // Pin OUT del sensor de color
#define LED_ILUMINACION 12     // Pin del LED de iluminación
#define SERVO_PIN 3            // Pin del servomotor
```

### Configuración del LCD
```cpp
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Dirección I2C, 16x2 caracteres
```

## Ajustes de Sensibilidad

### Umbral de Distancia
- **Valor actual**: 10cm
- **Rango recomendado**: 5-15cm
- **Ajuste**: Modificar condición `if (distancia <= 10)`

### Umbral de Clasificación de Color
- **Valor actual**: 0.35
- **Rango recomendado**: 0.2-0.5
- **Ajuste**: Valores menores = más estricto, mayores = más permisivo

### Configuración del Servomotor
```cpp
#define SERVO_CERRADO 0        // Posición cerrada
#define SERVO_ABIERTO 90       // Posición abierta
```

## Configuración de Colores Semi-Inteligente

### Valores de Referencia
Los valores están normalizados (R+G+B = 1.0):

```cpp
ColorReferencia colores[] = {
  {0.150, 0.442, 0.407, ORG_ROJO},      // Rojo orgánico
  {0.256, 0.264, 0.479, ORG_VERDE},     // Verde orgánico
  {0.262, 0.293, 0.446, ORG_AMARILLO},  // Amarillo orgánico
  {0.375, 0.386, 0.239, ORG_VIOLETA},   // Violeta orgánico
  {0.220, 0.391, 0.389, ORG_NARANJA},   // Naranja orgánico
  {0.423, 0.350, 0.227, INORG_AZUL},    // Azul inorgánico
  {0.365, 0.346, 0.288, INORG_BLANCO},  // Blanco inorgánico
  {0.356, 0.340, 0.303, INORG_NEGRO},   // Negro inorgánico
  {0.260, 0.337, 0.403, INORG_CAFE}     // Café inorgánico
};
```

### Proceso de Calibración Semi-Inteligente
1. Usar objetos de referencia conocidos
2. Medir valores RGB normalizados
3. Actualizar array con nuevos valores
4. Probar clasificación limitada por color

### Limitaciones de la Clasificación por Color
- **No distingue materiales**: Solo clasifica por color, no por composición
- **Dependencia de iluminación**: Requiere condiciones de luz consistentes
- **Falsos positivos**: Objetos del mismo color se clasifican igual
- **Calibración manual**: Necesita ajuste periódico según el entorno

## Configuración de Tiempos

### Delays y Timing
```cpp
delay(50);                    // Delay para estabilización del sensor
delay(5);                     // Delay entre muestras
delay(500);                   // Delay del loop principal
```

### Configuración del Sensor de Color
```cpp
digitalWrite(S0, HIGH);       // 20% de frecuencia
digitalWrite(S1, LOW);        // Configuración de frecuencia
```

## Configuración de Debug

### Logs Disponibles
- **Detección de objetos**: Distancia medida
- **Valores de color**: RGB crudos y normalizados
- **Clasificación**: Color detectado y distancia mínima
- **Errores**: Problemas de sensor o conexiones

### Habilitar/Deshabilitar Debug
```cpp
#define DEBUG true             // true = logs habilitados
#define DEBUG false            // false = logs deshabilitados
```

## Configuración de Alimentación

### Requisitos de Voltaje
- **Arduino**: 5V DC
- **Servomotor**: 4.8V-6V DC (recomendado independiente)
- **Sensores**: 5V desde Arduino
- **LCD**: 5V desde Arduino

### Consumo de Corriente
- **Arduino**: ~50mA
- **Servomotor**: ~500mA (picos)
- **Sensores**: ~20mA
- **LCD**: ~30mA
- **LED**: ~20mA

## Configuración de Ambiente

### Iluminación
- **Tipo**: Luz blanca consistente
- **Intensidad**: 200-1000 lux
- **Posición**: Evitar sombras en el sensor

### Temperatura
- **Rango operativo**: 0-50°C
- **Óptimo**: 20-30°C

### Humedad
- **Rango**: 20-80% RH
- **Evitar**: Condensación en sensores

## Configuración de Seguridad

### Límites de Operación
- **Distancia mínima**: 2cm
- **Distancia máxima**: 400cm
- **Tiempo máximo de apertura**: Configurable
- **Temperatura máxima**: 50°C

### Protecciones
- **Timeout de sensor**: 30 segundos
- **Validación de valores**: Rango de 0-1000
- **Verificación de conexiones**: Al inicio

## Configuración de Mantenimiento

### Calibración Periódica
- **Frecuencia**: Cada 3-6 meses
- **Condiciones**: Cambios de iluminación o entorno
- **Proceso**: Seguir guía de calibración

### Limpieza
- **Sensor de color**: Limpiar lente mensualmente
- **Sensor ultrasónico**: Limpiar superficie
- **Servomotor**: Verificar lubricación
- **LCD**: Limpiar pantalla según necesidad 