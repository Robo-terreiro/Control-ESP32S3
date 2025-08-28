#include <Arduino.h>
#include "USB.h"
#include "USBHIDGamepad.h"

// Instancia o objeto Gamepad
USBHIDGamepad gamepad;

// --- Mapeamento dos Pinos ---
// Joystick 1
const int PIN_AXIS_X = 1;
const int PIN_AXIS_Y = 2;

// Joystick 2
const int PIN_AXIS_RX = 3;
const int PIN_AXIS_RY = 4;

// Botões (1 a 8)
const int PIN_BUTTON_SW_1 = 5;
const int PIN_BUTTON_SW_2 = 6;
const int PIN_BUTTON_SW_3 = 7;
const int PIN_BUTTON_SW_4 = 8;
const int PIN_BUTTON_SW_5 = 9;
const int PIN_BUTTON_SW_6 = 10;
const int PIN_BUTTON_SW_7 = 11;
const int PIN_BUTTON_SW_8 = 12;

// Botões do segundo joystick
const int PIN_BUTTON_SW_9  = 13;
const int PIN_BUTTON_SW_10 = 14;

void setup() {
  Serial.begin(115200);

  // Configura todos os pinos dos botões como entrada com pull-up interno
  pinMode(PIN_BUTTON_SW_1, INPUT_PULLUP);
  pinMode(PIN_BUTTON_SW_2, INPUT_PULLUP);
  pinMode(PIN_BUTTON_SW_3, INPUT_PULLUP);
  pinMode(PIN_BUTTON_SW_4, INPUT_PULLUP);
  pinMode(PIN_BUTTON_SW_5, INPUT_PULLUP);
  pinMode(PIN_BUTTON_SW_6, INPUT_PULLUP);
  pinMode(PIN_BUTTON_SW_7, INPUT_PULLUP);
  pinMode(PIN_BUTTON_SW_8, INPUT_PULLUP);
  pinMode(PIN_BUTTON_SW_9, INPUT_PULLUP);
  pinMode(PIN_BUTTON_SW_10, INPUT_PULLUP);

  // Inicia o Gamepad e o USB
  gamepad.begin();
  USB.begin();
}

void loop() {
  // Leitura dos eixos dos dois joysticks
  int xValue  = analogRead(PIN_AXIS_X);
  int yValue  = analogRead(PIN_AXIS_Y);
  int rxValue = analogRead(PIN_AXIS_RX);
  int ryValue = analogRead(PIN_AXIS_RY);

  // Mapeamento para -127 a 127
  int8_t mappedX  = map(xValue,  0, 4095, -127, 127);
  int8_t mappedY  = map(yValue,  0, 4095, 127, -127);
  int8_t mappedRX = map(rxValue, 0, 4095, -127, 127);
  int8_t mappedRY = map(ryValue, 0, 4095, 127, -127);

  // Deadzone
  if (abs(mappedX) < 10)  mappedX  = 0;
  if (abs(mappedY) < 10)  mappedY  = 0;
  if (abs(mappedRX) < 10) mappedRX = 0;
  if (abs(mappedRY) < 10) mappedRY = 0;

  // Máscara de botões
  uint32_t buttonMask = 0;

  if (digitalRead(PIN_BUTTON_SW_1)  == LOW) buttonMask |= (1 << 0);
  if (digitalRead(PIN_BUTTON_SW_2)  == LOW) buttonMask |= (1 << 1);
  if (digitalRead(PIN_BUTTON_SW_3)  == LOW) buttonMask |= (1 << 2);
  if (digitalRead(PIN_BUTTON_SW_4)  == LOW) buttonMask |= (1 << 3);
  if (digitalRead(PIN_BUTTON_SW_5)  == LOW) buttonMask |= (1 << 4);
  if (digitalRead(PIN_BUTTON_SW_6)  == LOW) buttonMask |= (1 << 5);
  if (digitalRead(PIN_BUTTON_SW_7)  == LOW) buttonMask |= (1 << 6);
  if (digitalRead(PIN_BUTTON_SW_8)  == LOW) buttonMask |= (1 << 7);
  if (digitalRead(PIN_BUTTON_SW_9)  == LOW) buttonMask |= (1 << 8);
  if (digitalRead(PIN_BUTTON_SW_10) == LOW) buttonMask |= (1 << 9);

  // Enviar para o host
  gamepad.send(
    mappedX,       // X
    mappedY,       // Y
    0,             // Z
    0,             // RZ
    mappedRX,      // RX
    mappedRY,      // RY
    0,             // Hat
    buttonMask     // Botões
  );

  delay(10);
}
