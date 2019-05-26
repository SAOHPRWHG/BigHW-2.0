#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "90-b4.h"
class G2048
{
private:
	CONSOLE_GRAPHICS_INFO G2048_CGI; //声明一个CGI变量
	BLOCK **map;//二维BLOCK结构体指针，动态申请二维数组
	int row;
	int col;
	int score;//实际分数
	int dst_score;//目标分数
	int high_score;//最高分
	int speed;
	bool separator;
	/*private functions*/
	int up_check(int c);//c:目标列
	int down_check(int c);
	int left_check(int r);
	int right_check(int r);
public:
	G2048(const int Row = 8 ,const int Col = 8,const int Dst_score = 8192,const int Speed = 0,const bool Sep = true);
	~G2048();
	int set(const int Row = 8, const int Col = 8, const int Dst_score = 8192, const int Speed = 0, const bool Sep = true);
	int Produce_new_block(const int num = 1, const bool show = true);
	int Init_map();//初始化内部数组，
	int Init_frame();//初始化界面
	int Game_start();//核心函数，所有游戏过程全部置于其中
	int Check_remove(const int direction);
	int Move_animation(const int direction);
	int Clear_status();
	int Gameover();
	int print_map();
};

