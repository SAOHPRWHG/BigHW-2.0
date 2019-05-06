/* 1751957 ��2�� ������ */
#include <iostream>
#include "../common/cmd_console_tools.h"
#include "common.h"
using namespace std;


//�˵�����
char menu_choose(const char *tab)
{
	//������ѡ��tab����������
	char m;
	while (1) {
		m = _getche();
		if (m >= 'a'&&m <= 'z')
			m -= 32;
		if (tj_strchr(tab, m)) {
			break;
		}
	}
	return m;
}

char menu(const char *array[], const char sepline, const char *tab)
{
	setfontsize("", 14);
	setconsoleborder(120, 60, 600, 600);

	unsigned int len = 0, i;
	char m;
	//�����ַ�����󳤶�
	for (i = 0; i < OPT_NUM; i++) {
		if (strlen((array)[i]) > len)
			len = strlen(array[i]);
	}

	//��ӡ�Ϸָ���
	for (i = 0; i <= len; i++)
		cout << sepline;
	cout << endl;
	//��ӡ����
	for (i = 0; i < OPT_NUM; i++)
		cout << array[i] << endl;
	//��ӡ�·ָ���
	for (i = 0; i <= len; i++)
		cout << sepline;
	cout << endl;

	cout << "[��ѡ��]��";

	m = menu_choose(tab);
	return m;
}

//��ӡ�ڲ�������溯��
void Interface_array(int x, int y, struct ball_info map[][MAX_FRAME], int row, int col, const char str[], int sign_status)
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

	cout << str << endl;
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
				setcolor(3, 15);
			}
			else
				cout << right << setw(3) << map[i][j].value;
		}
		cout << endl;
	}


}

//��ӡ���б꺯��
void print_rc_sign(int X, int Y, int separator, int rc_sign, int row, int col, const char c[], const char r[])
{
	int i, gap = 1;
	if (rc_sign) {
		cout << "   ";
		if (separator) {
			gap = 2;
		}
		for (i = 0; i < col; i++) {
			cout << setw(2 * gap) << c[i];
		}
		for (i = 0; i < row; i++) {
			gotoxy(X, Y + gap * i + 2);
			cout << left << setw(2) << r[i];
		}
		gotoxy(X + 2, Y + 1);
	}
}

//��ʾ���ܺ���
void print_GUI(const char *frame[], int separator, int rc_sign, struct ball_info map[][MAX_FRAME], int row, int col, int opt)
{
	gotoxy(0, 0);
	int X, Y;
	int cols, lines, buffer_cols, buffer_lines;
	getconsoleborder(cols, lines, buffer_cols, buffer_lines);
	cout << "��Ļ��" << cols << "��" << lines << "��";
	if (opt)
		cout << "(˫���Ҽ��˳�)";
	cout << endl;
	getxy(X, Y);
	print_rc_sign(X, Y, separator, rc_sign, row, col, "012345678", "ABCDEFGHI");
	getxy(X, Y);

	//��ʼ����
	if (!separator) {
		showstr(X, Y, frame[0], COLOR_HWHITE, COLOR_BLACK);
		showstr(X + 2, Y, frame[1], COLOR_HWHITE, COLOR_BLACK, col);
		showstr(X + 2 + col * 2, Y, frame[2], COLOR_HWHITE, COLOR_BLACK);
		// "�X�T�[�U�U�T�a"
		cout << endl;
		for (int i = 1; i <= row; i++) {
			showstr(X, Y + i, frame[3], COLOR_HWHITE, COLOR_BLACK);
			showstr(X + 2, Y + i, frame[4], COLOR_HWHITE, COLOR_BLACK, col);
			showstr(X + 2 + col * 2, Y + i, frame[5], COLOR_HWHITE, COLOR_BLACK);
			cout << endl;
		}
		showstr(X, Y + 1 + row, frame[6], COLOR_HWHITE, COLOR_BLACK);
		showstr(X + 2, Y + 1 + row, frame[7], COLOR_HWHITE, COLOR_BLACK, col);
		showstr(X + 2 + col * 2, Y + 1 + row, frame[8], COLOR_HWHITE, COLOR_BLACK);
	}
	else {
		//frame_with_sep = {"�X�T","�j�T","�[","�U��","�U","�d�T","�p�T","�g","�U��","�U","�^�T","�m�T","�a"}
		const int b = 4;
		showstr(X, Y, frame[0], COLOR_HWHITE, COLOR_BLACK);
		showstr(X + b, Y, frame[1], COLOR_HWHITE, COLOR_BLACK, col - 1);
		showstr(X + col * b, Y, frame[2], COLOR_HWHITE, COLOR_BLACK);
		showstr(X, Y + 1, frame[3], COLOR_HWHITE, COLOR_BLACK, col);
		showstr(X + col * b, Y + 1, frame[4], COLOR_HWHITE, COLOR_BLACK);

		//�U�T �[�X
		cout << endl;
		for (int i = 1; i < row; i++) {
			showstr(X, Y + 2 * i, frame[5], COLOR_HWHITE, COLOR_BLACK);
			showstr(X + b, Y + 2 * i, frame[6], COLOR_HWHITE, COLOR_BLACK, col - 1);
			showstr(X + col * b, Y + 2 * i, frame[7], COLOR_HWHITE, COLOR_BLACK);
			showstr(X, Y + 2 * i + 1, frame[8], COLOR_HWHITE, COLOR_BLACK, col);
			showstr(X + col * b, Y + 2 * i + 1, frame[9], COLOR_HWHITE, COLOR_BLACK);
			cout << endl;
		}
		showstr(X, Y + 2 * row, frame[10], COLOR_HWHITE, COLOR_BLACK);
		showstr(X + b, Y + 2 * row, frame[11], COLOR_HWHITE, COLOR_BLACK, col - 1);
		showstr(X + col * b, Y + 2 * row, frame[12], COLOR_HWHITE, COLOR_BLACK);

	}

	//����
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			map[i][j].row = i;
			map[i][j].col = j;
			map[i][j].separator = separator;
			map[i][j].rc_sign = rc_sign;
			map[i][j].bg_color = (map[i][j].value + 4) % 16;
			draw_the_ball(map[i][j]);
		}
	}
	setcolor(3, 15);
}

//��ʾһ��С����
void draw_the_ball(struct ball_info ball)
{
	const char *str[] = { "��","��","��","��","��","��" };
	showstr(cx(ball.col, ball.separator, ball.rc_sign), cy(ball.row, ball.separator, ball.rc_sign), str[ball.status], ball.bg_color, ball.front_color);
}

//ͨ�����뺯��
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

//End����
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

/*
�װ�������ʦ��
	����ʱ������������������δ���ü���Color Linez�����ʵ��ϸ�ڵĳ����޸ģ����ύ��֮ǰ�İ汾��
�����еĹ��������������˼����Բ����ԸĽ��������½⡣
*/
