#pragma once
//Bomb the plane Client
#define _CRT_SECURE_NO_WARNINGS
#include "90-b5.h"

const int ROW = 10;
const int COL = 10;

class BplC
{
private:	
	int map[ROW][COL];//��άBLOCK�ṹ��ָ�룬��̬�����ά����
public:
	CONSOLE_GRAPHICS_INFO BplC_CGI; //����һ��CGI����
	int Init_map();//��ʼ���ڲ����飬
	int Init_frame();//��ʼ������
	int Predict();//������һ��Ԥ��
	int Select_space_by_mouse(char & row, int & col, char & head_row, int & head_col, char & tail_row, int & tail_col);//�����ѡ��
	int Explode_animation(const char row, const int col, const char head_row, const int head_col, const char tail_row, const int tail_col,const int sel, string spack);//��ը����
	int print_map();//��ӡ��ǰ�ڲ�����
};