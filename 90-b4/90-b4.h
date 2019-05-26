/* 1751957 计2班 俞少作 */
#pragma once
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdio>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include "../common/cmd_console_tools.h"
#include "../common/cmd_gmw_tools.h"

#define ZERO 0 //此色块为空
#define NORMAL 1 //此色块为普通色块
#define WAIT_COMBINE 2  //此色块为待合并色块，移动完成后等待合并
#define COMBINE 3  //此色块为合并色块，移动完成后立即合并

struct POS {
	int row;
	int col;
	int X;
	int Y;
};

struct BLOCK {
	int value;//值
	int drow;//移动后的目标位置行
	int dcol;//移动后的目标位置列
	int status;//状态如下定义：
};

const BLOCK_DISPLAY_INFO bdi_normal[] = {
	{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
	{2, COLOR_HYELLOW, COLOR_BLACK, NULL},
	{4, COLOR_HRED, COLOR_BLACK, NULL},
	{8, COLOR_GREEN, COLOR_BLACK, NULL},
	{16, COLOR_RED, COLOR_BLACK, NULL},
	{32, COLOR_HGREEN, COLOR_BLACK, NULL},
	{64, COLOR_HYELLOW, COLOR_BLACK, NULL},
	{128, COLOR_HRED, COLOR_BLACK, NULL},
	{256, COLOR_HWHITE, COLOR_BLACK, NULL},
	{512, COLOR_HBLACK, COLOR_BLACK, NULL},
	{1024, COLOR_HPINK, COLOR_BLACK, NULL},
	{2048, COLOR_WHITE, COLOR_BLACK, NULL},
	{4096, COLOR_YELLOW, COLOR_BLACK, NULL},
	{8192, COLOR_PINK, COLOR_BLACK, NULL},
	{16384, COLOR_HBLUE, COLOR_BLACK, NULL},
	{32768, COLOR_HCYAN, COLOR_BLACK, NULL},
	{65536, COLOR_HGREEN, COLOR_BLACK, NULL},
	{131072, COLOR_HPINK, COLOR_BLACK, NULL},  //如果开心，还可以继续加
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
};

const int MIN_FRAME = 4;
const int MAX_FRAME = 8;
const int CHAR_LEN = 256;
