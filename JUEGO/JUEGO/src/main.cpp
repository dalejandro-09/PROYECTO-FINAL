#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <paddle_sprite.h>
#include <ball_sprite.h>
#include <block_sprite.h>  

// Pines pantalla
#define TFT_DC   7
#define TFT_CS   6
#define TFT_MOSI 11
#define TFT_CLK  13
#define TFT_RST  10
#define TFT_MISO 12

// Pines joystick
#define VRX A0
#define VRY A1
//pin buzzer
#define BUZZER_PIN 3

// Constantes del juego
constexpr int SCREEN_WIDTH     = 320;
constexpr int SCREEN_HEIGHT    = 240;
constexpr int PADDLE_WIDTH     = 32;
constexpr int PADDLE_HEIGHT    = 32;
constexpr int BALL_SIZE        = 16;
constexpr int MOVE_STEP        = 15;
constexpr int JOYSTICK_CENTER  = 512;
constexpr int JOYSTICK_THRESH  = 100;
constexpr int UPDATE_INTERVAL  = 5;

// Constantes de bloques
constexpr int BLOCK_WIDTH      = 32;
constexpr int BLOCK_HEIGHT     = 16;
constexpr int BLOCK_ROWS       = 4;
constexpr int BLOCK_COLS       = 8;

bool blocks[BLOCK_ROWS][BLOCK_COLS]; 

// Estado del juego
bool gameOver = false;
bool gameWon = false;
int vidas = 3;

