/* 1751957 ��2�� ������ */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include "../common/cmd_console_tools.h"
#include "90-b1.h"
using namespace std;


void Interface_array(int x, int y, struct ball_info map[][MAX_FRAME], int row, int col,const char str[],int sign_status)
{
	/***************************************************************************
	  �������ƣ�
	  ��    �ܣ���ʾ����״̬�µĽ���
	  ���������x,yΪ��ӡλ�ã�row��colΪ����,optΪѡ��ͷ����
	  �� �� ֵ��
	  ˵    ����sign_statusΪ���θ�����ǵ����״̬
	***************************************************************************/
	int X, Y;//���ڻ�ȡ�ĸ���λ������
	int i, j;
	gotoxy(x, y);
	
	cout << str<<endl;
	/*if (opt == 1)
		cout << "��ʼ���������ͬɫ��ʶ����" << endl;
	else if (opt == -1)
		cout << "�ƶ������飺" << endl;
	else if (opt == 2)
		cout << "�����0�������(��ͬɫ��ʶ)��" << endl;
	else if(opt==3)
	else
		cout << "��ʼ���飺" << endl;*/
	//��ʼ����
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
	Interface_array(0, 4, map, row, col, "��ʼ���飺", WILL_REMOVE);
	getxy(X, Y);
	Read_enter(Y + 1, "���س�������Ѱ�ҳ�ʼ��������Ĳ���...");
	if (Check(map, row, col))
		Interface_array(X, Y + 2, map, row, col, "��ʼ���������ͬɫ��ʶ����", WILL_REMOVE);
	else
		cout << endl << "��ʼ���޿�������";
	getxy(X, Y);
	End(Y + 1);
}

void GameB(struct ball_info map[][MAX_FRAME], int row, int col)
{
	int X, Y, opt = 0;
	Init_array(map, row, col);
	Interface_array(0, 4, map, row, col, "��ʼ���飺", WILL_REMOVE);
	getxy(X, Y);
	Read_enter(Y + 1, "���س�������Ѱ�ҳ�ʼ��������Ĳ���...");
	while (Check(map, row, col)) {
		Interface_array(X, Y + 2, map, row, col, "��ʼ���������ͬɫ��ʶ����", WILL_REMOVE);
		getxy(X, Y);
		Read_enter(Y + 1, "���س����������������0����...");
		Fallen(map, row, col);
		Interface_array(X, Y + 2, map, row, col, "�����0�������(��ͬɫ��ʶ)��", NEW_FILL);
		getxy(X, Y);
		Read_enter(Y + 1, "���س���������ֵ���...");
		Fill(map, row, col);
		Interface_array(X, Y + 2, map, row, col, "�����0�������(��ͬɫ��ʶ)��", NEW_FILL);
		Clear_status(map, row, col);
	}		
	cout << endl << "��ʼ���޿�������";
	getxy(X, Y);
	End(Y + 1);
}

void GameC(struct ball_info map[][MAX_FRAME], int row, int col)
{
	int X, Y, opt = 0;
	Init_array(map, row, col);
	Interface_array(0, 4, map, row, col, "��ʼ���飺", WILL_REMOVE);
	getxy(X, Y);
	Read_enter(Y + 1, "���س�������Ѱ�ҳ�ʼ��������Ĳ���...");
	//ѭ������ʼ������
	while (Check(map, row, col)) {
		Interface_array(X, Y + 2, map, row, col, "��ʼ���������ͬɫ��ʶ����", WILL_REMOVE);
		getxy(X, Y);
		Read_enter(Y + 1, "���س����������������0����...");
		Fallen(map, row, col);
		Interface_array(X, Y + 2, map, row, col, "�����0�������(��ͬɫ��ʶ)��", NEW_FILL);
		getxy(X, Y);
		Read_enter(Y + 1, "���س���������ֵ���...");
		Fill(map, row, col);
		Interface_array(X, Y + 2, map, row, col, "�����0�������(��ͬɫ��ʶ)��", NEW_FILL);
		Clear_status(map, row, col);
	}
	cout << endl << "��ʼ���޿�������"<<endl;

	//����Ԥ����ʾ
	if (Pre_reminder(map, row, col)) {
		getxy(X, Y);
		Interface_array(X, Y + 2, map, row, col, "��ѡ���������ʾ����ͬɫ��ʶ����", PRE_REMIND);
	}
	else
		cout << "����������ʾ����Ϸ����" << endl;
	getxy(X, Y);
	End(Y + 1);
}

void GameI(struct ball_info map[][MAX_FRAME], int row, int col)
{
	int X, Y;
	int no;
	cout << "������ѧ��(���� 1859999 ��Ӧ��� test\\1859999.dat �ļ�)" << endl;
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
	Interface_array(X, Y + 1, map, row, col, "��ʼ���飺", WILL_REMOVE);
	while (Check(map, row, col)) {
		Fallen(map, row, col);
		Fill(map, row, col);
		Clear_status(map, row, col);
	}
	
	if (Pre_reminder(map, row, col)) {
		getxy(X, Y);
		Interface_array(X, Y + 2, map, row, col, "��ѡ���������ʾ����ͬɫ��ʶ����", PRE_REMIND);
	}

	fclose(infile);//�ر�xxx.dat
	
	//��xxx.ans
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
	Interface_array(X, Y + 2, map, row, col, "���ļ����ݣ�", PRE_REMIND);
	fclose(infile);//�ر�xxx.ans
	
	getxy(X, Y);
	End(Y+1);
}

