# greedy-snake
# author:cyb
# email:chengyb2022@shanghaitech.edu.cn
# platform:windows
# IDE:vs2022

FrameWork:Game class、Map class、Snake class、other common functions

single snake
	1. it has only one snake,and it is controlled by 'w'、's'、'a'、'd';
	2. when the snake eat food,it will add 10 score and speedup 10 and it will grow 1;	       
	   when the snake eat poison,it will lose 15 score and slow down 10 and it wil reduce 1;
	3. when the distance of the snake from the wall is less than or equal to 2,we will start a thread to flash the 
	   console 10 times to issue alert messages
double snake
	1. it has two snake.The first snake is controlled by 'w'、's'、'a'、'd' and the second snake is controlled by 
	   up,down,left,right arrows
	2. we design a threadpool and the maximum number of thread is default 10
	3. we use a mutex and condition variable to avoid data race
	4. every snake has a single thread



