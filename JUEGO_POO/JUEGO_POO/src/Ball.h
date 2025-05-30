#ifndef BALL_H
#define BALL_H

#include <Adafruit_ILI9341.h>
#include <ball_sprite.h>

class Ball {
  int x, y, dx, dy;
  int size;
  int prevX, prevY;
  Adafruit_ILI9341* screen;

public:
  Ball(Adafruit_ILI9341* scr);
  void update();
  void draw();
  void bounceX();
  void bounceY();
  void invertDY();
  void reset();
  int getX() const;
  int getY() const;
  int getDX() const;
  int getDY() const;
  int getSize() const;
  void setDX(int newDX);
  void setDY(int newDY);
};

#endif





