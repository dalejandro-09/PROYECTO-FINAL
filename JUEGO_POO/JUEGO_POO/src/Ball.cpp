#include "Ball.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define BALL_SIZE 16

Ball::Ball(Adafruit_ILI9341* scr)
    : x(SCREEN_WIDTH / 2), y(SCREEN_HEIGHT / 2), dx(2), dy(-4),
      size(BALL_SIZE), screen(scr), prevX(x), prevY(y) {}

void Ball::update() {
  x += dx;
  y += dy;
}

void Ball::draw() {
  if (x != prevX || y != prevY) {
    screen->fillRect(prevX, prevY, size, size, ILI9341_BLACK);
    screen->drawRGBBitmap(x, y, ball_sprite[0], size, size);
    prevX = x;
    prevY = y;
  }
}

void Ball::bounceX() { dx = -dx; }
void Ball::bounceY() { dy = -dy; }
void Ball::invertDY() { dy = -abs(dy); }

void Ball::reset() {
  x = SCREEN_WIDTH / 2;
  y = SCREEN_HEIGHT / 5;
  dx = (random(0, 2) == 0) ? 2 : -2;
  dy = 4;
  prevX = x;
  prevY = y;
}

int Ball::getX() const { return x; }
int Ball::getY() const { return y; }
int Ball::getDX() const { return dx; }
int Ball::getDY() const { return dy; }
int Ball::getSize() const { return size; }
void Ball::setDX(int newDX) { dx = newDX; }
void Ball::setDY(int newDY) { dy = newDY; }



