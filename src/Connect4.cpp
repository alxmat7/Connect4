#include "GameController.h"

int main()
{
    Connect4::GameController gameController(false);
    gameController.run();
    return 0;
}