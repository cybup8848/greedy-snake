#include"game.h"
using namespace GreedySnake;
int main(int argc, char* argv[]) {
    Map map(29);
    Snake snake1(50);
    Snake snake2(50);
    Game Game(std::move(map), std::move(snake1), std::move(snake2));
    Game();

    return 0;
}




