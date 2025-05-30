#ifndef BLOCKMANAGER_H
#define BLOCKMANAGER_H

#include <Adafruit_ILI9341.h>
#include <block_sprite.h>

#define BLOCK_WIDTH 32
#define BLOCK_HEIGHT 16
#define BLOCK_ROWS 4
#define BLOCK_COLS 8

class BlockManager {
  bool blocks[BLOCK_ROWS][BLOCK_COLS];
  Adafruit_ILI9341* screen;

public:
  BlockManager(Adafruit_ILI9341* scr);
  void initBlocks();
  bool checkCollision(int ballX, int ballY, int ballSize);
  bool anyRemaining();
};

#endif


