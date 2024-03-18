#include"game.h"
namespace GreedySnake {

    enum class FoodPoison {//格子的类别
        NORMAL,//正常，什么都没有
        FOOD,//食物
        POISON//毒药
    };

    Game::Game() {
        this->map = Map();
        this->snake = Snake();
        this->food = std::make_pair(0, 0);
        this->poison = std::make_pair(0, 0);
        this->key = 72;
        this->score = 0;
        this->grid = FoodPoison::NORMAL;
        initGame();
    }

    Game::Game(Map&& map, Snake&& snake) {//移动构造函数
        this->map = map;
        this->snake = snake;
        this->food = std::make_pair(0, 0);
        this->poison = std::make_pair(0, 0);
        this->key = 72;
        this->score = 0;
        this->grid = FoodPoison::NORMAL;
        initGame();
    }

    void Game::initGame() {
        this->map.initMap();

        int top = this->map.getTop();
        int bottom = this->map.getBottom();
        int left = this->map.getLeft();
        int right = this->map.getRight();

        int valx = (left + right) / 2;
        valx = valx + valx % 2;
        int valy = (top + bottom) / 2;
        this->snake.initSnake(valx, valy);

        this->food = createFoodOrPoison(0);
        this->poison = createFoodOrPoison(1);
        hideCursor();
    }

    void Game::checkSnakeStatus() {
        std::pair<int, int>snakeHead = snake.getSnakeHead();
        int x = snakeHead.first;
        int y = snakeHead.second;

        int top = this->map.getTop();
        int bottom = this->map.getBottom();
        int left = this->map.getLeft();
        int right = this->map.getRight();

        int len = snake.getLength();
        for (int i = 1;i < len;i++) {//蛇头咬到蛇身
            auto tmp = snake.getSpecialNode(i);
            if (x == tmp.first && y == tmp.second) {
                snake.setStatus(false);
                return;
            }
        }

        if (x <= left || x >= right || y <= top || y >= bottom) {//碰壁死亡
            snake.setStatus(false);
            return;
        }
        
        if (x - left <= 2 || right - x <= 2 || y - top <= 2 || bottom - y <= 2) {//距离墙壁太近
            std::thread(&Game::flashingScreen, this, 10).detach();
        }
    }

    void Game::checkFoodOrPoison() {
        std::pair<int, int> snakeHead = this->snake.getSnakeHead();
        int x = snakeHead.first;
        int y = snakeHead.second;
        int speed = this->snake.getSpeed();

        if (x == this->food.first && y == this->food.second) {
            this->grid = FoodPoison::FOOD;
            speed -= 10;
            this->score += 10;
            this->food = this->createFoodOrPoison(0);
        }
        else if (x == this->poison.first && y == this->poison.second) {
            grid= FoodPoison::POISON;
            speed += 10;
            this->score -= 15;
            this->poison = this->createFoodOrPoison(1);
        }
        else {
            this->grid= FoodPoison::NORMAL;
        }
        this->printScore();
        this->snake.setSpeed(speed);
    }

    void Game::keyDown() {//监听第二条蛇的按下键盘，使用向上、下、左、右箭头控制
        int preKey = this->key;
        if (_kbhit()) {
            fflush(stdin);
            this->key = _getch();
            this->key = _getch();
        }

        if (preKey == 72 && this->key == 80) {
            this->key = 72;
        }
        if (preKey == 80 && this->key == 72) {
            this->key = 80;
        }
        if (preKey == 75 && this->key == 77) {
            this->key = 75;
        }
        if (preKey == 77 && this->key == 75) {
            this->key = 77;
        }

        std::pair<int, int> snakeHead = this->snake.getSnakeHead();
        switch (this->key) {
        case 72:
            snakeHead.second--;
            break;
        case 80:
            snakeHead.second++;
            break;
        case 75:
            snakeHead.first -= 2;
            break;
        case 77:
            snakeHead.first += 2;
            break;
        default:;
        }

        switch (static_cast<int>(this->grid)) {
        case static_cast<int>(FoodPoison::FOOD):
        {
            gotoxy(snakeHead.first, snakeHead.second);
            color(static_cast<int>(Color::SNAKE));
            std::cout << "■";
            this->snake.increSnake(std::move(snakeHead));
            break;
        }
        case static_cast<int>(FoodPoison::POISON):
        {
            auto p = this->snake.getSnakeFoot();
            gotoxy(p.first, p.second);
            std::cout << ' ';
            gotoxy(snakeHead.first, snakeHead.second);
            color(static_cast<int>(Color::SNAKE));
            std::cout << "■";
            p = this->snake.decreSnake(std::move(snakeHead));
            gotoxy(p.first, p.second);
            std::cout << ' ';
            break;
        }
        case static_cast<int>(FoodPoison::NORMAL):
        {
            auto p = this->snake.getSnakeFoot();
            gotoxy(p.first, p.second);
            std::cout << ' ';
            gotoxy(snakeHead.first, snakeHead.second);
            color(static_cast<int>(Color::SNAKE));
            std::cout << "■";
            this->snake.moveSnake(std::move(snakeHead));
            
            break;
        }
        default:;
        }
    }


    void Game::operator()() {
        this->initGame();
        for (;;) {
            this->keyDown();
            this->checkSnakeStatus();
            if (!this->snake.getStatus()) {
                break;
            }
            this->checkFoodOrPoison();
            Sleep(this->snake.getSpeed());
        }
    }



    //mode：0，生成食物
    //      1，生成毒药
    std::pair<int, int> Game::createFoodOrPoison(int mode) {
        srand((unsigned int)time(nullptr));

        int left = this->map.getLeft();
        int top = this->map.getTop();
        int bottom = this->map.getBottom();
        int right = this->map.getRight();


        int x = 0;
        int y = 0;
        while (true) {
            x = rand() % (right - left - 1) + left + 1;
            y = rand() % (bottom - top - 1) + top + 1;
            if ((!this->snake.isExist(x, y)) && (x % 2 == 0) &&
                ((mode == 0 && (x != poison.first || y != poison.second)) ||
                    (mode == 1 && (x != food.first || y != food.second)))) {
                break;
            }
        }
        gotoxy(x, y);
        switch (mode) {
        case 0:
        {
            color(static_cast<int>(Color::FOOD));
            std::cout << "￥";
            break;
        }
        case 1:
        {
            color(static_cast<int>(Color::POISON));
            std::cout << "●";
            break;
        }
        default:;
        }
        return std::make_pair(x, y);
    }

    void Game::flashingScreen(int cn) {//当蛇头靠近墙壁距离<=2时，闪烁屏幕num次
        srand((unsigned int)time(nullptr));
        int num = rand() % 16;

        char str[10] = { "color " };
        
        for (int i = 0;i < cn;i++) {
            num = rand() % 16;
            str[6] = num < 10 ? num + '0' : num - 10 + 'A';
            num = rand() % 16;
            str[7] = num < 10 ? num + '0' : num - 10 + 'A';
            system(str);
            Sleep(100);
        }
        system("color 03");
    }

    void Game::printScore() {//打印分数
        int top = this->map.getTop();
        int bottom = this->map.getBottom();
        int right = this->map.getRight();
        gotoxy(right + 5, (top + bottom) / 2);
        std::cout << "score:" << this->score;
    }
}

