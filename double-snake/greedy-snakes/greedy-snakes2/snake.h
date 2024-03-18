#pragma once
#include"function.h"
namespace GreedySnake {
    class Snake {
    public:
        //���캯��
        Snake(int maxLen = 100, int speed = 200);

        void initSnake(int x, int y, Color c);

        int getSpeed() const;
        void setSpeed(int speed);

        int getLength() const;

        void swapBackForward();//��ǰ��������

        void moveSnake(std::pair<int, int>&& pos);//�ƶ���

        bool increSnake(std::pair<int, int>&& pos);//�Ե�ʳ���������

        std::pair<int, int> decreSnake(std::pair<int, int>&& pos);//�Ե���ҩ����������

        std::pair<int, int> getSnakeHead();//��ȡ��ͷ

        std::pair<int, int> getSnakeFoot();//��ȡ��β

        const std::pair<int, int> getSpecialNode(int index);//��ȡ�ض��߽ڵ�

        bool isExist(int x, int y);

        bool getStatus();//��ȡ�ߵ�״̬
        void setStatus(bool s);//�����ߵ�״̬

    private:
        std::vector< std::pair<int, int> > snakes;//�߽ڵ�(������ͷ)����Ϣ
        int len;//�ߵĳ���
        int speed;//�ߵ��ٶ�
        int maxLen;
        bool status;
    };
}








