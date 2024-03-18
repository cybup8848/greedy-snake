#pragma once
#include"map.h"
#include"snake.h"
namespace GreedySnake {
	enum class FoodPoison;//格子的类别

	class Game {
	public:
		Game();

		Game(Map&& map, Snake&& snake);//移动构造函数
		void operator()();//重载函数调用运算符

	private:
		//mode：0，生成食物
		//      1，生成毒药
		std::pair<int, int> createFoodOrPoison(int mode);//创建食物或毒药

		void initGame();//初始化游戏

		void checkSnakeStatus();//核查蛇的状态

		void checkFoodOrPoison();//吃的是食物还是毒药,食物+10，毒药-15

		void keyDown();

		void flashingScreen(int cn);//当蛇头靠近墙壁距离<=2时，闪烁屏幕num次

		void printScore();//打印分数
	private:
		Map map;
		Snake snake;
		std::pair<int, int> food;
		std::pair<int, int> poison;
		int key;
		int score;
		FoodPoison grid;
	};


}