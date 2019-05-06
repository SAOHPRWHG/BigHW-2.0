/* 1751957 ��2�� ������ */
#include <iostream>
#include "../common/cmd_console_tools.h"
#include "90-b1.h"
using namespace std;

void Input(char * x, int lower, int upper,const char except, const char str[])
{
	while (1) {
		cout << str;
		char ch;
		while ((ch = getchar()) != '\n')
			;
		cin >> *x;
		if (*x >= 'a'&&*x <= 'z')
			*x -= 32;

		if (!cin.good()) {
			cin.clear();
			cin.ignore(1024, '\n');
		}

		else if (*x <= upper && *x >= lower|| *x==except) {
			break;
		}
		cout << "����Ƿ�������������" << endl;
	}
}

void Input(int * x, int lower, int upper, const char str[])
{
	while (1) {
		cout << str;
		cin >> *x;
		if (!cin.good()) {
			cin.clear();
			cin.ignore(1024, '\n');
		}

		else if (*x <= upper && *x >= lower)
			break;
	}
}

void Read_enter(int Y, const char str[])
{
	gotoxy(0, Y);
	cout << str;
	while (_getch() != '\r')
		;
}

void End(int Y)
{
	/***************************************************************************
		  �������ƣ�
		  ��    �ܣ��ڵ�Y�п�ʼ������End���ز˵�
		  ���������
		  �� �� ֵ��
		  ˵    ������Сд����
	***************************************************************************/
	int X;
	const char str[] = "��С�������������end����...";
	char x[3];
	char* p = x;
	char ch1[] = "end";
	char ch;

	setcolor(3, 15);
	gotoxy(0, Y);
	while (1) {
		cout << str;
		getxy(X, Y);
		cout << "       ";
		gotoxy(X, Y);
		while ((ch = getchar()) != '\n')
			;
		for (p = x; (p - x) < 3; p++) {
			cin >> *p;
		}
		if (!cin.good()) {
			cin.clear();
			cin.ignore(1024, '\n');
		}

		else {
			//ͳһ��Сд
			for (p = x; *p; p++) {
				if (*p >= 'A'&&*p <= 'Z')
					*p += 32;
			}

			if (x[0] == ch1[0] && x[1] == ch1[1] && x[2] == ch1[2])
				break;
		}
		cout << "�����������������" << endl;
		gotoxy(0, Y);
	}
	cls();
}

void Init_array(struct ball_info map[][MAX_FRAME], int row, int col)
{
	int i, j;

	//�������������
	unsigned int t;
	t = (unsigned int)time(0);
	srand(t);

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (!map[i][j].value) {
				map[i][j].value = rand() % 9 + 1;
			}
		}
	}

}

int Check(struct ball_info map[][MAX_FRAME], int row, int col)
{
/***************************************************************************
	�������ƣ�
	��    �ܣ�����ڲ�����,���ر�Ǹ���
	���������
	�� �� ֵ��
	˵    ����������������status���Ϊ1
***************************************************************************/
	int i, j, k, count = 0;
	//�м��
	for (i = 0; i < row; i++) {
		for (j = 0; j + 2 < col; j++) {
			if (map[i][j + 1].value == map[i][j].value) {
				if (map[i][j + 2].value == map[i][j + 1].value) {
					map[i][j].status = WILL_REMOVE;
					map[i][j + 1].status = WILL_REMOVE;
					map[i][j + 2].status = WILL_REMOVE;
					for (k = j + 2; map[i][k + 1].value == map[i][k].value && k + 1 < col; k++) {
						map[i][k + 1].status = WILL_REMOVE;
					}	
				}
			}
		}
	}
	//�м��
	for (j = 0; j < col; j++) {
		for (i = 0; i + 2 < row; i++) {
			if (map[i + 1][j].value == map[i][j].value) {
				if (map[i + 2][j].value == map[i + 1][j].value) {
					map[i][j].status = WILL_REMOVE;
					map[i + 1][j].status = WILL_REMOVE;
					map[i + 2][j].status = WILL_REMOVE;
					for (k = i + 2; map[k + 1][j].value == map[k][j].value && k + 1 < row; k++) {
						map[k + 1][j].status = WILL_REMOVE;
					}
				}
			}
		}
	}

	//���������
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j].status == WILL_REMOVE)
				count++;
		}
	}
	int coef = 2 * (MAX_FRAME + 1) - row - col;
	return coef*count;
}

