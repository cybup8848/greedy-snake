#include"map.h"
#include"function.h"
namespace GreedySnake {
    Map::Map() {
        this->top = 0;
        this->bottom = 40;
        this->left = 0;
        this->right = 80;
    }

    Map::Map(int h) {
        this->top = 0;
        this->bottom = h;
        this->left = 0;
        this->right = h * 2;
    }

    Map::Map(int w, int h) {
        this->top = 0;
        this->bottom = h;
        this->left = 0;
        this->right = w;
    }

    Map::Map(int w, int h, int x, int y) {//一般宽高比为2:1，w=2*h
        this->top = y;
        this->bottom = y + h;
        this->left = x;
        this->right = x + w;
    }

    int Map::getTop() const {
        return this->top;
    }

    int Map::getBottom() const {
        return this->bottom;
    }

    int Map::getLeft() const {
        return this->left;
    }

    int Map::getRight() const {
        return this->right;
    }

    void Map::initMap() {
        //打印上下边框
        color(static_cast<int>(Color::WALL));
        for (int i = this->left;i <= this->right;i += 2) {
            gotoxy(i, this->top);
            std::cout << "■";
            gotoxy(i, this->bottom);
            std::cout << "■";
        }

        //打印左右边框
        for (int i = this->top;i <= this->bottom;i++) {
            gotoxy(this->left, i);
            std::cout << "■";
            gotoxy(this->right, i);
            std::cout << "■";
        }
    }

}
