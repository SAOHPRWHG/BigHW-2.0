#pragma once
#include <iostream>
#include<iomanip>
#include <cstdio>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include "../common/cmd_gmw_tools.h"

const int MAX_FRAME = 9 + 2;
const int MIN_FRAME = 7;
const int INF = 100000;
const int LEN = 100;//环形队列长100 
const int COLOR_NUM = 8;

/*输入函数部分*/
void Input(int * x, int lower, int upper, const char str[]);
void Input(char * x, int lower, int upper, const char str[]);
int check_input(int(*a)[MAX_FRAME], int y, int x, int op);
void move_which_ball(int a[][MAX_FRAME], int row, int col, int* _sx, int* _sy);
void move_destin(int a[][MAX_FRAME], int row, int col, int* _tx, int* _ty);
void End(int Y);

/*初始化数组和数组变动*/
void Init_array(int(*a)[MAX_FRAME], int row, int col, int n);
int* Pre_next(int(*a)[MAX_FRAME], int x, int y, int n, int flag);
void Do_next(int(*a)[MAX_FRAME], int row, int col, int *color, int n);
void static_data(int now[COLOR_NUM], float pro[COLOR_NUM], int del[COLOR_NUM], int a[][MAX_FRAME], int row, int col);

/*寻路检查移动和判断游戏结束*/
int bfs(int path[][MAX_FRAME*MAX_FRAME],int sy, int sx, int ty, int tx, int a[][MAX_FRAME], int row, int col);
int Check(int a[][MAX_FRAME], int row, int col);
void Move_ball(int a[][MAX_FRAME], const int sy, const int sx, const int ty, const int tx);
int Gameover(int a[][MAX_FRAME], int row, int col);

/*数组界面打印*/
void Interface_array(int x, int y, int row, int col, int(*a)[MAX_FRAME], int opt);

/* 定义1-7的数字用何种形式显示在界面上
   - 注：1、本例采用将“正常+选中”信息放在一个数组中的方法
			1-7表示正常，11-17表示选中
		 2、实际实现中的数组值不要求如此，此处仅仅是为了色块的不同状态的显示
		  3、在 test_magic_ball 中用了另外一种方式表示色块的不同状态，可以对照 */
const BLOCK_DISPLAY_INFO bdi[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //0不显示，用空格填充即可
	{1,  COLOR_HBLACK, -1, "〇"},	//正常状态数字1所对应的彩球
	{2,  COLOR_YELLOW, -1, "〇"},
	{3,  COLOR_HGREEN, -1, "〇"},
	{4,  COLOR_HCYAN, -1, "〇"},
	{5,  COLOR_HRED, -1, "〇"},
	{6,  COLOR_HPINK, -1, "〇"},
	{7,  COLOR_HYELLOW, -1, "〇"},
	{11, COLOR_HBLACK, -1, "◎"},//选中状态数字1所对应的彩球图案
	{12, COLOR_YELLOW, -1, "◎"},
	{13, COLOR_HGREEN, -1, "◎"},
	{14, COLOR_HCYAN, -1, "◎"},
	{15, COLOR_HRED, -1, "◎"},
	{16, COLOR_HPINK, -1, "◎"},
	{17, COLOR_HYELLOW, -1, "◎"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
};
const int BDI_NORMAL = 0, BDI_SELECTED = 10;