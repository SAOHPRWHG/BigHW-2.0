/* 1751957 ��2�� ������ */
#pragma once
#include <iostream>
#include<iomanip>
#include <cstdio>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <string.h>
#include "../common/cmd_gmw_tools.h"

struct ball_info {
	int row;//Y
	int col;//X
	int separator=1;//���޷ָ���
	int rc_sign=1;//���б�
	int value=0 ;//���ֵ1-9
	int front_color = 16;//ǰ��ɫ���������ɫ
	int bg_color = (value+7)%16;//����ɫ��Ĭ��Ϊ��ɫ
	int status=0;//���״̬
	/*status:
		0Ϊ��ͨ�� NORMAL
		1Ϊ�γ����Ӵ�����״̬ WILL_REMOVE
		2Ϊ�������� NEW_FILL
		3ΪԤ����ʾ��ǵ��� PRE_REMIND*/
};

#define NORMAL 0
#define WILL_REMOVE 3
#define NEW_FILL 4
#define PRE_REMIND 1
#define SELECTED 2

const int MAX_FRAME = 9;
const int MIN_FRAME = 5;

/*���뺯������*/
void Input(char * x, int lower, int upper, const char str[]);
void Input(int * x, int lower, int upper, const char str[]);
void Read_enter(int Y, const char str[]);
void End(int Y);

/*��ʼ������ͼ������䶯*/
void Init_array(struct ball_info map[][MAX_FRAME], int row, int col);
int Check(struct ball_info map[][MAX_FRAME], int row, int col);
void Fallen(struct ball_info map[][MAX_FRAME], int row, int col,const int GUI = 0);
void Fill(struct ball_info map[][MAX_FRAME], int row, int col, const int GUI = 0);
void Clear_status(struct ball_info map[][MAX_FRAME], int row, int col);
int Pre_reminder(struct ball_info map[][MAX_FRAME], int row, int col, const int GUI = 0);
int Judge_select(int r1, int c1, int r2, int c2, struct ball_info map[][MAX_FRAME], int row, int col);


/*������ʾ����*/
void Interface_array(int x, int y, struct ball_info map[][MAX_FRAME], int row, int col, const char str[], int sign_status);
void Fallen_GUI(int sy, int sx, int ty, int tx, struct ball_info map[][MAX_FRAME]);
void draw_the_ball(struct ball_info ball);

/* ����1-9�������ú�����ʽ��ʾ�ڽ�����
   - ע��1���������ý������� + ѡ�� + ��ʾ������ + ��ը����Ϣ�����ĸ������еķ���
		 2��ʵ��ʵ���е�����ֵ��Ҫ����ˣ��˴�������Ϊ��ɫ��Ĳ�ͬ״̬����ʾ
		 3���� test_color_linez ����������һ�ַ�ʽ��ʾɫ��Ĳ�ͬ״̬�����Զ��� */

		 /* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ�����״̬�� */
const BLOCK_DISPLAY_INFO bdi_normal[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //0����ʾ���ÿո���伴��
	{1, COLOR_HBLACK, -1, "��"},
	{2, COLOR_YELLOW, -1, "��"},
	{3, COLOR_HGREEN, -1, "��"},
	{4, COLOR_HCYAN, -1, "��"},
	{5, COLOR_HRED, -1, "��"},
	{6, COLOR_HPINK, -1, "��"},
	{7, COLOR_HYELLOW, -1, "��"},
	{8, COLOR_CYAN, -1, "��"},
	{9, COLOR_WHITE, -1, "��"},
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
};
/* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ�ѡ��״̬�� */
const BLOCK_DISPLAY_INFO bdi_selected[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //�հ�
	{1, COLOR_HBLACK, -1, "��"},
	{2, COLOR_YELLOW, -1, "��"},
	{3, COLOR_HGREEN, -1, "��"},
	{4, COLOR_HCYAN, -1, "��"},
	{5, COLOR_HRED, -1, "��"},
	{6, COLOR_HPINK, -1, "��"},
	{7, COLOR_HYELLOW, -1, "��"},
	{8, COLOR_CYAN, -1, "��"},
	{9, COLOR_WHITE, -1, "��"},
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
};
/* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ���������ʾ״̬�� */
const BLOCK_DISPLAY_INFO bdi_prompt[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //�հ�
	{1, COLOR_HBLACK, -1, "��"},
	{2, COLOR_YELLOW, -1, "��"},
	{3, COLOR_HGREEN, -1, "��"},
	{4, COLOR_HCYAN, -1, "��"},
	{5, COLOR_HRED, -1, "��"},
	{6, COLOR_HPINK, -1, "��"},
	{7, COLOR_HYELLOW, -1, "��"},
	{8, COLOR_CYAN, -1, "��"},
	{9, COLOR_WHITE, -1, "��"},
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
};
/* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ���ը/����״̬�� */
const BLOCK_DISPLAY_INFO bdi_exploded[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //�հ�
	{1, COLOR_HBLACK, -1, "��"},
	{2, COLOR_YELLOW, -1, "��"},
	{3, COLOR_HGREEN, -1, "��"},
	{4, COLOR_HCYAN, -1, "��"},
	{5, COLOR_HRED, -1, "��"},
	{6, COLOR_HPINK, -1, "��"},
	{7, COLOR_HYELLOW, -1, "��"},
	{8, COLOR_CYAN, -1, "��"},
	{9, COLOR_WHITE, -1, "��"},
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
};
