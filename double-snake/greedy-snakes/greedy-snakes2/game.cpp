#include"game.h"
namespace GreedySnake {

    enum class FoodPoison {//格子的类别
        NORMAL,//正常，什么都没有
        FOOD,//食物
        POISON//毒药
    };

    Game::Game()  :threadPool(10) {
        this->map = Map();
        this->snakes.first = Snake();
        this->snakes.second = Snake();
        this->food = std::make_pair(0, 0);
        this->poison = std::make_pair(0, 0);
        this->keys.first = 'w';
        this->keys.second = 75;
        this->mode = 0;
        this->key = 'w';
        this->scores.first = this->scores.second = 0;
        this->grids.first = FoodPoison::NORMAL;
        this->grids.second = FoodPoison::NORMAL;
        //this->threadPool = ThreadPool(10);
        initGame();
    }

    Game::Game(Map&& map, Snake&& snake1, Snake&& snake2) : threadPool(10) {//移动构造函数
        this->map = map;
        this->snakes.first = snake1;
        this->snakes.second = snake2;
        this->food = std::make_pair(0, 0);
        this->poison = std::make_pair(0, 0);
        this->keys.first = 'w';
        this->keys.second = 75;
        this->mode = 0;
        this->key = 'w';
        this->scores.first = this->scores.second = 0;
        this->grids.first = FoodPoison::NORMAL;
        this->grids.second = FoodPoison::NORMAL;
        //this->threadPool = ThreadPool(10);
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
        valy = valy + valy % 2;
        this->snakes.first.initSnake(valx, valy, Color::SNAKE1);
        this->snakes.second.initSnake(valx, valy + 3, Color::SNAKE2);

        this->food = createFoodOrPoison(0);
        this->poison = createFoodOrPoison(1);
        hideCursor();
    }

