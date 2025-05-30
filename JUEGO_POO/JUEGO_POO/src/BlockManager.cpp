#include "BlockManager.h"

BlockManager::BlockManager(Adafruit_ILI9341* scr) : screen(scr) {}

void BlockManager::initBlocks() {
  for (int row = 0; row < BLOCK_ROWS; row++) {
    for (int col = 0; col < BLOCK_COLS; col++) {
      blocks[row][col] = true;
      int blockX = col * BLOCK_WIDTH;
      int blockY = row * BLOCK_HEIGHT;
      screen->drawRGBBitmap(blockX, blockY, block_sprite[0], BLOCK_WIDTH, BLOCK_HEIGHT);
    }
  }
}

bool BlockManager::checkCollision(int ballX, int ballY, int ballSize) {
  for (int row = 0; row < BLOCK_ROWS; row++) {
    for (int col = 0; col < BLOCK_COLS; col++) {
      if (blocks[row][col]) {
        int blockX = col * BLOCK_WIDTH;
        int blockY = row * BLOCK_HEIGHT;
        if (ballX + ballSize > blockX && ballX < blockX + BLOCK_WIDTH &&
            ballY + ballSize > blockY && ballY < blockY + BLOCK_HEIGHT) {
          blocks[row][col] = false;
          screen->fillRect(blockX, blockY, BLOCK_WIDTH, BLOCK_HEIGHT, ILI9341_BLACK);
          return true;
        }
      }
    }
  }
  return false;
}

bool BlockManager::anyRemaining() {
  for (int row = 0; row < BLOCK_ROWS; row++)
    for (int col = 0; col < BLOCK_COLS; col++)
      if (blocks[row][col]) return true;
  return false;
}

