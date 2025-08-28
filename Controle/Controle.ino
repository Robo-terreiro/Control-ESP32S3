#include <Arduino.h>
#include "USB.h"
#include "USBHIDGamepad.h"

// Instancia o objeto Gamepad
USBHIDGamepad gamepad;

// --- Mapeamento dos Pinos ---
// Eixos do Joystick
const int PIN_AXIS_X = 1;
const int PIN_AXIS_Y = 2;

// Botão do clique do Joystick
const int PIN_BUTTON_SW = 3;

void setup() {
  Serial.begin(115200);

  // Configura os pinos dos botões como entrada com pull-up interno
  pinMode(PIN_BUTTON_SW, INPUT_PULLUP);
  
  // Inicia o Gamepad
  gamepad.begin();

  // Inicia o USB
  USB.begin();

  Serial.println("Controle HID iniciado.");
}

void loop() {
  // 1. LER OS VALORES ANALÓGICOS E DIGITAIS
  int xValue = analogRead(PIN_AXIS_X);
  int yValue = analogRead(PIN_AXIS_Y);
  bool buttonState = (digitalRead(PIN_BUTTON_SW) == LOW);

  // 2. PREPARAR OS DADOS PARA A FUNÇÃO send()

  // Mapear os eixos para a faixa int8_t (-127 a 127)
  int8_t mappedX = map(xValue, 0, 4095, -127, 127);
  int8_t mappedY = map(yValue, 0, 4095, 127, -127);

  // Pequena "zona morta" (deadzone)
  if (abs(mappedX) < 10) mappedX = 0;
  if (abs(mappedY) < 10) mappedY = 0;

  // Criar a máscara de bits para os botões
  uint32_t buttonMask = 0;
  if (buttonState) {
    buttonMask |= (1 << 0);
    //buttonMask 2 |= (2 << 0);
    //buttonMask 3 |= (3 << 0);
    //buttonMask 4 |= (4 << 0);
  }

  // 3. ENVIAR TODOS OS DADOS DE UMA SÓ VEZ
  gamepad.send(
    mappedX,      // int8_t x
    mappedY,      // int8_t y
    0,            // int8_t z
    0,            // int8_t rz
    0,            // int8_t rx
    0,            // int8_t ry
    0,            // uint8_t hat
    buttonMask    // uint32_t buttons
  );

  // Pequeno delay para não sobrecarregar a comunicação USB
  delay(10); 
}