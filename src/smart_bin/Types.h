#pragma once

/// Código de color
enum ColorCodigo {
  ORG_ROJO,
  ORG_VERDE,
  ORG_AMARILLO,
  ORG_VIOLETA,
  ORG_NARANJA,
  INORG_AZUL,
  INORG_BLANCO,
  INORG_NEGRO,
  INORG_CAFE,
  COLOR_INDEFINIDO
};

/// Estados del sistema
enum EstadoSistema {
  ESPERANDO = 0,
  DETECTANDO = 1,
  ANALIZANDO = 2,
  CLASIFICADO = 3,
  INACTIVO = 4
};