// TFT
Adafruit_ILI9341 screen(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

// Posiciones
int paddleX = 0, paddleY = SCREEN_HEIGHT - PADDLE_HEIGHT;
int prevPaddleX = 0;

int ballX = SCREEN_WIDTH / 2, ballY = SCREEN_HEIGHT / 2;
int prevBallX = 0, prevBallY = 0;

// Movimiento de pelota
int ballDX = 2, ballDY = 4;

unsigned long lastUpdateTime = 0;

// Función para mover la paleta
void moverBarra(int xValue) {
  if (xValue > JOYSTICK_CENTER + JOYSTICK_THRESH && paddleX > 0) {
    paddleX -= MOVE_STEP;
  } else if (xValue < JOYSTICK_CENTER - JOYSTICK_THRESH && paddleX < SCREEN_WIDTH - PADDLE_WIDTH) {
    paddleX += MOVE_STEP;
  }
}
//Dibujar las vidas que le quedan
void dibujarVidas(int vidasRestantes) {
  int spacing = BALL_SIZE + 4;
  int iconY = 5;
  int totalWidth = spacing * 3;
  int iconX = SCREEN_WIDTH - totalWidth - 5;

  screen.fillRect(iconX, iconY, totalWidth, BALL_SIZE, ILI9341_BLACK);

  for (int i = 0; i < vidasRestantes; i++) {
    screen.drawRGBBitmap(iconX + i * spacing, iconY, ball_sprite[0], BALL_SIZE, BALL_SIZE);
  }
}
//sonidos
void sonidoDerrota() {
  int melody[] = { 330, 262, 196, 165 }; // FRECUENCIAS
  int noteDurations[] = { 300, 300, 300, 600 }; //DURACION

  for (int repeat = 0; repeat < 2; repeat++) {
    for (int i = 0; i < 4; i++) {
      tone(BUZZER_PIN, melody[i], noteDurations[i]);
      delay(noteDurations[i] * 1.3); // 
    }
  }
  noTone(BUZZER_PIN);
}


void sonidoVictoria() {
  int melody[] = { 523, 659, 784, 1047 }; // FRECUENCIAS
  int noteDurations[] = { 200, 200, 200, 400 };//DURACION

  for (int repeat = 0; repeat < 2; repeat++) {
    for (int i = 0; i < 4; i++) {
      tone(BUZZER_PIN, melody[i], noteDurations[i]);
      delay(noteDurations[i] * 1.3);  
    }
  }
  noTone(BUZZER_PIN);
}

void sonidoBloqueRoto() {
  tone(BUZZER_PIN, 880, 60);  
  delay(60);
  tone(BUZZER_PIN, 988, 40);  
  delay(50);
  noTone(BUZZER_PIN);
}

void sonidoPerderVida() {
  tone(BUZZER_PIN, 880, 150);  
  delay(150);
  tone(BUZZER_PIN, 659, 150);  
  delay(150);
  tone(BUZZER_PIN, 440, 300);  
  delay(300);
  noTone(BUZZER_PIN);
}


void sonidoReboteBarra() {
  tone(BUZZER_PIN, 1000, 30);
  delay(30);
  tone(BUZZER_PIN, 1200, 30);
  delay(30);
  noTone(BUZZER_PIN);
}

// Destruir bloques
void destruirBloque(int row, int col) {
  blocks[row][col] = false;
  int blockX = col * BLOCK_WIDTH;
  int blockY = row * BLOCK_HEIGHT;
  screen.fillRect(blockX, blockY, BLOCK_WIDTH, BLOCK_HEIGHT, ILI9341_BLACK);
}

// Verifica si quedan bloques
bool quedanBloques() {
  for (int row = 0; row < BLOCK_ROWS; row++) {
    for (int col = 0; col < BLOCK_COLS; col++) {
      if (blocks[row][col]) return true;
    }
  }
  return false;
}

// Muestra mensaje en el centro de la pantalla
void mostrarMensaje(const char* mensaje) {
  screen.fillScreen(ILI9341_BLACK);
  screen.setTextColor(ILI9341_WHITE);
  screen.setTextSize(3);
  int16_t x = (SCREEN_WIDTH - strlen(mensaje) * 18) / 2;
  int16_t y = SCREEN_HEIGHT / 2 - 12;
  screen.setCursor(x, y);
  screen.print(mensaje);
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(VRX, INPUT);
 
  Serial.begin(9600);

  screen.begin();
  screen.setRotation(1);
  screen.fillScreen(ILI9341_BLACK);

  // Inicializar bloques
  for (int row = 0; row < BLOCK_ROWS; row++) {
    for (int col = 0; col < BLOCK_COLS; col++) {
      blocks[row][col] = true;
      int blockX = col * BLOCK_WIDTH;
      int blockY = row * BLOCK_HEIGHT;
      screen.drawRGBBitmap(blockX, blockY, block_sprite[0], BLOCK_WIDTH, BLOCK_HEIGHT);
    }
  }

  // Dibujar paleta y pelota
  screen.drawRGBBitmap(paddleX, paddleY, paddle_sprite[0], PADDLE_WIDTH, PADDLE_HEIGHT);
  screen.drawRGBBitmap(ballX, ballY, ball_sprite[0], BALL_SIZE, BALL_SIZE);
  dibujarVidas(vidas);
}

void loop() {
  
  if (gameOver || gameWon) return;

  int xValue = analogRead(VRX);
  moverBarra(xValue);

  // Mover pelota
  ballX += ballDX;
  ballY += ballDY;

  // Colisión con bordes
  if (ballX <= 0 || ballX >= SCREEN_WIDTH - BALL_SIZE) ballDX *= -1;
  if (ballY <= 0) ballDY *= -1;

  // Colisión con paddle
  if (ballY + BALL_SIZE >= paddleY && ballY + BALL_SIZE <= paddleY + PADDLE_HEIGHT &&
      ballX + BALL_SIZE >= paddleX && ballX <= paddleX + PADDLE_WIDTH) {

    int ballCenterX = ballX + BALL_SIZE / 2;
    float relativeImpact = (float)(ballCenterX - paddleX) / PADDLE_WIDTH;

    ballDX = (int)(8 * (relativeImpact - 0.5));
    if (ballDX == 0) ballDX = (random(0, 2) == 0) ? -1 : 1;

    ballDY = -abs(ballDY);
    sonidoReboteBarra();
    ballY = paddleY - BALL_SIZE;
  }

  // Colisión con bloques
  for (int row = 0; row < BLOCK_ROWS; row++) {
    for (int col = 0; col < BLOCK_COLS; col++) {
      if (blocks[row][col]) {
        int blockX = col * BLOCK_WIDTH;
        int blockY = row * BLOCK_HEIGHT;

        if (ballX + BALL_SIZE > blockX && ballX < blockX + BLOCK_WIDTH &&
            ballY + BALL_SIZE > blockY && ballY < blockY + BLOCK_HEIGHT) {

          destruirBloque(row, col);
          ballDY *= -1;
          sonidoBloqueRoto();
          goto finColisionBloque;
        }
      }
    }
  }
finColisionBloque:

  // GAME OVER si cae la pelota
if (ballY > SCREEN_HEIGHT) {
  vidas--;
  sonidoPerderVida();
  dibujarVidas(vidas);

  if (vidas == 0) {
    gameOver = true;
    mostrarMensaje("GAME OVER");
    sonidoDerrota();
    return;
  } else {
    // Reiniciar posición de la pelota
    ballX = SCREEN_WIDTH / 2;
    ballY = SCREEN_HEIGHT / 5;
    ballDX = (random(0, 2) == 0) ? 2 : -2;
    ballDY = 4;
    prevBallX = ballX;
    prevBallY = ballY;

    delay(1000); // Espera un momento antes de seguir
  }
}

  // YOU WIN si no quedan bloques
  if (!quedanBloques()) {
    gameWon = true;
    mostrarMensaje("YOU WIN!");
    sonidoVictoria();
    return;
  }

  // Redibujar si se movió
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
    if (paddleX != prevPaddleX) {
      screen.fillRect(prevPaddleX, paddleY, PADDLE_WIDTH, PADDLE_HEIGHT, ILI9341_BLACK);
      screen.drawRGBBitmap(paddleX, paddleY, paddle_sprite[0], PADDLE_WIDTH, PADDLE_HEIGHT);
      prevPaddleX = paddleX;
    }

    if (ballX != prevBallX || ballY != prevBallY) {
      screen.fillRect(prevBallX, prevBallY, BALL_SIZE, BALL_SIZE, ILI9341_BLACK);
      screen.drawRGBBitmap(ballX, ballY, ball_sprite[0], BALL_SIZE, BALL_SIZE);
      prevBallX = ballX;
      prevBallY = ballY;
      
    }
    
    lastUpdateTime = currentTime;
  }
}






