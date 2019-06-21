/* 1751957 计2班 俞少作 */
#pragma once
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdio>
#include <conio.h>
#include <time.h>
#include <Winsock2.h>
#include <Windows.h>
#include <string>
#include<string.h>
#include "../common/cmd_net_tools.h"
#include "../common/md5.h"
#include "../common/cmd_console_tools.h"
#include "../common/cmd_gmw_tools.h"

#define UNKNOWN 0 //此处为未知 白色
#define HIT_FAIL 3 // 未击中 白色打X
#define HIT_BODY 1 //击中身体 红色背景方块
#define HIT_HEAD 2 //击中头部 红色背景三角

struct POS {
	int row;
	int col;
	int X;
	int Y;
};

class Point{
public:
	int row;
	int col;
	Point(const int r = 0, const int c = 0) 
	{
		row = r;
		col = c;
	}
	//friend class Plane;
};


const int PLANE_SIZE = 10;//飞机身体所占空间
class Plane {
private:
	Point head;
	Point tail;	
	Point plane[PLANE_SIZE];
	double possible;//每架飞机的存在可能性
public:
	friend class BplC;
	Plane(const int hdr = -1, const int hdc = -1, const int tlr = -1, const int tlc = -1)
	{
		head.row = hdr;
		head.col = hdc;
		tail.row = tlr;
		tail.col = tlc;
		int dir_row = tlr - hdr;//横行方向，若为正则<-  ,若为负则->,为零则为纵向
		int dir_col = tlc - hdc;//若为正机头向上，若为负机头向下
		if (dir_row == 0) {
			/*上*/
			if (dir_col > 0) {
				//上
				plane[0] = head;
				plane[9] = tail;
				/*机翼*/
				plane[1].row = head.row;
				plane[1].col = head.col + 1;
				plane[2].row = head.row - 1;
				plane[2].col = head.col + 1;
				plane[3].row = head.row - 2;
				plane[3].col = head.col + 1;
				plane[4].row = head.row + 1;
				plane[4].col = head.col + 1;
				plane[5].row = head.row + 2;
				plane[5].col = head.col + 1;

				plane[6].row = head.row;
				plane[6].col = head.col + 2;

				/*机尾*/
				plane[7].row = head.row - 1;
				plane[7].col = head.col + 3;
				plane[8].row = head.row + 1;
				plane[8].col = head.col + 3;
			}
			else if (dir_col < 0) {
				//下
				plane[0] = head;
				plane[9] = tail;
				/*机翼*/
				plane[1].row = head.row;
				plane[1].col = head.col + (-1);
				plane[2].row = head.row - (-1);
				plane[2].col = head.col + (-1);
				plane[3].row = head.row - (-2);
				plane[3].col = head.col + (-1);
				plane[4].row = head.row + (-1);
				plane[4].col = head.col + (-1);
				plane[5].row = head.row + (-2);
				plane[5].col = head.col + (-1);

				plane[6].row = head.row;
				plane[6].col = head.col + (-2);

				/*机尾*/
				plane[7].row = head.row - (-1);
				plane[7].col = head.col + (-3);
				plane[8].row = head.row + (-1);
				plane[8].col = head.col + (-3);
			}
		}
		else if (dir_col == 0) {
			if (dir_row > 0) {
				/*左*/
				plane[0] = head;
				plane[9] = tail;
				/*机翼*/
				plane[1].row = head.row + 1;
				plane[1].col = head.col;
				plane[2].row = head.row + 1;
				plane[2].col = head.col - 1;
				plane[3].row = head.row + 1;
				plane[3].col = head.col - 2;
				plane[4].row = head.row + 1;
				plane[4].col = head.col + 1;
				plane[5].row = head.row + 1;
				plane[5].col = head.col + 2;

				plane[6].row = head.row + 2;
				plane[6].col = head.col;

				/*机尾*/
				plane[7].row = head.row + 3;
				plane[7].col = head.col - 1;
				plane[8].row = head.row + 3;
				plane[8].col = head.col + 1;


			}
			else if(dir_row < 0) {
				//右
				plane[0] = head;
				plane[9] = tail;
				/*机翼*/
				plane[1].row = head.row + (-1);
				plane[1].col = head.col;
				plane[2].row = head.row + (-1);
				plane[2].col = head.col - (-1);
				plane[3].row = head.row + (-1);
				plane[3].col = head.col - (-2);
				plane[4].row = head.row + (-1);
				plane[4].col = head.col + (-1);
				plane[5].row = head.row + (-1);
				plane[5].col = head.col + (-2);

				plane[6].row = head.row + (-2);
				plane[6].col = head.col;

				/*机尾*/
				plane[7].row = head.row + (-3);
				plane[7].col = head.col - (-1);
				plane[8].row = head.row + (-3);
				plane[8].col = head.col + (-1);
			}
		}
	}
	void set(Point Head, Point Tail)
	{
		head.row = Head.row;
		head.col = Head.col;
		tail.row = Tail.row;
		tail.col = Tail.col;
		int dir_row = Tail.row - Head.row;//横行方向，若为正则<-  ,若为负则->,为零则为纵向
		int dir_col = Tail.col - Head.col;//若为正机头向上，若为负机头向下
		if (dir_row == 0) {
			/*上*/
			if (dir_col > 0) {
				//上
				plane[0] = head;
				plane[9] = tail;
				/*机翼*/
				plane[1].row = head.row;
				plane[1].col = head.col + 1;
				plane[2].row = head.row - 1;
				plane[2].col = head.col + 1;
				plane[3].row = head.row - 2;
				plane[3].col = head.col + 1;
				plane[4].row = head.row + 1;
				plane[4].col = head.col + 1;
				plane[5].row = head.row + 2;
				plane[5].col = head.col + 1;

				plane[6].row = head.row;
				plane[6].col = head.col + 2;

				/*机尾*/
				plane[7].row = head.row - 1;
				plane[7].col = head.col + 3;
				plane[8].row = head.row + 1;
				plane[8].col = head.col + 3;
			}
			else if (dir_col < 0) {
				//下
				plane[0] = head;
				plane[9] = tail;
				/*机翼*/
				plane[1].row = head.row;
				plane[1].col = head.col + (-1);
				plane[2].row = head.row - (-1);
				plane[2].col = head.col + (-1);
				plane[3].row = head.row - (-2);
				plane[3].col = head.col + (-1);
				plane[4].row = head.row + (-1);
				plane[4].col = head.col + (-1);
				plane[5].row = head.row + (-2);
				plane[5].col = head.col + (-1);

				plane[6].row = head.row;
				plane[6].col = head.col + (-2);

				/*机尾*/
				plane[7].row = head.row - (-1);
				plane[7].col = head.col + (-3);
				plane[8].row = head.row + (-1);
				plane[8].col = head.col + (-3);
			}
		}
		else if (dir_col == 0) {
			if (dir_row > 0) {
				/*左*/
				plane[0] = head;
				plane[9] = tail;
				/*机翼*/
				plane[1].row = head.row + 1;
				plane[1].col = head.col;
				plane[2].row = head.row + 1;
				plane[2].col = head.col - 1;
				plane[3].row = head.row + 1;
				plane[3].col = head.col - 2;
				plane[4].row = head.row + 1;
				plane[4].col = head.col + 1;
				plane[5].row = head.row + 1;
				plane[5].col = head.col + 2;

				plane[6].row = head.row + 2;
				plane[6].col = head.col;

				/*机尾*/
				plane[7].row = head.row + 3;
				plane[7].col = head.col - 1;
				plane[8].row = head.row + 3;
				plane[8].col = head.col + 1;


			}
			else if (dir_row < 0) {
				//右
				plane[0] = head;
				plane[9] = tail;
				/*机翼*/
				plane[1].row = head.row + (-1);
				plane[1].col = head.col;
				plane[2].row = head.row + (-1);
				plane[2].col = head.col - (-1);
				plane[3].row = head.row + (-1);
				plane[3].col = head.col - (-2);
				plane[4].row = head.row + (-1);
				plane[4].col = head.col + (-1);
				plane[5].row = head.row + (-1);
				plane[5].col = head.col + (-2);

				plane[6].row = head.row + (-2);
				plane[6].col = head.col;

				/*机尾*/
				plane[7].row = head.row + (-3);
				plane[7].col = head.col - (-1);
				plane[8].row = head.row + (-3);
				plane[8].col = head.col + (-1);
			}
		}

	}
	int is_plane()
	{
		int plane_len = abs(tail.row + tail.col - head.col - head.row);
		if (plane_len != 3)
			return 0;
		else
			return 1;
	}
};

const BLOCK_DISPLAY_INFO bdi_normal[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //0不显示，用空格填充即可
	{1, COLOR_RED, -1, "■"},
	{2, COLOR_RED, -1, "●"},
	{3, -1, -1, "×"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
};

/* 定义1-9的数字用何种形式显示在界面上（选中状态） */
const BLOCK_DISPLAY_INFO bdi_selected[] = {
	{BDI_VALUE_BLANK, -1, -1, "　"},  //空白
	{1, COLOR_RED,COLOR_HGREEN, "★"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
};

const BLOCK_DISPLAY_INFO bdi_exploded[] = {
	{BDI_VALUE_BLANK, -1, -1, "　"},  //空白
	{1, COLOR_RED,COLOR_HBLACK, "¤"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
};

const int CHAR_LEN = 256;