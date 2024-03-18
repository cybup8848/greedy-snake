#pragma once
#include"function.h"
namespace GreedySnake {
    class Snake {
    public:
        //构造函数
        Snake(int maxLen = 100, int speed = 200);

        void initSnake(int x, int y, Color c);

        int getSpeed() const;
        void setSpeed(int speed);

        int getLength() const;

        void swapBackForward();//后前交换数据

        void moveSnake(std::pair<int, int>&& pos);//移动蛇

        bool increSnake(std::pair<int, int>&& pos);//吃到食物，增长蛇身

        std::pair<int, int> decreSnake(std::pair<int, int>&& pos);//吃到毒药，缩短蛇身

        std::pair<int, int> getSnakeHead();//获取蛇头

        std::pair<int, int> getSnakeFoot();//获取蛇尾

        const std::pair<int, int> getSpecialNode(int index);//获取特定蛇节点

        bool isExist(int x, int y);

        bool getStatus();//获取蛇的状态
        void setStatus(bool s);//设置蛇的状态

    private:
        std::vector< std::pair<int, int> > snakes;//蛇节点(包括蛇头)的信息
        int len;//蛇的长度
        int speed;//蛇的速度
        int maxLen;
        bool status;
    };
}








