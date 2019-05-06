/* 1751957 计2班 俞少作 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include "../common/cmd_console_tools.h"
#include "90-b1.h"
using namespace std;


void Interface_array(int x, int y, struct ball_info map[][MAX_FRAME], int row, int col,const char str[],int sign_status)
{
	/***************************************************************************
	  函数名称：
	  功    能：显示数组状态下的界面
	  输入参数：x,y为打印位置，row，col为行列,opt为选择头文字
	  返 回 值：
	  说    明：sign_status为本次高亮标记的球的状态
	***************************************************************************/
	int X, Y;//用于获取的辅助位置坐标
	int i, j;
	gotoxy(x, y);
	
	cout << str<<endl;
	/*if (opt == 1)
		cout << "初始可消除项（不同色标识）：" << endl;
	else if (opt == -1)
		cout << "移动后数组：" << endl;
	else if (opt == 2)
		cout << "下落除0后的数组(不同色标识)：" << endl;
	else if(opt==3)
	else
		cout << "初始数组：" << endl;*/
	//开始画表·
	cout << "  |";
	for (i = 0; i < col; i++) {
		cout << right << setw(3) << i;
	}
	getxy(X, Y);
	cout << endl << "--+";
	for (i = 3; i <= X; i++)
		cout << "-";
	cout << endl;
	for (i = 0; i < row; i++) {
		cout << left << setw(2) << char('A' + i) << "|";
		for (j = 0; j < col; j++) {		
			if (map[i][j].status == sign_status) {
				cout << "  ";
				getxy(X, Y);
				showch(X, Y, char(map[i][j].value + '0'), COLOR_HYELLOW, COLOR_BLACK, 1);
				setcolor(3,15);
			}
			else
				cout << right << setw(3) << map[i][j].value;
		}
		cout << endl;
	}


}

void GameA(struct ball_info map[][MAX_FRAME], int row, int col)
{
	int X, Y, opt = 0;
	Init_array(map, row, col);
	Interface_array(0, 4, map, row, col, "初始数组：", WILL_REMOVE);
	getxy(X, Y);
	Read_enter(Y + 1, "按回车键进行寻找初始可消除项的操作...");
	if (Check(map, row, col))
		Interface_array(X, Y + 2, map, row, col, "初始可消除项（不同色标识）：", WILL_REMOVE);
	else
		cout << endl << "初始已无可消除项";
	getxy(X, Y);
	End(Y + 1);
}

void GameB(struct ball_info map[][MAX_FRAME], int row, int col)
{
	int X, Y, opt = 0;
	Init_array(map, row, col);
	Interface_array(0, 4, map, row, col, "初始数组：", WILL_REMOVE);
	getxy(X, Y);
	Read_enter(Y + 1, "按回车键进行寻找初始可消除项的操作...");
	while (Check(map, row, col)) {
		Interface_array(X, Y + 2, map, row, col, "初始可消除项（不同色标识）：", WILL_REMOVE);
		getxy(X, Y);
		Read_enter(Y + 1, "按回车键进行数组下落除0操作...");
		Fallen(map, row, col);
		Interface_array(X, Y + 2, map, row, col, "下落除0后的数组(不同色标识)：", NEW_FILL);
		getxy(X, Y);
		Read_enter(Y + 1, "按回车键进行新值填充...");
		Fill(map, row, col);
		Interface_array(X, Y + 2, map, row, col, "下落除0后的数组(不同色标识)：", NEW_FILL);
		Clear_status(map, row, col);
	}		
	cout << endl << "初始已无可消除项";
	getxy(X, Y);
	End(Y + 1);
}

void GameC(struct ball_info map[][MAX_FRAME], int row, int col)
{
	int X, Y, opt = 0;
	Init_array(map, row, col);
	Interface_array(0, 4, map, row, col, "初始数组：", WILL_REMOVE);
	getxy(X, Y);
	Read_enter(Y + 1, "按回车键进行寻找初始可消除项的操作...");
	//循环检查初始消除项
	while (Check(map, row, col)) {
		Interface_array(X, Y + 2, map, row, col, "初始可消除项（不同色标识）：", WILL_REMOVE);
		getxy(X, Y);
		Read_enter(Y + 1, "按回车键进行数组下落除0操作...");
		Fallen(map, row, col);
		Interface_array(X, Y + 2, map, row, col, "下落除0后的数组(不同色标识)：", NEW_FILL);
		getxy(X, Y);
		Read_enter(Y + 1, "按回车键进行新值填充...");
		Fill(map, row, col);
		Interface_array(X, Y + 2, map, row, col, "下落除0后的数组(不同色标识)：", NEW_FILL);
		Clear_status(map, row, col);
	}
	cout << endl << "初始已无可消除项"<<endl;

	//给出预判提示
	if (Pre_reminder(map, row, col)) {
		getxy(X, Y);
		Interface_array(X, Y + 2, map, row, col, "可选择的消除提示（不同色标识）：", PRE_REMIND);
	}
	else
		cout << "已无消除提示，游戏结束" << endl;
	getxy(X, Y);
	End(Y + 1);
}

void GameI(struct ball_info map[][MAX_FRAME], int row, int col)
{
	int X, Y;
	int no;
	cout << "请输入学号(输入 1859999 对应会打开 test\\1859999.dat 文件)" << endl;
	char filename[20];	
	FILE *infile;

	cin >> no;
	sprintf(filename, ".\\test\\%d.dat", no);
	infile = fopen(filename, "r");
	if (infile == NULL)
	{
		printf("%s cannot be opened", filename);
		End(21);
	}

	fscanf(infile, "%d %d", &row, &col);
	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			fscanf(infile, "%d", &map[i][j].value);
		}
	}

	getxy(X, Y);
	Interface_array(X, Y + 1, map, row, col, "初始数组：", WILL_REMOVE);
	while (Check(map, row, col)) {
		Fallen(map, row, col);
		Fill(map, row, col);
		Clear_status(map, row, col);
	}
	
	if (Pre_reminder(map, row, col)) {
		getxy(X, Y);
		Interface_array(X, Y + 2, map, row, col, "可选择的消除提示（不同色标识）：", PRE_REMIND);
	}

	fclose(infile);//关闭xxx.dat
	
	//打开xxx.ans
	sprintf(filename, ".\\test\\%d.ans", no);
	infile = fopen(filename, "r");
	
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			fscanf(infile, "%d", &map[i][j].value);
			if (map[i][j].value > 90) {
				map[i][j].value -= 90;
				map[i][j].status = PRE_REMIND;
			}
		}
	}

	getxy(X, Y);
	Interface_array(X, Y + 2, map, row, col, "答案文件内容：", PRE_REMIND);
	fclose(infile);//关闭xxx.ans
	
	getxy(X, Y);
	End(Y+1);
}

