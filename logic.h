#ifndef LOGIC_ONCE
#define LOGIC_ONCE

#define INVALID_MOVES -3

#include <cstdint>

namespace Logic
{

extern uint8_t grid[3][3];
extern uint8_t count;
extern int8_t turn;

uint8_t TakeTurn();
bool CheckTriple(uint8_t a, uint8_t b, uint8_t c);
uint8_t CheckWinner();
bool HasValidMoves();
void Restart();

}

#endif
