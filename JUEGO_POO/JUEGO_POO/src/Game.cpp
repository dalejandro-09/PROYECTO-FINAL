#include "Game.h"
#include <Arduino.h>

#define VRX A0
#define JOYSTICK_BTN_PIN 2
#define BUZZER_PIN 3

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define BALL_SIZE 16
#define UPDATE_INTERVAL 5

Game::Game(Adafruit_ILI9341* scr)
    : screen(scr), vidas(3), gameOver(false), gameWon(false), lastUpdateTime(0) {
  paddle = new Paddle(screen, SCREEN_HEIGHT);
  ball = new Ball(screen);
  blocks = new BlockManager(screen);
}

void Game::setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(VRX, INPUT);
  
  // Pantalla de bienvenida
  screen->fillScreen(ILI9341_BLACK);
  screen->setTextColor(ILI9341_WHITE);
  screen->setTextSize(3);
  screen->setCursor(80, 100);
  screen->print("BREAKOUT");
  screen->setTextSize(2);
  screen->setCursor(90, 140);
  screen->print("Listo?");
  delay(2000);

  screen->fillScreen(ILI9341_BLACK);

  // Inicialización del juego
  blocks->initBlocks();
  screen->drawRGBBitmap(paddle->getX(), paddle->getY(), paddle_sprite[0], paddle->getWidth(), paddle->getHeight());
  screen->drawRGBBitmap(ball->getX(), ball->getY(), ball_sprite[0], BALL_SIZE, BALL_SIZE);
  dibujarVidas();
}

void Game::update() {
  if (gameOver || gameWon) return;

  int xValue = analogRead(VRX);
  paddle->move(xValue);
  paddle->draw();

  ball->update();

  if (ball->getX() <= 0 || ball->getX() >= SCREEN_WIDTH - BALL_SIZE) ball->bounceX();
  if (ball->getY() <= 0) ball->bounceY();

  if (ball->getY() + BALL_SIZE >= paddle->getY() &&
      ball->getY() + BALL_SIZE <= paddle->getY() + paddle->getHeight() &&
      ball->getX() + BALL_SIZE >= paddle->getX() &&
      ball->getX() <= paddle->getX() + paddle->getWidth()) {

    int ballCenterX = ball->getX() + BALL_SIZE / 2;
    float relativeImpact = (float)(ballCenterX - paddle->getX()) / paddle->getWidth();
    int newDX = (int)(8 * (relativeImpact - 0.5));
    if (newDX == 0) newDX = (random(0, 2) == 0) ? -1 : 1;
    ball->setDX(newDX);
    ball->invertDY();
    sonidoReboteBarra();
  }

  if (blocks->checkCollision(ball->getX(), ball->getY(), ball->getSize())) {
    ball->bounceY();
    sonidoBloqueRoto();
  }

  if (ball->getY() > SCREEN_HEIGHT) {
    vidas--;
    sonidoPerderVida();
    dibujarVidas();

    if (vidas == 0) {
      gameOver = true;
      mostrarMensaje("GAME OVER");
      sonidoDerrota();
      return;
    } else {
      ball->reset();
      delay(1000);
    }
  }

  if (!blocks->anyRemaining()) {
    gameWon = true;
    mostrarMensaje("YOU WIN!");
    sonidoVictoria();
    return;
  }

  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
    ball->draw();
    lastUpdateTime = currentTime;
  }
}

bool Game::isGameOver() const { return gameOver; }
bool Game::isGameWon() const { return gameWon; }

void Game::dibujarVidas() {
  int spacing = BALL_SIZE + 4;
  int iconY = 5;
  int totalWidth = spacing * 3;
  int iconX = SCREEN_WIDTH - totalWidth - 5;

  screen->fillRect(iconX, iconY, totalWidth, BALL_SIZE, ILI9341_BLACK);

  for (int i = 0; i < vidas; i++) {
    screen->drawRGBBitmap(iconX + i * spacing, iconY, ball_sprite[0], BALL_SIZE, BALL_SIZE);
  }
}

void Game::mostrarMensaje(const char* mensaje) {
  screen->fillScreen(ILI9341_BLACK);
  screen->setTextColor(ILI9341_WHITE);
  screen->setTextSize(3);
  int16_t x = (SCREEN_WIDTH - strlen(mensaje) * 18) / 2;
  int16_t y = SCREEN_HEIGHT / 2 - 12;
  screen->setCursor(x, y);
  screen->print(mensaje);
}

void Game::sonidoDerrota() {
  int melody[] = { 330, 262, 196, 165 };
  int noteDurations[] = { 300, 300, 300, 600 };
  for (int repeat = 0; repeat < 2; repeat++) {
    for (int i = 0; i < 4; i++) {
      tone(BUZZER_PIN, melody[i], noteDurations[i]);
      delay(noteDurations[i] * 1.3);
    }
  }
  noTone(BUZZER_PIN);
}

void Game::sonidoVictoria() {
  int melody[] = { 523, 659, 784, 1047 };
  int noteDurations[] = { 200, 200, 200, 400 };
  for (int repeat = 0; repeat < 2; repeat++) {
    for (int i = 0; i < 4; i++) {
      tone(BUZZER_PIN, melody[i], noteDurations[i]);
      delay(noteDurations[i] * 1.3);
    }
  }
  noTone(BUZZER_PIN);
}

void Game::sonidoBloqueRoto() {
  tone(BUZZER_PIN, 880, 60);
  delay(60);
  tone(BUZZER_PIN, 988, 40);
  delay(50);
  noTone(BUZZER_PIN);
}

void Game::sonidoPerderVida() {
  tone(BUZZER_PIN, 880, 150);
  delay(150);
  tone(BUZZER_PIN, 659, 150);
  delay(150);
  tone(BUZZER_PIN, 440, 300);
  delay(300);
  noTone(BUZZER_PIN);
}

void Game::sonidoReboteBarra() {
  tone(BUZZER_PIN, 1000, 30);
  delay(30);
  tone(BUZZER_PIN, 1200, 30);
  delay(30);
  noTone(BUZZER_PIN);
}

void Game::reset() {
  gameOver = false;
  gameWon = false;
  vidas = 3;
  lastUpdateTime = 0;
  screen->fillScreen(ILI9341_BLACK);

  blocks->initBlocks();
  ball->reset();
  screen->drawRGBBitmap(paddle->getX(), paddle->getY(), paddle_sprite[0], paddle->getWidth(), paddle->getHeight());
  screen->drawRGBBitmap(ball->getX(), ball->getY(), ball_sprite[0], BALL_SIZE, BALL_SIZE);
  dibujarVidas();
}

