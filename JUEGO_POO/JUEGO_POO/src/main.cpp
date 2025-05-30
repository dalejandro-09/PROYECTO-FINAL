#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include "Game.h"


// Pines pantalla TFT
#define TFT_DC   7
#define TFT_CS   6
#define TFT_MOSI 11
#define TFT_CLK  13
#define TFT_RST  10
#define TFT_MISO 12
#define JOYSTICK_BTN_PIN 2

// Crear instancia de pantalla
Adafruit_ILI9341 screen(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

// Instancia del juego
Game* juego;

void setup() {
  pinMode(JOYSTICK_BTN_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  screen.begin();
  screen.setRotation(1);
  screen.fillScreen(ILI9341_BLACK);
  
  juego = new Game(&screen);
  juego->setup();
}

void loop() {
  juego->update();

  if ((juego->isGameOver() || juego->isGameWon()) && digitalRead(JOYSTICK_BTN_PIN) == LOW) {
    delay(300);
    juego->reset();
  }
}








