/* 1751957 ��2�� ������ */
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

#define UNKNOWN 0 //�˴�Ϊδ֪ ��ɫ
#define HIT_FAIL 3 // δ���� ��ɫ��X
#define HIT_BODY 1 //�������� ��ɫ��������
#define HIT_HEAD 2 //����ͷ�� ��ɫ��������

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


const int PLANE_SIZE = 10;//�ɻ�������ռ�ռ�
class Plane {
private:
	Point head;
	Point tail;	
	Point plane[PLANE_SIZE];
	double possible;//ÿ�ܷɻ��Ĵ��ڿ�����
public:
	friend class BplC;
	Plane(const int hdr = -1, const int hdc = -1, const int tlr = -1, const int tlc = -1)
	{
		head.row = hdr;
		head.col = hdc;
		tail.row = tlr;
		tail.col = tlc;
		int dir_row = tlr - hdr;//���з�����Ϊ����<-  ,��Ϊ����->,Ϊ����Ϊ����
		int dir_col = tlc - hdc;//��Ϊ����ͷ���ϣ���Ϊ����ͷ����
		if (dir_row == 0) {
			/*��*/
			if (dir_col > 0) {
				//��
				plane[0] = head;
				plane[9] = tail;
				/*����*/
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

				/*��β*/
				plane[7].row = head.row - 1;
				plane[7].col = head.col + 3;
				plane[8].row = head.row + 1;
				plane[8].col = head.col + 3;
			}
			else if (dir_col < 0) {
				//��
				plane[0] = head;
				plane[9] = tail;
				/*����*/
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

				/*��β*/
				plane[7].row = head.row - (-1);
				plane[7].col = head.col + (-3);
				plane[8].row = head.row + (-1);
				plane[8].col = head.col + (-3);
			}
		}
		else if (dir_col == 0) {
			if (dir_row > 0) {
				/*��*/
				plane[0] = head;
				plane[9] = tail;
				/*����*/
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

				/*��β*/
				plane[7].row = head.row + 3;
				plane[7].col = head.col - 1;
				plane[8].row = head.row + 3;
				plane[8].col = head.col + 1;


			}
			else if(dir_row < 0) {
				//��
				plane[0] = head;
				plane[9] = tail;
				/*����*/
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

				/*��β*/
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
		int dir_row = Tail.row - Head.row;//���з�����Ϊ����<-  ,��Ϊ����->,Ϊ����Ϊ����
		int dir_col = Tail.col - Head.col;//��Ϊ����ͷ���ϣ���Ϊ����ͷ����
		if (dir_row == 0) {
			/*��*/
			if (dir_col > 0) {
				//��
				plane[0] = head;
				plane[9] = tail;
				/*����*/
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

				/*��β*/
				plane[7].row = head.row - 1;
				plane[7].col = head.col + 3;
				plane[8].row = head.row + 1;
				plane[8].col = head.col + 3;
			}
			else if (dir_col < 0) {
				//��
				plane[0] = head;
				plane[9] = tail;
				/*����*/
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

				/*��β*/
				plane[7].row = head.row - (-1);
				plane[7].col = head.col + (-3);
				plane[8].row = head.row + (-1);
				plane[8].col = head.col + (-3);
			}
		}
		else if (dir_col == 0) {
			if (dir_row > 0) {
				/*��*/
				plane[0] = head;
				plane[9] = tail;
				/*����*/
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

				/*��β*/
				plane[7].row = head.row + 3;
				plane[7].col = head.col - 1;
				plane[8].row = head.row + 3;
				plane[8].col = head.col + 1;


			}
			else if (dir_row < 0) {
				//��
				plane[0] = head;
				plane[9] = tail;
				/*����*/
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

				/*��β*/
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
	{BDI_VALUE_BLANK, -1, -1, "  "},  //0����ʾ���ÿո���伴��
	{1, COLOR_RED, -1, "��"},
	{2, COLOR_RED, -1, "��"},
	{3, -1, -1, "��"},
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
};

/* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ�ѡ��״̬�� */
const BLOCK_DISPLAY_INFO bdi_selected[] = {
	{BDI_VALUE_BLANK, -1, -1, "��"},  //�հ�
	{1, COLOR_RED,COLOR_HGREEN, "��"},
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
};

const BLOCK_DISPLAY_INFO bdi_exploded[] = {
	{BDI_VALUE_BLANK, -1, -1, "��"},  //�հ�
	{1, COLOR_RED,COLOR_HBLACK, "��"},
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
};

const int CHAR_LEN = 256;