#ifndef PADDLE_H
#define PADDLE_H

#include <Adafruit_ILI9341.h>
#include <paddle_sprite.h>

class Paddle {
  int x, y;
  int width, height;
  Adafruit_ILI9341* screen;
  int prevX;

public:
  Paddle(Adafruit_ILI9341* scr, int screenHeight);
  void move(int xValue);
  void draw();
  int getX() const;
  int getY() const;
  int getWidth() const;
  int getHeight() const;
};

#endif