void Remove_ball(struct ball_info map[][MAX_FRAME], int row, int col)
{
	/***************************************************************************
		  �������ƣ�
		  ��    �ܣ���WILL_REMOVE״̬�����value��Ϊ0
		  ���������
		  �� �� ֵ��
		  ˵    ����ͬʱ��״̬�û�NORMAL
	***************************************************************************/

	int i, j;
	//���������
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j].status == WILL_REMOVE) {
				map[i][j].value = 0;
				map[i][j].status = NORMAL;
			}

		}
	}
}

void Fallen(struct ball_info map[][MAX_FRAME], int row, int col,const int GUI)
{
/***************************************************************************
		�������ƣ�
		��    �ܣ����Ϸ�����������м�Ŀ�λ
		���������GUI�Ƿ����ͼ�λ���Ĭ��Ϊ0�������
		�� �� ֵ��
		˵    �������¶���ɨ�裬ͬʱ���Ϸ�������0��λ��status��ΪNEW_FILL
***************************************************************************/

	int i, j,k;
	Remove_ball(map, row, col);

	//���¶���ɨ��
	for (j = 0; j < col; j++) {
		for (i = row-1; i >= 0; i--) {
			if (map[i][j].value==0) {
				for (k = i - 1; k >= 0 && !map[k][j].value; k--)
					;//�ҵ��Ϸ���Ϊ���Ҳ��������
				if (k >= 0) {
					map[i][j].value = map[k][j].value;
					map[k][j].value = 0;
					if (GUI) {
						Fallen_GUI(k, j, i, j, map);//��ʾ����켣
					}
				}
				//������˵���Ϸ�ȫΪ0
			}

		}
	}
	//���ϴ������
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j].value == 0) {
				map[i][j].status = NEW_FILL;
			}
		}
	}

}

void Fill(struct ball_info map[][MAX_FRAME], int row, int col,const int GUI)
{
	/***************************************************************************
	  �������ƣ�
	  ��    �ܣ���䶥��Ϊ0�Ŀ�λ
	  ���������
	  �� �� ֵ��
	  ˵    �������ı�status�Ա������ʾ��֮�����Clear_status��״̬����Ϊ0
	***************************************************************************/
	//�������������
	unsigned int t;
	t = (unsigned int)time(0);
	srand(t);
	
	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j].value == 0) {
				map[i][j].value = rand() % 9 + 1;
				map[i][j].bg_color = (map[i][j].value + 4) % 16;
				if (GUI) {
					draw_the_ball(map[i][j]);
					Sleep(100);
				}				
			}
		}
	}
	setcolor(3, 15);
}

void Clear_status(struct ball_info map[][MAX_FRAME], int row, int col)
{
	/***************************************************************************
	  �������ƣ�
	  ��    �ܣ�����0��status����ΪNORMAL
	  ���������
	  �� �� ֵ��
	  ˵    ������ӡ��������base����֮��������״̬
	***************************************************************************/

	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j].status) {
				map[i][j].status = NORMAL;
			}
		}
	}
}

int scanx(int aim_row, struct ball_info map[][MAX_FRAME], int col)
{
	int j,flag=0;
	for (j = 0; j+2 < col; j++) {
		if (map[aim_row][j + 1].value == map[aim_row][j].value) {
			if (map[aim_row][j + 2].value == map[aim_row][j + 1].value) {
				flag++;
			}
		}
	}
	return flag;
}

int scany(int aim_col, struct ball_info map[][MAX_FRAME], int row)
{
	int i, flag = 0;
	for (i = 0; i+2 < row; i++) {
		if (map[i + 1][aim_col].value == map[i][aim_col].value) {
			if (map[i + 2][aim_col].value == map[i + 1][aim_col].value) {
				flag++;
			}
		}
	}
	return flag;
}

