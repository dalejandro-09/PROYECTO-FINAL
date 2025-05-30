#include "Paddle.h"

#define PADDLE_WIDTH 32
#define PADDLE_HEIGHT 32
#define MOVE_STEP 15
#define SCREEN_WIDTH 320
#define JOYSTICK_CENTER 512
#define JOYSTICK_THRESH 100

Paddle::Paddle(Adafruit_ILI9341* scr, int screenHeight)
    : x(0), y(screenHeight - PADDLE_HEIGHT), width(PADDLE_WIDTH), height(PADDLE_HEIGHT), screen(scr), prevX(0) {}

void Paddle::move(int xValue) {
  prevX = x;
  if (xValue > JOYSTICK_CENTER + JOYSTICK_THRESH && x > 0) x -= MOVE_STEP;
  else if (xValue < JOYSTICK_CENTER - JOYSTICK_THRESH && x < SCREEN_WIDTH - width) x += MOVE_STEP;
}

void Paddle::draw() {
  if (x != prevX) {
    screen->fillRect(prevX, y, width, height, ILI9341_BLACK);
    screen->drawRGBBitmap(x, y, paddle_sprite[0], width, height);
  }
}

int Paddle::getX() const { return x; }
int Paddle::getY() const { return y; }
int Paddle::getWidth() const { return width; }
int Paddle::getHeight() const { return height; }



