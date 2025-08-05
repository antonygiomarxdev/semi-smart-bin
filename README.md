# Smart Bin - Sistema de Clasificación de Basura Inteligente

## Descripción
Sistema de clasificación de basura inteligente desarrollado con Arduino que detecta automáticamente objetos, clasifica su color y determina si es orgánico o inorgánico, abriendo el contenedor correspondiente.

## Características Principales
- 🎨 **Clasificación por Color**: Utiliza sensor TCS3200 para detectar colores específicos
- 📏 **Detección de Proximidad**: Sensor ultrasónico HC-SR04 para detectar objetos
- 🤖 **Actuación Automática**: Servomotor SG90 para abrir/cerrar contenedores
- 📺 **Interfaz de Usuario**: Display LCD I2C para mostrar información
- 💡 **Iluminación Inteligente**: LED para mejorar la detección de colores

## Estructura del Proyecto
```
smart-bin/
├── src/                    # Código fuente principal
│   └── smart_bin.ino      # Sistema principal de clasificación
├── docs/                   # Documentación técnica
│   ├── componentes.md      # Especificaciones de componentes
│   └── calibracion.md     # Guía de calibración
├── schematics/             # Diagramas y esquemas
│   └── conexiones.fzz      # Esquema de conexiones (Fritzing)
├── tests/                  # Pruebas y validaciones
│   └── test_calibracion.ino # Herramientas de calibración
└── README.md              # Documentación principal
```

## Componentes Requeridos
- **Arduino Uno/Nano/Mega** - Microcontrolador principal
- **Sensor de Color TCS3200** - Detección y clasificación de colores
- **Sensor Ultrasónico HC-SR04** - Detección de proximidad
- **Servomotor SG90** - Actuador para apertura/cierre
- **Display LCD I2C 16x2** - Interfaz de usuario
- **LED de Alta Intensidad** - Iluminación del área de detección
- **Fuente de Alimentación 5V** - Alimentación del sistema

## Instalación y Configuración

### 1. Preparación del Hardware
1. Conecta los componentes según el esquema en `schematics/conexiones.fzz`
2. Verifica todas las conexiones eléctricas
3. Asegura una fuente de alimentación estable de 5V

### 2. Configuración del Software
1. Instala Arduino IDE (versión 1.8.x o superior)
2. Instala las librerías requeridas:
   - `Servo.h` (incluida en Arduino IDE)
   - `LiquidCrystal_I2C.h` (instalar desde Library Manager)
3. Abre `src/smart_bin.ino` en Arduino IDE
4. Selecciona tu placa Arduino
5. Compila y sube el código

### 3. Calibración Inicial
1. Ejecuta `tests/test_calibracion.ino` para verificar componentes
2. Sigue la guía en `docs/calibracion.md`
3. Ajusta los valores de color según tu entorno

## Uso del Sistema

### Funcionamiento Normal
1. **Espera**: El sistema muestra "Esperando objeto" en el LCD
2. **Detección**: Cuando un objeto se acerca (< 10cm), se activa la detección
3. **Clasificación**: El sensor de color analiza el objeto
4. **Resultado**: El LCD muestra "ORGÁNICO" o "INORGÁNICO"
5. **Actuación**: El servomotor abre el contenedor correspondiente

### Configuración de Colores
El sistema clasifica objetos según estos colores:
- **Orgánicos**: Rojo, Verde, Amarillo, Violeta, Naranja
- **Inorgánicos**: Azul, Blanco, Negro, Café

### Monitoreo y Debug
- Conecta el Arduino a la computadora
- Abre el Serial Monitor (9600 baud)
- Observa los logs detallados del sistema

## Mantenimiento

### Calibración Periódica
- Recalibrar cada 3-6 meses
- Verificar precisión de clasificación
- Ajustar valores según cambios de iluminación

### Limpieza
- Limpiar lentes del sensor de color
- Verificar conexiones eléctricas
- Revisar funcionamiento del servomotor

## Solución de Problemas

### Problemas Comunes
1. **Sensor no detecta colores**: Verificar iluminación y distancia
2. **Servomotor no responde**: Verificar alimentación y conexiones
3. **LCD no muestra**: Verificar dirección I2C (0x27)
4. **Falsos positivos**: Ajustar THRESHOLD en el código

### Debug Avanzado
- Usar `tests/test_calibracion.ino` para diagnóstico
- Revisar logs del Serial Monitor
- Verificar valores de calibración

## Licencia
MIT License - Ver archivo LICENSE para más detalles 