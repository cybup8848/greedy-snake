#include"snake.h"
namespace GreedySnake {

    Snake::Snake(int maxLen, int speed) { //���캯��
        this->maxLen = maxLen;
        this->speed = speed;
        this->len = 0;
        this->snakes.resize(maxLen);
        this->status = true;
    }

    void Snake::initSnake(int x, int y) {
        this->len = 3;
        //��ӡ��
        for (int i = 0;i < len;i++) {
            gotoxy(x, y);
            std::cout << "��";
            this->snakes[i]=std::make_pair(x, y);
            x += 2;
        }
    }

    int Snake::getSpeed() const {
        return this->speed;
    }

    void Snake::setSpeed(int speed) {
        this->speed = speed;
    }

    int Snake::getLength() const {
        return this->len;
    }

    void Snake::swapBackForward() {//��ǰ��������
        for (int i = this->len - 1;i > 0;i--) {
            this->snakes[i] = std::move(this->snakes[i - 1]);
        }
    }
     
    
    void Snake::moveSnake(std::pair<int,int> &&pos) {//�ƶ���
        this->swapBackForward();
        this->snakes[0] = pos;
    }

    bool Snake::increSnake(std::pair<int, int>&& pos) {//�Ե�ʳ���������
        if (this->len < this->maxLen) {
            this->len++;
            this->moveSnake(std::forward<std::pair<int, int>&&>(pos));
            return true;
        }
        return false;
    }

    std::pair<int, int> Snake::decreSnake(std::pair<int, int>&& pos) {//�Ե���ҩ����������
        if (this->len > 0) {
            this->moveSnake(std::forward<std::pair<int,int>>(pos));
            this->len--;
            auto it = this->snakes[this->len];
            return it;
        }
        return std::make_pair(-1, -1);
    }

    std::pair<int, int> Snake::getSnakeHead() {//��ȡ��ͷ
        if (this->len>0) {
            return this->snakes.front();
        }
        return std::make_pair(-1, -1);
    }

    std::pair<int, int> Snake::getSnakeFoot() {//��ȡ��β
        if (this->len>0) {
            return this->snakes[this->len - 1];
        }
        return std::make_pair(-1, -1);
    }

    const std::pair<int, int> Snake::getSpecialNode(int index) {//��ȡ�ض��߽ڵ�
        if (index < 0 || index >= this->len) {
            return std::make_pair(-1, -1);
            //return 
        }
        return this->snakes[index];
    }

    bool Snake::isExist(int x, int y) {
        //auto it = this->snakes.begin();
        //return std::find(it, it+this->len, std::make_pair(x,y)) != this->snakes.end();
        
        std::pair<int, int> p = std::make_pair(x,y);
        for (int i = 0;i < this->len;i++) {
            if (p == this->snakes[i]) {
                return true;
            }
        }
        return false;
    }

    bool Snake::getStatus() {//��ȡ�ߵ�״̬
        return this->status;
    }

    void Snake::setStatus(bool s) {//�����ߵ�״̬
        this->status = s;
    }
}