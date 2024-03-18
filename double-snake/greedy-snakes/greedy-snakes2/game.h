#pragma once
#include"map.h"
#include"snake.h"
#include"ThreadPool.h"
namespace GreedySnake {
	enum class FoodPoison;//格子的类别

	class Game {
	public:
		Game();

		Game(Map&& map, Snake&& snake1, Snake&& snake2);//移动构造函数
		void operator()();//重载函数调用运算符

	private:
		//mode：0，生成食物
		//      1，生成毒药
		std::pair<int, int> createFoodOrPoison(int mode);//创建食物或毒药

		void initGame();//初始化游戏

		void checkSnakeStatus(Snake& snake);//核查一条蛇的状态
		void checkSnakeStatus();//核查两条蛇的状态

		void checkFoodOrPoison(Snake& snake, int mode);//吃的是食物还是毒药

		void snake1();//第一条蛇的运动
		void snake2();//第二条蛇运动
		//void 
		void keyDown1();//监听第一条蛇的按下键盘，使用w(向上)、s(向下)、a(向左)、d(向右)控制
		void keyDown2();//监听第二条蛇的按下键盘，使用向上、下、左、右箭头控制

		void flashingScreen(int cn);//当蛇头靠近墙壁距离<=2时，闪烁屏幕num次

		void printScore();
	private:
		ThreadPool threadPool;
		Map map;
		std::pair<Snake, Snake> snakes;
		std::pair<int, int> food;
		std::pair<int, int> poison;
		std::pair<int, int> keys;//两条蛇的按键
		int mode;//0：蛇1    1：蛇2
		int key;//按键值
		std::mutex keyMutex;

		std::pair<int, int> scores;//两条蛇的分数
		std::pair<FoodPoison, FoodPoison> grids;
	};


}