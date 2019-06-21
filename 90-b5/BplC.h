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
	int map[ROW][COL];//��άBLOCK�ṹ��ָ�룬��̬�����ά����
	double possible_map[ROW][COL];//���ʵ�ͼ
	Plane Airport[PLANE_NUMBER];//��ͼ�ɻ��⣬������ͼ�Ͽ��ܳ��ֵ�ȫ���ɻ�
public:
	CONSOLE_GRAPHICS_INFO BplC_CGI; //����һ��CGI����
	int Init_map();//��ʼ���ڲ����飬
	int Init_frame();//��ʼ������
	int Init_Airport();//��ʼ���ɻ���
	int Init_possible_map();//��ʼ�����ʵ�ͼ
	int Update_possible(const string spack, const int sel, const char row, const int col, const char head_row, const int head_col, const char tail_row, const int tail_col);
	int Predict(char & row, int & col, char & head_row, int & head_col, char & tail_row, int & tail_col);//������һ��Ԥ��
	int Select_space_by_mouse(char & row, int & col, char & head_row, int & head_col, char & tail_row, int & tail_col);//�����ѡ��
	int Explode_animation(const char row, const int col, const char head_row, const int head_col, const char tail_row, const int tail_col,const int sel, string spack, const int show_explode = 1);//��ը����
	int print_map();//��ӡ��ǰ�ڲ�����
	int print_possible_map();//��ӡ��ǰ�ڲ�����
	int print_Airport();//��ӡ�ɻ������
};