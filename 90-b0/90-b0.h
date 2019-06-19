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
const int LEN = 100;//���ζ��г�100 
const int COLOR_NUM = 8;

/*���뺯������*/
void Input(int * x, int lower, int upper, const char str[]);
void Input(char * x, int lower, int upper, const char str[]);
int check_input(int(*a)[MAX_FRAME], int y, int x, int op);
void move_which_ball(int a[][MAX_FRAME], int row, int col, int* _sx, int* _sy);
void move_destin(int a[][MAX_FRAME], int row, int col, int* _tx, int* _ty);
void End(int Y);

/*��ʼ�����������䶯*/
void Init_array(int(*a)[MAX_FRAME], int row, int col, int n);
int* Pre_next(int(*a)[MAX_FRAME], int x, int y, int n, int flag);
void Do_next(int(*a)[MAX_FRAME], int row, int col, int *color, int n);
void static_data(int now[COLOR_NUM], float pro[COLOR_NUM], int del[COLOR_NUM], int a[][MAX_FRAME], int row, int col);

/*Ѱ·����ƶ����ж���Ϸ����*/
int bfs(int path[][MAX_FRAME*MAX_FRAME],int sy, int sx, int ty, int tx, int a[][MAX_FRAME], int row, int col);
int Check(int a[][MAX_FRAME], int row, int col);
void Move_ball(int a[][MAX_FRAME], const int sy, const int sx, const int ty, const int tx);
int Gameover(int a[][MAX_FRAME], int row, int col);

/*��������ӡ*/
void Interface_array(int x, int y, int row, int col, int(*a)[MAX_FRAME], int opt);

/* ����1-7�������ú�����ʽ��ʾ�ڽ�����
   - ע��1���������ý�������+ѡ�С���Ϣ����һ�������еķ���
			1-7��ʾ������11-17��ʾѡ��
		 2��ʵ��ʵ���е�����ֵ��Ҫ����ˣ��˴�������Ϊ��ɫ��Ĳ�ͬ״̬����ʾ
		  3���� test_magic_ball ����������һ�ַ�ʽ��ʾɫ��Ĳ�ͬ״̬�����Զ��� */
const BLOCK_DISPLAY_INFO bdi[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //0����ʾ���ÿո���伴��
	{1,  COLOR_HBLACK, -1, "��"},	//����״̬����1����Ӧ�Ĳ���
	{2,  COLOR_YELLOW, -1, "��"},
	{3,  COLOR_HGREEN, -1, "��"},
	{4,  COLOR_HCYAN, -1, "��"},
	{5,  COLOR_HRED, -1, "��"},
	{6,  COLOR_HPINK, -1, "��"},
	{7,  COLOR_HYELLOW, -1, "��"},
	{11, COLOR_HBLACK, -1, "��"},//ѡ��״̬����1����Ӧ�Ĳ���ͼ��
	{12, COLOR_YELLOW, -1, "��"},
	{13, COLOR_HGREEN, -1, "��"},
	{14, COLOR_HCYAN, -1, "��"},
	{15, COLOR_HRED, -1, "��"},
	{16, COLOR_HPINK, -1, "��"},
	{17, COLOR_HYELLOW, -1, "��"},
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
};
const int BDI_NORMAL = 0, BDI_SELECTED = 10;