#pragma once
#include"map.h"
#include"snake.h"
#include"ThreadPool.h"
namespace GreedySnake {
	enum class FoodPoison;//���ӵ����

	class Game {
	public:
		Game();

		Game(Map&& map, Snake&& snake1, Snake&& snake2);//�ƶ����캯��
		void operator()();//���غ������������

	private:
		//mode��0������ʳ��
		//      1�����ɶ�ҩ
		std::pair<int, int> createFoodOrPoison(int mode);//����ʳ���ҩ

		void initGame();//��ʼ����Ϸ

		void checkSnakeStatus(Snake& snake);//�˲�һ���ߵ�״̬
		void checkSnakeStatus();//�˲������ߵ�״̬

		void checkFoodOrPoison(Snake& snake, int mode);//�Ե���ʳ�ﻹ�Ƕ�ҩ

		void snake1();//��һ���ߵ��˶�
		void snake2();//�ڶ������˶�
		//void 
		void keyDown1();//������һ���ߵİ��¼��̣�ʹ��w(����)��s(����)��a(����)��d(����)����
		void keyDown2();//�����ڶ����ߵİ��¼��̣�ʹ�����ϡ��¡����Ҽ�ͷ����

		void flashingScreen(int cn);//����ͷ����ǽ�ھ���<=2ʱ����˸��Ļnum��

		void printScore();
	private:
		ThreadPool threadPool;
		Map map;
		std::pair<Snake, Snake> snakes;
		std::pair<int, int> food;
		std::pair<int, int> poison;
		std::pair<int, int> keys;//�����ߵİ���
		int mode;//0����1    1����2
		int key;//����ֵ
		std::mutex keyMutex;

		std::pair<int, int> scores;//�����ߵķ���
		std::pair<FoodPoison, FoodPoison> grids;
	};


}