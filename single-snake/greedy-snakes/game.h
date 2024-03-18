#pragma once
#include"map.h"
#include"snake.h"
namespace GreedySnake {
	enum class FoodPoison;//���ӵ����

	class Game {
	public:
		Game();

		Game(Map&& map, Snake&& snake);//�ƶ����캯��
		void operator()();//���غ������������

	private:
		//mode��0������ʳ��
		//      1�����ɶ�ҩ
		std::pair<int, int> createFoodOrPoison(int mode);//����ʳ���ҩ

		void initGame();//��ʼ����Ϸ

		void checkSnakeStatus();//�˲��ߵ�״̬

		void checkFoodOrPoison();//�Ե���ʳ�ﻹ�Ƕ�ҩ,ʳ��+10����ҩ-15

		void keyDown();

		void flashingScreen(int cn);//����ͷ����ǽ�ھ���<=2ʱ����˸��Ļnum��

		void printScore();//��ӡ����
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