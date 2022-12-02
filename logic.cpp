#include "logic.h"

namespace Logic
{

uint8_t grid[3][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
uint8_t count = 0;
int8_t turn = 1;

uint8_t TakeTurn()
{
    count++;
    
    if ( count < 5 )
    {
        turn = turn * -1;
        return 0;
    }

    uint8_t winner = CheckWinner();

    if ( winner != 0 ) return winner;
    if (!HasValidMoves()) return INVALID_MOVES;

    turn *= -1;
    return 0;
}

bool CheckTriple(uint8_t a, uint8_t b, uint8_t c)
{
    return a != 0 && a == b && a == c;
}

// Check after a player has made 3 turns and onwards | After 5th move
uint8_t CheckWinner()
{
    if (CheckTriple(grid[0][0], grid[1][1], grid[2][2])) return grid[0][0];
    if (CheckTriple(grid[0][2], grid[1][1], grid[2][0])) return grid[0][2];

    for (size_t row = 0; row < 3; row++)
    {
        if (CheckTriple(grid[row][0], grid[row][1], grid[row][2])) return grid[row][0];
        if (CheckTriple(grid[0][row], grid[1][row], grid[2][row])) return grid[0][row];
    }

    return 0;
}

bool HasValidMoves()
{
    uint8_t free = 0;

    if (CheckTriple(grid[0][0], grid[1][1], grid[2][2])) return false;
    if (CheckTriple(grid[0][2], grid[1][1], grid[2][0])) return false;

    for (size_t row = 0; row < 3; row++)
    {
        if (CheckTriple(grid[row][0], grid[row][1], grid[row][2])) return false;
        if (CheckTriple(grid[0][row], grid[1][row], grid[2][row])) return false;

        for (size_t col = 0; col < 3; col++)
            if (grid[row][col] == 0) free += 1;
    }
    
    if (free == 0) return false;
    return true;
}

void Restart()
{
    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++)
            grid[i][j] = 0;

    count = 0;
    turn = 1;
}

}
