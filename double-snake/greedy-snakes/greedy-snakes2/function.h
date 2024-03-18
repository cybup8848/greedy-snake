#pragma once
#include"lib.h"
namespace GreedySnake {
    enum class Color {
        WALL = 2,
        FOOD = 3,
        POISON = 4,
        SNAKE1 = 5,
        SNAKE2 = 6
    };

    void color(int c);//set console text color

    void gotoxy(int x, int y);//jump console cursor to special position

    void hideCursor();//hide the console cursor
}


