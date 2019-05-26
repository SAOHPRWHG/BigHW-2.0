/* 1751957 ��2�� ������ */
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

#define ZERO 0 //��ɫ��Ϊ��
#define NORMAL 1 //��ɫ��Ϊ��ͨɫ��
#define WAIT_COMBINE 2  //��ɫ��Ϊ���ϲ�ɫ�飬�ƶ���ɺ�ȴ��ϲ�
#define COMBINE 3  //��ɫ��Ϊ�ϲ�ɫ�飬�ƶ���ɺ������ϲ�

struct POS {
	int row;
	int col;
	int X;
	int Y;
};

struct BLOCK {
	int value;//ֵ
	int drow;//�ƶ����Ŀ��λ����
	int dcol;//�ƶ����Ŀ��λ����
	int status;//״̬���¶��壺
};

const BLOCK_DISPLAY_INFO bdi_normal[] = {
	{BDI_VALUE_BLANK, -1, -1, NULL},  //0����ʾ���ÿո���伴��
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
	{131072, COLOR_HPINK, COLOR_BLACK, NULL},  //������ģ������Լ�����
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
};

const int MIN_FRAME = 4;
const int MAX_FRAME = 8;
const int CHAR_LEN = 256;
