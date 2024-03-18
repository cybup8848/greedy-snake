#include"function.h"
namespace GreedySnake {
    //set console text color
    void color(int c) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
    }

    //jump console cursor to special position
    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    //hide the console cursor
    void hideCursor() {
        CONSOLE_CURSOR_INFO curInfo; //define the information of cursor struct
        curInfo.dwSize = 1; //if no assign value,hiding cursor is invalid
        curInfo.bVisible = FALSE; //set the console cursor is not visible
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //get the console handle
        SetConsoleCursorInfo(handle, &curInfo); //set the information of cursor
    }


}








