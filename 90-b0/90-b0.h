#pragma once
#include <iostream>
#include<iomanip>
#include <cstdio>
#include <conio.h>
#include <time.h>
#include <Windows.h>

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
