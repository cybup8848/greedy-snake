#include"game.h"
using namespace GreedySnake;
int main(int argc, char* argv[]) {
    Map map(29);
    Snake snake(20);
    Game Game(std::move(map),std::move(snake));
    Game();

    return 0;
}




