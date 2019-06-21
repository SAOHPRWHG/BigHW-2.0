#pragma once
//Bomb the plane Client
#define _CRT_SECURE_NO_WARNINGS
#include "90-b5.h"

const int ROW = 10;
const int COL = 10;

const int PLANE_NUMBER = 42 * 4;
class BplC
{
private:	
	int map[ROW][COL];//二维BLOCK结构体指针，动态申请二维数组
	double possible_map[ROW][COL];//概率地图
	Plane Airport[PLANE_NUMBER];//地图飞机库，包含地图上可能出现的全部飞机
public:
	CONSOLE_GRAPHICS_INFO BplC_CGI; //声明一个CGI变量
	int Init_map();//初始化内部数组，
	int Init_frame();//初始化界面
	int Init_Airport();//初始化飞机库
	int Init_possible_map();//初始化概率地图
	int Update_possible(const string spack, const int sel, const char row, const int col, const char head_row, const int head_col, const char tail_row, const int tail_col);
	int Predict(char & row, int & col, char & head_row, int & head_col, char & tail_row, int & tail_col);//给出下一步预测
	int Select_space_by_mouse(char & row, int & col, char & head_row, int & head_col, char & tail_row, int & tail_col);//用鼠标选择
	int Explode_animation(const char row, const int col, const char head_row, const int head_col, const char tail_row, const int tail_col,const int sel, string spack, const int show_explode = 1);//爆炸动画
	int print_map();//打印当前内部数组
	int print_possible_map();//打印当前内部数组
	int print_Airport();//打印飞机库概率
};