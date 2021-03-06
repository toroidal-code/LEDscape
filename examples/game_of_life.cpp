#include <iostream>
#include "../matrix.hpp"
#define HEIGHT 8
#define WIDTH 64
 
const uint16_t BLANK = PixelBone_Pixel::Color(0,0,0);
const uint16_t WHITE = PixelBone_Pixel::Color(255,255,255);

struct Shape {
  uint8_t xCoord;
  uint8_t yCoord;
  uint8_t height;
  uint8_t width;
  uint16_t **figure;
};
 
struct Glider : public Shape {
  static const uint8_t GLIDER_SIZE = 3;
  Glider( uint8_t x , uint8_t y );
  ~Glider();
};
 
struct Blinker : public Shape {
  static const uint8_t BLINKER_HEIGHT = 3;
  static const uint8_t BLINKER_WIDTH = 1;
  Blinker( uint8_t x , uint8_t y );
  ~Blinker();
};
 
class GameOfLife {
public:
  PixelBone_Matrix* world;
  Shape shape;
  GameOfLife( Shape sh );
  ~GameOfLife();
  void update();
  uint8_t getState(uint16_t state , uint8_t xCoord , uint8_t yCoord);
  void iterate(unsigned int iterations);
};
 
GameOfLife::GameOfLife( Shape sh ) :
    shape(sh) 
{
  world = new PixelBone_Matrix(16, 8, 4, 1, MATRIX_TOP + MATRIX_LEFT + MATRIX_ROWS + MATRIX_ZIGZAG + TILE_TOP + TILE_LEFT + TILE_ROWS);
  world->clear();

  for ( int i = shape.yCoord; i - shape.yCoord < shape.height; i++ ) {
    for ( int j = shape.xCoord; j - shape.xCoord < shape.width; j++ ) {
      if ( i < HEIGHT && j < WIDTH ) {
        world->drawPixel(j, i, shape.figure[ i - shape.yCoord ][j - shape.xCoord ]);
      }
    }
  }
  world->show();
}

GameOfLife::~GameOfLife(){
  delete world;
}

void GameOfLife::update() {
  for (uint16_t i = 0; i < WIDTH; i++) {
    for (uint16_t j = 0; j < HEIGHT; j++) {
      world->drawPixel(i, j, GameOfLife::getState(world->getPixelColor(i, j), j, i));
    }
  }
}
 
uint8_t GameOfLife::getState(uint16_t state, uint8_t yCoord, uint8_t xCoord) {
  uint8_t neighbors = 0;
  for ( int8_t i = yCoord - 1; i <= yCoord + 1; i++ ) {
    for ( int8_t j = xCoord - 1; j <= xCoord + 1; j++ ) {
      if ( i == yCoord && j == xCoord ) {
        continue;
      }
      if ( i > -1 && i < HEIGHT && j > -1 && j < WIDTH ) {
        if ( world->getPixelColor(j, i) == WHITE) {
          neighbors++;
        }
      }
    }
  }
  if (state == WHITE) {
    return ( neighbors > 1 && neighbors < 4 ) ? WHITE : BLANK;
  }
  else {
    return ( neighbors == 3 ) ? WHITE : BLANK;
  }
}
 
void GameOfLife::iterate(uint iterations) {
  for (uint i = 0; i < iterations; i++ ) {
    update();
    if (getchar() == ' '){
    world->show();
    world->wait();
    }
    //world->moveToNextBuffer();
  }
}
 
Glider::Glider( uint8_t x , uint8_t y ) {
  xCoord = x;
  yCoord = y;
  height = GLIDER_SIZE;
  width = GLIDER_SIZE;
  figure = new uint16_t*[GLIDER_SIZE];
  for ( uint8_t i = 0; i < GLIDER_SIZE; i++ ) {
    figure[i] = new uint16_t[GLIDER_SIZE];
  }
  for ( uint8_t i = 0; i < GLIDER_SIZE; i++ ) {
    for ( uint8_t j = 0; j < GLIDER_SIZE; j++ ) {
      figure[i][j] = BLANK;
    }
  }
  figure[0][1] = WHITE;
  figure[1][2] = WHITE;
  figure[2][0] = WHITE;
  figure[2][1] = WHITE;
  figure[2][2] = WHITE;
}
 
Glider::~Glider() {
  for ( uint8_t i = 0; i < GLIDER_SIZE; i++ ) {
    delete[] figure[i];
  }
  delete[] figure;
}
 
Blinker::Blinker( uint8_t x , uint8_t y ) {
  xCoord = x;
  yCoord = y;
  height = BLINKER_HEIGHT;
  width = BLINKER_WIDTH;
  figure = new uint16_t*[BLINKER_HEIGHT];
  for ( uint8_t i = 0; i < BLINKER_HEIGHT; i++ ) {
    figure[i] = new uint16_t[BLINKER_WIDTH];
  }
  for ( uint8_t i = 0; i < BLINKER_HEIGHT; i++ ) {
    for ( uint8_t j = 0; j < BLINKER_WIDTH; j++ ) {
      figure[i][j] = WHITE;
    }
  }
}
 
Blinker::~Blinker() {
  for ( uint8_t i = 0; i < BLINKER_HEIGHT; i++ ) {
    delete[] figure[i];
  }
  delete[] figure;
}
 
int main() {
  Glider glider(0,0);
  GameOfLife gol(glider);
  gol.iterate(20);
  // Blinker blinker(1,0);
  // GameOfLife gol2(blinker);
  // gol2.iterate(4);
}
 