    void Game::checkSnakeStatus(Snake& snake) {
        if (snake.getLength() == 0) {
            snake.setStatus(false);
        }

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
            //threadPool.enqueue(&Game::flashingScreen, this, 10);
            std::thread(&Game::flashingScreen,this,10).detach();
        }
    }

    void Game::checkSnakeStatus() {//核查两条蛇的状态
        this->checkSnakeStatus(this->snakes.first);
        this->checkSnakeStatus(this->snakes.second);

        //核查两条蛇是否互咬
        std::pair<int, int> snakeHead1 = this->snakes.first.getSnakeHead();
        std::pair<int, int> snakeHead2 = this->snakes.second.getSnakeHead();
        if (this->snakes.second.isExist(snakeHead1.first, snakeHead1.second)||
            this->snakes.first.isExist(snakeHead2.first,snakeHead2.second)) {
            this->snakes.first.setStatus(false);
            this->snakes.second.setStatus(false);
        }
    }

    void Game::checkFoodOrPoison(Snake& snake, int mode) {
        std::pair<int, int> snakeHead = snake.getSnakeHead();
        int x = snakeHead.first;
        int y = snakeHead.second;
        FoodPoison grid;
        int speed = snake.getSpeed();
        int score = 0;
        if (x == this->food.first && y == this->food.second) {
            grid = FoodPoison::FOOD;
            speed -= 10;
            score = 10;
            this->food = this->createFoodOrPoison(0);
        }
        else if (x == this->poison.first && y == this->poison.second) {
            grid = FoodPoison::POISON;
            speed += 10;
            score = -10;
            this->poison = this->createFoodOrPoison(1);
        }
        else {
            grid = FoodPoison::NORMAL;
        }
        snake.setSpeed(speed);
        if (mode == 0) {
            this->grids.first = grid;
            this->scores.first += score;
        }
        else {
            this->grids.second = grid;
            this->scores.second += score;
        }
    }

    void Game::keyDown1() {//监听第一条蛇的按下键盘，使用w(向上)、s(向下)、a(向左)、d(向右)控制
        int preKey = this->keys.first;
        
        this->keyMutex.lock();
        if (mode == 0) {
            this->keys.first = this->key;
        }
        this->keyMutex.unlock();
        
        if (preKey == 'w' && this->keys.second == 's') {
            this->keys.first = 'w';
        }
        if (preKey == 's' && this->keys.first == 'w') {
            this->keys.first = 's';
        }
        if (preKey == 'a' && this->keys.first == 'd') {
            this->keys.first = 'a';
        }
        if (preKey == 'd' && this->keys.first == 'a') {
            this->keys.first = 'd';
        }

        std::pair<int, int> snakeHead = this->snakes.first.getSnakeHead();
        switch (this->keys.first) {
        case 'w':
            snakeHead.second--;
            break;
        case 's':
            snakeHead.second++;
            break;
        case 'a':
            snakeHead.first -= 2;
            break;
        case 'd':
            snakeHead.first += 2;
            break;
        default:;
        }

        switch (static_cast<int>(this->grids.first)) {
        case static_cast<int>(FoodPoison::FOOD):
        {
            std::lock_guard<std::mutex> lock(this->keyMutex);
            gotoxy(snakeHead.first, snakeHead.second);
            color(static_cast<int>(Color::SNAKE1));
            std::cout << "■";
            this->snakes.first.increSnake(std::move(snakeHead));
            break;
        }
        case static_cast<int>(FoodPoison::POISON):
        {
            std::lock_guard<std::mutex> lock(this->keyMutex);
            auto p = this->snakes.first.getSnakeFoot();
            gotoxy(p.first, p.second);
            std::cout << ' ';
            gotoxy(snakeHead.first, snakeHead.second);
            color(static_cast<int>(Color::SNAKE1));
            std::cout << "■";
            p = this->snakes.first.decreSnake(std::move(snakeHead));
            gotoxy(p.first, p.second);
            std::cout << ' ';
            break;
        }
        case static_cast<int>(FoodPoison::NORMAL):
        {
            {
                std::lock_guard<std::mutex> lock(this->keyMutex);
                auto p = this->snakes.first.getSnakeFoot();
                gotoxy(p.first, p.second);
                std::cout << ' ';
                gotoxy(snakeHead.first, snakeHead.second);
                color(static_cast<int>(Color::SNAKE1));
                std::cout << "■";
                this->snakes.first.moveSnake(std::move(snakeHead));
                break;
            }
            
        }
        default:;
        }
    }

    void Game::keyDown2() {//监听第二条蛇的按下键盘，使用向上、下、左、右箭头控制
        int preKey = this->keys.second;    
        this->keyMutex.lock();
        if (mode == 1) {
            this->keys.second = this->key;
        }
        this->keyMutex.unlock();
        

        if (preKey == 72 && this->keys.second == 80) {
            this->keys.second = 72;
        }
        if (preKey == 80 && this->keys.second == 72) {
            this->keys.second = 80;
        }
        if (preKey == 75 && this->keys.second == 77) {
            this->keys.second = 75;
        }
        if (preKey == 77 && this->keys.second == 75) {
            this->keys.second = 77;
        }

        std::pair<int, int> snakeHead = this->snakes.second.getSnakeHead();
        switch (this->keys.second) {
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

        switch (static_cast<int>(this->grids.second)) {
        case static_cast<int>(FoodPoison::FOOD):
        {
            std::unique_lock<std::mutex> lock(this->keyMutex);
            gotoxy(snakeHead.first, snakeHead.second);
            color(static_cast<int>(Color::SNAKE2));
            std::cout << "■";
            this->snakes.second.increSnake(std::move(snakeHead));
            break;
        }
        case static_cast<int>(FoodPoison::POISON):
        {
            std::unique_lock<std::mutex> lock(this->keyMutex);
            auto p = this->snakes.second.getSnakeFoot();
            gotoxy(p.first, p.second);
            std::cout << ' ';
            gotoxy(snakeHead.first, snakeHead.second);
            color(static_cast<int>(Color::SNAKE2));
            std::cout << "■";
            p = this->snakes.second.decreSnake(std::move(snakeHead));
            gotoxy(p.first, p.second);
            std::cout << ' ';
            break;
        }
        case static_cast<int>(FoodPoison::NORMAL):
        {
            std::unique_lock<std::mutex> lock(this->keyMutex);
            auto p = this->snakes.second.getSnakeFoot();
            gotoxy(p.first, p.second);
            std::cout << ' ';
            gotoxy(snakeHead.first, snakeHead.second);
            color(static_cast<int>(Color::SNAKE2));
            std::cout << "■";
            this->snakes.second.moveSnake(std::move(snakeHead));
            break;
        }
        default:;
        }
    }
    
    void Game::snake1() {//第一条蛇的运动,监听第一条蛇的按下键盘，使用w(向上)、s(向下)、a(向左)、d(向右)控制
        for (;;) {
            this->keyDown1();
            {
                std::unique_lock<std::mutex> lock(this->keyMutex);
                this->checkSnakeStatus();
                if (!this->snakes.first.getStatus() || !this->snakes.second.getStatus()) {
                    break;
                }
            }
            this->checkFoodOrPoison(this->snakes.first,0);
            Sleep(this->snakes.first.getSpeed());
        }
    }

    void Game::snake2() {//第二条蛇运动,监听第二条蛇的按下键盘，使用向上、下、左、右箭头控制
        for (;;) {
            this->keyDown2();
            { 
                std::unique_lock<std::mutex> lock(this->keyMutex);
                this->checkSnakeStatus();
                if ((!this->snakes.first.getStatus()) || (!this->snakes.second.getStatus())) {
                    break;
                }
            }
            
            this->checkFoodOrPoison(this->snakes.second, 1);
            Sleep(this->snakes.second.getSpeed());
        }
    }

    void Game::operator()() {
        this->initGame(); 
        this->threadPool.enqueue(&Game::snake1,this);
        this->threadPool.enqueue(&Game::snake2, this);
        for (;;) {
            if (_kbhit()) {
                {
                    std::unique_lock<std::mutex> lock(this->keyMutex);
                    this->key = _getch();
                    if (this->key == 0 || this->key == 224) {
                        this->mode = 1;
                        this->key = _getch();
                    }
                    else {
                        this->mode = 0;
                    }
                } 
            }

            {
                std::unique_lock<std::mutex> lock(this->keyMutex);
                if ((!this->snakes.first.getStatus()) || (!this->snakes.second.getStatus())) {
                    break;
                }
            }
        }
        this->threadPool.~ThreadPool();
        system("color 02");
        this->printScore();
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
            if ((!this->snakes.first.isExist(x, y)) && (!this->snakes.second.isExist(x, y)) && (x % 2 == 0) &&
                ((mode == 0 && (x != poison.first || y != poison.second)) ||
                    (mode == 1 && (x != food.first || y != food.second)))) {
                break;
            }
            //std::cout << "food" << std::endl;
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

    void Game::printScore() {
        int top = this->map.getTop();
        int bottom = this->map.getBottom();
        int left = this->map.getLeft();
        int right = this->map.getRight();
        {
            std::unique_lock<std::mutex> lock(this->keyMutex);
            gotoxy((left + right) / 2, (top + bottom) / 2);
            std::cout << "score1:" << this->scores.first << "     " << "score2:" << this->scores.second;
        }
    }
}

