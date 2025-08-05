# Guía de Calibración - Smart Bin

## Calibración del Sensor de Color

### 1. Preparación
- Asegurar iluminación consistente
- Usar objetos de referencia de colores conocidos
- Mantener distancia constante (2-5cm del sensor)

### 2. Proceso de Calibración

#### Paso 1: Medición de Referencia
```cpp
// Código para medir valores de referencia
void calibrarColor(String nombreColor) {
  Serial.println("Calibrando: " + nombreColor);
  Serial.println("Coloca el objeto y presiona cualquier tecla...");
  while (!Serial.available()) delay(100);
  
  int red = medirColorPromedio(LOW, LOW);
  int green = medirColorPromedio(HIGH, HIGH);
  int blue = medirColorPromedio(LOW, HIGH);
  
  float total = red + green + blue;
  float rNorm = red / total;
  float gNorm = green / total;
  float bNorm = blue / total;
  
  Serial.print("Valores normalizados: ");
  Serial.print(rNorm, 3); Serial.print(", ");
  Serial.print(gNorm, 3); Serial.print(", ");
  Serial.println(bNorm, 3);
}
```

#### Paso 2: Actualizar Valores
Reemplazar los valores en el array `colores[]` con los nuevos valores calibrados:

```cpp
ColorReferencia colores[] = {
  {rNorm_rojo, gNorm_rojo, bNorm_rojo, ORG_ROJO},
  {rNorm_verde, gNorm_verde, bNorm_verde, ORG_VERDE},
  // ... continuar para todos los colores
};
```

### 3. Ajuste del Umbral

#### Umbral de Distancia (THRESHOLD)
- **Valor actual**: 0.35
- **Rango recomendado**: 0.2 - 0.5
- **Ajuste**: Valores menores = más estricto, mayores = más permisivo

#### Proceso de Ajuste
1. Probar con objetos conocidos
2. Observar valores de distancia en Serial Monitor
3. Ajustar THRESHOLD según precisión deseada

### 4. Calibración del Sensor Ultrasónico

#### Verificación de Distancia
```cpp
void verificarDistancia() {
  float dist = medirDistancia();
  Serial.print("Distancia medida: ");
  Serial.print(dist);
  Serial.println(" cm");
  
  if (dist < 2 || dist > 400) {
    Serial.println("⚠ Error: Distancia fuera de rango");
  }
}
```

#### Ajuste del Umbral de Proximidad
- **Valor actual**: 10cm
- **Ajuste**: Modificar condición `if (distancia <= 10)`
- **Consideraciones**: 
  - Menor distancia = más sensible
  - Mayor distancia = menos preciso

### 5. Configuración del Servomotor

#### Ajuste de Posiciones
```cpp
// Posición cerrada (ajustar según mecanismo)
#define SERVO_CERRADO 0
// Posición abierta (ajustar según mecanismo)
#define SERVO_ABIERTO 90
```

#### Calibración de Movimiento
1. Verificar rango de movimiento del servo
2. Ajustar valores según el mecanismo físico
3. Probar apertura/cierre suave

### 6. Verificación Final

#### Checklist de Calibración
- [ ] Sensor de color responde correctamente
- [ ] Valores RGB normalizados son consistentes
- [ ] Clasificación de colores es precisa
- [ ] Sensor ultrasónico detecta objetos correctamente
- [ ] Servomotor abre/cierra completamente
- [ ] LCD muestra información correcta
- [ ] LED de iluminación funciona

#### Pruebas de Validación
1. **Prueba de Objetos Conocidos**: Usar objetos de colores específicos
2. **Prueba de Distancia**: Verificar detección a diferentes distancias
3. **Prueba de Funcionamiento**: Ciclo completo de detección y clasificación

### 7. Mantenimiento

#### Recalibración Periódica
- **Frecuencia**: Cada 3-6 meses
- **Condiciones**: Cambios de iluminación o entorno
- **Proceso**: Seguir pasos 1-6

#### Monitoreo Continuo
- Revisar logs de Serial Monitor
- Verificar precisión de clasificación
- Ajustar parámetros según necesidad 