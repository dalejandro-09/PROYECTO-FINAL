#ifndef GAME_H
#define GAME_H

#include <Adafruit_ILI9341.h>
#include "Paddle.h"
#include "Ball.h"
#include "BlockManager.h"

class Game {
  Adafruit_ILI9341* screen;
  Paddle* paddle;
  Ball* ball;
  BlockManager* blocks;

  int vidas;
  bool gameOver;
  bool gameWon;
  unsigned long lastUpdateTime;

  void dibujarVidas();
  void mostrarMensaje(const char* mensaje);
  void sonidoDerrota();
  void sonidoVictoria();
  void sonidoBloqueRoto();
  void sonidoPerderVida();
  void sonidoReboteBarra();

public:
  Game(Adafruit_ILI9341* scr);
  void setup();
  void update();
  bool isGameOver() const;
  bool isGameWon() const;
  void reset();

};

#endif



