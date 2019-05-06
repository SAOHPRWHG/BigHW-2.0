#include "../common/cmd_hdc_tools.h"
#include "90-b0.h"
using namespace std;

void Interface_array(int x, int y, int row, int col, int(*a)[MAX_FRAME], int opt)
{
	/***************************************************************************
	  �������ƣ�
	  ��    �ܣ���ʾ����״̬�µĽ���
	  ���������x,yΪ��ӡλ�ã�row��colΪ����,optΪѡ��ͷ����
	  �� �� ֵ��
	  ˵    �����������
	***************************************************************************/
	int X, Y;//���ڻ�ȡ�ĸ���λ������
	int i, j;
	gotoxy(x, y);
	if (opt > 0)
		cout << "��ǰ���飺" << endl;
	else if (opt == -1)
		cout << "�ƶ������飺" << endl;
	else
		cout << "��ʼ���飺" << endl;
	//��ʼ����
	cout << "  |";
	for (i = 1; i <= col; i++) {
		cout << right << setw(3) << i;
	}
	getxy(X, Y);
	cout << endl << "--+";
	for (i = 3; i <= X; i++)
		cout << "-";
	cout << endl;
	for (i = 1; i <= row; i++) {
		cout << left << setw(2) << char('A' - 1 + i) << "|";
		for (j = 1; j <= col; j++) {
			if (a[i][j]) {
				cout << "  ";
				getxy(X, Y);
				showch(X, Y, char(a[i][j] + '0'), COLOR_HYELLOW, COLOR_BLACK, 1);
				setcolor();
			}
			else
				cout << right << setw(3) << a[i][j];
		}
		cout << endl;
	}


}

void show_array_path(int path[][MAX_FRAME*MAX_FRAME], int sy, int sx, int ty, int tx, int row, int col, int(*a)[MAX_FRAME])
{
	int X, Y;
	int i, j, d = 0;//dΪ��ԭ��ľ���
	cout << endl << "���ҽ�����飺" << endl;
	//��ʼ����
	cout << "  |";
	for (i = 1; i <= col; i++) {
		cout << right << setw(3) << i;
	}
	getxy(X, Y);
	cout << endl << "--+";
	for (i = 3; i <= X; i++)
		cout << "-";
	cout << endl;
	for (i = 1; i <= row; i++) {
		cout << left << setw(2) << char('A' - 1 + i) << "|";
		for (j = 1; j <= col; j++) {
			if (i == path[0][d] / 10 && j == path[0][d] % 10) {
				cout << right << setw(3) << '*';
				d++;
			}
			else
				cout << right << setw(3) << 0;
		}
		cout << endl;
	}
	d = 0;
	cout << endl << "�ƶ�·��(��ͬɫ��ʶ)��" << endl;
	cout << "  |";
	for (i = 1; i <= col; i++) {
		cout << right << setw(3) << i;
	}
	getxy(X, Y);
	cout << endl << "--+";
	for (i = 3; i <= X; i++)
		cout << "-";
	cout << endl;
	for (i = 1; i <= row; i++) {
		cout << left << setw(2) << char('A' - 1 + i) << "|";
		for (j = 1; j <= col; j++) {
			if (i == path[0][d] / 10 && j == path[0][d] % 10) {
				cout << "  ";
				getxy(X, Y);
				showch(X, Y, char(a[i][j] + '0'), COLOR_HYELLOW, COLOR_BLACK, 1);
				setcolor();
				d++;
			}
			else
				cout << right << setw(3) << a[i][j];
		}
		cout << endl;
	}
}

void Game1(int a[][MAX_FRAME], int row, int col)
{
	int X, Y, opt = 0;
	Init_array(a, row, col, 5);
	Interface_array(0, 6, row, col, a, opt++);
	getxy(X, Y);
	End(Y + 1);
}

void Game2(int a[][MAX_FRAME], int row, int col)
{
	int X, Y;
	int  sy = 0, ty = 0, sx = 0, tx = 0;
	int opt = 0;
	int path[2][MAX_FRAME*MAX_FRAME];
	Init_array(a, row, col, int(row*col*0.6));
	Interface_array(0, 5, row, col, a, opt++);
	getxy(X, Y);
	Pre_next(a, X, Y + 1, 3, 1);
	move_which_ball(a, row, col, &sx, &sy);
	move_destin(a, row, col, &tx, &ty);
	if (!bfs(path, sy, sx, ty, tx, a, row, col))
		cout << "�޷��ҵ��ƶ�·��" << endl;
	else
		show_array_path(path, sy, sx, ty, tx, row, col, a);
	getxy(X, Y);
	End(Y + 1);
}

void Game3(int a[][MAX_FRAME], int row, int col)
{
	int X, Y;
	int  sy = 0, ty = 0, sx = 0, tx = 0;
	int opt = 0;
	int path[2][MAX_FRAME*MAX_FRAME];
	int *color;
	int score = 0, sumscore = 0;
	Init_array(a, row, col, 5);
	for (int i = 0;; i++) {
		getxy(X, Y);
		Interface_array(0, Y + 2, row, col, a, opt++);
		getxy(X, Y);
		color = Pre_next(a, X, Y + 2, 3, score);
		move_which_ball(a, row, col, &sx, &sy);
		move_destin(a, row, col, &tx, &ty);
		if (!bfs(path, sy, sx, ty, tx, a, row, col)) {
			cout << "�޷��ҵ��ƶ�·��" << endl;
			continue;
		}
		else {
			Move_ball(a, sy, sx, ty, tx);
			Do_next(a, row, col, color, 3);
			getxy(X, Y);
			Interface_array(0, Y + 2, row, col, a, -1);
			score = Check(a, row, col);
			sumscore += score;
			cout << "���ε÷֣�" << score << " �ܵ÷֣�" << sumscore << endl;
		}
		if (Gameover(a, row, col))
			break;
	}
	getxy(X, Y);
	End(Y + 1);
}