void exchange_ball(int r1, int c1, int r2, int c2, struct ball_info map[][MAX_FRAME])
{
	/***************************************************************************
		�������ƣ�
		��    �ܣ�����map[r1][c1]��map[r2][c2]��value��status
		���������
		�� �� ֵ��0
		˵    ����
	***************************************************************************/
	int tem;
	tem = map[r1][c1].value;
	map[r1][c1].value = map[r2][c2].value;
	map[r2][c2].value = tem;

	tem = map[r1][c1].status;
	map[r1][c1].status = map[r2][c2].status;
	map[r2][c2].status = tem;
}

int exchange_check(int aimr, int aimc, struct ball_info map[][MAX_FRAME], int row, int col)
{
/***************************************************************************
	�������ƣ�
	��    �ܣ�Ŀ����ֱ����ҷ����·����򽻻���������ʱ�������Ƿ�ɴﵽ����
	���������aimrĿ���У�aimcĿ����
	�� �� ֵ��flag,�ɽ���������
	˵    �������ж��Ƿ�ɽ���������ɴ��������status���ΪPRE_REMIND
***************************************************************************/
	int flag = 0;
	//�Ҳ�ɽ���
	if (aimc + 1 < col) {
		//����
		exchange_ball(aimr, aimc, aimr, aimc + 1, map);
		//ɨ����
		//ֻ��齻�����������ڵ����м��ɣ��˴�һ������
		if (scanx(aimr, map, col) + scany(aimc, map, row) + scany(aimc + 1, map, row)) {
			map[aimr][aimc].status = PRE_REMIND;
			map[aimr][aimc + 1].status = PRE_REMIND;
			flag++;
		}
		//������
		exchange_ball(aimr, aimc, aimr, aimc + 1, map);
	}

	//�·�����
	if (aimr + 1 < row) {
		//����
		exchange_ball(aimr, aimc, aimr + 1, aimc, map);
		//ɨ����
		//ֻ��齻�����������ڵ����м��ɣ��˴�һ������
		if (scanx(aimr, map, col) + scanx(aimr + 1, map, col) + scany(aimc, map, row)) {
			map[aimr][aimc].status = PRE_REMIND;
			map[aimr + 1][aimc].status = PRE_REMIND;
			flag++;
		}
		//������
		exchange_ball(aimr, aimc, aimr + 1, aimc, map);
	}
	return flag;
}

int Pre_reminder(struct ball_info map[][MAX_FRAME], int row, int col,const int GUI)
{
/***************************************************************************
	�������ƣ�
	��    �ܣ�������ʾ
	���������
	�� �� ֵ������ֵΪ0���޿���������Ϸ������
	˵    ����
***************************************************************************/
	int count = 0;
	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			count += exchange_check(i,j,map,row,col);
			if (GUI) {
				draw_the_ball(map[i][j]);
				Sleep(20);
			}
		}
	}
	setcolor(3, 15);
	return count;
}

int Judge_select(int r1,int c1,int r2,int c2,struct ball_info map[][MAX_FRAME], int row, int col)
{
/***************************************************************************
	�������ƣ�
	��    �ܣ��ж�ѡ�е��򽻻����Ƿ��ܴ������
	���������
	�� �� ֵ����ֵΪ�Ϸ���0Ϊ�Ƿ����ܴ������
	˵    �����Ƚ�������Checkһ�Σ���Ϊ����ֱ�ӽ��������ֵ�����Ƿ����򻻻���
***************************************************************************/
	int tem,judge;
	tem = map[r1][c1].value;
	map[r1][c1].value = map[r2][c2].value;
	map[r2][c2].value = tem;

	judge = Check(map, row, col);
	if (!judge) {
		tem = map[r1][c1].value;
		map[r1][c1].value = map[r2][c2].value;
		map[r2][c2].value = tem;
	}
	
	return judge;
}


